#ifndef __TASK_H__
#define __TASK_H__

#define TASK_UART       0  /* uart support */
#define TASK_TIMER      5  /* test the timer by 1 Hz buzz */



void inline set_task(unsigned long *taskset, int task);
void inline clr_task(unsigned long *taskset, int task);
int inline test_task(unsigned long taskset, int task);

void task_init(unsigned long *taskset);
void task_loop(unsigned long *taskset);

#endif

