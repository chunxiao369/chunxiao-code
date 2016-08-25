
qemu-img create -f qcow disk.img 128M
qemu -m 128 -hda linux-0.2.img
vncviewer 127.0.0.1:5900

---install---
kvm -m 1024 -net nic,macaddr=00:11:11:EE:EE:EE -net tap,script=/etc/openvswitch/ovs-ifup,downscript=/etc/openvswitch/ovs-ifdown -drive file=centos.img -cdrom /home/chunxiao/Downloads/CentOS-7-x86_64-Minimal-1503.iso -boot d
----boot---
kvm -m 1024 -net nic,macaddr=00:11:11:EE:EE:EE -net tap,script=/etc/openvswitch/ovs-ifup,downscript=/etc/openvswitch/ovs-ifdown -drive file=centos.img -boot c

kvm -m 512 -net nic,macaddr=00:11:22:33:44:55 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown /home/lance/Project/vm_img/centos.img -drive file=/home/lance/Project/vm_img/disk_xx.img

sudo kvm -m 1024 -net nic,macaddr=00:11:22:33:44:56 -net tap,script=./etc/ovs-ifup,downscript=./etc/ovs-ifdown /home/lance/Project/vm_img/win7-64.img

docker run -d -p 5000:5000 --restart=always --name registry -v /mnt/disk_10G/:/var/lib/registry registry:2
