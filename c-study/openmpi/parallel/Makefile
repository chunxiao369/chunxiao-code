CC = gcc
MPICC = mpicc
CFLAGS = -Wall -O0 -g -I/usr/lib/x86_64-linux-gnu/openmpi/include/
LDFLAGS = -lmpi
TARGET = test_mpi
OBJS = para01.o
COMPILE  = $(MPICC) $(CFLAGS) -MD -c -o $@ $<
#LINK = $(CC) $< $(LDFLAGS) -o $@
LINK2 = $(MPICC) $< -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK2)

run:
	mpirun -np 4 ./$(TARGET)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)


