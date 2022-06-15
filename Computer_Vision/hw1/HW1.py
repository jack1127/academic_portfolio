import numpy as np
import cv2
import sklearn
from PIL import Image

pic = cv2.imread("lena.bmp")
height = pic.shape[0]
width = pic.shape[1]
color = pic.shape[2]
pic2 = np.ones((height, width, color))

#1a
for x in range(height):
	pic2[x]=pic[height-1-x]
cv2.imwrite("1a.bmp", pic2)

#1b
for x in range(height):
	for y in range(width):
		pic2[x][y] = pic[x][width-1-y]
cv2.imwrite("1b.bmp", pic2)

#1c
for x in range(height):
	for y in range(width):
		pic2[x][y] = pic[y][x]
cv2.imwrite("1c.bmp", pic2)

#2d
im = Image.open("lena.bmp")
im2 = im.rotate(45)
im2.save("2d.bmp")

#2e
w = im.width
h = im.height 
new_w = int(w/2)
new_h = int(h/2)
im2 = im.resize((new_w, new_h))
im2.save("2e.bmp")

#2f
th = 128
im2f = np.array(Image.open('lena.bmp').convert('L'))
im_bool = im2f > th
im_bin_128 = (im2f > th) * 255
Image.fromarray(np.uint8(im_bin_128)).save('2f.bmp')