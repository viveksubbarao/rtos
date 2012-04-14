#include ".\kernel.h"
#define MAXRESOURCES	10
#define MAXMUTEXES	10
#define MAXWAITINGTASKS 10

int noofmutexes;

enum mutex_status {
	free,aquired
};

/*
 * TODO: Don't statically allocate the mutexes.
 * A mutex should be created as and when required!
 */
struct mutex {
	int		resid;
	int		nooftasks;
	node		tasks[10];
	int		first;
	int		last;
	int		taskid;
	task		*task;
	mutex_status	status;
	int		ownerID;
}mutex[MAXRESOURCES];

int getmutex(int resid);
int createmutex(int resid);
int aquiremutex(int resid);
int releasemutex(int resid);
int deletemutex(int resid);
