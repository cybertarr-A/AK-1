#include <stdint.h>

#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_FLAGS 0
#define MULTIBOOT_CHECKSUM \
-(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

__attribute__((section(".multiboot")))
const unsigned long multiboot_header[] =
{
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    MULTIBOOT_CHECKSUM
};

volatile uint16_t* vga =
(uint16_t*)0xB8000;

void print(const char* s)
{
    int i=0;

    while(s[i])
    {
        vga[i] =
        (0x0F<<8)|s[i];

        i++;
    }
}

void kernel_main(void)
{
    print("AK-1 AI Kernel Booted");

    for(;;)
    {
        __asm__("hlt");
    }
}