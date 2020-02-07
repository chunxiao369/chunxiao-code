mtrace/dmalloc/valgrind/sar

valgrind应该是较好一个工具,mtrace/dmalloc只能检查堆内存的泄露
valgrind能够检测出mmap及栈的内存泄露

# mmap
mmap是将一个文件映射到进程的虚拟内存地址，后以操作内存的方式操作文件。
mmap比read/write的效率高，
read将文件先拷贝到内核空间，再从内核空间拷贝到用户空间。经过两次拷贝。
mmap直接将文件内容拷贝到用户空间，经过一次拷贝。
mmap可申请的最低的地址由一个mmap_min_addr内核参数

    sysctl vm.mmap_min_addr

# sbrk
brk通过传递的addr来重新设置program break(end_brk)，成功则返回0，否则返回-1。
sbrk用来增加heap，增加的大小通过参数increment决定，返回增加大小前的heap的program break(end_brk)，如果increment为0则返回program break
  ## 想到bss段的地址在32位机器上每次运行是固定，64位机器有个设置，开启后固定。
  ## 看一个程序的换页次数

    ps -o majflt,minflt -C chrome

# ASLR的话就是Address space layout randomization

地址空间布局随机化是一种安全机制，主要防止缓冲区溢出攻击。
默认是2，以下命令关闭

    echo "0" > /proc/sys/kernel/randomize_va_space
以下命令查看一个进程的地址布局情况

    pmap -d 3251
    pmap -X 3251
    cat /proc/3251/maps
a.out 是运行程序的名字
.so 是使用的动态链接库
stack 使用的栈空间
anon 预分配的虚拟内存，还未有数据占用
