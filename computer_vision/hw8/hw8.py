import numpy as np
import cv2
import math
import matplotlib.pyplot as plt

kernel = [  [0, 0], [0, 1], [0, 2], [0, -1], [0, -2],
			[1, 0], [1, 1], [1, 2], [1, -1], [1, -2],
			[-1, 0], [-1, 1], [-1, 2], [-1, -1], [-1, -2],
			[2, -1], [2, 0], [2, 1],
			[-2, -1], [-2, 0], [-2, 1] ]

def gaussion(img, amplitude):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			ans[x, y] = img[x, y] + amplitude * np.random.normal(0, 1)
	return ans

def salt_and_pepper(img, threshold):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			uniform = np.random.uniform(0, 1)
			if uniform < threshold:
				ans[x, y] = 0
			elif uniform > 1 - threshold:
				ans[x, y] = 255
			else:
				ans[x, y] = img[x, y]
	return ans

def box(img, size):
	ans = np.zeros((img.shape), np.int)
	tmp = np.zeros((img.shape[0] + size -1, img.shape[1] + size - 1), np.int)
	half = int((size - 1) / 2)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			tmp[x + half, y + half] = img[x, y]

	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			total = 0
			for a in range(int(-half), int(half + 1)):
				for b in range(int(-half), int(half +1)):
					total += tmp[x + half + a, y + half + b]
			ans[x, y] = round(total / (size * size))
	return ans

def median(img, size):
	ans = np.zeros((img.shape), np.int)
	tmp = np.zeros((img.shape[0] + size -1, img.shape[1] + size - 1), np.int)
	half = int((size - 1) / 2)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			tmp[x + half, y + half] = img[x, y]

	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			total = []
			for a in range(int(-half), int(half + 1)):
				for b in range(int(-half), int(half +1)):
					total.append(tmp[x + half + a, y + half + b])
			ans[x, y] = np.median(total)
	return ans

def dilation(img):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			maxn = 0
			for idx in kernel:
				if img.shape[0] > x - idx[0] >= 0 and img.shape[1] > y-idx[1] >= 0:
					if img[x - idx[0], y - idx[1]]  > maxn:
						maxn = img[x - idx[0], y - idx[1]]
			ans[x, y] = maxn
	return ans

def erosion(img, k):
	ans = np.zeros((img.shape), np.int)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			mini = 255
			for idx in kernel:
				if img.shape[0] > x + idx[0] >= 0 and img.shape[1] > y + idx[1] >= 0:
					if img[x + idx[0], y + idx[1]] < mini:
						mini = img[x + idx[0], y + idx[1]]
			ans[x, y] = mini
	
	return ans

def opening(img):
	ans = erosion(img, kernel)
	ans = dilation(ans)
	return ans

def closing(img):
	ans = dilation(img)
	ans = erosion(ans, kernel)
	return ans

def snr(img, noise):
	vs = u = vn = un = 0
	ans = 0
	size = img.shape[0] * img.shape[1]
	for x in range (img.shape[0]):
		for y in range(img.shape[1]):
			u += img[x, y]
	u /= size

	for x in range (img.shape[0]):
		for y in range(img.shape[1]):
			vs += (img[x, y] - u)**2
	vs /= size

	for x in range( noise.shape[0]):
		for y in range(noise.shape[1]):
			un += (noise[x, y] - img[x, y])
	un /= size
	
	for x in range( noise.shape[0]):
		for y in range(noise.shape[1]):
			vn += (noise[x, y] - img[x, y] - un)**2
	vn /=  size

	ans_snr = 20 * math.log( math.sqrt(vs) / math.sqrt(vn), 10)

	return ans_snr

img_gray = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
g10 = gaussion(img_gray, 10)
g30 = gaussion(img_gray, 30)
sap005 = salt_and_pepper(img_gray, 0.05)
sap01 = salt_and_pepper(img_gray, 0.1)
box_g10_3 = box(g10, 3)
box_g30_3 = box(g30, 3)
box_sap005_3 = box(sap005, 3)
box_sap01_3 = box(sap01, 3)
box_g10_5 = box(g10, 5)
box_g30_5 = box(g30, 5)
box_sap005_5 = box(sap005, 5)
box_sap01_5 = box(sap01, 5)
median_g10_3 = median(g10, 3)
median_g30_3 = median(g30, 3)
median_sap005_3 = median(sap005, 3)
median_sap01_3 = median(sap01, 3)
median_g10_5 = median(g10, 5)
median_g30_5 = median(g30, 5)
median_sap005_5 = median(sap005, 5)
median_sap01_5 = median(sap01, 5)
oc_g10 = closing(opening(g10))
co_g10 = opening(closing(g10))
oc_g30 = closing(opening(g30))
co_g30 = opening(closing(g30))
oc_sap005 = closing(opening(sap005))
co_sap005 = opening(closing(sap005))
oc_sap01 = closing(opening(sap01))
co_sap01 = opening(closing(sap01))

