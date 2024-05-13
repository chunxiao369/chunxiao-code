#coding=utf-8
import csv
with open('sing.csv','rb') as f:
    #indata = f.read()
    #print indata
    reader=csv.reader(f)
    for row in reader:
        new_item = tuple(row)
        print new_item
        new_item1 = row[0]
        new_item2 = row[1]
        print new_item1 + new_item2
