#!/bin/bash
#
echo -n "Circle num ="
echo $1

i=0
while [ $i -lt $1 ]
do
	i=`expr $i + 1`
	#echo -n "i="
	#echo $i

	num=`expr $RANDOM % 1000`
	echo -n "add&delete_num="
	echo $num

	j=0
	while [ $j -lt $num ]
	do
		j=`expr $j + 1`
		ruletmp=`expr $RANDOM % 64`
		ruleid=`expr $ruletmp + 1`
		ip_p1=`expr $RANDOM % 256`
		ip_p2=`expr $RANDOM % 256`
		ip_p3=`expr $RANDOM % 256`
		ip_p4=`expr $RANDOM % 256`
		ip_p11=`expr $RANDOM % 256`
		ip_p22=`expr $RANDOM % 256`
		ip_p33=`expr $RANDOM % 256`
		ip_p44=`expr $RANDOM % 256`
		# ip_pair
		cli -c add rule L4:$ruleid ip=$ip_p1.$ip_p2.$ip_p3.$ip_p4,$ip_p11.$ip_p22.$ip_p33.$ip_p44
	
                id[$j]=$ruleid
		ip[$j]=$ip_p1.$ip_p2.$ip_p3.$ip_p4,$ip_p11.$ip_p22.$ip_p33.$ip_p44
		#echo -n "Add: "
		#echo -n ${id[$j]}
		#echo -n "  "
		#echo ${ip[$j]}

        done
        
	j=0
	while [ $j -lt $num ]
	do
		j=`expr $j + 1`
		#echo -n "Delete: "
		#echo -n ${id[$j]}
		#echo -n "  "
		#echo ${ip[$j]}
        	cli -c delete rule L4:${id[$j]} ip=${ip[$j]}
	done

done
