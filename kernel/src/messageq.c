#include<stdio.h>
#include".\MessageQ.h"

int CreateQueue(int ID)
{
	int i=0;

	if(noOfQueues == MAXQUEUES)
	{
		ErrorNo=9;
		return -1;
	}

	for(i=0 ; i<MAXQUEUES ; i++)
	{
		if(MessageQ[i].status == NOTUSED)
		{
			MessageQ[i].status=USED;
			MessageQ[i].id=ID;
			MessageQ[i].ownerID=CurrentTask->id;

			if(noOfQueues == 0)
			{
				MessageQ[i].right=NULL;
				MessageQ[i].left=NULL;
				MsgQstart=i;
				MsgQend=i;
			}
			else
			{
				MessageQ[i].right=-1;
				MessageQ[i].left=MsgQend;
				MessageQ[MsgQend].right=i;
				MsgQend=i;
			}
			noOfQueues++;
			break;
		}
	}
return 0;
}

int DeleteQueue(int ID)
{
	int i=0;

	if(noOfQueues == 0)
	{
		ErrorNo=10;
		return -1;
	}

	for(i=MsgQstart ; i!=-1 ; i=MessageQ[i].right)
	{
		if(MessageQ[i].id == ID)
		{
			if(MessageQ[i].ownerID == CurrentTask->id)
			{
			if(i == MsgQstart)
			{
				if(noOfQueues == 1)
				{
					MsgQstart=-1;
					MsgQend=-1;
				}
				else
				{
					MsgQstart=MessageQ[i].right;
					MessageQ[MsgQstart].left=-1;
				}
			}
			else if(i == MsgQend)
			{
				MsgQend=MessageQ[i].left;
				MessageQ[MsgQend].right=-1;
			}
			else
			{
				MessageQ[MessageQ[i].left].right=MessageQ[i].right;
				MessageQ[MessageQ[i].right].left=MessageQ[i].left;
				MessageQ[i].left=-1;
				MessageQ[i].right=-1;
			}
			MessageQ[i].status=NOTUSED;
			MessageQ[i].right=-1;
			MessageQ[i].left=-1;
			MessageQ[i].first=NULL;
			MessageQ[i].last=NULL;
			MessageQ[i].noOfMessages=0;
			MessageQ[i].id=-1;
			noOfQueues--;
			break;
			}
			else
			{
				ErrorNo=12;
				return -1;
			}
		}
	}
	if(i == -1)
	{
		ErrorNo=11;
		return -1;
	}

return 0;
}

int Post(int MsgQID,MSG *msg)
{
	int i=0;

	if(noOfQueues == 0)
	{
		ErrorNo=10;
		return -1;
	}

	for(i=MsgQstart ; i!=-1 ; i=MessageQ[i].right)
	{
		if(MessageQ[i].id == MsgQID)
		{
			if(MessageQ[i].noOfMessages == 10)
			{
				ErrorNo=13;
				return-1;
			}
			else
			{
			if(MessageQ[i].noOfMessages == 0)
			{
				msg->left=NULL;
				msg->right=NULL;
				MessageQ[i].first=msg;
				MessageQ[i].last=msg;
			}
			else
			{
				(MessageQ[i].last)->right=msg;
				msg->left=MessageQ[i].last;
				msg->right=NULL;
				MessageQ[i].last=msg;
			}
			MessageQ[i].noOfMessages++;
			break;
			}
		}
	}

	if(i == -1)
	{
		ErrorNo=11;
		return -1;
	}

return 0;
}

MSG* Read(int MsgQID)
{
	int i=0;
	MSG *msg;

	if(noOfQueues == 0)
	{
		ErrorNo=10;
		return NULL;
	}

	for(i=MsgQstart ; i!=-1 ; i=MessageQ[i].right)
	{
		if(MessageQ[i].id == MsgQID)
		{
			if(MessageQ[i].noOfMessages == 0)
			{
			    ErrorNo=14;  
				return NULL;
			}
			else
			{
				msg=MessageQ[i].first;

				if(MessageQ[i].noOfMessages == 1)
				{
					MessageQ[i].first=NULL;
					MessageQ[i].last=NULL;
				}
				else
				{
					MessageQ[i].first=(MessageQ[i].first)->right;
				}
				MessageQ[i].noOfMessages--;
				break;
			}
		}
	}

	if(i == -1)
	{
		ErrorNo=11;
		return NULL;
	}

return msg;
}
