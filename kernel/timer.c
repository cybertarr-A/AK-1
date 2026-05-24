#include "timer.h"
#include <stdint.h>

uint32_t ticks=0;

void timer_init(void)
{
    ticks=0;
}