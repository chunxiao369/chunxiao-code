https://blog.csdn.net/u013253075/article/details/119361574

这种方法可以实现pci设备直通，而之前用的是vfio-pci
1. unbind pci设备
  modprobe pci_stub
  lspci -D -nn查询pci设备(domain,slot,bus,function)和(vendor id, device id )
  以设备(01:00.0, vendor & device ID 8086:10b9)为例：
  echo "8086 10b9" > /sys/bus/pci/drivers/pci-stub/new_id
  echo 0000:01:00.0 > /sys/bus/pci/devices/0000:01:00.0/driver/unbind
  echo 0000:01:00.0 > /sys/bus/pci/drivers/pci-stub/bind
2. 启动虚拟机
  -device pci-assign,host=01:00.0

VFIO在kernel3.6/qemu1.4以后支持,眼下仅仅支持PCI设备。
VFIO是一套用户态驱动框架，提供两种基本服务：
向用户态提供设备訪问接口
向用户态提供配置IOMMU接口
VFIO能够用于实现高效的用户态驱动。
在虚拟化场景能够用于device passthrough。
通过用户态配置IOMMU接口，能够将DMA地址空间映射限制在进程虚拟空间中。
这对高性能驱动和虚拟化场景device passthrough尤其重要。
相对于传统方式，VFIO对UEFI支持更好。
VFIO技术实现了用户空间直接訪问设备。无须root特权，更安全
