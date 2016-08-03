#!/bin/sh
cut -d ',' -f 1 data > 1.txt
cut -d ',' -f 2 data > 2.txt
paste 1.txt 2.txt -d ',' > merge1_2.txt
#-d是分隔符
#-f是选择哪一列
