1. intel DLB
DLB_Driver_User_Guide.pdf
lspci -d :2710
#To check DLB2.0 device id (For ex: eb:00.0)
lspci -d :2714
#To check DLB2.5 device id (For ex: 14:00.0)

4th xeon cpu list
DSA(data streaming accelerator)
high-performance data copy and transform accelerator
https://ark.intel.com/content/www/us/en/ark/products/series/228622/4th-generation-intel-xeon-scalable-processors.html

2. keepalived
main and backup

3. performance efficiency core 1P-core = 4E-cores
P-core E-core
pc有混合架构
server暂时不做混合架构

4. VM live migration cross HOST
dirty page Trcking HW DPT 4th Gen XEON

5. P4 runtime API based on gRPC

6. google 
Intel IPU as backend of the latest gVNIC
200Gbps GVE google virtual Ethernet driver
Google 虚拟 NIC (gVNIC) 是专为 Compute Engine 设计的虚拟网络接口。gVNIC 是基于 virtIO 的以太网驱动程序的替代方案。
作为成功使用 VirtIO 的下一代网络接口，gVNIC 会将 VirtIO-Net 替换成 Compute Engine 中为所有新机器类型（第 3 代及更高版本）唯一支持的网络接口。较新的机器系列和网络功能需要使用 gVNIC 而不是 VirtIO。

7. hyperscan 
universal database 一次编译，各个平台性能适配
