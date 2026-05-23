CC=gcc
LD=ld

CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T kernel/linker.ld

BUILD=build
ISO=$(BUILD)/iso

all: $(BUILD)/kernel.bin iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.bin: $(BUILD)/kernel.o
	$(LD) $(LDFLAGS) $< -o $@

iso: $(BUILD)/kernel.bin
	mkdir -p $(ISO)/boot/grub

	cp $(BUILD)/kernel.bin \
	$(ISO)/boot/kernel.bin

	cp boot/grub.cfg \
	$(ISO)/boot/grub/grub.cfg

	grub-mkrescue \
	-o $(BUILD)/ak1.iso \
	$(ISO)

run: all
	qemu-system-i386 \
	-cdrom $(BUILD)/ak1.iso

clean:
	rm -rf build

.PHONY: all run clean iso