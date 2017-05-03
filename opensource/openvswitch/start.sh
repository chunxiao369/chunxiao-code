#!/bin/bash
modprobe vxlan
modprobe gre
insmod /home/chunxiao/Project/open-source/openvswitch-2.3.1/datapath/linux/openvswitch.ko
sleep 1
./sbin/ovsdb-server --remote=punix:/home/chunxiao/Project/open-source/openvswitch-2.3.1/install/var/run/openvswitch/db.sock --remote=db:Open_vSwitch,Open_vSwitch,manager_options --private-key=db:Open_vSwitch,SSL,private_key --certificate=db:Open_vSwitch,SSL,certificate --bootstrap-ca-cert=db:Open_vSwitch,SSL,ca_cert --pidfile --detach
./bin/ovs-vsctl --no-wait init
sleep 1
./sbin/ovs-vswitchd --pidfile --detach

sleep 2
ifconfig eth0 0
ip addr add 10.6.1.56/16 dev br0
ip link set br0 up
ip route add default via 10.6.0.1 dev br0
