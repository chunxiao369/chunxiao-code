
# time几个值的意思
real 0m26.137s 真正运行的时间
user 0m45.179s 运行在用户态的时间, 如果是多核/多线程同时工作, 那耗时就是多核CPU耗时相加
sys  0m5.362s  运行在内核态的时间, 如果是多核/多线程同时工作, 那耗时就是多核CPU耗时相加

# cpu情况

L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            4096K

# 设置程序的堆大小
ulimit -a
ulimit -s 268435456

# perf查看l1d cache miss

cache-miss# perf stat -e cache-misses ./xuexe

cache-miss# perf stat -e L1-dcache-load-misses ./xuexe
 Performance counter stats for './xuexe':
         1,418,564      L1-dcache-load-misses
       0.195073078 seconds time elapsed

总共有8192*8192次内存写入, 理论上64字节调入内存一次。8192*8192/64==1048576
root@lance-T470s:~/Summary/chunxiao-code/c-study/cache-miss# perf stat -e L1-dcache-load-misses ./xuexe 2
 Performance counter stats for './xuexe 2':
        84,155,381      L1-dcache-load-misses
       1.217093933 seconds time elapsed

cache-miss# perf record -e cache-misses ./xuexe 2
cache-miss# perf report

