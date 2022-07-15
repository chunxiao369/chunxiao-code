
# losetup -a | grep img
losetup /dev/loop3 cxxu.img
kpartx -av /dev/loop3
#ls -l /dev/mapper/
mkfs.minix /dev/mapper/loop3p1
kpartx -d /dev/loop3
losetup -d /dev/loop3

# mount -t minix -o offset=512 cxxu.img /mnt/new/
