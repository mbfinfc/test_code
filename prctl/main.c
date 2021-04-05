#include <stdio.h>
#include <sys/prctl.h>

int
main(int argc, char **argv)
{
    int i;

    int err = prctl(PR_SET_NAME, "/opt/hp/lwsmd", NULL, NULL, NULL);
    if(err < 0)
    {
        perror("prctl");
        exit(1);
    }

    sleep (15);

    exit (0);
}
