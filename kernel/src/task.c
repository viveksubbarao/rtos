#include<stdio.h>
#include<DOS.h>
#include ".\TASKS.H"

int PIDcount;

int createtask(void(*task)(void),int priority)
{
	/***************************************************
	*this function just creates a task by initializing the
	*variables of the TCB.
	*********************************************************/

	int i,location;
	unsigned int ip;
	int pri=priority-1;

	if(noOfTasks == 10)
	{
		ErrorNo=1;
		return -1;
	}

	if(priority >5 || priority <1)
	{
		ErrorNo=2;
		return -1;
	}

	for(i=0;i<MAXTASK;i++)
	{
		if(Tasks[i].status==UNUSED)
		{
			location=i;
			break;
		}
	}


	/******SETTING UP THE TCB************/
	Tasks[location].ss=FP_SEG(stack[location]);
	Tasks[location].sp=FP_OFF(stack[location]+3999);
	Tasks[location].ip=(unsigned)task;
	Tasks[location]._new=1;
	Tasks[location].priority=priority;
	Tasks[location].id=++PIDcount;
	Tasks[location].status=READY;
	disable();
	_SS=Tasks[location].ss;      //load a new stack
	_SP=Tasks[location].sp;      //--+
	enable();                    //  |
	ip=(unsigned)task;           //  +-Just keeping things
								 //	 | ready for the
	asm push cs                  //  | scheduler
	asm push ip                  //--+
	Tasks[location].sp=_SP;

	/*************************************/


	/*****INITIALIZE DATASTRUCTURES******/
	for(i=0;i<MAXTASK;i++)
	{
		if(RQ.ReadyQ[pri][i].p==NULL)
		{
			RQ.ReadyQ[pri][i].p=&Tasks[location];
			if(RQ.start[pri]==-1 && RQ.end[pri]==-1)
			{
				RQ.ReadyQ[pri][i].left=-1;
				RQ.ReadyQ[pri][i].right=-1;
				RQ.start[pri]=i;
			}
			else
			{
				RQ.ReadyQ[pri][i].left=RQ.end[pri];
				RQ.ReadyQ[pri][i].right=-1;
				RQ.ReadyQ[pri][RQ.end[pri]].right=i;
			}
			RQ.end[pri]=i;
			break;
		}
	}
	noOfTasks++;
	//return Tasks[location].id;
	return 0;
}


int DeleteTask(int tid)
{
	int curr=0,prev=-1;
	int pri=0;
	int i=0;

	for(i=0;i<MAXTASK;i++)
	{
		if(Tasks[i].id==tid)
		{
			if(CurrentTask->id == tid)
			{
				Tasks[i].status=UNUSED;
				CurrentTask=NULL;
				scheduler();
			}
			else if(Tasks[i].status==BLOCKED)
			{
				for(curr=BQ.start;curr!=-1;curr=BQ.BlockedQ[curr].right)
				{
					prev=BQ.BlockedQ[curr].left;
					if((BQ.BlockedQ[curr].p)->id == tid)
					{
						if(curr == BQ.start)
						{
							if(BQ.noOfTasks==1)
							{
							BQ.start=-1;
							BQ.end=-1;
							}
							else
							{
							BQ.start=BQ.BlockedQ[BQ.start].right;
							BQ.BlockedQ[curr].right=-1;
							BQ.BlockedQ[BQ.start].left=-1;
							}
						}
						else if(curr == BQ.end)
						{
							BQ.BlockedQ[prev].right=-1;
							BQ.BlockedQ[curr].left=-1;
							BQ.end=prev;
						}
						else
						{
						BQ.BlockedQ[prev].right=BQ.BlockedQ[curr].right;
						BQ.BlockedQ[BQ.BlockedQ[curr].right].left=prev;
						BQ.BlockedQ[curr].right=-1;
						BQ.BlockedQ[curr].left=-1;
						}

					BQ.BlockedQ[curr].p=NULL;
					BQ.noOfTasks--;
					break;
					}
				}
			}
			else if(Tasks[i].status==READY)
			{
				pri=Tasks[i].priority-1;
				for(i=0;i<MAXTASK;i++)
				{
					if((RQ.ReadyQ[pri][i].p)->id==tid)
					{
						if(RQ.start[pri]==i)
						{
							if(RQ.start[pri]==RQ.end[pri])
							{
							RQ.start[pri]=-1;
							RQ.end[pri]=-1;
							RQ.ReadyQ[pri][i].left=-1;
							RQ.ReadyQ[pri][i].right=-1;
							}
							else
							{
							RQ.start[pri]=RQ.ReadyQ[pri][i].right;
							RQ.ReadyQ[pri][i].right=-1;
							RQ.ReadyQ[pri][RQ.start[pri]].left=-1;
							}
						}
						else if(RQ.end[pri]==i)
						{
							RQ.end[pri]=RQ.ReadyQ[pri][i].left;
							RQ.ReadyQ[pri][RQ.end[pri]].right=-1;
						}
						else
						{
							RQ.ReadyQ[pri][RQ.ReadyQ[pri][i].left].right=RQ.ReadyQ[pri][i].right;
							RQ.ReadyQ[pri][RQ.ReadyQ[pri][i].right].left=RQ.ReadyQ[pri][i].left;
							RQ.ReadyQ[pri][i].left=-1;
							RQ.ReadyQ[pri][i].right=-1;
						}

					RQ.ReadyQ[pri][i].p=NULL;
					break;
					}
				}
			}
		Tasks[i].status=UNUSED;
		break;
		}
	}
	if(i == MAXTASK)
	{
		ErrorNo=3;
		return -1;
	}
	return 0;
}

