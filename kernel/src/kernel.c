#include "../inc/kernel.h"

char far *scr = (char far *) 0xB8000000L;
static int tickcount;

void scheduler()
{
	asm int SCHED_INTR
}

struct task * runnable_task()
{
	int priority;
	struct task *t;

	for(priority = 0 ; priority < 5 ; priority++) {
		if (reqdyq[priority].ntasks == 0)
			continue;

		isrunning = 1;

		t = rq_dequeue(&readyq[priority]);
		break;
	}
	return t;
}

void schedule_task()
{
	/*
	 * save the context of the currently running task
	 * and enqueue it on the appropriate ready queue
	 */
	save_context();
	rq_enqueue(&readyq[currenttask->priority], currenttask);

	/*
	 * choose a task to run
	 */
	currenttask = runnable_task();

	/*
	 * load the choosen tasks stack pointers and run the task
	 */
	load_context();

	if(currenttask->new)
		currenttask->new=0;
	
	return;
}

void timer_isr()
{
	struct task *t;
	struct list_head *p;

	if(blkdq.ntasks != 0) {
		p = &blkdq.tasks->task_list;
		while (p != NULL) {
			t = container_of(p, task, next_list);
			t->timerticks--;
			
			if(t->timerticks == 0) {
				bq_dequeue(t);
				rq_enqueue(&readyq[t->priority], t);
			}
			p = p->next;
		 }
	}

	/*
	 * If 10 timeticks have elapsed since the last time a task
	 * was scheduled, invoke the scheduler.
	 */	   
	if((++tickcount) % 10 == 0)
		scheduler_task();

	return;
}

void switcher()
{
	scheduler_task();
}
