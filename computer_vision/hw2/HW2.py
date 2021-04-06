import numpy as np
import cv2
import sklearn
from PIL import Image
import matplotlib.pyplot as plt

def threshold(img):
	th = 128
	height = img.shape[0]
	width = img.shape[1]

	thpic = np.zeros((img.shape), np.int)
	for x in range(height):
		for y in range(width):
			if img[x,y] >= th:
				thpic[x,y] = 255
			else:
				thpic[x,y] = 0
	return thpic
	
def histogram(img):
	height = img.shape[0]
	width = img.shape[1]

	his = np.zeros(256, np.int)
	for x in range(width):
		for y in range(height):
			his[img[x,y]] += 1

	plt.bar(range(len(his)), his, width = 1.8)
	plt.savefig("histogram.png")

def connect(img, img2):
	binary = threshold(img)
	visited = np.zeros(img.shape, np.int)
	tmp = img2.copy().astype(np.int32)
	for x in range(img.shape[0]):
		for y in range(img.shape[1]):
			cnt, cntr, cntc = 0, 0, 0
			L, R, U, B = 100000000, -1, 100000000, -1
			stack = [(x, y)]
			while stack:
				r, c = stack.pop()
				if 512 > r >= 0 and 512 > c >= 0 and visited[r, c] == 0 and binary[r, c] != 0:
					visited[r, c] = 1
					cnt += 1
					cntr += r
					cntc += c
					L = min(L, c)
					R = max(R, c)
					U = min(U, r)
					B = max(B, r)
					stack.extend([(r, c + 1), (r + 1, c)])
					if c > 0 :
						stack.extend([(r, c - 1)])
					if r > 0 :
						stack.extend([(r - 1, c)])
					
			if cnt >= 500:
				cv2.rectangle(tmp, (L, B), (R, U), (255, 0, 0), 3)
				cv2.circle(tmp, (int(cntc / cnt), int(cntr / cnt)), 5, (0, 0, 255), -1)
	return tmp



pic = cv2.imread("lena.bmp")
pic_gray = cv2.imread("lena.bmp", cv2.IMREAD_GRAYSCALE)

cv2.imwrite("threshold.bmp", threshold(pic_gray))
histogram(pic_gray)
cv2.imwrite("connected.bmp",connect(pic_gray,pic))

