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
rm -f $topfile
while read line ; do
	line_num=$current
	list_url=`echo $line|awk '{print $1}'`
	name=`echo $line|awk '{print $2}'|awk -F'>' '{print $2}'`
	echo "processing $name ..."
	if wget $list_url -O list.htm -q
	then
		cat list.htm | grep "class=d" | awk -F\" '{print $2 }' | head -10 > list.dat
		rm -f list.htm
		stop=0
		while read line && [ $stop -eq 0 ] ; do
			if wget "$line" -O song.htm -q
			then
				line=`cat song.htm`
				rm -f song.htm
				line=${line#*var F=}
				F=${line%%,*}
				I=${line#*var I=\"}
				I=${I%%\"*}
				url=`./decrypt "$I" $F`
				suffix=${url##*.}
				#file="$current.mp3"
				file=$name.$suffix
				if wget "$url" -O "$file"
				then
					stop=1
				else
					echo "get $name file error in $current times"
					continue
				fi
					
			else
				echo "download $name page error in trying $current times"
				continue
			fi
		done < list.dat
		rm -f list.dat
	else
		echo "download $name list page error"
		continue	
	fi
	rm -f $current.htm
	let current=$current+1
done < all.dat
rm -f all.dat

echo
 
