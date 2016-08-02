#!/bin/sh
#sort命令的k选项大讨论
sort -t ' ' -k1,1 -k2n,2 data
sort -t ' ' -k1 -k2n data
sort -t ' ' -k2nr -k1 data
