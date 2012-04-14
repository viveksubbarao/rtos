#define err1 "No of tasks in the system is already 10"
#define err2 "Priority should be between 1 and 5 inclusive"
#define err3 "No task with the given ID exists in the system"
#define err4 "No task with the specified ID is currently blocked"
#define err5 "NO of mutexes in the system is already 10"
#define err6 "NO mutex has been created for the particular resource"
#define err7 "Currently running task does not hold the mutex"
#define err8 "Currently running task is not the owner of the mutex"
#define err9 "No of message queues in the system is already 10"
#define err10 "No message queue exists in the system"
#define err11 "No message queue with the specified ID exists in the system"
#define err12 "Currently running task is not the owner of the queue"
#define err13 "No of messages in the queue is already 10"
#define err14 "No messages exist in the queue"

int errorno;

void errormsg(int errorno);
