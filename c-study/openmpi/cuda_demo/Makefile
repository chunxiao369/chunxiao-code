CC = gcc
NVCC = nvcc
CFLAGS = -Wall -O0 -g -I/usr/lib/x86_64-linux-gnu/openmpi/include/
NVCUFLAGS  := -O0 -g 

CUDA_HOME ?= /usr/local/cuda
CUDA_LIB ?= $(CUDA_HOME)/lib64
LDFLAGS    := -L${CUDA_LIB} -lcudart -lrt
NVLDFLAGS  := -L${CUDA_LIB} -lrt

TARGET_H = cuda_hello
TARGET_M = cuda_mem
TARGET_T = cuda_thread
OBJS_HELLO = hello_world.o
OBJS_MEM = memory.o
OBJS_THREAD = thread.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@
NV_COMPILE = $(NVCC) $(NVCUFLAGS) -c -o $@ $<
NV_LINK = $(NVCC) -o $@ $^ ${NVLDFLAGS}

ALL:$(TARGET)

$(TARGET_H):$(OBJS_HELLO)
	$(NV_LINK)
$(TARGET_M):$(OBJS_MEM)
	$(NV_LINK)
$(TARGET_T):$(OBJS_THREAD)
	$(NV_LINK)

%.o:%.cu
	$(NV_COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET_H) $(TARGET_T) $(TARGET_M)


