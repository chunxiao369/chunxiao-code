#!/bin/sh
ufw disable
ip link set br0 up
ip addr add 192.168.3.1/24 dev br0
sysctl -w net.ipv4.ip_forward=1
iptables -t nat -A POSTROUTING -o wlp58s0 -s 192.168.3.0/24 -j MASQUERADE
iptables -t nat -A POSTROUTING -o enp0s31f6 -s 192.168.3.0/24 -j MASQUERADE

#kvm -m 2048 -net nic,macaddr=00:11:22:33:44:55 -soundhw all -net tap,ifname=cxxu0,script=ovs-ifup,downscript=ovs-ifdown /home/lance/Project/vm_img/win7-64.img &
#kvm -m 4096 -nographic -smp 2,sockets=1,cores=2,threads=1 -net nic,macaddr=00:11:22:33:44:55 -net tap,script=ovs-ifup,downscript=ovs-ifdown -drive file=/home/lance/Project/vm_img/centos1.img
ovs-vsctl del-port br0 cxxu0
ovs-vsctl del-port br0 cxxu1
ovs-vsctl del-port br0 cxxu2
qemu-system-x86_64 --enable-kvm -cpu Haswell -smp 8,sockets=1,cores=8,threads=1 -nographic -m 8192 -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=net1  -netdev tap,id=net1,ifname=cxxu0,script=./ovs-ifup,downscript=./ovs-ifdown -device virtio-net-pci,mac=00:00:00:00:00:02,netdev=net2  -netdev tap,id=net2,ifname=cxxu1,script=./ovs-ifup,downscript=./ovs-ifdown -device virtio-net-pci,mac=00:00:00:00:00:03,netdev=net3  -netdev tap,id=net3,ifname=cxxu2,script=./ovs-ifup,downscript=./ovs-ifdown -drive file=/home/lance/Project/vm_img/centos_new1.img -drive file=/home/lance/Project/vm_img/disk_xx.img  &
qemu-system-x86_64 --enable-kvm -cpu Haswell -smp 4,sockets=1,cores=4,threads=1 -nographic -m 8192 -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=net1  -netdev tap,id=net1,vhost=on,script=./ovs-ifup,downscript=./ovs-ifdown -device virtio-net-pci,mac=00:00:00:00:ff:01,netdev=net3  -netdev tap,id=net3,vhost=on,script=./ovs-ifup,downscript=./ovs-ifdown -drive file=/home/lance/Project/vm_img/centos_new1.img -drive file=/home/lance/Project/vm_img/disk_xx.img  &

qemu-system-x86_64 -cpu help
-daemonize -display none


qemu-system-x86_64 --enable-kvm -cpu Haswell -nographic -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:02,netdev=net2  -netdev tap,id=net2,script=./ovs-ifup -drive file=/home/lance/Project/vm_img/centos_new2.img -drive file=/home/lance/Project/vm_img/disk_xx.img &

ovs-vsctl list mirror
ovs-vsctl clear bridge br0 mirrors
ovs-vsctl -- set Bridge br0 mirrors=@m \
-- --id=@tap0 get Port tap0 \
-- --id=@tap1 get Port tap1 \
-- --id=@m create Mirror name=mirror_test select-dst-port=@tap0 select-src-port=@tap0 output-port=@tap1

ovs-vsctl \
-- --id=@m create mirror name=mymirror \
-- add bridge ovsbr0 mirrors @m \
-- --id=@tap1 get port tap1 \
-- set mirror mymirror select_all=true output-port=@tap1
