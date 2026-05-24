#include <stdint.h>
#include "irq.h"

void irq_init(void)
{
    __asm__ volatile("sti");
}