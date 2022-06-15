import numpy as np
import cv2
import sklearn
from PIL import Image
import matplotlib.pyplot as plt
import math

def histogram(img):
	height = img.shape[0]
	width = img.shape[1]

	his = np.zeros(256, np.int)
	for x in range(width):
		for y in range(height):
			his[img[x,y]] += 1
	return his
	

def equalization(img):
    equ_img = np.zeros((img.shape), np.int)
    his = histogram(img)
    equ_his = (256 - 1) * (np.cumsum(his)/(img.size * 1.0))
    equ_his = equ_his.astype('uint8')
    height, width = img.shape
    equ_img = np.zeros(img.shape, dtype='uint8')  # Note the type of elements
    for i in range(height):
        for j in range(width):
            equ_img[i,j] = equ_his[img[i,j]]

    return equ_img

#1
pic_gray = cv2.imread("lena.bmp", cv2.IMREAD_GRAYSCALE)
his1 = histogram(pic_gray)
plt.bar(range(len(his1)), his1, width = 1.8)
plt.savefig("histogram1.png")

#2
pic_d3 = pic_gray // 3
cv2.imwrite("pic_d3.bmp",pic_d3)
his2 = histogram(pic_d3)
plt.figure()
plt.bar(range(len(his2)), his2, width = 1.8)
plt.savefig("histogram2.png")

#3
pic_equ = equalization(pic_d3)
cv2.imwrite("pic_equ.bmp",pic_equ)
his3 = histogram(pic_equ)
plt.figure()
plt.bar(range(len(his3)), his3, width = 1.8)
plt.savefig("histogram3.png")
