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

#define NUM_C_THREADS	8
#define NUM_P_THREADS	8

#define LAUNCH_THREADS(tArray, count, startFunc) \
{ \
   int i; \
   for(i = 0; i < count; ++i) \
   { \
       pthread_create(&tArray[i], NULL, startFunc, NULL); \
   } \
}
#define JOIN_THREADS(tArray, count) \
{ \
   int i; \
   for(i = 0; i < count; ++i) \
   { \
       pthread_join(tArray[i], NULL); \
   } \
}

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
    int i = 0;
    fprintf(stderr,"alarm sounded, quitting\n");
    while(i < 16)
    {
        sem_post(&qSem);
	++i;
    }

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
	sem_post(&qSem);
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
	if(sem_trywait(&qSem) < 0)
	{
	    ++noWorkCount;
	    sem_wait(&qSem);
	}
	POP(workerQueue, pQe);
	++i;
        usleep(50000);
	if(pQe)
	{
	    free(pQe);
	}
    }
    return;
}

int
bad_function(int a, int b, int c, int d)
{
    char buf[8];
    int i;
    int j;

    for(i = 0, j = 0; i < 64; i++, j--)
    {
        buf[i] = j;
    }

    return -j;
}


int
main(int argc, char *argv[])
{
   int numThreads = 8;
   struct sigaction sa;
   int sleepTime = 15;
   int i;

   pthread_t producerThreads[8], consumerThreads[16];

   sa.sa_handler = handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;

   i = bad_function(1, 2, 3, 4);

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

   LAUNCH_THREADS(consumerThreads, NUM_C_THREADS, consumer);
   LAUNCH_THREADS(producerThreads, NUM_P_THREADS, producer);

   printf("Joining threads here\n");

   JOIN_THREADS(consumerThreads, NUM_C_THREADS);
   JOIN_THREADS(producerThreads, NUM_P_THREADS);

   printf("noWorkCount = %d, totalWork = %d\n", noWorkCount, totalWork);
   return 0;
}
