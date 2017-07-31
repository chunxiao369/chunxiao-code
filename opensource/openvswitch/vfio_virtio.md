#kvm with vfio
    echo "0000:02:00.0" /sys/bus/pci/devices/0000\:02\:00.0/driver/unbind
    echo 0000\:02\:00.0 /sys/bus/pci/devices/0000\:02\:00.0/driver/unbind
    echo 0000\:02\:00.1 /sys/bus/pci/devices/0000\:02\:00.1/driver/unbind

    [root@ceph-node1 install]# lspci -s "0000:02:00.0" -n
    02:00.0 0200: 8086:10fb (rev 01)

    /home/cxxu/dpdk-stable-16.11.2/tools/dpdk-devbind.py --bind=vfio-pci 0000:02:00.0
    /home/cxxu/dpdk-stable-16.11.2/tools/dpdk-devbind.py --bind=vfio-pci 0000:02:00.1

    echo "8086 10fb" > /sys/bus/pci/drivers/vfio-pci/new_id
    lspci -s 0000:02:00.0 -k
    qemu-system-x86_64 --enable-kvm -m 2048 -net none -device vfio-pci,host=02:00.0 -drive file=centos7_no1.img,if=virtio -nographic

#kvm with virtio
##host1
    qemu-system-x86_64 --enable-kvm -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=net1 -nographic -netdev tap,id=net1,script=./etc/ovs-ifup -drive file=centos7_no1.img,if=virtio
##host2
    qemu-system-x86_64 --enable-kvm -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:02,netdev=net2 -nographic -netdev tap,id=net2,script=./etc/ovs-ifup -drive file=centos7_no2.img,if=virtio

