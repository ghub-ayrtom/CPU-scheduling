#include "CPU.h"
#include "list.h"
#include "schedulers.h"
#include "stdio.h"
#include "stdlib.h"
#include "task.h"

int tasks_waiting_time[MAX_TASKS_NUMBER];
int tasks_turnaround_time[MAX_TASKS_NUMBER];

int current_task_index = 0;
float average_waiting_time = 0.0;
float average_turnaround_time = 0.0;

struct task *tasks_array_list[MAX_TASKS_NUMBER];

void add_task(char *name, int priority, int burst)
{
    struct task *new_task = malloc(sizeof(struct task));

    new_task->id = current_task_index + 1;
    new_task->name = name;
    new_task->priority = priority;
    new_task->same_priority_tasks_count = 0;
    new_task->burst = burst;
    new_task->is_running = false;

    tasks_array_list[current_task_index] = new_task;
    ++current_task_index;
}

void schedule()
{
    struct task *temp_task;

    int tasks_array_list_size = current_task_index, completed_tasks_count = 0, previous_task_burst_time = 0;
    tasks_waiting_time[0] = 0;

    for (int i = 0; i < tasks_array_list_size; ++i)
    {
        for (int j = i + 1; j < tasks_array_list_size; ++j)
        {
            // Сортируем массив процессов в порядке убывания их приоритета
            if (tasks_array_list[i]->priority < tasks_array_list[j]->priority)
            {
                temp_task = tasks_array_list[i];
                tasks_array_list[i] = tasks_array_list[j];
                tasks_array_list[j] = temp_task;
            }
            // Если приоритеты равны
            else if (tasks_array_list[i]->priority == tasks_array_list[j]->priority)
            {
                // Сортируем массив процессов в порядке возрастания времени их выполнения на ЦПУ
                if (tasks_array_list[i]->burst > tasks_array_list[j]->burst)
                {
                    temp_task = tasks_array_list[i];
                    tasks_array_list[i] = tasks_array_list[j];
                    tasks_array_list[j] = temp_task;
                }
            }
        }
    }

    // Для каждого процесса подсчитываем количество процессов с таким же приоритетом как и у исходного
    for (int i = 0; i < tasks_array_list_size; ++i)
    {
        for (int j = i + 1; j < tasks_array_list_size; ++j)
        {
            if (tasks_array_list[i]->priority == tasks_array_list[j]->priority)
                ++tasks_array_list[i]->same_priority_tasks_count;
        }
    }

    // Перебираем весь массив процессов
    for (int i = 0; i < tasks_array_list_size; ++i)
    {
        if (completed_tasks_count != tasks_array_list_size)
        {
            // Если в нём есть несколько процессов с равным приоритетом
            if (tasks_array_list[i]->same_priority_tasks_count != 0)
            {
                int same_priority_tasks_count = tasks_array_list[i]->same_priority_tasks_count;
                int task_index = i;

                while (tasks_array_list[i]->same_priority_tasks_count != 0)
                {
                    // Если процесс уже был запущен на выполнение виртуальным центральным процессором, значит время ожидания для него посчитано
                    if (task_index != 0 && !tasks_array_list[task_index]->is_running)
                    {
                        tasks_array_list[task_index]->is_running = true;

                        // Время ожидания текущего процесса - это время ожидания всех процессов до него + время выполнения предыдущего процесса
                        tasks_waiting_time[task_index] = tasks_waiting_time[task_index - 1] + previous_task_burst_time;
                        average_waiting_time += tasks_waiting_time[task_index];

                        previous_task_burst_time = 0;
                    }

                    // Если время выполнения процесса больше 0 и больше или равно длине кванта времени (10)
                    if (tasks_array_list[task_index]->burst > 0 && tasks_array_list[task_index]->burst >= QUANTUM)
                    {
                        run(tasks_array_list[task_index], QUANTUM); // Выполняем его в течение кванта времени
                        previous_task_burst_time += QUANTUM;
                        tasks_array_list[task_index]->burst -= QUANTUM;
                    }
                    // Иначе, если время выполнения процесса меньше длины кванта времени (10) и не равно 0
                    else if (tasks_array_list[task_index]->burst < QUANTUM && tasks_array_list[task_index]->burst != 0)
                    {
                        run(tasks_array_list[task_index], tasks_array_list[task_index]->burst); // Довыполняем его в течение оставшегося времени
                        previous_task_burst_time += tasks_array_list[task_index]->burst;
                        tasks_array_list[task_index]->burst = 0;
                    }
                    // Иначе, процесс считается выполненным
                    else
                    {
                        ++completed_tasks_count;

                        // Оборотное время текущего процесса - это его время ожидания + его время выполнения
                        tasks_turnaround_time[task_index] = tasks_waiting_time[task_index] + previous_task_burst_time;
                        average_turnaround_time += tasks_turnaround_time[task_index];

                        --tasks_array_list[i]->same_priority_tasks_count;
                    }

                    ++task_index;

                    if (task_index == i + same_priority_tasks_count + 1)
                        task_index = i;
                }

                i += same_priority_tasks_count;
            }
            // Иначе, если только текущий процесс из списка имеет такой приоритет
            else
            {
                if (i != 0 && !tasks_array_list[i]->is_running)
                {
                    tasks_array_list[i]->is_running = true;

                    tasks_waiting_time[i] = tasks_waiting_time[i - 1] + previous_task_burst_time;
                    average_waiting_time += tasks_waiting_time[i];

                    previous_task_burst_time = 0;
                }

                run(tasks_array_list[i], tasks_array_list[i]->burst); // Выполняем процесс полностью
                previous_task_burst_time += tasks_array_list[i]->burst;
                tasks_array_list[i]->burst = 0;
                
                ++completed_tasks_count;

                tasks_turnaround_time[i] = tasks_waiting_time[i] + previous_task_burst_time;
                average_turnaround_time += tasks_turnaround_time[i];
            }
        }
        else
        {
            break;
        }
    }

    printf("\nAverage waiting time = average response time = %.3f.\n", average_waiting_time / (float)tasks_array_list_size);
    printf("Average turnaround time = %.3f.\n", average_turnaround_time / (float)tasks_array_list_size);
}
