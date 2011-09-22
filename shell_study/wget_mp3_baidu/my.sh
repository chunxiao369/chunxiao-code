#!/bin/bash

current=1
times=${1:-10}

top="http://list.mp3.baidu.com/topso/mp3topsong.html"
topfile=top.html

wget $top -O $topfile -q

if [ ! -s $topfile ] ; then
	echo "download top page error"
	exit
fi

cat $topfile | grep "<td" | grep "word=" | awk -F 'href=\"' '{print $2 $3}'  | sed 's/target=_blank>//g' | sed 's/<\/a>//g' | sed 's/<b>//g' | sed 's/<\/b>//g' | head -$times > all.dat
echo
 
