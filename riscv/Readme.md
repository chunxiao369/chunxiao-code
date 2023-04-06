OpenSBI（Open Supervisor Binary Interface）
包括uboot
apt install qemu-system-misc opensbi u-boot-qemu qemu-utils

qemu-system-riscv64 -machine virt -nographic -m 2048 -smp 4 -bios /usr/lib/riscv64-linux-gnu/opensbi/generic/fw_jump.elf -kernel /usr/lib/u-boot/qemu-riscv64_smode/uboot.elf -device virtio-net-device,netdev=eth0 -netdev user,id=eth0 -drive file=/home/lance/Project/vm_img/ubuntu-22.10-preinstalled-server-riscv64+unmatched.img,format=raw,if=virtio

qemu-system-riscv64 -machine virt -nographic -m 2048 -smp 4 -bios /usr/lib/riscv64-linux-gnu/opensbi/generic/fw_jump.elf -kernel /usr/lib/u-boot/qemu-riscv64_smode/uboot.elf -device virtio-net-device,netdev=eth0 -netdev user,id=eth0,hostfwd=tcp::3333-:22 -drive file=/home/lance/Project/vm_img/ubuntu-20.04.2-preinstalled-server-riscv64.img,format=raw,if=virtio

qemu-system-riscv64 -machine virt -nographic -m 2048 -smp 4 -bios /usr/lib/riscv64-linux-gnu/opensbi/generic/fw_jump.elf -kernel /usr/lib/u-boot/qemu-riscv64_smode/uboot.elf -device virtio-net-device,netdev=eth0 -netdev user,id=eth0,hostfwd=tcp::3333-:22 -drive file=/home/lance/Project/vm_img/ubuntu-20.04.2-preinstalled-server-riscv64.img,format=raw,if=virtio

