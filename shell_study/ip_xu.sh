#!/bin/bash

usage()
{
	echo "Usage: `basename $0` -l rule_num -i start_ip -s step -c counter"
	exit 1
}

while getopts "l:i:s:c:" opt; do
case $opt in 
        l)
                        rule_num=$OPTARG;;
        i)
                        start_ip=$OPTARG;;
        s)
                        step=$OPTARG;;
        c)
                        counter=$OPTARG;;
        ?)
                        usage
						exit 1;;
esac
done

ip4=`echo $start_ip | awk -F. '{print $1}'`
ip3=`echo $start_ip | awk -F. '{print $2}'`
ip2=`echo $start_ip | awk -F. '{print $3}'`
ip1=`echo $start_ip | awk -F. '{print $4}'`

echo $step
i=0
let "start_ip_addr=$ip4 * 256 * 256 * 256 + $ip3 * 256 * 256 + $ip2 * 256 + $ip1"

ip_addr=$start_ip_addr
while [ $i -lt $counter ]
do
	i=`expr $i + 1`
	ip1=`expr $ip_addr % 256`
	
	ip2=`expr $ip_addr / 256`
	ip2=`expr $ip2 % 256`
	
	ip3=`expr $ip_addr / 256 / 256`
	ip3=`expr $ip3 % 256`
	
	ip4=`expr $ip_addr / 256 / 256 / 256`
	ip4=`expr $ip4 % 256`
	
	#echo " rule L4:$ruleid port=$port" >> port.txt
	echo "add rule L4:$rule_num ip=$ip4.$ip3.$ip2.$ip1"
	
	ip_addr=`expr $ip_addr + $step`
	ip_addr=`expr $ip_addr % 4294967295`
done
