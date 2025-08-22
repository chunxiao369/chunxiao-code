root@lance-vm5:/home/mpi/allreduce# time ./reduce_stddev 1000000000
element number: 1000000000.
Mean - 0.016777, Standard deviation = 0.129527

real	1m16.194s
user	0m19.638s
sys	0m33.540s
root@lance-vm5:/home/mpi/allreduce# time ./reduce_stddev 1000000000
element number: 1000000000.
Mean - 0.016777, Standard deviation = 0.129527

real	1m17.724s
user	0m19.628s
sys	0m33.362s

nvcc mini_nccl_thread.c -o mini_nccl_thread -lnccl -lcudart -lpthread

配置了这么环境变量挂起的问题并没有解决。
declare -x NCCL_DEBUG="INFO"
declare -x NCCL_IB_DISABLE="1"
declare -x NCCL_P2P_DISABLE="1"
declare -x NCCL_SHM_DISABLE="1"
declare -x NCCL_SOCKET_IFNAME="lo"

去拔了两个CX7的400G光模块，挂起的问题解决了。
A800:6801:6801 [0] NCCL INFO cudaDriverVersion 12060
A800:6801:6801 [0] NCCL INFO Bootstrap: Using ens85f0:10.17.8.207<0>
A800:6801:6801 [0] NCCL INFO NCCL version 2.27.3+cuda12.9
A800:6801:6839 [1] NCCL INFO NET/Plugin: Plugin name set by env to libnccl-net.so
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclNetPlugin_v10 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclNetPlugin_v9 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclNetPlugin_v8 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclNetPlugin_v7 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Loaded net plugin NCCL RDMA Plugin v6 (v6)
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclCollNetPlugin_v10 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclCollNetPlugin_v9 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclCollNetPlugin_v8 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclCollNetPlugin_v7 symbol.
A800:6801:6839 [1] NCCL INFO NET/Plugin: Failed to find ncclCollNetPlugin_v6 symbol.
A800:6801:6839 [1] NCCL INFO Successfully loaded external plugin libnccl-net.so
A800:6801:6839 [1] NCCL INFO Plugin Path : /usr/local/lib/libnccl-net.so
A800:6801:6839 [1] NCCL INFO Plugin version : 1.3@Tencent Cloud
A800:6801:6839 [1] NCCL INFO P2P plugin IBext
A800:6801:6839 [1] NCCL INFO NET/IB : No device found.
A800:6801:6839 [1] NCCL INFO NET/IB : No device found.
A800:6801:6839 [1] NCCL INFO NET/IB : Using [RO]; OOB ens85f0:10.17.8.207<0>
A800:6801:6839 [1] NCCL INFO NET/Socket : Using [0]ens85f0:10.17.8.207<0> [1]br-e0df4395d684:172.19.0.1<0> [2]br-9ba9ebd32537:172.20.0.1<0> [3]br-83633be27e6c:172.21.0.1<0> [4]br-ead5afbaeee3:172.18.0.1<0> [5]veth37669ca:fe80::9c8a:8aff:fe87:5487%veth37669ca<0> [6]vethd21d5c6:fe80::b081:20ff:fe9e:1f39%vethd21d5c6<0> [7]veth1ff3b86:fe80::bc4a:f2ff:fea9:1313%veth1ff3b86<0> [8]vetheb5eac5:fe80::c495:86ff:fe11:4e49%vetheb5eac5<0> [9]veth7db6b06:fe80::8dd:f1ff:fed6:944c%veth7db6b06<0> [10]veth1159c57:fe80::c4f8:9fff:fe7e:e572%veth1159c57<0> [11]veth2727cff:fe80::f865:17ff:fe8a:9814%veth2727cff<0> [12]veth3f843de:fe80::d4fb:54ff:fe7d:8829%veth3f843de<0> [13]vetha11d774:fe80::84f5:3ff:fe44:ab9b%vetha11d774<0> [14]veth9a45e04:fe80::681d:7aff:feb1:72a0%veth9a45e04<0> [15]veth10da4e9:fe80::1497:f6ff:fe16:a554%veth10da4e9<0>
A800:6801:6839 [1] NCCL INFO Initialized NET plugin Socket
A800:6801:6839 [1] NCCL INFO Assigned NET plugin Socket to comm
A800:6801:6839 [1] NCCL INFO Using network Socket
A800:6801:6838 [0] NCCL INFO Assigned NET plugin Socket to comm
A800:6801:6838 [0] NCCL INFO Using network Socket
A800:6801:6839 [1] NCCL INFO ncclCommInitAll comm 0x55b99fe024c0 rank 1 nranks 2 cudaDev 1 nvmlDev 1 busId d6000 commId 0x2253366728668019 - Init START
A800:6801:6838 [0] NCCL INFO ncclCommInitAll comm 0x55b99f28bf90 rank 0 nranks 2 cudaDev 0 nvmlDev 0 busId 52000 commId 0x2253366728668019 - Init START

