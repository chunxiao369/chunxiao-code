#host1 and host2
mkdir -p /home/cxxu/openvswitch-2.7.1/install/etc/openvswitch
mkdir -p /home/cxxu/openvswitch-2.7.1/install/var/run/openvswitch
rm -f /home/cxxu/openvswitch-2.7.1/install/etc/openvswitch/conf.db
/home/cxxu/openvswitch-2.7.1/install/bin/ovsdb-tool create /home/cxxu/openvswitch-2.7.1/install/etc/openvswitch/conf.db  \
/home/cxxu/openvswitch-2.7.1/install/share/openvswitch/vswitch.ovsschema

/home/cxxu/openvswitch-2.7.1/install/sbin/ovsdb-server --remote=punix:/home/cxxu/openvswitch-2.7.1/install/var/run/openvswitch/db.sock \
--remote=db:Open_vSwitch,Open_vSwitch,manager_options \
--pidfile --detach

/home/cxxu/openvswitch-2.7.1/install/bin/ovs-vsctl --no-wait set Open_vSwitch . other_config:dpdk-init=true
/home/cxxu/openvswitch-2.7.1/install/share/openvswitch/scripts/ovs-ctl --no-ovsdb-server --db-sock="$DB_SOCK" start

./bin/ovs-vsctl add-br br0 -- set bridge br0 datapath_type=netdev

#host1
#ens1f0 ipaddr 192.168.100.1
./bin/ovs-vsctl add-port br0 ens1f0
ip addr del 192.168.100.1/24 dev ens1f0
ip addr add 192.168.100.1/24 dev br0

./bin/ovs-vsctl add-br br1
ip addr add 192.1.1.1/16 dev br1 
ip link set br1 up
./bin/ovs-vsctl add-port br1 vx1 -- set interface vx1 type=vxlan options:remote_ip=192.168.100.2
./bin/ovs-vsctl add-port br1 vx1 -- set interface vx1 type=vxlan \
    options:remote_ip=192.168.100.2 options:key=flow options:dst_port=8472

#host2
#ens1f0 ipaddr 192.168.100.2
./bin/ovs-vsctl add-port br0 ens1f0
ip addr del 192.168.100.2/24 dev ens1f0
ip addr add 192.168.100.2/24 dev br0

./bin/ovs-vsctl add-br br1
ip addr add 192.1.2.1/16 dev br1 
ip link set br1 up
./bin/ovs-vsctl add-port br1 vx1 -- set interface vx1 type=vxlan options:remote_ip=192.168.100.1
./bin/ovs-vsctl add-port br1 vx1 -- set interface vx1 type=vxlan \
    options:remote_ip=192.168.100.1 options:key=flow options:dst_port=8472

./bin/ovs-vsctl del-port br1 myport0
./bin/ovs-vsctl del-port br1 myport1
./bin/ovs-vsctl add-port br1 myport0 -- set Interface myport0 type=dpdkvhostuser
./bin/ovs-vsctl add-port br1 myport1 -- set Interface myport1 type=dpdkvhostuser

qemu-system-x86_64 -m 1024 -cpu host -hda centos7_no1.img -boot c -enable-kvm -no-reboot -nographic -net none \
-chardev socket,id=char0,path=./var/run/openvswitch/myport0 \
-netdev type=vhost-user,id=mynet0,chardev=char0,vhostforce \
-device virtio-net-pci,mac=00:00:00:00:00:01,netdev=mynet0 \
-object memory-backend-file,id=mem,size=1024M,mem-path=/dev/hugepages,share=on \
-numa node,memdev=mem -mem-prealloc

qemu-system-x86_64 -m 1024 -cpu host -hda centos7_no2.img -boot c -enable-kvm -no-reboot -nographic -net none \
-chardev socket,id=char1,path=./var/run/openvswitch/myport1 \
-netdev type=vhost-user,id=mynet1,chardev=char1,vhostforce \
-device virtio-net-pci,mac=00:00:00:00:00:02,netdev=mynet1 \
-object memory-backend-file,id=mem,size=1024M,mem-path=/dev/hugepages,share=on \
-numa node,memdev=mem -mem-prealloc


