import csv
from collections import Counter

predict = []
with open('prediction1.csv', 'r') as f:
    rows = csv.reader(f)
    header = next(rows)
    for row in rows:
        maxlabel = max(row, key=row.count)
        predict.append(maxlabel)

with open('prediction_merged.csv', 'w') as f:
    f.write('Id,Class\n')
    for i, y in enumerate(predict):
        f.write('{},{}\n'.format(i, y))