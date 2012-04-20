#include "list.h"
#define MAX_TASKS 1024 

num task_status {
	UNUSED,
	READY,
	BLOCKED
};

/*
 * task ctrl block
 */
typedef struct tcb {
	int             new;		// new task ?
	unsigned int    ss, sp;		// stack pointers
	unsigned int    ip;		// instruction pointer
	int             priority;	// task priority
	int             timerticks;	// sleep/schedule duration
	task_status     status;		// current status of the task
	int             id;		// task id
	list_head	next;
}task;

char far(stack[MAXTASK][4000]);	//stacks, one for each task

void createtask(void(*task)(void), int priority);
void deletetask(int tid);
void resume(int tid);
void _sleep(int timerticks);
