#include "scheduler.h"

process_t task_queue[MAX_TASKS];

uint32_t task_count=0;

void scheduler_init(void)
{
    task_count=1;

    task_queue[0].pid=1;
    task_queue[0].state=1;
}