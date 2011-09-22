#!/bin/bash

usage()
{
	echo "Usage: `basename $0` -t tcp:udp -l rule_num -p start_port -s step -c counter"
	exit 1
}

while getopts "t:l:p:s:c:" opt; do
case $opt in 
        t)
                        port_type=$OPTARG;;
        l)
                        rule_num=$OPTARG;;
        p)
                        start_port=$OPTARG;;
        s)
                        step=$OPTARG;;
        c)
                        counter=$OPTARG;;
        ?)
                        usage
						exit 1;;
esac
done

i=0
port=$start_port
while [ $i -lt $counter ]
do
	i=`expr $i + 1`
	port=`expr $port + $step`
	port=`expr $port % 65535`

	#echo " rule L4:$ruleid port=$port" >> port.txt
	echo " rule L4:$rule_num $port_type port=$port"
done
