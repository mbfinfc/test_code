#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>

#define MY_SIG SIGPWR

#define handle_error_en(en, msg) \
       do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
       do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info {    /* Used as argument to thread_start() */
   pthread_t thread_id;        /* ID returned by pthread_create() */
   int       thread_num;       /* Application-defined thread # */
   char     *argv_string;      /* From command-line argument */
};

/* Thread start function: display address near top of our stack,
  and return upper-cased copy of argv_string */

static void *
thread_start(void *arg)
{
   struct thread_info *tinfo = arg;
   int err, sigcount;
   char *uargv, *p;
   sigset_t sset = {0};
   siginfo_t info = {0};
   struct timespec timeout = {0};

   timeout.tv_sec = 3;
   sigcount = 0;

   printf("Thread running\n");

   sigaddset(&sset, MY_SIG);
   pthread_sigmask(SIG_BLOCK, &sset, NULL);

   do
   {
       err =  sigtimedwait(&sset, &info, &timeout);
       if(err > 0)
       {
           printf("Caught signal %d\n",err);
           ++sigcount;
       }
       else
       {
           perror("sigtimedwait");
       }
   } while((err == EAGAIN) || (sigcount < 4));

   return uargv;
}

int
main(int argc, char *argv[])
{
   int s, tnum, opt, num_threads;
   struct thread_info *tinfo;
   pthread_attr_t attr;
   void *res;
   cpu_set_t cpuSet = {0};
   sigset_t sset = {0};


   sigaddset(&sset, MY_SIG);
//   sigaddset(&sset, SIGRTMAX);
   sigprocmask(SIG_BLOCK, &sset, NULL);

   num_threads = 1;

   /* Initialize thread creation attributes */

   s = pthread_attr_init(&attr);
   if (s != 0)
       handle_error_en(s, "pthread_attr_init");

   /* Allocate memory for pthread_create() arguments */

   tinfo = calloc(num_threads, sizeof(struct thread_info));
   if (tinfo == NULL)
       handle_error("calloc");

   /* Create one thread for each command-line argument */

   for (tnum = 0; tnum < num_threads; tnum++) {
       tinfo[tnum].thread_num = tnum + 1;

       /* The pthread_create() call stores the thread ID into
          corresponding element of tinfo[] */

       s = pthread_create(&tinfo[tnum].thread_id, &attr,
                          &thread_start, &tinfo[tnum]);
       if (s != 0)
           handle_error_en(s, "pthread_create");
   }

   s = pthread_attr_destroy(&attr);
   if (s != 0)
       handle_error_en(s, "pthread_attr_destroy");

   /* Now join with each thread, and display its returned value */

   for(tnum = 0; tnum < 5; ++tnum)
   {
       sleep(1);
       printf("sending MY_SIG\n");
       kill(getpid(), MY_SIG);
       kill(getpid(), SIGRTMAX);
   }

   for (tnum = 0; tnum < num_threads; tnum++) {
       s = pthread_join(tinfo[tnum].thread_id, &res);
       if (s != 0)
           handle_error_en(s, "pthread_join");

       printf("Joined with thread %d; returned value was %s\n",
               tinfo[tnum].thread_num, (char *) res);
   }

   free(tinfo);
   exit(EXIT_SUCCESS);
}

