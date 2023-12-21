#ifndef _CPU_H_
#define _CPU_H_

#include "task.h"

// Квант времени виртуального процессора
#define QUANTUM 10

// Эмулирует выполнение процесса task на виртуальном процессоре в течение времени его выполнения slice
void run(Task *task, int slice);

#endif
