import serial
import numpy as np
from PIL import Image
import sys

ser = serial.Serial("/dev/ttyACM0",115200)
img = bytes()


# sizes
height = 64
width = 64

while True:
    linein = ser.readline()

    if linein == b'IMG\n' :
        print("starting image transfer")
        while True:
            linein = ser.readline()
            # if linein == b'END\n' :
            #     break
            # else :
            img += linein
            if len(img) >= height*width*2:
                break
        print("transfer complete, len = ", len(img))
        if (len(img) > height*width*2):
            print("buffer data bigger than the image, delete the rest (usually useless)...")
            img = img[:height*width*2]
        break
    print(linein)


# Convert the byte array to a numpy array of uint16
pixels = np.frombuffer(img, dtype=np.uint16)#.byteswap()

# Reshape the numpy array to match the image dimensions
pixels = pixels.reshape(height, width)

# Function to convert from RGB565 to RGB888
def rgb565_to_rgb888(value):
    r = ((value & 0xF800) >> 11) * (255/31)
    g = ((value & 0x07E0) >> 5) * (255/63)
    b = (value & 0x001F) * (255/31)
    return np.array([r, g, b], dtype=np.uint8)

# Reshape the pixels array to 1D, apply the conversion function, 
# and then reshape the result to 3D (height x width x 3)
rgb_pixels = np.array([rgb565_to_rgb888(pixel) for pixel in pixels.flatten()]).reshape(height, width, 3)

# Create an image from the numpy array
image = Image.fromarray(rgb_pixels, 'RGB')

# save
filename = "/tmp/gest.bmp"
if len(sys.argv) < 2:
    print("no filename specified, using default: /tmp/gest.bmp")
else:
    filename = sys.argv[1]
print("saving image to ", filename)
image.save(filename)