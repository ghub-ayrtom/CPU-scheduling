#ifndef _TASK_H_
#define _TASK_H_

#define MAX_TASKS_NUMBER 100

#include "stdbool.h"

typedef struct task
{
    int id;
    char *name;
    int priority;
    int same_priority_tasks_count;
    int burst;
    bool is_running;
} Task;

#endif
