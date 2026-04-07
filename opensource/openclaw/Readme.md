ssh 10.60.110.66
sudo qemu-system-x86_64 --enable-kvm -M q35 -cpu host,kvm=off -smp 4,sockets=1,cores=4,threads=1 -m 4096 -device virtio-net-pci,mac=00:00:00:00:05:01,netdev=net1  -netdev tap,id=net1,ifname=cxxu50,script=./ovs-ifup,downscript=./ovs-ifdown -device virtio-net-pci,mac=00:00:00:00:05:02,netdev=net2,mq=on,vectors=10  -netdev tap,id=net2,queues=4,ifname=cxxu60,script=./ifup,downscript=./ifdown  -drive file=/mnt/img_os/ubuntu24_32G_60.img -device intel-iommu,device-iotlb=on -daemonize -display none
cat /home/lance/.openclaw/openclaw.json
openclaw gateway --port 18789 --log-level debug

sudo ip route add 192.168.3.0/24 via 10.60.110.66 dev enp0s31f6
ssh -L 18789:localhost:18789 lance@192.168.3.60
http://127.0.0.1:18789/?token=98fcdfc5b08b538459112d25264fe296a5b03a1b95eb1de7
