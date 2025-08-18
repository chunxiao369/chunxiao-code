SM is stream multiprocess
SP is stream process, is cuda core
cuda helloworld
Hello, world from GPU!
Hello, world from GPU!
Hello, world from CPU!
Device 0: NVIDIA A10, has SM Count: 72
Device 1: NVIDIA A10, has SM Count: 72

    /*
    The function cudaEventSynchronize() blocks CPU execution
    until the specified event is recorded.
    The cudaEventElapsedTime() function
    returns in the first argument the number of milliseconds time elapsed
    between the recording of start and stop.
    This value has a resolution of approximately one half microsecond.
    */

| 关键字       | 编译目标                                        | 可调用位置               | 用途        |
| ------------ | ----------------------------------------------- | -------------------------| ----------- |
| `__host__`   | 仅在 **CPU** 端生成代码                         | 只能由 **CPU** 调用      | 普通 C/C++ 函数 |
| `__device__` | 仅在 **GPU** 端生成代码                         | 只能由 **GPU** 调用（kernel 或其他 device 函数）  | GPU 内部辅助函数  |
| `__global__` | 在 **GPU** 端生成 **kernel**，同时在 **CPU** 端生成启动 stub | 只能由 **CPU** 用 `<<<…>>>` 语法调用 | 入口 kernel   |
| 无关键字     | **默认** = `__host__`                             | 只能由 **CPU** 调用                             | 普通主机函数      |

但实际上 GPU 并不能直接理解 <<< >>>，必须把它拆成：把参数拷到 GPU 常量区, 设置 grid/block 维度, 调用驱动 API cudaLaunchKernel(...)
编译器将这些繁琐步骤被封装在 stub 中，stub 的名字通常是
_Z10myKernelPfS_i(...params...)
在 CPU 端看到的 <<< >>> 最终就是一次普通的 C 函数调用，目标函数就是这个stub。stub 内部再完成真正的 GPU 启动。
桩（Stub / Method Stub ）是指用来替换一部分功能的程序段。 桩程序可以用来模拟已有程序的行为（比如一个远端机器的过程）或是对将要开发的代码的一种临时替代。
