import csv
from collections import Counter

file_num = 4
row_list = []
row_count = 0
result = []

for i in range(1, file_num+1):
    with open('predict_inc'+ str(i) + '.csv', newline='') as f:
        next(f)
        row_list.append(f.readlines())

row_count = int(len(row_list[0]))

with open('ensemble_output.csv', 'w', newline='') as outfile:
    writer = csv.writer(outfile)
    writer.writerow(['Id', 'Category'])  #header
    for i in range(row_count):
        temp_dict = {}
        for j in range(file_num):
            if row_list[j][i] not in temp_dict:
                temp_dict[row_list[j][i]] = 1
            else :
                temp_dict[row_list[j][i]] += 1
                if(temp_dict[row_list[j][i]] > file_num/2):
                    break
        
        idx, result = max(temp_dict, key=temp_dict.get).split(",")
        #print(type(idx))
        writer.writerow([str(idx).strip(), str(result).strip()])
