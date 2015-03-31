#!/bin/sh
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
