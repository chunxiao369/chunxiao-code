
qemu-img create -f qcow disk.img 128M
qemu -m 128 -hda linux-0.2.img
vncviewer 127.0.0.1:5900

---install---
kvm 1024 -net nic,macaddr=00:11:11:EE:EE:EE -net tap,script=/etc/openvswitch/ovs-ifup,downscript=/etc/openvswitch/ovs-ifdown -drive file=centos.img -cdrom /home/chunxiao/Downloads/CentOS-7-x86_64-Minimal-1503.iso -boot c
----boot---
kvm -m 1024 -net nic,macaddr=00:11:11:EE:EE:EE -net tap,script=/etc/openvswitch/ovs-ifup,downscript=/etc/openvswitch/ovs-ifdown -drive file=centos.img -boot d
