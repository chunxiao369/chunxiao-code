#!/bin/sh
modprobe nbd max_part=8
qemu-nbd --connect=/dev/nbd0 /mnt/d-disk/vmware-disk/share-vmare.vmdk
#qemu-nbd --connect=/dev/nbd1 /mnt/d-disk/edisk-files/CentOS-/Other\ Linux\ 2.6.x\ kernel.vmdk
sleep 1
mount /dev/nbd0p1 /mnt/share_disk/
#sleep 1
#vgchange -ay /dev/VolGroup00
#mount /dev/VolGroup00/LogVol00 /mnt/centos/
