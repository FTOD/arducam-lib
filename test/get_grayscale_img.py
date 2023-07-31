import serial
import numpy as np
from PIL import Image
import sys

ser = serial.Serial("/dev/ttyACM0",115200)
img = bytes()


# sizes
height = 40 
width = 40

img_num = 0
while True:
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
                if len(img) >= height*width:
                    print ("len(img) = ", len(img))
                    break
            print("transfer complete, len = ", len(img))
            if (len(img) > height*width):
                print("buffer data bigger than the image, delete the rest (usually useless)...")
                img = img[:height*width]
            break
        #print(linein)


    # Convert the byte array to a numpy array of uint16
    pixels = np.frombuffer(img, dtype=np.uint8)#.byteswap()

    # Reshape the numpy array to match the image dimensions
    pixels = pixels.reshape(height, width)

    # Create an image from the numpy array
    image = Image.fromarray(pixels, 'L')

    # save
    filename = "./gest" + str(img_num) + ".bmp"
    if len(sys.argv) < 2:
        print("no filename specified, using default: ./gest.bmp")
    else:
        filename = sys.argv[1]
    print("saving image to ", filename)
    image.save(filename)
    img_num += 1