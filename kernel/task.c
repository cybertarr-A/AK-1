#include "task.h"

task_t current_task;

void task_init(void)
{
    current_task.pid=1;
    current_task.state=1;
}