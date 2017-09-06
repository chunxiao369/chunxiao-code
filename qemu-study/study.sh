
qemu-img create -f qcow disk.img 128M
qemu -m 128 -hda linux-0.2.img
vncviewer 127.0.0.1:5900

---install---
kvm -m 1024 -net nic,macaddr=00:11:11:EE:EE:EE -net tap,script=/etc/openvswitch/ovs-ifup,downscript=/etc/openvswitch/ovs-ifdown -drive file=centos.img -cdrom /home/chunxiao/Downloads/CentOS-7-x86_64-Minimal-1503.iso -boot d
----boot---
kvm -m 1024 -net nic,macaddr=00:11:11:EE:EE:EE -net tap,script=/etc/openvswitch/ovs-ifup,downscript=/etc/openvswitch/ovs-ifdown -drive file=centos.img -boot c

kvm -m 512 -net nic,macaddr=00:11:22:33:44:55 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown /home/lance/Project/vm_img/centos1.img
#this is new method
kvm -m 512 -device rtl8139,mac=00:11:22:33:44:55,netdev=tap0 -netdev tap,id=tap0,ifname=tap0,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown /home/lance/Project/vm_img/centos1.img

qemu-system-x86_64 --enable-kvm -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=net1  -netdev tap,id=net1,script=./etc/ovs-ifup /home/lance/Project/vm_img/centos1.img
qemu-system-x86_64 --enable-kvm -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:02,netdev=net2  -netdev tap,id=net2,script=./etc/ovs-ifup /home/lance/Project/vm_img/centos2.img

sudo kvm -m 1024 -net nic,macaddr=00:11:22:33:44:56 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown /home/lance/Project/vm_img/win7-64.img

docker run -d -p 5000:5000 --restart=always --name registry -v /mnt/disk_10G/:/var/lib/registry registry:2

#------------------with cable-------------------------------------
#------------------my pc
#   vm:
#   by dhcp or set ip
#
#   host:
#   ip addr del 10.6.1.56/16 dev eth0
#   ip addr add 10.6.1.56/16 dev br0
#   ip link set br0 up
#   ip route add default via 10.6.0.1 dev br0
#   ./bin/ovs-vsctl add-port br0 eth0

#------------------my notebook
#   vm:
#   by dhcp or set ip
#
#   host:
#   ip addr del 192.168.33.29/24 dev enp0s25
#   ip addr add 192.168.33.29/24 dev br0
#   ip link set br0 up
#   ip route add default via 192.168.33.1 dev br0
#   ovs-vsctl add-port br0 enp0s25


#-------------------with wifi--------------------------------------
#------------------my notebook
#   vm:
#   set ip 192.168.3.3 255.255.255.0
#   set gatewayip 192.168.3.1
#   set dns ip 10.0.1.1
#   
#   host:
#   ip link set br0 up
#   ip addr add 192.168.3.1/24 dev br0
#   sysctl -w net.ipv4.ip_forward=1
#   iptables -t nat -A POSTROUTING -o enp0s25 -s 192.168.3.0/24 -j MASQUERADE
#   iptables -t nat -A POSTROUTING -o wlp3s0  -s 192.168.3.0/24 -j MASQUERADE


