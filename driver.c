#include "list.h"
#include "schedulers.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "task.h"

struct node *tasks_linked_list;

int main(int argc, char *argv[])
{
    tasks_linked_list = malloc(sizeof(struct node));

    // Открываем для чтения файл schedule.txt со списком процессов
    FILE *schedule_tasks_list = fopen(argv[1], "r");

    // Строки считанного из файла процесса
    char task[MAX_TASKS_NUMBER];
    char *task_temp;

    char *name;
    int priority;
    int burst;
    
    // Пока не дошли до конца файла
    while (fgets(task, MAX_TASKS_NUMBER, schedule_tasks_list) != NULL)
    {
        task_temp = strdup(task);

        name = strsep(&task_temp, ",");
        priority = atoi(strsep(&task_temp, ","));
        burst = atoi(strsep(&task_temp, ","));

        // Добавляем данные о процессе в одну из структур данных (массив или односвязный список), выбор которой зависит от алгоритма планирования
        add_task(name, priority, burst);

        free(task_temp);
    }

    fclose(schedule_tasks_list);

    // Планируем список считанных и сохранённых выше процессов на выполнение виртуальным процессором
    schedule();

    return 0;
}
