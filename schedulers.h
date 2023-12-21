#ifndef _SCHEDULERS_H_
#define _SCHEDULERS_H_

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// Добавляет данные о процессе в одну из структур данных (массив или односвязный список), выбор которой зависит от алгоритма планирования
void add_task(char *name, int priority, int burst);

// Планирует список процессов на выполнение виртуальным процессором
void schedule();

#endif
