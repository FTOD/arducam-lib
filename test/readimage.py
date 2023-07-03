import numpy as np
from PIL import Image


with open("/tmp/gest_one.raw", "rb") as f:
    img = f.read()

height = 120
width = 160
if (len(img) != height*width*2):
    print("buffer data not exactly the size of the image, aborting...")
    exit(0)
data = img

