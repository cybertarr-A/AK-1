CC=gcc
LD=ld

CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T kernel/linker.ld

BUILD=build
ISO=$(BUILD)/iso

# ==========================
# Object Files
# ==========================

OBJS=\
$(BUILD)/kernel.o \
$(BUILD)/idt.o \
$(BUILD)/timer.o \
$(BUILD)/pic.o \
$(BUILD)/irq.o \
$(BUILD)/task.o

# ==========================
# Main Build
# ==========================

all: $(BUILD)/kernel.bin iso

# ==========================
# Build Directory
# ==========================

$(BUILD):
	mkdir -p $(BUILD)

# ==========================
# Compile Sources
# ==========================

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: kernel/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/timer.o: kernel/timer.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pic.o: kernel/pic.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/irq.o: kernel/irq.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/task.o: kernel/task.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# ==========================
# Link Kernel
# ==========================

$(BUILD)/kernel.bin: $(OBJS)
	$(LD) \
	$(LDFLAGS) \
	$(OBJS) \
	-o $@

# ==========================
# Create Bootable ISO
# ==========================

iso: $(BUILD)/kernel.bin
	mkdir -p $(ISO)/boot/grub

	cp $(BUILD)/kernel.bin \
	$(ISO)/boot/kernel.bin

	cp boot/grub.cfg \
	$(ISO)/boot/grub/grub.cfg

	grub-mkrescue \
	-o $(BUILD)/ak1.iso \
	$(ISO)

# ==========================
# Run Kernel
# ==========================

run: all
	qemu-system-i386 \
	-cdrom $(BUILD)/ak1.iso

# ==========================
# Clean
# ==========================

clean:
	rm -rf $(BUILD)

.PHONY: all iso run clean