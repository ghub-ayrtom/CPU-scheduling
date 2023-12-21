#ifndef _LIST_H_
#define _LIST_H_

#include "task.h"

// Элемент односвязного списка процессов
struct node
{
    Task *task;
    struct node *next_task;
};

extern struct task *tasks_array_list[MAX_TASKS_NUMBER]; // Массив процессов
extern struct node *tasks_linked_list; // Односвязный список процессов

extern int tasks_waiting_time[MAX_TASKS_NUMBER]; // Время ожидания каждого из процессов
extern int tasks_turnaround_time[MAX_TASKS_NUMBER]; // Оборотное время каждого из процессов

// Вставляет процесс new_task в конец односвязного списка
void insert(struct node **start_node, Task *new_task);

// Удаляет процесс deleting_task из односвязного списка
void delete(struct node **start_node, Task *deleting_task);

// Переворачивает односвязный список (первый его элемент становится последним, а последний первым)
struct node* reverse(struct node *start_node);

#endif