[2025-08-22 17:44:36] A800:6801:6839 [1] ras/client_support.cc:159 NCCL WARN Call to bind failed: Address already in use
A800:6801:6839 [1] NCCL INFO RAS failed to establish a client listening socket at localhost:28028
A800:6801:6839 [1] NCCL INFO Bootstrap timings total 0.001527 (create 0.000048, send 0.000203, recv 0.000469, ring 0.000062, delay 0.000000)
A800:6801:6838 [0] NCCL INFO Bootstrap timings total 0.001497 (create 0.000067, send 0.000291, recv 0.000532, ring 0.000061, delay 0.000000)
A800:6801:6839 [1] NCCL INFO Setting affinity for GPU 1 to 22-31,48-63
A800:6801:6838 [0] NCCL INFO Setting affinity for GPU 0 to 0-1,32-47
A800:6801:6839 [1] NCCL INFO comm 0x55b99fe024c0 rank 1 nRanks 2 nNodes 1 localRanks 2 localRank 1 MNNVL 0
A800:6801:6838 [0] NCCL INFO comm 0x55b99f28bf90 rank 0 nRanks 2 nNodes 1 localRanks 2 localRank 0 MNNVL 0
A800:6801:6839 [1] NCCL INFO Trees [0] -1/-1/-1->1->0 [1] -1/-1/-1->1->0
A800:6801:6839 [1] NCCL INFO P2P Chunksize set to 131072
A800:6801:6838 [0] NCCL INFO Channel 00/02 : 0 1
A800:6801:6838 [0] NCCL INFO Channel 01/02 : 0 1
A800:6801:6838 [0] NCCL INFO Trees [0] 1/-1/-1->0->-1 [1] 1/-1/-1->0->-1
A800:6801:6838 [0] NCCL INFO P2P Chunksize set to 131072
A800:6801:6839 [1] NCCL INFO PROFILER/Plugin: Could not find: libnccl-profiler.so.
A800:6801:6859 [1] NCCL INFO [Proxy Service] Device 1 CPU core 27
A800:6801:6838 [0] NCCL INFO PROFILER/Plugin: Could not find: libnccl-profiler.so.
A800:6801:6838 [0] NCCL INFO Check P2P Type isAllDirectP2p 0 directMode 1
A800:6801:6860 [1] NCCL INFO [Proxy Service UDS] Device 1 CPU core 24
A800:6801:6861 [0] NCCL INFO [Proxy Service] Device 0 CPU core 0
A800:6801:6862 [0] NCCL INFO [Proxy Service UDS] Device 0 CPU core 45
A800:6801:6839 [1] NCCL INFO threadThresholds 8/8/64 | 16/8/64 | 512 | 512
A800:6801:6839 [1] NCCL INFO 2 coll channels, 2 collnet channels, 0 nvls channels, 2 p2p channels, 2 p2p channels per peer
A800:6801:6838 [0] NCCL INFO threadThresholds 8/8/64 | 16/8/64 | 512 | 512
A800:6801:6838 [0] NCCL INFO 2 coll channels, 2 collnet channels, 0 nvls channels, 2 p2p channels, 2 p2p channels per peer
A800:6801:6838 [0] NCCL INFO CC Off, workFifoBytes 1048576
A800:6801:6839 [1] NCCL INFO TUNER/Plugin: Could not find: libnccl-tuner.so. Using internal tuner plugin.
A800:6801:6839 [1] NCCL INFO TUNER/Plugin: Failed to find ncclTunerPlugin_v4 symbol.
A800:6801:6839 [1] NCCL INFO TUNER/Plugin: Failed to find ncclTunerPlugin_v3 symbol.
A800:6801:6839 [1] NCCL INFO TUNER/Plugin: Failed to find ncclTunerPlugin_v2 symbol, using internal tuner instead.
A800:6801:6839 [1] NCCL INFO ncclCommInitAll comm 0x55b99fe024c0 rank 1 nranks 2 cudaDev 1 nvmlDev 1 busId d6000 commId 0x2253366728668019 - Init COMPLETE
A800:6801:6839 [1] NCCL INFO Init timings - ncclCommInitAll: rank 1 nranks 2 total 0.85 (kernels 0.75, alloc 0.05, bootstrap 0.00, allgathers 0.00, topo 0.03, graphs 0.00, connections 0.01, rest 0.01)
A800:6801:6838 [0] NCCL INFO ncclCommInitAll comm 0x55b99f28bf90 rank 0 nranks 2 cudaDev 0 nvmlDev 0 busId 52000 commId 0x2253366728668019 - Init COMPLETE
A800:6801:6838 [0] NCCL INFO Init timings - ncclCommInitAll: rank 0 nranks 2 total 0.85 (kernels 0.75, alloc 0.05, bootstrap 0.00, allgathers 0.00, topo 0.03, graphs 0.00, connections 0.01, rest 0.00)
[Rank 0] start ...
[Rank 1] start ...
[Rank 1] start ...
[Rank 1] start ...line: 58
[Rank 0] start ...
[Rank 0] start ...line: 58
A800:6801:6866 [1] NCCL INFO Channel 00 : 1[1] -> 0[0] via SHM/direct/direct
A800:6801:6866 [1] NCCL INFO Channel 01 : 1[1] -> 0[0] via SHM/direct/direct
A800:6801:6867 [0] NCCL INFO Channel 00 : 0[0] -> 1[1] via SHM/direct/direct
A800:6801:6867 [0] NCCL INFO Channel 01 : 0[0] -> 1[1] via SHM/direct/direct
A800:6801:6867 [0] NCCL INFO Connected all rings, use ring PXN 0 GDR 1
A800:6801:6866 [1] NCCL INFO Connected all rings, use ring PXN 0 GDR 1
[Rank 1] start ...line: 61
[Rank 0] start ...line: 61
[Rank 0] start ...line: 65
[Rank 0] Result = 3.000000
[Rank 1] start ...line: 65
[Rank 1] Result = 3.000000
A800:6801:6865 [0] NCCL INFO comm 0x55b99f28bf90 rank 0 nranks 2 cudaDev 0 busId 52000 - Destroy COMPLETE
A800:6801:6865 [1] NCCL INFO comm 0x55b99fe024c0 rank 1 nranks 2 cudaDev 1 busId d6000 - Destroy COMPLETE

