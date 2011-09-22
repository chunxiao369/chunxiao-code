#!/bin/sh
umount /mnt/centos/
umount /mnt/work-disk/

qemu-nbd --disconnect /dev/nbd0 
qemu-nbd --disconnect /dev/nbd1

rmmod nbd
