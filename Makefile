CC=gcc
LD=ld

CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T kernel/linker.ld

BUILD=build
ISO=$(BUILD)/iso

all: $(BUILD)/kernel.bin iso

$(BUILD):
	mkdir -p $(BUILD)

# Compile source files

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: kernel/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/timer.o: kernel/timer.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel

$(BUILD)/kernel.bin: \
$(BUILD)/kernel.o \
$(BUILD)/idt.o \
$(BUILD)/timer.o

	$(LD) \
	$(LDFLAGS) \
	$(BUILD)/kernel.o \
	$(BUILD)/idt.o \
	$(BUILD)/timer.o \
	-o $@

# Create ISO

iso: $(BUILD)/kernel.bin
	mkdir -p $(ISO)/boot/grub

	cp $(BUILD)/kernel.bin \
	$(ISO)/boot/kernel.bin

	cp boot/grub.cfg \
	$(ISO)/boot/grub/grub.cfg

	grub-mkrescue \
	-o $(BUILD)/ak1.iso \
	$(ISO)

# Run

run: all
	qemu-system-i386 \
	-cdrom $(BUILD)/ak1.iso

clean:
	rm -rf $(BUILD)

.PHONY: all iso run clean