#include "ll.h"

#define MAXQUEUES 10
#define MAXMESSAGES 10

struct messagequeue {
	int		id;
	int		ownerid;
	list_head	q;
};

int createqueue(int id);
int deletequeue(int id);
int post(int msgqid, char *msg);
char* read(int msgqid);
