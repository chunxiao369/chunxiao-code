# C
编译：make
运行：sudo ./hello
查看输出：在另一个终端sudo cat /sys/kernel/debug/tracing/trace_pipe（每次execve如ls时打印“Hello, World!”）。

llvm-objdump -S ip_log.bpf.o
ip link set dev enp0s31f6 xdp obj ip_log.bpf.o
ip link set dev ens25np0 xdp obj ip_log.bpf.o sec xdp
ip link set dev enp0s31f6 xdp off

XDP（eXpress Data Path）是Linux内核的一项高性能网络技术，它允许开发者使用eBPF（Extended Berkeley Packet Filter）程序在网络数据路径的早期阶段处理数据包，从而实现高效的包过滤、转发或修改，而无需经过完整的内核网络栈。   XDP钩子（hook）是XDP程序附加的关键点，位于网卡驱动的接收（RX）路径的最低层，即在数据包从NIC（网络接口卡）进入内核后、分配SKB（Socket Kernel Buffer）之前。 这使得XDP能够以极低的延迟处理包，常用于DDoS防护、负载均衡或自定义路由等场景。

kernel 4.8+即支持

XDP钩子集成在支持的NIC驱动中（如Intel i40e、Mellanox mlx5），数据包从硬件RX环进入驱动后立即触发XDP程序。 如果驱动不支持原生XDP，会回落到通用模式（skb-based），但性能较低。

XDP程序可以返回以下动作：

XDP_PASS：允许包继续进入内核栈。
XDP_DROP：丢弃包（用于过滤）。
XDP_TX：从同一接口发送包（用于反射）。
XDP_REDIRECT：重定向到其他接口或CPU。
XDP_ABORTED：错误处理。

优势：相比传统iptables或nftables，XDP在更早阶段运行，能处理数百万PPS（Packets Per Second），减少CPU开销，并支持可编程性（通过eBPF代码）
