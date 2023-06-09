
# lvm 

    pvcreate /dev/nvme0n1 /dev/nvme1n1 /dev/nvme2n1
    vgcreate test /dev/nvme0n1 /dev/nvme1n1 /dev/nvme2n1
    lvcreate -l 100%VG --stripes 3 --stripesize=16384 --name bench test
    mkfs.ext4  /dev/mapper/test-bench
    mount /dev/mapper/test-bench /mnt/data3/ -o noatime,nodiratime,nobarrier,data=ordered

    umount /mnt/data3 
    vgremove test

# fio

    fio -name=test -group_reporting -ioengine=libaio -nrfiles=1 -threads -numjobs=1 -direct=1 -iodepth=16 -rw=randread -bs=1M  -filename=/mnt/data3/nvme_000.data -runtime=30
    taskset -c 32-48 fio -name=test -group_reporting -ioengine=libaio -nrfiles=1 -threads -numjobs=16 -direct=1 -iodepth=128 -rw=read -bs=1M  -filename=/mnt/data3/nvme_000.data -runtime=30
    -rw=read顺序读
    -ioengine=gfapi_async
    fio -name=test -filename=/mnt/data3/nvme_001.data  -group_reporting -iodepth=16 -ioengine=libaio -nrfiles=1 -rw=randwrite -bs=2M -size=64G -threads -numjobs=1 -direct=1
    taskset -c 32-48 fio -name=test -filename=/mnt/data3/nvme_002.data  -group_reporting -iodepth=128 -ioengine=libaio -nrfiles=1 -rw=write -bs=2M -size=128G -threads -numjobs=16 -direct=1

# dd

    dd if=/dev/zero of=/mnt/data3/nvme_000.data bs=1M count=65536
    记录了65536+0 的读入
    记录了65536+0 的写出
    68719476736 bytes (69 GB, 64 GiB) copied, 138.329 s, 497 MB/s

# env

MZQL23T8HCLS-00B7C
PM9A3 3.84T M.2 22110 PCIE 4.0 x4 NVMe
lsblk -o KNAME,TYPE,SIZE,MODEL

dd if=/dev/zero of=/mnt/data3/nvme_000.data bs=1M count=65536
记录了65536+0 的读入
记录了65536+0 的写出
68719476736 bytes (69 GB, 64 GiB) copied, 137.23 s, 501 MB/s

# log

taskset -c 32-48 fio -name=test -group_reporting -ioengine=libaio -nrfiles=1 -threads -numjobs=16 -direct=1 -iodepth=128 -rw=read -bs=1M  -filename=/mnt/data3/nvme_001.data -runtime=60
test: (g=0): rw=read, bs=(R) 1024KiB-1024KiB, (W) 1024KiB-1024KiB, (T) 1024KiB-1024KiB, ioengine=libaio, iodepth=128
fio-3.12
Starting 1 process
Jobs: 1 (f=1): [R(1)][100.0%][r=12.6GiB/s][r=12.9k IOPS][eta 00m:00s]
test: (groupid=0, jobs=1): err= 0: pid=51529: Fri Jun  9 13:10:31 2023
  read: IOPS=12.9k, BW=12.6GiB/s (13.5GB/s)(128GiB/10191msec)
    slat (usec): min=48, max=1007, avg=70.72, stdev=21.43
    clat (usec): min=1051, max=23801, avg=9862.98, stdev=467.86
     lat (usec): min=1136, max=24354, avg=9934.88, stdev=469.54
    clat percentiles (usec):
     |  1.00th=[ 9110],  5.00th=[ 9241], 10.00th=[ 9372], 20.00th=[ 9503],
     | 30.00th=[ 9634], 40.00th=[ 9765], 50.00th=[ 9896], 60.00th=[ 9896],
     | 70.00th=[10028], 80.00th=[10159], 90.00th=[10421], 95.00th=[10552],
     | 99.00th=[10814], 99.50th=[10814], 99.90th=[11207], 99.95th=[15664],
     | 99.99th=[21103]
   bw (  MiB/s): min=12276, max=12914, per=99.92%, avg=12851.50, stdev=136.75, samples=20
   iops        : min=12276, max=12914, avg=12851.60, stdev=136.76, samples=20
  lat (msec)   : 2=0.01%, 4=0.02%, 10=63.68%, 20=36.27%, 50=0.02%
  cpu          : usr=7.26%, sys=92.74%, ctx=19, majf=0, minf=1050
  IO depths    : 1=0.1%, 2=0.1%, 4=0.1%, 8=0.1%, 16=0.1%, 32=0.1%, >=64=100.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.1%
     issued rwts: total=131072,0,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=128

Run status group 0 (all jobs):
   READ: bw=12.6GiB/s (13.5GB/s), 12.6GiB/s-12.6GiB/s (13.5GB/s-13.5GB/s), io=128GiB (137GB), run=10191-10191msec

Disk stats (read/write):
    dm-0: ios=129940/218, merge=0/0, ticks=146210/0, in_queue=146310, util=99.02%, aggrios=65536/111, aggrmerge=0/0, aggrticks=70327/31, aggrin_queue=73750, aggrutil=97.52%
  nvme2n1: ios=65536/108, merge=0/0, ticks=59149/31, in_queue=62520, util=97.41%
  nvme1n1: ios=65536/115, merge=0/0, ticks=81505/31, in_queue=84980, util=97.52%

