# !/bin/sh
# 

#echo $0
#echo $1
echo -n "There are " 
echo -n $# 
echo " paras."

echo ${12}
if [ $# != 1 ] ; then
echo "USAGE: $0 TABNAME"
echo " e.g.: $0 CDR_CALL_20040701"
fi

