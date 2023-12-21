#include "CPU.h"
#include "list.h"
#include "schedulers.h"
#include "stdio.h"
#include "stdlib.h"
#include "task.h"

struct task *tasks_array_list[MAX_TASKS_NUMBER];

int tasks_waiting_time[MAX_TASKS_NUMBER];
int tasks_turnaround_time[MAX_TASKS_NUMBER];

int current_task_index = 0;
float average_waiting_time = 0.0;
float average_turnaround_time = 0.0;

void add_task(char *name, int priority, int burst)
{
    struct task *new_task = malloc(sizeof(struct task));

    new_task->id = current_task_index + 1;
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = burst;

    tasks_array_list[current_task_index] = new_task;
    ++current_task_index;
}

void schedule()
{
    struct task *temp_task;

    for (int i = 0; i < current_task_index; ++i)
    {
        for (int j = i + 1; j < current_task_index; ++j)
        {
            // Сортируем массив процессов в порядке возрастания времени их выполнения на ЦПУ
            if (tasks_array_list[i]->burst > tasks_array_list[j]->burst)
            {
                temp_task = tasks_array_list[i];
                tasks_array_list[i] = tasks_array_list[j];
                tasks_array_list[j] = temp_task;
            }
            // Если время одинаково
            else if (tasks_array_list[i]->burst == tasks_array_list[j]->burst)
            {
                // Сортируем массив процессов в порядке убывания их приоритета
                if (tasks_array_list[i]->priority < tasks_array_list[j]->priority)
                {
                    temp_task = tasks_array_list[i];
                    tasks_array_list[i] = tasks_array_list[j];
                    tasks_array_list[j] = temp_task;
                }
            }
        }
    }

    tasks_waiting_time[0] = 0;

    // Перебираем весь массив процессов
    for (int task_index = 0; task_index < current_task_index; ++task_index)
    {
        // Для первого процесса время ожидания равно нулю (строка 59)
        if (task_index != 0)
        {
            // Время ожидания текущего процесса - это время ожидания всех процессов до него + время выполнения предыдущего процесса
            tasks_waiting_time[task_index] = tasks_waiting_time[task_index - 1] + tasks_array_list[task_index - 1]->burst;
            average_waiting_time += tasks_waiting_time[task_index];
        }

        run(tasks_array_list[task_index], tasks_array_list[task_index]->burst);

        // Оборотное время текущего процесса - это его время ожидания + его время выполнения
        tasks_turnaround_time[task_index] = tasks_waiting_time[task_index] + tasks_array_list[task_index]->burst;
        average_turnaround_time += tasks_turnaround_time[task_index];
    }

    printf("\nAverage waiting time = average response time = %.3f.\n", average_waiting_time / (float)current_task_index);
    printf("Average turnaround time = %.3f.\n", average_turnaround_time / (float)current_task_index);
}
