#include <stdint.h>

volatile uint16_t* video_memory=(uint16_t*)0xB8000;

void print(const char* str)
{
    int i=0;

    while(str[i]!=0)
    {
        video_memory[i]=(0x0F<<8)|str[i];
        i++;
    }
}

void kernel_main()
{
    print("AK-1 AI Kernel Booted");

    while(1)
    {
        __asm__("hlt");
    }
}