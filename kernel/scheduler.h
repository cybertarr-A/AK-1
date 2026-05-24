#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 16

typedef struct
{
    uint32_t pid;
    uint32_t state;
} process_t;

void scheduler_init(void);

#endif