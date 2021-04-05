#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <atomic>

std::atomic<int> foo (0);

int
main(int argc, char **argv)
{
    int a, b, c;
    time_t tt;

    int i;

    a = -3;

    tt = time(NULL);

    if (!(a != -3)) {
        printf("waiting: (!(%d != -3))\n",a);
    }
    else {
        printf("not waiting: (!(%d != -3))\n",a);
    }
    a = ~3;
    if (!(a != -3)) {
        printf("waiting: (!(%d != -3))\n",a);
    }
    else {
        printf("not waiting: (!(%d != -3))\n",a);
    }

    return 0;
}

