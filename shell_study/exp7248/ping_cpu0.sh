#!/bin/bash
cpu_ip=192.168.0.3
#cpu_ip=127.0.0.1
main() {
times=0
date_s=`date +%Y%m%d%H%M%S`
echo "ping $cpu_ip start, $date_s" 
for i in `seq 8`;
do ping -c1 $cpu_ip > /dev/null
    if [ $? -eq 0 ];
        then
        echo "success"
        sleep 10
        else
        date_s=`date +%Y%m%d%H%M%S`
        echo "ping $cpu_ip failed, $date_s" 
        times=`expr $times + 1`
    fi
done
echo failed times: $times
if [ $times -eq 8 ];
    then
        echo "need reboot cpu"
        echo "port xe50 enable=f" > /tmp/cpu0_port_down.soc
        echo "port xe51 enable=f" >> /tmp/cpu0_port_down.soc
        echo -e "\nrcload /tmp/cpu0_port_down.soc\nexit" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1
        export OCTEON_REMOTE_PROTOCOL=PCI:0
        oct-remote-boot --board=cust_cn6880 /appfs/Uboot_Octeon_cn6880_EP_V1.1.9-p8.bin
        oct-remote-load 0x20000000 /appfs/vmlinux.64_Octeon2_Prog_V1.1.11-p8.bin
        oct-remote-bootcmd "bootoctlinux 0x20000000 rdinit=/preinit"
        sleep 120;
        echo "port xe50 enable=t" > /tmp/cpu0_port_down.soc
        echo "port xe51 enable=t" >> /tmp/cpu0_port_down.soc
        echo -e "\nrcload /tmp/cpu0_port_down.soc\nexit" | /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1
    else
    echo "good state"
fi
}
while [ 1 ];
do 
   main 
done

