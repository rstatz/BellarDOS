OS=StatzOS
mpoint=fatgrub
arch ?=x86_64
path=src/arch/$(arch)
kernel=$(OS)/boot/kernel.bin

CC=x86_64-elf-gcc
CCLINKER=x86_64-elf-ld

linker_script=$(path)/linker.ld
grub_cfg=$(path)/grub.cfg

C_FLAGS=-ffreestanding -mno-red-zone -Wall -Wextra -g -c

C_FILES=$(wildcard $(path)/*.c)
C_OBJ=kmain.o vga_cd.o ps2_cd.o strings.o math.o print.o
ASM_OBJ=multiboot_header.o boot.o long_mode_init.o

LIBS= -nostdlib -lgcc

$(OS): $(kernel) $(grub_cfg)
	dd if=/dev/zero of=$(OS).img bs=512 count=32768
	parted $(OS).img mklabel msdos
	parted $(OS).img mkpart primary fat32 2048s 30720s
	parted $(OS).img set 1 boot on
	sudo losetup /dev/loop9878 $(OS).img
	sudo losetup /dev/loop9879 $(OS).img -o 1048576
	sudo mkdosfs -F32 -f 2 /dev/loop9879
	sudo mount /dev/loop9879 /mnt/$(mpoint)
	sudo grub-install --root-directory=/mnt/$(mpoint) --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop9878
	sudo cp -r $(OS)/* /mnt/$(mpoint)
	sudo umount /mnt/$(mpoint)	
	sudo losetup -d /dev/loop9878
	sudo losetup -d /dev/loop9879
	rm *.o

run: $(OS).img
	qemu-system-x86_64 -s -drive format=raw,file=$(OS).img -serial stdio

$(kernel): $(ASM_OBJ) $(linker_script) $(C_FILES)
	mkdir -p $(OS)/boot/grub
	cp $(grub_cfg) $(OS)/boot/grub/grub.cfg
	$(CC) -o ps2_cd.o $(C_FLAGS) $(path)/ps2_cd.c $(LIBS)
	$(CC) -o math.o $(C_FLAGS) $(path)/math.c $(LIBS)
	$(CC) -o print.o $(C_FLAGS) $(path)/print.c $(LIBS)
	$(CC) -o strings.o $(C_FLAGS) $(path)/strings.c $(LIBS)
	$(CC) -o vga_cd.o $(C_FLAGS) $(path)/vga_cd.c $(LIBS)
	$(CC) -o kmain.o $(C_FLAGS) $(path)/kmain.c $(LIBS)
	$(CCLINKER) -n -T $(linker_script) -o $(kernel) $(ASM_OBJ) $(C_OBJ)

%.o: src/arch/$(arch)/%.asm
	nasm -f elf64 $< -o $@

.PHONY: clean

clean:
	rm *.img
	rm -r $(OS)

