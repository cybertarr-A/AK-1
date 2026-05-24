#include <stdint.h>
#include "idt.h"
#include "timer.h"
#include "pic.h"

/* ======================================
   MULTIBOOT HEADER
====================================== */

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

/* ======================================
   VGA TERMINAL
====================================== */

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 0x0F

volatile uint16_t* vga=(uint16_t*)0xB8000;

int row=0;
int col=0;

void clear_screen()
{
    for(int y=0;y<VGA_HEIGHT;y++)
    {
        for(int x=0;x<VGA_WIDTH;x++)
        {
            vga[y*VGA_WIDTH+x]=
            (VGA_COLOR<<8)|' ';
        }
    }

    row=0;
    col=0;
}

void putchar(char c)
{
    /* newline */

    if(c=='\n')
    {
        row++;
        col=0;
        return;
    }

    /* backspace */

    if(c==8)
    {
        if(col>0)
        {
            col--;

            vga[row*VGA_WIDTH+col]=
            (VGA_COLOR<<8)|' ';
        }

        return;
    }

    vga[row*VGA_WIDTH+col]=
    (VGA_COLOR<<8)|c;

    col++;

    if(col>=VGA_WIDTH)
    {
        col=0;
        row++;
    }

    if(row>=VGA_HEIGHT)
    {
        row=0;
    }
}

void print(const char* s)
{
    int i=0;

    while(s[i])
    {
        putchar(s[i]);
        i++;
    }
}

/* ======================================
   PORT I/O
====================================== */

static inline uint8_t inb(uint16_t port)
{
    uint8_t result;

    __asm__ volatile(
        "inb %1,%0"
        : "=a"(result)
        : "Nd"(port)
    );

    return result;
}

/* ======================================
   KEYBOARD
====================================== */

char keyboard_map[128]=
{
    0,27,'1','2','3','4','5',
    '6','7','8','9','0',
    '-','=',8,'\t',

    'q','w','e','r','t',
    'y','u','i','o','p',
    '[',']','\n',0,

    'a','s','d','f','g',
    'h','j','k','l',
    ';','\'','`',0,

    '\\',
    'z','x','c','v',
    'b','n','m',
    ',', '.', '/'
};

void keyboard_poll()
{
    if(!(inb(0x64)&1))
        return;

    uint8_t scancode=
    inb(0x60);

    if(scancode&0x80)
        return;

    if(scancode<128)
    {
        char c=
        keyboard_map[scancode];

        if(c)
            putchar(c);
    }
}

/* ======================================
   MEMORY MANAGER
====================================== */

#define HEAP_SIZE (1024*1024)

uint8_t kernel_heap[HEAP_SIZE];

uint32_t heap_index=0;

void* kmalloc(uint32_t size)
{
    if(heap_index+size>=HEAP_SIZE)
    {
        return 0;
    }

    void* ptr=
    &kernel_heap[heap_index];

    heap_index+=size;

    return ptr;
}

/* ======================================
   KERNEL ENTRY
====================================== */

void kernel_main()
{
    clear_screen();

    print("AK-1 AI Kernel\n");
    print("=====================\n");

    print("Kernel Boot Success\n");

    print("AI Core: Offline\n");

    print("Keyboard Driver: Active\n");

    void* mem = kmalloc(64);

    if(mem)
    {
        print("Memory Manager: Active\n");
    }
    else
    {
        print("Memory Manager: Failed\n");
    }

    print("Loading IDT...\n");

    idt_init();

    print("Interrupt System: Active\n");

    print("Initializing PIC...\n");

    pic_init();

    print("PIC: Active\n");

    timer_init();

    print("Timer System: Active\n");

    print("\nType below:\n");

    while(1)
    {
        keyboard_poll();
    }
}