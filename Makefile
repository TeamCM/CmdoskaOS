BOOT_S = ./bootloader/boot.s
BOOT_O = ./bootloader/boot.o

KERNEL_C = ./kernel/kernel.c
KERNEL_O = ./kernel/kernel.o

OS_BIN = Cmdoska.bin

OUTPUT = Cmdoska.iso

all: compile link build_iso clean

compile:
	as --32 $(BOOT_S) -o $(BOOT_O)
	gcc -m32 -c $(KERNEL_C) -o $(KERNEL_O) -std=gnu99 -ffreestanding -O2 -Wall -Wextra

link:
	ld -m elf_i386 -T linker.ld $(KERNEL_O) $(BOOT_O) -o $(OS_BIN) -nostdlib

build_iso:
	cp ./Cmdoska.bin isodir/boot
	mv $(OS_BIN) isodir/boot
	grub-mkrescue -o $(OUTPUT) isodir

clean:
	rm -rf $(BOOT_O)
	rm -rf $(KERNEL_O)
	#rm -rf $(OS_BIN)