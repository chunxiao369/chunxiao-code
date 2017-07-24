#!/bin/sh

###查看虚拟交换机的端口计数
    ./bin/ovs-vsctl list interface myport0
###查看虚拟交换机的端口状态, openflow控制器相关
    ./bin/ovs-ofctl show br0
###查看与虚拟交换机相连接的设备的MAC等
    ./bin/ovs-appctl fdb/show br0
#./bin/ovs-dpctl show -s

modprobe vxlan
modprobe gre
insmod /home/chunxiao/project/openvswitch-2.3.0/datapath/linux/openvswitch.ko
sleep 1;
#init
./bin/ovsdb-tool create /home/chunxiao/project/program_install/etc/openvswitch/conf.db ./share/openvswitch/vswitch.ovsschema
sleep 1;

./sbin/ovsdb-server \
--remote=punix:/home/chunxiao/project/program_install/var/run/openvswitch/db.sock \
--remote=db:Open_vSwitch,Open_vSwitch,manager_options \
--private-key=db:Open_vSwitch,SSL,private_key \
--certificate=db:Open_vSwitch,SSL,certificate \
--bootstrap-ca-cert=db:Open_vSwitch,SSL,ca_cert \
--pidfile --detach
sleep 1;

./bin/ovs-vsctl --no-wait init
sleep 1;
./sbin/ovs-vswitchd --pidfile --detach
sleep 1;

./bin/ovsdb-client dump > db.dump

example1:
make vm connect external
0. create br0;
./bin/ovs-vsctl add-br br0

1. br0 add eth0;
./bin/ovs-vsctl add-port br0 eth0

2. br0 add vport1;
ip tuntap add mod tap vport1
./bin/ovs-vsctl add-port br0 vport1

3. virtual box create vm, vm bridge to vport1;
4. ifconfig eth0 0;
5. set br0 ip addr;
6. set vm addr;

example2:
vlan tag set in openvswitch:
./bin/ovs-vsctl set port tap0 tag=80
./bin/ovs-vsctl list port tap0
./bin/ovs-vsctl set port tap1 tag=80

example3:
gre
how to understand network namespace?
