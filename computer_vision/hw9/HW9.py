import numpy as np
import cv2
import sys
import math

def extend(img):
    img_ext = np.zeros((img.shape[0] + 2, img.shape[1] + 2))
    img_ext[0, 1:-1] = img[0, :]
    img_ext[-1, 1:-1] = img[-1, :]
    img_ext[1:-1, 0] = img[:, 0]
    img_ext[1:-1, -1] = img[:, -1]
    img_ext[0, 0] = img[0, 0]
    img_ext[0, -1] = img[0, -1]
    img_ext[-1, 0] = img[-1, 0]
    img_ext[-1, -1] = img[-1, -1]
    img_ext[1:-1, 1:-1] = img[:, :]
    return img_ext

def robert(img, threshold):
	tmp = np.zeros((img.shape[0] + 1, img.shape[1] + 1), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			tmp[x, y] = img[x, y]
	for x in range(img.shape[0]):
		tmp[x, img.shape[1]] = img[x, img.shape[1] - 1]
	for y in range(img.shape[1]):
		tmp[img.shape[0], y] = img[img.shape[0] - 1, y]

	ans = np.zeros(img.shape, np.int);
	
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			ans[x, y] = 255
			gx = int(tmp[x + 1, y + 1]) - int(tmp[x,y])
			gy = int(tmp[x + 1, y]) - int(tmp[x, y + 1])
			g = (gx**2) + (gy**2)
			if g >= (threshold**2):
				ans[x,y] = 0
	return ans

def prewitt(img, threshold):
	ans = np.zeros(img.shape, np.int)
	tmp = extend(img)
	for x in range(1, img.shape[0] + 1):
		for y in range(1, img.shape[1] + 1):
			ans[x - 1, y - 1] = 255
			gx = int(tmp[x + 1, y - 1]) + int(tmp[x + 1, y]) + int(tmp[x + 1, y + 1]) - int(tmp[x - 1, y - 1]) - int(tmp[x - 1, y]) - int(tmp[x - 1, y + 1])
			gy = int(tmp[x - 1, y + 1]) + int(tmp[x, y + 1]) + int(tmp[x + 1, y + 1]) - int(tmp[x - 1, y - 1]) - int(tmp[x, y - 1]) - int(tmp[x + 1, y - 1])
			g = (gx**2) + (gy**2)
			if g >= (threshold**2):
				ans[x - 1, y - 1] = 0
	return ans

def sobel(img, threshold):
	ans = np.zeros(img.shape, np.int)
	tmp = extend(img)
	for x in range(1, img.shape[0] + 1):
		for y in range(1, img.shape[1] + 1):
			ans[x - 1, y - 1] = 255
			gx = int(tmp[x + 1, y - 1]) + int(tmp[x + 1, y] * 2) + int(tmp[x + 1, y + 1]) - int(tmp[x - 1, y - 1]) - int(tmp[x - 1, y] * 2) - int(tmp[x - 1, y + 1])
			gy = int(tmp[x - 1, y + 1]) + int(tmp[x, y + 1] * 2) + int(tmp[x + 1, y + 1]) - int(tmp[x - 1, y - 1]) - int(tmp[x, y - 1] * 2) - int(tmp[x + 1, y - 1])
			g = (gx**2) + (gy**2)
			if g >= (threshold**2):
				ans[x - 1, y - 1] = 0
	return ans

def frei_chen(img, threshold):
	ans = np.zeros(img.shape, np.int)
	tmp = extend(img)
	for x in range(1, img.shape[0] + 1):
		for y in range(1, img.shape[1] + 1):
			ans[x - 1, y - 1] = 255
			gx = int(tmp[x + 1, y - 1]) + int(tmp[x + 1, y] * math.sqrt(2)) + int(tmp[x + 1, y + 1]) - int(tmp[x - 1, y - 1]) - int(tmp[x - 1, y] * math.sqrt(2)) - int(tmp[x - 1, y + 1])
			gy = int(tmp[x - 1, y + 1]) + int(tmp[x, y + 1] * math.sqrt(2)) + int(tmp[x + 1, y + 1]) - int(tmp[x - 1, y - 1]) - int(tmp[x, y - 1] * math.sqrt(2)) - int(tmp[x + 1, y - 1])
			g = (gx**2) + (gy**2)
			if g >= (threshold**2):
				ans[x - 1, y - 1] = 0
	return ans

def kirsch(img, threshold):
	ans = np.zeros(img.shape, np.int)
	tmp = extend(img)
	k = [-3, -3, 5, 5, 5, -3, -3, -3]
	for x in range(1, img.shape[0] + 1):
		for y in range(1, img.shape[1] + 1):
			ans[x - 1, y - 1] = 255
			tmp_max = 0
			for z in range(8):
				gx = tmp[x - 1, y - 1] * k[z] + tmp[x - 1, y] * k[(z+1)%8] + tmp[x - 1, y + 1] * k[(z+2)%8] + tmp[x, y + 1] * k[(z+3)%8] + tmp[x + 1, y + 1] * k[(z+4)%8] + tmp[x + 1, y] * k[(z+5)%8] + tmp[x + 1, y - 1] * k[(z+6)%8] +tmp[x, y - 1] * k[(z+7)%8]
				if gx > tmp_max:
					tmp_max = gx

			g = tmp_max
			if g >= threshold:
				ans[x - 1, y - 1] = 0
	return ans

def robinson(img, threshold):
	ans = np.zeros(img.shape, np.int)
	tmp = extend(img)
	k = [-1, 0, 1, 2, 1, 0, -1, -2]
	for x in range(1, img.shape[0] + 1):
		for y in range(1, img.shape[1] + 1):
			ans[x - 1, y - 1] = 255
			tmp_max = 0
			for z in range(8):
				gx = tmp[x - 1, y - 1] * k[z] + tmp[x - 1, y] * k[(z+1)%8] + tmp[x - 1, y + 1] * k[(z+2)%8] + tmp[x, y + 1] * k[(z+3)%8] + tmp[x + 1, y + 1] * k[(z+4)%8] + tmp[x + 1, y] * k[(z+5)%8] + tmp[x + 1, y - 1] * k[(z+6)%8] +tmp[x, y - 1] * k[(z+7)%8]
				if gx > tmp_max:
					tmp_max = gx

			g = tmp_max
			if g >= threshold:
				ans[x - 1, y - 1] = 0
	return ans

def nevatia(img, threshold):
	ans = np.zeros(img.shape, np.int)
	tmp = extend(extend(img))
	kernel = [ [-2, -2], [-1, -2], [0, -2], [1, -2], [2, -2],
				[-2, -1], [-1, -1], [0, -1], [1, -1], [2, -1],
				[-2, 0], [-1, 0], [0, 0], [1, 0], [2, 0],
				[-2, 1], [-1, 1], [0, 1], [1, 1], [2, 1],
				[-2, 2], [-1, 2], [0, 2], [1, 2], [2, 2] ]
	k0 = [ 100, 100, 0, -100, -100, 100, 100, 0, -100, -100, 100, 100, 0,
			-100, -100, 100, 100, 0, -100, -100, 100, 100, 0, -100, -100]
	k1 = [ 100, 100, 100, 100, 100, 100, 100, 100, 78, -32, 100, 92, 0, -92,
			-100, 32, -78, -100, -100, -100, -100, -100, -100, -100, -100]
	k2 = [-100, -100, -100, -100, -100, 32, -78, -100, -100, -100, 100, 92,
			0, -92, -100, 100, 100, 100, 78, -32, 100, 100, 100, 100, 100]
	k3 = [ 100, 100, 100, 32, -100, 100, 100, 92, -78, -100, 100, 100, 0,
			-100, -100, 100, 78, -92, -100, -100, 100, -32, -100, -100, -100]
	k4 = [ -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, 0,
			0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100]
	k5 = [ 100, -32, -100, -100, -100, 100, 78, -92, -100, -100, 100, 100,
			0, -100, -100, 100, 100, 92, -78, -100, 100, 100, 100, 32, -100]

	for x in range(2, img.shape[0] + 2):
		for y in range(2, img.shape[1] + 2):
			ans[x - 2, y - 2] = 255
			cnt = 0
			tmp_max = 0
			g0 = g1 = g2 = g3 = g4 = g5 = 0
			for z in kernel:
				g0 += tmp[x + z[0], y + z[1]] * k0[cnt]
				g1 += tmp[x + z[0], y + z[1]] * k1[cnt]
				g2 += tmp[x + z[0], y + z[1]] * k2[cnt]
				g3 += tmp[x + z[0], y + z[1]] * k3[cnt]
				g4 += tmp[x + z[0], y + z[1]] * k4[cnt]
				g5 += tmp[x + z[0], y + z[1]] * k5[cnt]
				cnt += 1

			g = max(g0, g1, g2, g3, g4, g5)
			if g >= threshold:
				ans[x - 2, y - 2] = 0
	return ans

img_gray = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
cv2.imwrite("roberts_12.bmp", robert(img_gray, 12))
cv2.imwrite("prewitt_24.bmp", prewitt(img_gray, 24))
cv2.imwrite("sobel_38.bmp", sobel(img_gray, 38))
cv2.imwrite("frei_chen_30.bmp", frei_chen(img_gray, 30))
cv2.imwrite("kirsch_135.bmp", kirsch(img_gray, 135))
cv2.imwrite("robinson_43.bmp", robinson(img_gray, 43))
cv2.imwrite("nevatia_12500.bmp", nevatia(img_gray, 12500))