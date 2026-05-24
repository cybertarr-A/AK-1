#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct
{
    uint32_t pid;
    uint32_t state;
} task_t;

void task_init(void);

#endif