#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include "../inc/mutex.h"

char far *scr = (char far *) 0xB8000000L;
int priority;
TASK *temp;
int tickCount;
int i;

void scheduler()
{
	asm int SCHED_INTR
}

void interrupt Timer_isr()
{

	int i;
	static int timeToSchedule=0;
	oldhandler();
//	k_printf("\nHardware INTERRUPT %d\n",tickCount);
	disable();
	/* search the blocked queue for tasks that have to be run */
      if(BQ.noOfTasks != 0)
      {
	      for(i=BQ.start ; i!=-1 ; i=BQ.BlockedQ[i].right)
		{
			BQ.BlockedQ[i].p->timerticks--;
			if((BQ.BlockedQ[i].p->timerticks) == 0)
			{
				BQ.BlockedQ[i].p->status=READY;
				Priority=(BQ.BlockedQ[i].p->priority)-1;
				temp=BQ.BlockedQ[i].p;
				BQ.BlockedQ[i].p=NULL;
	
				if(BQ.noOfTasks==1)
				{
					BQ.start=-1;
					BQ.end=-1;
				}
				else
				{
					BQ.start=BQ.BlockedQ[i].right;
					//BQ.BlockedQ[i].right=-1;
					BQ.BlockedQ[BQ.start].left=-1;
				}
				for(i=0 ; i<10 ; i++)
				{
					if(RQ.ReadyQ[Priority][i].p == NULL)
					{
						RQ.ReadyQ[Priority][i].p=temp;

						if(RQ.start[Priority]==-1 && RQ.end[Priority]==-1)
						{
							RQ.ReadyQ[Priority][i].right=-1;
							RQ.ReadyQ[Priority][i].left=-1;
							RQ.start[Priority]=i;
							RQ.end[Priority]=i;
							BQ.noOfTasks--;
							timeToSchedule=1;

							break;
						}
						else
						{
							RQ.ReadyQ[Priority][i].right=-1;
							RQ.ReadyQ[Priority][i].left=RQ.end[Priority];
							RQ.ReadyQ[Priority][RQ.end[Priority]].right=i;
							RQ.end[Priority]=i;
							BQ.noOfTasks--;
							timeToSchedule=1;

							break;
						}
					}
					BQ.noOfTasks--;
					timeToSchedule=1;
				}
			}
		 }
	}
	//the 10 here means the ticks to switch after
	if((tickCount++%10==0) || (timeToSchedule))
	{
		timeToSchedule=0;
		if(Isrunning == -1)   //if there isnt a task running
			;
		else
		{
			 /*save the context of the currently running task*/
			CurrentTask->ss=_SS;
			CurrentTask->sp=_SP;
	
			for(i=0;i<10;i++)
			{
			//put the currently running task to the back of the ready queue
	
				if(RQ.ReadyQ[CurrentPriority][i].p == NULL)
				{
					RQ.ReadyQ[CurrentPriority][i].p=CurrentTask;
					if(RQ.start[CurrentPriority]==-1 && RQ.end[CurrentPriority]==-1)
					{
						RQ.ReadyQ[CurrentPriority][i].left=-1;
						RQ.ReadyQ[CurrentPriority][i].right=-1;
						RQ.start[Priority]=i;
						RQ.end[Priority]=i;
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
		}
		//choose a task to run
		for(Priority=0 ; Priority<5 ; Priority++)
		{
			if(RQ.start[Priority]== -1 && RQ.end[Priority] == -1)
			{    /*no tasks exist in the priority queu e*/
				continue;
			}
	
			SelectedTask=RQ.start[Priority];
			CurrentTask=RQ.ReadyQ[Priority][SelectedTask].p;
			RQ.ReadyQ[Priority][SelectedTask].p=NULL;

			CurrentPriority=Priority;
			Isrunning=1;
	
			if(RQ.start[Priority]!=RQ.end[Priority])
			{
				RQ.start[Priority]=RQ.ReadyQ[Priority][RQ.start[Priority]].right;
				RQ.ReadyQ[Priority][RQ.start[Priority]].left=-1;
				RQ.ReadyQ[Priority][SelectedTask].right=-1;
			}
			else
			{
				RQ.start[Priority]=-1;
				RQ.end[Priority]=-1;
			}
			break;
		}

		/*load the choosen tasks stack pointers and run the task*/
		_SS=CurrentTask->ss;
		_SP=CurrentTask->sp;
		if(CurrentTask->_new)
		{
			CurrentTask->_new=0;
			asm ret;
		}
	}
	enable();
}


void interrupt switcher()
{
	if(currenttask != NULL) {
		/*
		 * save the context of the currently running task
		 */
		save_context();

		/*
		 * Put the current task at the back of the ready Q only
		 * if it isn't blocked
		 */
		if(currenttask->status != BLOCKED)
			rq_enqueue(&readyq[priority], currenttask);
	}

	/*
	 * choose a task to run
	 */
	for(priority = 0 ; priority < 5 ; priority++) {
		if (reqdyq[priority].len == 0)
			continue;

		currentpriority = priority;
		isrunning = 1;

		currenttask = rq_dequeue(&readyq[priority]);
		break;
	}

	/*
	 * load the choosen tasks stack pointers and run the task
	 */
	load_context();

	if(currenttask->new)
		currenttask->new=0;
	
	asm ret;
}
