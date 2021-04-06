import numpy as np
import cv2
import sklearn
from PIL import Image
import matplotlib.pyplot as plt
import math

kernel = [  [0, 0], [0, 1], [0, 2], [0, -1], [0, -2],
			[1, 0], [1, 1], [1, 2], [1, -1], [1, -2],
			[-1, 0], [-1, 1], [-1, 2], [-1, -1], [-1, -2],
			[2, -1], [2, 0], [2, 1], [-2, -1], [-2, 0], [-2, 1] ]

def threshold(img):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			if img[x, y] >= 128:
				ans[x, y] = 255
			else:
				ans[x, y] = 0
	return ans

def dilation(img):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			if img[x, y] == 255:
				for idx in kernel:
					if img.shape[0] > x + idx[0] and x + idx[0] >= 0 and img.shape[1] > y + idx[1] and y + idx[1] >= 0:
						ans[x + idx[0], y + idx[1]] = 255
	return ans

def erosion(img, ker):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			flag = 1
			for idx in ker:
				if not (img.shape[0] > x + idx[0] and x + idx[0] >= 0 and img.shape[1] > y + idx[1] and y + idx[1]>= 0):
					flag = 0
					break
				elif img[x + idx[0], y + idx[1]] != 255 :
					flag = 0
					break
			if flag:
				ans[x, y] = 255
	return ans

def opening(img):
	ans = erosion(img, kernel)
	ans = dilation(ans)
	return ans

def closing(img):
	ans = dilation(img)
	ans = erosion(ans, kernel)
	return ans

def reverse(img):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			if img[x, y] == 255:
				ans[x, y] = 0
			else:
				ans[x, y] = 255
	return ans

def intersection(img1, img2):
	ans = np.zeros((img1.shape), np.int)
	for x in range(img1.shape[0]):
		for y in range(img1.shape[1]):
			if img1[x, y] == 255 and img2[x, y] == 255:
				ans[x, y] = 255
	return ans

def hit_and_miss(img):
	j = [ [0, 0], [0, -1], [1, 0] ]
	k = [ [0, 1], [-1, 0], [-1, 1] ]
	ans = np.zeros((img.shape), np.int)
	img_r = reverse(img)
	img1 = erosion(img, j)
	img2 = erosion(img_r, k)
	ans = intersection(img1, img2)
	return ans

img_gray = threshold(cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE))
cv2.imwrite("threshold.bmp", img_gray)
cv2.imwrite("dilation.bmp", dilation(img_gray))
cv2.imwrite("erosion.bmp", erosion(img_gray, kernel))
cv2.imwrite("opening.bmp", opening(img_gray))
cv2.imwrite("closing.bmp", closing(img_gray))
cv2.imwrite("hit_and_miss.bmp", hit_and_miss(img_gray))