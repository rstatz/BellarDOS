OS = StatzOS
mpoint = fatgrub
arch ?= x86_64
kernel = $(OS)/boot/kernel.bin

linker_script = src/arch/x86_64/linker.ld
grub_cfg = src/arch/x86_64/grub.cfg
asm_obj_files = multiboot_header.o boot.o long_mode_init.o

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
	rm -r $(OS)

run: $(OS).img
	qemu-system-x86_64 -s -drive format=raw,file=$(OS).img -serial stdio

$(kernel): $(asm_obj_files) $(linker_script)
	mkdir -p $(OS)/boot/grub
	cp $(grub_cfg) $(OS)/boot/grub/grub.cfg
	ld -n -T $(linker_script) -o $(kernel) $(asm_obj_files)

%.o: src/arch/$(arch)/%.asm
	nasm -f elf64 $< -o $@

.PHONY: clean

clean:
	rm *.img
	rm *.o
