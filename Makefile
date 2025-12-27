CROSS=aarch64-linux-gnu-

all: kernel_2712.img

boot.o: boot.S
	$(CROSS)gcc -ffreestanding -c boot.S -o boot.o

kernel.o: kernel.c mailbox.c mailbox.h
	$(CROSS)gcc -ffreestanding -c kernel.c -o kernel.o
	$(CROSS)gcc -ffreestanding -c mailbox.c -o mailbox.o

kernel_2712.elf: boot.o kernel.o mailbox.o linker.ld
	$(CROSS)ld -T linker.ld boot.o kernel.o mailbox.o -o kernel_2712.elf

kernel_2712.img: kernel_2712.elf
	$(CROSS)objcopy kernel_2712.elf -O binary kernel_2712.img

clean:
	rm -f *.o *.elf *.img
