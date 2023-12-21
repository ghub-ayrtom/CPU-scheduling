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
    new_task->is_running = false;

    insert(&tasks_linked_list, new_task);
}

void schedule()
{
    // Переворачиваем односвязный список процессов, так как каждый новый процесс добавлялся в его начало
    tasks_linked_list = reverse(tasks_linked_list);
    struct node *current_node = tasks_linked_list;
    
    int task_index = 0, completed_tasks_count = 0, previous_task_burst_time;
    tasks_waiting_time[task_index] = 0;

    while (current_node != NULL)
    {
        if (completed_tasks_count != tasks_number)
        {
            if (current_node->task != NULL)
            {
                // Если процесс уже был запущен на выполнение виртуальным центральным процессором, значит время ожидания для него посчитано
                if (task_index != 0 && !current_node->task->is_running)
                {
                    current_node->task->is_running = true;

                    // Время ожидания текущего процесса - это время ожидания всех процессов до него + время выполнения предыдущего процесса
                    tasks_waiting_time[task_index] = tasks_waiting_time[task_index - 1] + previous_task_burst_time;
                    average_waiting_time += tasks_waiting_time[task_index];
                }

                // Если время выполнения процесса больше 0 и больше или равно длине кванта времени (10)
                if (current_node->task->burst > 0 && current_node->task->burst >= QUANTUM)
                {
                    run(current_node->task, QUANTUM); // Выполняем его в течение кванта времени
                    previous_task_burst_time = QUANTUM;
                    current_node->task->burst -= QUANTUM;
                }
                // Иначе, если время выполнения процесса меньше длины кванта времени (10) и не равно 0
                else if (current_node->task->burst < QUANTUM && current_node->task->burst != 0)
                {
                    run(current_node->task, current_node->task->burst); // Довыполняем его в течение оставшегося времени
                    previous_task_burst_time = current_node->task->burst;
                    current_node->task->burst = 0;
                }
                // Иначе, процесс считается выполненным
                else
                {
                    ++completed_tasks_count;
                    previous_task_burst_time = 0;
                }

                // Оборотное время текущего процесса - это его время ожидания + его время выполнения
                tasks_turnaround_time[task_index] = tasks_waiting_time[task_index] + current_node->task->burst;
                average_turnaround_time += tasks_turnaround_time[task_index];
            }

            ++task_index;

            // Берём следующий процесс из списка
            current_node = current_node->next_task;

            if (current_node == NULL)
            {
                task_index = 0;
                current_node = tasks_linked_list;
            }
        }
        else
        {
            break;
        }
    }

    printf("\nAverage waiting time = average response time = %.3f.\n", average_waiting_time / (float)tasks_number);
    printf("Average turnaround time = %.3f.\n", average_turnaround_time / (float)tasks_number);
}
