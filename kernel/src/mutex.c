#include<stdio.h>
#include".\Mutex.h"

int GetMutex(int resourceID)
{
	int i=0;
	for(i=0 ; i<MAXMUTEXES ; i++)
	{
		if(mutex[i].resourceID == resourceID)
			return 0;
	}
return -1;
}

int CreateMutex(int resourceID)
{
	int i=0;

	if(noOfMutexes == MAXMUTEXES)
	{
		ErrorNo=5;
		return -1;
	}

	for(i=0;i<MAXMUTEXES;i++)
	{
		if(mutex[i].resourceID == -1)
		{
			mutex[i].resourceID=resourceID;
			mutex[i].status=free;
			mutex[i].first=-1;
			mutex[i].last=-1;
			mutex[i].task=NULL;
			mutex[i].ownerID=CurrentTask->id;
			noOfMutexes++;
			break;
		}
	}
return 0;
}

int AquireMutex(int resourceID)
{
	int i=0,j=0;

	for(i=0;i<noOfMutexes;i++)
	{
		if(mutex[i].resourceID==resourceID)
		{
			if(mutex[i].taskID == CurrentTask->id)
			{
				return 0;
			}
			else if(mutex[i].status==free)
			{
				mutex[i].status=aquired;
				mutex[i].taskID=CurrentTask->id;
				mutex[i].task=CurrentTask;
			}
			else
			{
				if(mutex[i].noOfTasks == MAXWAITINGTASKS)
				return -1;

				for(j=0;j<MAXWAITINGTASKS;j++)
				{
					if(mutex[i].tasks[j].p==NULL)
					{
						mutex[i].tasks[j].p=CurrentTask;
						if(mutex[i].noOfTasks == 0)
						{
							mutex[i].tasks[j].left=-1;
							mutex[i].tasks[j].right=-1;
							mutex[i].first=j;
						}
						else
						{
							mutex[i].tasks[j].left=mutex[i].last;
							mutex[i].tasks[j].right=-1;
							mutex[i].tasks[mutex[i].last].right=j;
						}
						mutex[i].last=j;
						mutex[i].noOfTasks++;
						CurrentTask->status=BLOCKED;
						break;
					}
				}
				scheduler();

			}
		break;
		}
	}
	if(i == noOfMutexes)
	{
		ErrorNo=6;
		return -1;
	}
	return 0;
}

int ReleaseMutex(int ResourceID)
{
	int i=0,index=0;
	int priority=0;
	TASK *tempTaskPtr=NULL;

	for(i=0 ; i<MAXMUTEXES ; i++)
	{
		if(mutex[i].resourceID == ResourceID)
		{
			if(mutex[i].taskID == CurrentTask->id)
			{
				if(mutex[i].noOfTasks == 0)
				{
					mutex[i].status=free;
					mutex[i].taskID=-1;
					mutex[i].task=NULL;
				}
				else if(mutex[i].noOfTasks == 1)
				{
 					tempTaskPtr=mutex[i].tasks[mutex[i].first].p;
					mutex[i].taskID=(mutex[i].tasks[mutex[i].first].p)->id;
					mutex[i].task=mutex[i].tasks[mutex[i].first].p;
					mutex[i].tasks[mutex[i].first].p=NULL;
					mutex[i].tasks[mutex[i].first].right=-1;
					mutex[i].tasks[mutex[i].first].left=-1;
					mutex[i].first=-1;
					mutex[i].last=-1;
					mutex[i].noOfTasks--;
				}
				else
				{
					tempTaskPtr=mutex[i].tasks[mutex[i].first].p;
					index=mutex[i].first;
					mutex[i].taskID=(mutex[i].tasks[mutex[i].first].p)->id;
					mutex[i].task=mutex[i].tasks[mutex[i].first].p;
					mutex[i].first=mutex[i].tasks[mutex[i].first].right;
					mutex[i].tasks[index].p=NULL;
					mutex[i].tasks[index].right=-1;
					mutex[i].tasks[index].left=-1;
					mutex[i].tasks[mutex[i].first].left=-1;
					mutex[i].noOfTasks--;
				}
				break;
			}
			else
			{
				ErrorNo=7;
				return -1;
			}
		}
	}

	if(i == MAXMUTEXES)
	{
			ErrorNo=6;
			return -1;
	}

	if(tempTaskPtr != NULL)
	{
	priority=(tempTaskPtr->priority)-1;
	tempTaskPtr->status=READY;
	for(i=0 ; i<10 ; i++)
	{
		if(RQ.ReadyQ[priority][i].p == NULL)
		{
			RQ.ReadyQ[priority][i].p=tempTaskPtr;
			if(RQ.start[priority]==-1 && RQ.end[priority]==-1)
			{
				RQ.ReadyQ[priority][i].left=-1;
				RQ.ReadyQ[priority][i].right=-1;
				RQ.start[priority]=i;
				RQ.end[priority]=i;
				break;
			}
			else
			{
				RQ.ReadyQ[CurrentPriority][i].left=RQ.end[CurrentPriority];
				RQ.ReadyQ[CurrentPriority][i].right=-1;
				RQ.ReadyQ[CurrentPriority][RQ.end[CurrentPriority]].right=i;
				RQ.end[CurrentPriority]=i;
				break;
			}
		}
	}
	scheduler();
	}

return 0;
}

int DeleteMutex(int ResourceID)
{
	int i=0,j=0;

	for(i=0 ; i<MAXMUTEXES ; i++)
	{
		if(mutex[i].resourceID == ResourceID)
		{
			if(mutex[i].ownerID == CurrentTask->id)
			{
				mutex[i].resourceID=-1;
				mutex[i].noOfTasks=0;
				for(j=0;j<MAXWAITINGTASKS;j++)
				{
					mutex[i].tasks[j].p=NULL;
					mutex[i].tasks[j].right=-1;
					mutex[i].tasks[j].left=-1;
				}
				mutex[i].first=-1;
				mutex[i].last=-1;
				mutex[i].taskID=-1;
				mutex[i].task=NULL;
				mutex[i].status=free;
				mutex[i].ownerID=-1;
			}
			else
			{
				ErrorNo=8;
				return -1;
			}
			break;
		}
	}

	if(i == MAXMUTEXES)
	{
		ErrorNo=6;
		return -1;
	}
return 0;
}
