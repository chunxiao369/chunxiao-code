good summary website

RYU is a component-based software defined networking framework.
Ryu provides software components with well defined API that make it easy for developers to create new network management and control applications. 

# ovs mirror

ovs-vsctl -- set Bridge br0 mirrors=@m \
-- --id=@tap0 get Port tap0 \
-- --id=@tap1 get Port tap1 \
-- --id=@m create Mirror name=mirror_test select-dst-port=@tap0 select-src-port=@tap0 output-port=@tap1

https://www.intel.com/content/www/us/en/developer/articles/technical/ovs-dpdk-datapath-classifier-part-2.html
http://vinllen.com/
http://vinllen.com/ovs-2-3-datapatchnei-he-liu-biao-pi-pei-guo-cheng/
https://www.redhat.com/en/blog/amazing-new-observability-features-open-vswitch
https://www.sdnlab.com/20901.html src code by V2.7.2
https://zhaozhanxu.com/archives/233/

Well, this is something quite important in OVS as one of the main causes of drops and (lack of) scalability. OVS has a main caching layer called EMC where the action for packets a cached (the flows). If a flow for a specific packet is missing, the packet is sent to the ovs-vswitchd daemon in the slow path (figure 1) and then typically the datapath is populated with the related flow. Now the action of sending a packet is called upcall. Why is it important? Because the packet processing time increases dramatically. Here is where SMC comes into play, caching up to 1M flows (vs. 8k of EMC).

此补丁在完全匹配缓存 (EMC) 之后添加了签名匹配缓存 (SMC)。 SMC 和 EMC 之间的区别在于 SMC 仅存储流的签名，因此它的内存效率更高。 在相同的内存空间下，EMC 可以存储 8k 流，而 SMC 可以存储 1M 流。 通常打开 SMC 是有益的，但在流量计数远大于 EMC 大小时关闭 EMC。
SMC 缓存会将签名映射到 flow_table 中的 dp_netdev_flow 索引。 因此，我们在 cmap 中添加了两个新的 API，用于按索引查找键和按键查找索引。

dpif DataPath InterFace
upcall和reinject流程，
收到新的流的第一个包，会有流表查询不到的问题，所以upcall到用户态；
在用户态vswitchd查询，根据查询结果(检查数据库，看目的端口在哪)更新到内核态流表；
数据包reinject到内核态，查询流表即命中。根据行为转发到某口或者drop。

三级流表匹配过程：1. 查询EMC精确匹配，2. 后面再查dpcls，3. 再查ofproto openflow分类器，这是最慢
1. EMC是以PMD为边界的，每个PMD拥有自己的EMC；
2. dpcls是以端口为边界的，每个端口拥有自己的dpcls； The Datapath classifier - aka dpcls - is composed of subtables.
子表以按不同的元组，不同掩码来区分的。例如sip=10.1.1.1 dip=2.2.2.2和sip=1.1.1.1是两个不同的子表；
sip=1.1.1.1/24与sip=1.1.1.1/16又是不同子表。
mask的链表用来计算出查子表时的hash值，有了不同元组以及mask，才算hash。
mask/value表又来匹配子表的内容。
3. ofproto classifier是以桥bridge为边界的，每个桥拥有自己的ofproto classifier

openvswitch-2.11.0 src code

struct dfc_cache {
    struct emc_cache emc_cache;
    struct smc_cache smc_cache;
};

Exact match cache(EMC) Signature match cache (SMC) SMC使用相同的存储空间存储效率提高
ovs-vsctl --no-wait set Open_vSwitch . other_config:smc-enable=true
EMC cache and SMC cache compose the datapath flow cache (DFC)
emc_processing -> dfc_processing

main() in ~/openvswitch-2.7.2/vswitchd/ovs-vswitchd.c
        pmd_thread_main in lib/dpif-netdev.c
    dp_netdev_process_rxq_port emc_cache_slow_sweep
1 dp_packet_batch_init(&batch); 2 netdev_rxq_recv; 3 dp_netdev_input(process pkts)->dp_netdev_input__;
2.1 rx->netdev->netdev_class->rxq_recv(rx, batch, qfill);
3.1 ./lib/dpif-netdev.c dfc_processing() 3.2 fast_path_processing
3.1.1 miniflow_extract A sparse representation of a "struct flow". 稀疏矩阵 3.1.2 emc_lookup 3.1.3 smc_lookup_batch
3.2.1 dpcls_lookup 3.2.2 emc_insert insert的逻辑好好看
