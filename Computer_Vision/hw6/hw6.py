import numpy as np
import cv2
import sys
np.set_printoptions(threshold = sys.maxsize)

def downsample(img):
	ans = np.zeros((66, 66), np.int)
	for x in range(64):
		for y in range(64):
			if img[x * 8, y * 8] >= 128:
				ans[x + 1, y + 1] = 255
			else:
				ans[x + 1, y + 1] = 0
	return ans

def yokoi(img):
	kernel = [[0, 1], [1, 1], [1, 0], [1, -1],
			[0, -1], [-1, -1], [-1, 0], [-1, 1]]
	ans = np.zeros((64, 64), np.int)
	for x in range(1, 65):
		for y in range(1, 65):
			if img[x, y] ==  255:
				flag = 0
				count = 0
				cnt = 0
				for i in range(9):
					if img[x + kernel[i % 8][0], y + kernel[i % 8][1]] == 255:
						cnt += 1
						if flag == 1 and i == 8:
							count -= 1
						if flag == 0 and i % 2 == 0 and i < 8:
							count += 1
							flag = 1
					else:
						if flag == 1:
							flag = 0
				if cnt == 9:
					ans[x - 1, y - 1] = 5
				else:
					ans[x - 1, y - 1] = count

	f = open("yokoi.txt", "w")
	for x in range(64):
		for y in range(64):
			if ans[x, y] == 0:
				f.write(" ")
			else:
				f.write(str(ans[x, y]))
			f.write(" ")
		f.write("\n")

img_gray = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
cv2.imwrite("downsample.bmp", downsample(img_gray))
yokoi(downsample(img_gray))
