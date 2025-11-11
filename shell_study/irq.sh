if [ $# -lt 1 ] ;then
echo "Input the interface_name!"
echo "Example: sh $0 ens5f0"
exit
fi

IRQ_LIST=$(grep $1 /proc/interrupts | awk -F: '{print $1}')
i=0
for irq in $IRQ_LIST;
do 
    #echo $i
    #i=`expr $i + 1`
    #echo $i > /proc/irq/$irq/smp_affinity_list
    cat /proc/irq/$irq/smp_affinity_list
done
