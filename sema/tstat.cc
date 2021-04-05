#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


class TestSema
{
public:
    void *getReq(void);
    static sem_t inSem;

};

class Container
{
public:
    int i;
    TestSema nlServer;
    TestSema tcpServer;
};

sem_t TestSema::inSem ;

int
main (int argc, char **argv)
{
    Container theOne;

    theOne.i = 4;

    sem_init(&TestSema::inSem, 0, 0);
}
