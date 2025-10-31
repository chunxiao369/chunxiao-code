# python
sudo python3 execve_trace.py
在另一个终端执行命令（如ls或echo hello），观察输出：
会看到类似... -1234: Hello World!的内核消息（通过/sys/kernel/debug/tracing/trace_pipe读取）。

# C
编译：make
运行：sudo ./hello
查看输出：在另一个终端sudo cat /sys/kernel/debug/tracing/trace_pipe（每次execve如ls时打印“Hello, World!”）。
