#include <stdint.h>
#include "pic.h"

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile(
        "outb %0,%1"
        :
        : "a"(value),
          "Nd"(port)
    );
}

void pic_init(void)
{
    /* Start initialization */

    outb(0x20,0x11);
    outb(0xA0,0x11);

    /* Remap offsets */

    outb(0x21,0x20);
    outb(0xA1,0x28);

    /* Tell master/slave relationship */

    outb(0x21,0x04);
    outb(0xA1,0x02);

    /* 8086 mode */

    outb(0x21,0x01);
    outb(0xA1,0x01);

    /* Enable interrupts */

    outb(0x21,0x00);
    outb(0xA1,0x00);
}