#include <stdio.h>

#define MAX_RETRIES 2

int
main(int arcg, char **argv)
{
    int retry_count = 0;

    do
    {
        printf("About to sleep, retry_count = %d\n", retry_count);
        sleep(1);
    } while(retry_count++ < MAX_RETRIES);

    return 0;
}
