#include "list.h"

void interrupt(*oldhandler)(__CPPARGS);
void interrupt Timer_isr();
void interrupt switcher();

#define save_context() \
	asm volatile ("movl %%esp, %0\n\t" \
			"mov %%ss, %1\n\t" \
			: "=m" (currenttask->sp), "=m" (currenttask->ss) \
			: \
			: \
		     )

#define load_context() \
	asm volatile ("movl %0, %%esp\n\t" \
			"mov %1, %%ss\n\t" \
			: "=m" (currenttask->sp), "=m" (currenttask->ss) \
			: \
			: \
		     )

#define TIMER_INTR 	8  /*0X1C*/     /* The clock tick interrupt */
#define SCHED_INTR 	200
#define MAX_TASK_PRI	5

unsigned int m_ss, m_sp;

struct interrupt {
	unsigned bp, di, si, ds, es, dx, cx, bx, ax, ip, cs, fl;
};

/*
 * Contains 5 queues for the 5 different priorities supported.
 */
struct readyq {
	int priority;
	int ntasks;
	task *t;
}rq[MAX_TASK_PRI];

struct blkdq {
	int ntasks;
	task *t;
}bq;

task *currenttask;		// pointer to the currently running task
int isrunning = -1;		// checks wheather a task is running or not
