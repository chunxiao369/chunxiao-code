
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

---

以下是cache不对齐的情况
root@lance-T470s:~/Summary/chunxiao-code/c-study/cache-miss# perf stat -d -e cache-misses ./xuexe 
system has 4 processor(s), use 0xf
thread coreid: 0.
thread coreid: 1.
thread coreid: 2.
thread coreid: 3.

 Performance counter stats for './xuexe':

         4,815,913      cache-misses                                                  (80.02%)
    30,515,656,377      L1-dcache-loads                                               (80.00%)
     1,872,452,088      L1-dcache-load-misses     #    6.14% of all L1-dcache hits    (79.98%)
         2,888,089      LLC-loads                                                     (80.01%)
           251,869      LLC-load-misses           #    8.72% of all LL-cache hits     (79.99%)

      53.847276877 seconds time elapsed

以下是cache对齐的情况
root@lance-T470s:~/Summary/chunxiao-code/c-study/cache-miss# perf stat -d -e cache-misses ./xuexe
system has 4 processor(s), use 0xf
thread coreid: 1.
thread coreid: 0.
thread coreid: 3.
thread coreid: 2.

 Performance counter stats for './xuexe':

         2,587,650      cache-misses                                                  (80.00%)
    30,445,585,023      L1-dcache-loads                                               (80.02%)
         4,221,466      L1-dcache-load-misses     #    0.01% of all L1-dcache hits    (79.99%)
           987,122      LLC-loads                                                     (79.98%)
           124,593      LLC-load-misses           #   12.62% of all LL-cache hits     (80.01%)

      16.048510069 seconds time elapsed


