#!/bin/sh
find `pwd` -name "*.[ch]" -type f > cscope.files
ctags -R
cscope -qb -i cscope.files

#find . \( -path "*.git*" -o  -path "*work.linux-2.2*" \) -prune -o -print > cscope.temp
#grep "\.[ch]$" cscope.temp > cscope.files
#rm -f cscope.temp

