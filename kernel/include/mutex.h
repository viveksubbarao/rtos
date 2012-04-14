#include "./kernel.h"

#define MAX_MUTEXES	10

enum mutex_status {
	FREE,
	AQUIRED
};

struct mutex {
	int		id;	// mutex identifier
	list_head	tasks;	// tasks waiting on the mutex
	int		taskid;	// task id currently holding the  mutex
	task		*task;	// tcb of the task currently holding the mutex
	mutex_status	status;	// mutex status
};

int getmutex(int resid);
int createmutex(int resid);
int aquiremutex(int resid);
int releasemutex(int resid);
int deletemutex(int resid);
