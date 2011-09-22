#/bin/sh

lspci | grep "Cavium Networks" > /dev/null
if [ $? -eq 0 ];then
    echo "find pa card"
else 
    exit 1
fi
sleep 1

lsmod | grep "icore_drv" > /dev/null 
if [ $? -eq 0 ];then
    echo "have"
else 
    exit 1 
fi

echo "come here"