cv2.imwrite("g10.bmp", g10)
cv2.imwrite("g30.bmp", g30)
cv2.imwrite("sap005.bmp", sap005)
cv2.imwrite("sap01.bmp", sap01) 
cv2.imwrite("box_g10_3.bmp", box_g10_3)
cv2.imwrite("box_g30_3.bmp", box_g30_3)
cv2.imwrite("box_sap005_3.bmp", box_sap005_3)
cv2.imwrite("box_sap01_3.bmp", box_sap01_3)
cv2.imwrite("box_g10_5.bmp", box_g10_5)
cv2.imwrite("box_g30_5.bmp", box_g30_5)
cv2.imwrite("box_sap005_5.bmp", box_sap005_5)
cv2.imwrite("box_sap01_5.bmp", box_sap01_5)
cv2.imwrite("median_g10_3.bmp", median_g10_3)
cv2.imwrite("median_g30_3.bmp", median_g30_3)
cv2.imwrite("median_sap005_3.bmp", median_sap005_3)
cv2.imwrite("median_sap01_3.bmp", median_sap01_3)
cv2.imwrite("median_g10_5.bmp", median_g10_5)
cv2.imwrite("median_g30_5.bmp", median_g30_5)
cv2.imwrite("median_sap005_5.bmp", median_sap005_5)
cv2.imwrite("median_sap01_5.bmp", median_sap01_5)
cv2.imwrite("co_g10.bmp", co_g10)
cv2.imwrite("oc_g10.bmp", oc_g10)
cv2.imwrite("co_g30.bmp", co_g30)
cv2.imwrite("oc_g30.bmp", oc_g30)
cv2.imwrite("co_sap005.bmp", co_sap005)
cv2.imwrite("oc_sap005.bmp", oc_sap005)
cv2.imwrite("co_sap01.bmp", co_sap01)
cv2.imwrite("oc_sap01.bmp", oc_sap01)
print("g10:",snr(img_gray, g10))
print("box_g10_3:",snr(img_gray, box_g10_3))
print("box_g10_5:",snr(img_gray, box_g10_5))
print("median_g10_3:",snr(img_gray, median_g10_3))
print("median_g10_5:",snr(img_gray, median_g10_5))
print("oc_g10:",snr(img_gray, oc_g10))
print("co_g10:",snr(img_gray, co_g10))
print("g30:",snr(img_gray, g30))
print("box_g30_3:",snr(img_gray, box_g30_3))
print("box_g30_5:",snr(img_gray, box_g30_5))
print("median_g30_3:",snr(img_gray, median_g30_3))
print("median_g30_5:",snr(img_gray, median_g30_5))
print("oc_g30:",snr(img_gray, oc_g30))
print("co_g30:",snr(img_gray, co_g30))
print("sap005:",snr(img_gray, sap005))
print("box_sap005_3:",snr(img_gray, box_sap005_3))
print("box_sap005_5:",snr(img_gray, box_sap005_5))
print("median_sap005_3:",snr(img_gray, median_sap005_3))
print("median_sap005_5:",snr(img_gray, median_sap005_5))
print("oc_sap005:",snr(img_gray, oc_sap005))
print("co_sap005:",snr(img_gray, co_sap005))
print("sap01:",snr(img_gray, sap01))
print("box_sap01_3:",snr(img_gray, box_sap01_3))
print("box_sap01_5:",snr(img_gray, box_sap01_5))
print("median_sap01_3:",snr(img_gray, median_sap01_3))
print("median_sap01_5:",snr(img_gray, median_sap01_5))
print("oc_sap01:",snr(img_gray, oc_sap01))
print("co_sap01:",snr(img_gray, co_sap01))