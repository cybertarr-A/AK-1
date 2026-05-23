CC=gcc
LD=ld

CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T kernel/linker.ld

BUILD=build
ISO=$(BUILD)/iso

# ======================
# Main Build Target
# ======================

all: $(BUILD)/kernel.bin iso

# ======================
# Build Directory
# ======================

$(BUILD):
	mkdir -p $(BUILD)

# ======================
# Compile Sources
# ======================

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: kernel/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# ======================
# Link Kernel
# ======================

$(BUILD)/kernel.bin: \
$(BUILD)/kernel.o \
$(BUILD)/idt.o

	$(LD) \
	$(LDFLAGS) \
	$(BUILD)/kernel.o \
	$(BUILD)/idt.o \
	-o $@

# ======================
# Build ISO
# ======================

iso: $(BUILD)/kernel.bin

	mkdir -p \
	$(ISO)/boot/grub

	cp $(BUILD)/kernel.bin \
	$(ISO)/boot/kernel.bin

	cp boot/grub.cfg \
	$(ISO)/boot/grub/grub.cfg

	grub-mkrescue \
	-o $(BUILD)/ak1.iso \
	$(ISO)

# ======================
# Run QEMU
# ======================

run: all
	qemu-system-i386 \
	-cdrom $(BUILD)/ak1.iso

# ======================
# Clean
# ======================

clean:
	rm -rf $(BUILD)

.PHONY: \
all \
iso \
run \
clean