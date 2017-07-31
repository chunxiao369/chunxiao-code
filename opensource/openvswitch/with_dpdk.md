
http://docs.openvswitch.org/en/latest/intro/install/dpdk/

export DPDK_DIR=/home/cxxu/dpdk-stable-16.11.2/
export DPDK_TARGET=x86_64-native-linuxapp-gcc
export DPDK_BUILD=$DPDK_DIR/$DPDK_TARGET
make install T=$DPDK_TARGET DESTDIR=install

cd /home/cxxu/openvswitch-2.7.1/
./configure --with-dpdk=$DPDK_BUILD --prefix=/home/cxxu/openvswitch-2.7.1/install
make
make install

echo 'vm.nr_hugepages=2048' > /etc/sysctl.d/hugepages.conf
or sysctl -w vm.nr_hugepages=2048

export PATH=$PATH:/home/cxxu/openvswitch-2.7.1/install/share/openvswitch/scripts
export DB_SOCK=/home/cxxu/openvswitch-2.7.1/install/var/run/openvswitch/db.sock

cd /home/cxxu/dpdk-stable-16.11.2/x86_64-native-linuxapp-gcc/lib
ldconfig

mkdir -p /home/cxxu/openvswitch-2.7.1/install/etc/openvswitch
mkdir -p /home/cxxu/openvswitch-2.7.1/install/var/run/openvswitch
rm -f /home/cxxu/openvswitch-2.7.1/install/etc/openvswitch/conf.db
/home/cxxu/openvswitch-2.7.1/install/bin/ovsdb-tool create /home/cxxu/openvswitch-2.7.1/install/etc/openvswitch/conf.db  \
       /home/cxxu/openvswitch-2.7.1/install/share/openvswitch/vswitch.ovsschema
 
/home/cxxu/openvswitch-2.7.1/install/sbin/ovsdb-server --remote=punix:/home/cxxu/openvswitch-2.7.1/install/var/run/openvswitch/db.sock \
--remote=db:Open_vSwitch,Open_vSwitch,manager_options \
--pidfile --detach 

--private-key=db:Open_vSwitch,SSL,private_key \
--certificate=Open_vSwitch,SSL,certificate \
--bootstrap-ca-cert=db:Open_vSwitch,SSL,ca_cert --pidfile --detach 
 
/home/cxxu/openvswitch-2.7.1/install/bin/ovs-vsctl --no-wait init
/home/cxxu/openvswitch-2.7.1/install/bin/ovs-vsctl --no-wait set Open_vSwitch . other_config:dpdk-init=true
/home/cxxu/openvswitch-2.7.1/install/share/openvswitch/scripts/ovs-ctl --no-ovsdb-server --db-sock="$DB_SOCK" start

modprobe vfio-pci
$DPDK_DIR/tools/dpdk-devbind.py --bind=vfio-pci 0000:02:00.0
$DPDK_DIR/tools/dpdk-devbind.py --bind=vfio-pci 0000:02:00.1
$DPDK_DIR/tools/dpdk-devbind.py --status
 
./bin/ovs-vsctl add-br br0 -- set bridge br0 datapath_type=netdev

./bin/ovs-vsctl del-port br0 myport0
./bin/ovs-vsctl del-port br0 myport1
./bin/ovs-vsctl add-port br0 myport0 -- set Interface myport0 type=dpdk options:dpdk-devargs=0000:02:00.0
./bin/ovs-vsctl add-port br0 myport1 -- set Interface myport1 type=dpdk options:dpdk-devargs=0000:02:00.1

./bin/ovs-vsctl set-controller br0 tcp:192.168.6.253:6653
./bin/ovs-vsctl del-controller br0

./bin/ovs-vsctl del-port br0 myport0
./bin/ovs-vsctl del-port br0 myport1
./bin/ovs-vsctl add-port br0 myport0 -- set Interface myport0 type=dpdkvhostuser
./bin/ovs-vsctl add-port br0 myport1 -- set Interface myport1 type=dpdkvhostuser
./bin/ovs-vsctl add-port br0 myport0 -- set Interface myport0 type=dpdkvhostuser options:dpdk-devargs=0000:02:00.0
./bin/ovs-vsctl add-port br0 myport1 -- set Interface myport1 type=dpdkvhostuser options:dpdk-devargs=0000:02:00.1

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

qemu-system-x86_64 --enable-kvm -m 1024 -net nic,macaddr=00:00:00:00:00:01 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown centos7_no1.img -spice port=5906,disable-ticketing

qemu-system-x86_64 --enable-kvm -m 1024 -net nic,model=i82559er,macaddr=00:00:00:00:00:01 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown centos7_no1.img -nographic
qemu-system-x86_64 --enable-kvm -m 1024 -net nic,model=i82559er,macaddr=00:00:00:00:00:02 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown centos7_no2.img -nographic

