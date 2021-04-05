
#include <stdio.h>
#include <sys/types.h>


#define ListSize 32

int freeList[ListSize];
int nextFreeSlot = 0;
int freeCount = ListSize;

void
printFreeList()
{
    int i, j;

    for(i = 0, j = nextFreeSlot; i < freeCount; ++i)
    {
        printf("[%d]: %d ",j, freeList[j]); 
        j = freeList[j];
    }
    putchar('\n');
    return;
}

int main(int argc, char **argv)
{
    int i, j, k;
    char buf[1024];

    int newFree = -1;

    for(i = 0; i < ListSize; ++i)
    {
        freeList [i] = i + 1;
    }
    nextFreeSlot = 0;

    printf("Enter some numbers < %d\n",ListSize);
    while(gets(buf) != NULL)
    {
        j = atoi(buf);
        if(j == 99)
        {
            printFreeList();
            continue;
        }

        /*
        ** Add an entry
        */
        if(j >= ListSize)
        {
            newFree = freeList[nextFreeSlot];
            freeList[nextFreeSlot] = j + 100;
            nextFreeSlot = newFree;
            freeCount--;
        }

        /*
        ** Free an entry
        */
        else
        {
            freeList[j] = nextFreeSlot;
            nextFreeSlot = j;
            freeCount++;
        }
    }
    return 0;
}
