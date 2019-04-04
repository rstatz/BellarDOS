OS = StatzOS
mpoint = fatgrub
arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%.o, $(assembly_source_files))

.PHONY: all clean run img

all: $(kernel)

clean:
	@rm -r build

run: $(OS).img
	@qemu-system-x86_64 -s -drive format=raw, file=$(OS).img -serial stdio

img:
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@dd if=/dev/zero of=$(OS).img bs=512 count=32768
	@parted $(OS).img mklabel msdos
	@parted $(OS).img mkpart primary fat32 2048s 30720s
	@parted $(OS).img set | boot on
	@sudo losetup /dev/loop0 $(OS).img
	@sudo losetup /dev/loop1 $(OS).img -o 1048576
	@sudo mkdosfs -F32 -f 2 /dev/loop1
	@sudo mount /dev/loop1 /mnt/$(mpoint)
	@sudo grub-install --root-directory=/mnt/$(mpoint) --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop0
	@cp -r .img/* /mnt/$(mpoint)
	@umount /mnt/$(mpoint)
	@losetup -d /dev/loop0
	@losetup -d /dev/loop1	

$(kernel): $(assembbly_object_files) $(linker_script)
	@ld -n -T $(linker_script) -o $(kernel) $(assembly_object_files)

build/arch/$(arch)/%.o:src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@
