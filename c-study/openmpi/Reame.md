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
