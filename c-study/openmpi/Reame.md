apt install libopenmpi-dev openmpi-bin openmpi-common
Signal: Segmentation fault， Signal code: Address not mapped

apt install libmpich-dev openmpi-bin openmpi-common

mpicc -o cpi cpi.c
mpirun -n 4 --hostfile ./hostfile cpi

hostfile cfg format:

	192.168.3.5 slots=2
	192.168.3.6 slots=2
	192.168.3.7 slots=2

mpirun --allow-run-as-root -n 6 --hostfile ./hostfile /home/mpi/cpi
Process 0 of 6 is on lance-vm5
Process 1 of 6 is on lance-vm5
Process 2 of 6 is on lance-vm6
Process 4 of 6 is on lance-vm7
Process 3 of 6 is on lance-vm6
Process 5 of 6 is on lance-vm7
pi is approximately 3.1415926544231239, Error is 0.0000000008333307
wall clock time = 0.010764

mpirun --allow-run-as-root -n 6 -x UCX_NET_DEVICES=rxe_0 --hostfile ./hostfile /home/mpi/test_mpi
not use roce, why?
but ib_send_bw -d rxe_0 192.168.3.5 uses rocev2.
What is the difference?

# issue1

   [lance-vm5:01354] Process received signal
   [lance-vm5:01354] Signal: Segmentation fault (11)
   [lance-vm5:01354] Signal code: Address not mapped (1)
   [lance-vm5:01354] Failing at address: 0x440000e8
   [lance-vm5:01354] [ 0] /lib/x86_64-linux-gnu/libc.so.6(+0x43090)[0x7f6ffe720090]
   [lance-vm5:01354] [ 1] /lib/x86_64-linux-gnu/libmpi.so.40(MPI_Comm_size+0x3b)[0x7f6ffe93b32b]
   [lance-vm5:01354] [ 2] ./test_mpi(+0x1351)[0x55dbdf586351]
   [lance-vm5:01354] [ 3] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3)[0x7f6ffe701083]
   [lance-vm5:01354] [ 4] ./test_mpi(+0x11ee)[0x55dbdf5861ee]
   [lance-vm5:01354] *** End of error message ***
   Segmentation fault (core dumped)
use 2nd mpi.h
/usr/include/x86_64-linux-gnu/mpich/mpi.h
/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h

