#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

sem_t qSem;
static pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
int running;
int noWorkCount;
int totalWork = 0;

typedef struct _queueElement
{
    int val;
    struct _queueElement *pNext;
} queueElement_t;

typedef struct _queue
{
    int count;
    queueElement_t *pHead;
} queue_t;

queue_t workerQueue;

#define PUSH(q,pE) \
{ \
    pthread_mutex_lock(&queueMutex); \
    if(q.pHead) \
    { \
        pE->pNext = q.pHead;\
    } \
    else \
    { \
	pE->pNext = NULL; \
    } \
    q.pHead = pE; \
    ++q.count; pE->val = q.count;\
    pthread_mutex_unlock(&queueMutex); \
}

#define POP(q,pE) \
{ \
    pthread_mutex_lock(&queueMutex); \
    if(q.pHead) \
    { \
        pE = q.pHead; q.pHead = q.pHead->pNext ; --q.count;\
	++totalWork; \
    } \
    else \
    { \
        pE = NULL; \
    } \
    pthread_mutex_unlock(&queueMutex); \
}

static void
handler(int sig)
{
    running = 0;
    fprintf(stderr,"alarm sounded, quitting\n");
    printf("noWorkCount = %d, totalWork = %d\n", noWorkCount, totalWork);

    return;
}



void *
producer(void *arg)
{
    queueElement_t *pQe;
    int i = 0;
    int j, k;

    while(running)
    {
        pQe = malloc(sizeof(queueElement_t));
	PUSH(workerQueue, pQe);
	++i;
	usleep(50000);
    }
    return;
}

void *
consumer(void *arg)
{
    queueElement_t *pQe;
    int i = 0;
    int j, k;

    while(running)
    {
	POP(workerQueue, pQe);
	++i;
	for (j = 0, k = 0; k < 4096; ++j)
	{
	    k = j * 2 - 20;
	}
	if(pQe)
	{
	    free(pQe);
	    usleep(50000);
	}
	else
	{
	    usleep(5000);
	}
    }
    return;
}


int
main(int argc, char *argv[])
{
   int numThreads = 8;
   struct sigaction sa;
   int sleepTime = 15 ;
   int i;

   pthread_t producerThreads[8], consumerThreads[16];

   sa.sa_handler = handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   if (sigaction(SIGALRM, &sa, NULL) == -1)
   {
       perror("sigaction");
       exit(-1);
   }

   if(argc == 2)
   {
       sleepTime = atoi(argv[1]);
   }

   if (sem_init(&qSem, 0, 0) == -1)
   {
       perror("sem_init");
       exit(-1);
   }

   workerQueue.count = 0;
   workerQueue.pHead = NULL;
   running = 1;
   noWorkCount = 0;
   alarm(sleepTime);

   for(i = 0; i < 8; ++i)
   {
       pthread_create(&consumerThreads[i], NULL, consumer, NULL);
   }
   for(i = 0; i < 4; ++i)
   {
       pthread_create(&producerThreads[i], NULL, producer, NULL);
   }

   printf("Joining threads here\n");

   pthread_join(consumerThreads[0], NULL);
   pthread_join(producerThreads[0], NULL);

   printf("Done, noWorkCount = %d\n", noWorkCount);
   return 0;
}
