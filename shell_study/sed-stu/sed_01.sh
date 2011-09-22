#!/bin/sh
v=xuchunxiao-v
p=i386
cat a.txt |sed -e "s%version=.*%version=${v}%"   > temp 
#cat temp |sed -e "s%platform=.*%platform=${p}%" > install
#rm -f temp

