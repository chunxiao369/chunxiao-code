good summary website
http://vinllen.com/
http://vinllen.com/ovs-2-3-datapatchnei-he-liu-biao-pi-pei-guo-cheng/
https://www.redhat.com/en/blog/amazing-new-observability-features-open-vswitch

Well, this is something quite important in OVS as one of the main causes of drops and (lack of) scalability. OVS has a main caching layer called EMC where the action for packets a cached (the flows). If a flow for a specific packet is missing, the packet is sent to the ovs-vswitchd daemon in the slow path (figure 1) and then typically the datapath is populated with the related flow. Now the action of sending a packet is called upcall. Why is it important? Because the packet processing time increases dramatically. Here is where SMC comes into play, caching up to 1M flows (vs. 8k of EMC).

dpif DataPath InterFace
upcall和reinject流程，
收到新的流的第一个包，会有流表查询不到的问题，所以upcall到用户态；
在用户态vswitchd查询，根据查询结果(检查数据库，看目的端口在哪)更新到内核态流表；
数据包reinject到内核态，查询流表即命中。根据行为转发到某口或者drop。

查询流表查不到会查询EMC精确匹配，后面再查dpcls，再查ofproto openflow分类器，这是最慢
EMC是以PMD为边界的，每个PMD拥有自己的EMC；
dpcls是以端口为边界的，每个端口拥有自己的dpcls；
ofproto classifier是以桥为边界的，每个桥拥有自己的ofproto classifier

openvswitch-2.11.0 src code
Exact match cache(EMC) Signature match cache (SMC) SMC使用相同的存储空间存储效率提高
ovs-vsctl --no-wait set Open_vSwitch . other_config:smc-enable=true
EMC cache and SMC cache compose the datapath flow cache (DFC)

        pmd_thread_main
    dp_netdev_process_rxq_port
1 dp_packet_batch_init(&batch); 2 netdev_rxq_recv; 3 dp_netdev_input(process pkts)->dp_netdev_input__;
2.1 rx->netdev->netdev_class->rxq_recv(rx, batch, qfill);
3.1 ./lib/dpif-netdev.c dfc_processing() 3.2 fast_path_processing

