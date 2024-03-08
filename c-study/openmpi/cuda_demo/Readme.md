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
