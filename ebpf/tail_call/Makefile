BPF_CLANG ?= clang
BPF_CFLAGS ?= -O2 -g -Wall -Wextra
CFLAGS_CL =  -I/usr/include/x86_64-linux-gnu/

all: xdp_tailcall.bpf.o xdp_user

xdp_tailcall.bpf.o: xdp_tailcall.bpf.c
	$(BPF_CLANG) $(CFLAGS_CL) -O2 -g -target bpf -c xdp_tailcall.bpf.c -o xdp_tailcall.bpf.o

xdp_user: xdp_user.c
	gcc -O2 xdp_user.c -o xdp_user -lbpf -lelf -lz -pthread

clean:
	rm -f xdp_tailcall.bpf.o xdp_user

