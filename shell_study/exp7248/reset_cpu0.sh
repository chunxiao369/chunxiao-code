#!/bin/sh
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

