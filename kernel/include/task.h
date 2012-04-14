#include ".\kernel.h"

#define MAXTASK 3

num task_status {
	UNUSED,
	READY,
	BLOCKED
};

/*
 * task ctrl block
 */
typedef struct tcb {
	int             _new;
	unsigned int    ss, sp;
	unsigned int    ip;
	int             priority;
	int             timerticks;
	task_status     status;
	int             id;
}task;

/*
 * TODO: Don't restrict the no of tasks to 3 !
 */
char far(stack[MAXTASK][4000]);	//stacks, one for each task

task tasks[MAXTASK];
int nooftasks;
void createtask(void(*task)(void), int priority);
void deletetask(int tid);
void resume(int tid);
void _sleep(int timerticks);
