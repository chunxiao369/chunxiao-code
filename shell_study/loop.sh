#!/bin/sh
#
if [ $# -lt 6 ] ;then
echo "paras not enough!"
fi

Sum=0 
i=0 
rm -f command.txt
touch command.txt

while [ $i != "100" ] 
do 
i=`expr $i + 1` 
echo -n $i " " >>command.txt
Sum=`expr $Sum + $i` 
echo $Sum >>command.txt
done 
echo $i $Sum 

 