void resume(int tid)
{
	TASK *temp;
	int i=0;
	int curr=0,prev=0;
	int priority;

	for(curr=BQ.start ; curr!=-1 ; curr=BQ.BlockedQ[curr].right)
	{
		prev=BQ.BlockedQ[curr].left;
		if((BQ.BlockedQ[curr].p)->id == tid)
		{
			if(curr == BQ.start)
			{
				if(BQ.noOfTasks==1)
				{
					BQ.start=-1;
					BQ.end=-1;
				}
				else
				{
					BQ.start=BQ.BlockedQ[BQ.start].right;
					BQ.BlockedQ[curr].right=-1;
					BQ.BlockedQ[BQ.start].left=-1;
				}
			}
			else if(curr == BQ.end)
			{
				BQ.BlockedQ[prev].right=-1;
				BQ.BlockedQ[curr].left=-1;
				BQ.end=prev;
			}
			else
			{
				BQ.BlockedQ[prev].right=BQ.BlockedQ[curr].right;
				BQ.BlockedQ[BQ.BlockedQ[curr].right].left=prev;
				BQ.BlockedQ[curr].right=-1;
				BQ.BlockedQ[curr].left=-1;
			}
			temp=BQ.BlockedQ[curr].p;
			BQ.BlockedQ[curr].p=NULL;
			BQ.noOfTasks--;
			break;
		}
      }

	if(curr == -1)
	{
		ErrorNo=4;
		return -1;
	}
	
	priority=(temp->priority)-1;

	for(i=0 ; i<10 ; i++)
	{
		if(RQ.ReadyQ[priority][i].p == NULL)
		{
			RQ.ReadyQ[priority][i].p=temp;
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
				RQ.ReadyQ[priority][i].left=RQ.end[priority];
				RQ.ReadyQ[priority][i].right=-1;
				RQ.ReadyQ[priority][RQ.end[priority]].right=i;
				RQ.end[priority]=i;
				break;
			}
		}
	}	
	scheduler();
}


void _sleep(int TimerTicks)
{
	 int temp,i;	//points to the first free node in the blocked queue
	 int curr=0,prev=0;
	 CurrentTask->timerticks=TimerTicks;
	 CurrentTask->ss=_SS;
	 CurrentTask->sp=_SP;
	 CurrentTask->status=BLOCKED;
	 Isrunning=-1;

	//checks for a free node in the blocked queue
	 for(i=0 ; i<10 ; i++)
	 {
	    if(BQ.BlockedQ[i].p == NULL)
	    {
	     BQ.BlockedQ[i].p=CurrentTask;
	     temp=i;
	     break;
	    }
	 }

	 if(BQ.noOfTasks == 0)
	 {
		 BQ.start=temp;
		 BQ.end=temp;
		 BQ.noOfTasks++;
		 scheduler();
		 return;
	 }



	  /*search for the right place to insert the blocked task so that the queue is sorted
     according to increasing timertick value*/
	 for(curr=BQ.start ; curr!=-1 ; curr=BQ.BlockedQ[curr].right)
	 {
	     prev=BQ.BlockedQ[curr].left;

	     if( (BQ.BlockedQ[curr].p)->timerticks > TimerTicks)
	     {
		 if(curr == BQ.start)
		 {
		 BQ.BlockedQ[temp].left=-1;
		 BQ.BlockedQ[temp].right=BQ.start;
		 BQ.BlockedQ[BQ.start].left=temp;
		 BQ.start=temp;
		 break;
		 }
		 else
		 {
		 BQ.BlockedQ[temp].left=prev;
		 BQ.BlockedQ[temp].right=curr;
		 BQ.BlockedQ[prev].right=temp;
		 BQ.BlockedQ[curr].right=temp;
		 break;
		 }
	      }
	    }

		 //end of the blocked queue
		 if(curr == -1)
		 {
		  BQ.BlockedQ[temp].left=BQ.end;
		  BQ.BlockedQ[temp].right=-1;
		  BQ.BlockedQ[BQ.end].right=temp;
		  BQ.end=temp;
		 }
		 BQ.noOfTasks++;
		 scheduler();

}
