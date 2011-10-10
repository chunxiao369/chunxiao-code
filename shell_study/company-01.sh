#!/bin/sh
#
if [ $# -lt 1 ] ;then
echo "paras not enough!"
else

rm -f command.txt
touch command.txt

echo -n "There are "
echo -n $#
echo " paras."

rule_mask =0

#echo $1 $2 $3 $4 $5 $6 $7 $8 $9 $9 ${10} ${11} ${12} ${13}
first_param=$1
param=0
param_num=$#
while [ $param -lt $param_num ]
do
 echo $1
 last_param=$1
 case $1 in 
 sip)
    echo "have sip."
    sip_base=$2
    if [ $3 = "-a" ]; then
    sip_ascend=$4
    rule_mask = `expr $rule_mask + 110000000`
    else
    rule_mask = `expr $rule_mask + 100000000`
    ;;
 dip)
    echo "have dip"
    dip_base=$2
    if [ $3 = "-a" ]; then
    dip_ascend=$4
    rule_mask = `expr $rule_mask + 001100000`
    else
    rule_mask = `expr $rule_mask + 001000000`
    ;;
 sport)
    echo "have sport"
    sport_base=$2
    if [ $3 = "-a" ]; then
    sport_ascend=$4
    rule_mask = `expr $rule_mask + 000011000`
    else
    rule_mask = `expr $rule_mask + 000010000`
    ;;
 dport)
    echo "have dport"
    dport_base=$2
    if [ $3 = "-a" ]; then
    dport_ascend=$4
    rule_mask = `expr $rule_mask + 000000110`
    else
    rule_mask = `expr $rule_mask + 000000100`
    ;;
 protocol)
    echo "have protocol"
    protocol=$2
    rule_mask = `expr $rule_mask + 000000001`
    ;;
 *)
    ;;
 esac
 param=`expr $param + 1`
 shift
done
echo $last_param

times=0
while [ $times -lt $last_param ]
do 
 
 case $rule_mask in
   111111111)
   ;;
   
 echo  "add " $first_param  >>command.txt
 times=`expr $times + 1`
done

fi


