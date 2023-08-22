qemu-system-x86_64 --enable-kvm -smp 2,sockets=1,cores=2,threads=1 -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:01,netdev=net1  -netdev tap,id=net1,ifname=mgmt1,script=./ovs-ifup,downscript=./ovs-ifdown -drive file=/home/lance/Project/vm_img/centos_new1.img -nographic

qemu-system-x86_64 --enable-kvm -smp 2,sockets=1,cores=2,threads=1 -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:02,netdev=net1  -netdev tap,id=net1,ifname=mgmt2,script=./ovs-ifup,downscript=./ovs-ifdown -drive file=/home/lance/Project/vm_img/centos_new2.img -nographic

qemu-system-x86_64 --enable-kvm -smp 2,sockets=1,cores=2,threads=1 -m 2048 -device virtio-net-pci,mac=00:00:00:00:00:03,netdev=net1  -netdev tap,id=net1,ifname=mgmt3,script=./ovs-ifup,downscript=./ovs-ifdown -device virtio-net-pci,mac=00:00:00:00:00:04,netdev=net2  -netdev tap,id=net2,ifname=mirror,script=./ovs-ifup,downscript=./ovs-ifdown -drive file=/home/lance/Project/vm_img/centos_new3.img -nographic

ovs-vsctl list mirror
ovs-vsctl -- set Bridge br0 mirrors=@m -- --id=@mgmt1 get Port mgmt1 -- --id=@mirr1 get Port mirror -- --id=@m create Mirror name=mirror_mgmt1 select-dst-port=@mgmt1 select-src-port=@mgmt1 output-port=@mirr1

ovs-vsctl -- set Bridge br0 mirrors=@m -- --id=@mgmt1 get Port mgmt1 -- --id=@mgmt2 get Port mgmt2 -- --id=@mirr1 get Port mirror -- --id=@m create Mirror name=mirror_mgmt select-dst-port=@mgmt1,@mgmt2 select-src-port=@mgmt1,@mgmt2  output-port=@mirr1

ovs-vsctl (Open vSwitch) 2.13.8
ubuntu 20.04

systemctl restart ovs-vswitchd
ovs-ofctl dump-flows br0
 # cookie=0x0, duration=161.979s, table=0, n_packets=118, n_bytes=13736, priority=0 actions=NORMAL
ovs-ofctl del-flows br0

# 没有明白tp_src的意思
ovs-ofctl add-flow br0 tp_src=22,actions=output:1
ovs-ofctl del-flows br0 tp_src=22

ovs-ofctl add-flow br0 tcp,tcp_src=22/0xfff0,actions=drop
ovs-ofctl del-flows br0 tcp,tcp_src=22/0xfff0

ovs-ofctl show br0
ovs-appctl fdb/show br0

in_port: switch的入端口
dl_src: 源mac
dl_dst:目的mac
dl_type:以太网协议类型 
nw_src:源IP
nw_dst:目的ip
nw_proto:协议类型
tp_src: tcp udp源端口
tp_dst: tcp udp目的端口

action: drop or outport
