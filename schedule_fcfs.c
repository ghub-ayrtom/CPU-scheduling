#include "CPU.h"
#include "list.h"
#include "schedulers.h"
#include "stdio.h"
#include "stdlib.h"
#include "task.h"

int tasks_waiting_time[MAX_TASKS_NUMBER];
int tasks_turnaround_time[MAX_TASKS_NUMBER];

int tasks_number = 0;
float average_waiting_time = 0.0;
float average_turnaround_time = 0.0;

void add_task(char *name, int priority, int burst)
{
    ++tasks_number;

    struct task *new_task = malloc(sizeof(struct task));

    new_task->id = tasks_number;
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = burst;

    insert(&tasks_linked_list, new_task);
}

void schedule()
{
    // Переворачиваем односвязный список процессов, так как каждый новый процесс добавлялся в его начало
    tasks_linked_list = reverse(tasks_linked_list);
    struct node *current_node = tasks_linked_list;

    int task_index = 0, previous_task_burst_time;
    tasks_waiting_time[task_index] = 0;

    while (current_node != NULL)
    {
        if (current_node->task != NULL)
        {
            // Для первого процесса время ожидания равно нулю (строка 36)
            if (task_index != 0)
            {
                // Время ожидания текущего процесса - это время ожидания всех процессов до него + время выполнения предыдущего процесса
                tasks_waiting_time[task_index] = tasks_waiting_time[task_index - 1] + previous_task_burst_time;
                average_waiting_time += tasks_waiting_time[task_index];
            }

            run(current_node->task, current_node->task->burst);

            // Оборотное время текущего процесса - это его время ожидания + его время выполнения
            tasks_turnaround_time[task_index] = tasks_waiting_time[task_index] + current_node->task->burst;
            average_turnaround_time += tasks_turnaround_time[task_index];

            ++task_index;
            previous_task_burst_time = current_node->task->burst;
        }

        // Берём следующий процесс из списка
        current_node = current_node->next_task;
    }

    printf("\nAverage waiting time = average response time = %.3f.\n", average_waiting_time / (float)tasks_number);
    printf("Average turnaround time = %.3f.\n", average_turnaround_time / (float)tasks_number);
}
