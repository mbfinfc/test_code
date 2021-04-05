#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int
main(int argc, char **argv)
{
    int fd;
    int i;

    char *cp;

    if(argv != 2)
    {
        usage();
        return -1;
    }

    fd = open(argv[1], O_RDWR|O_CREAT|O_SYNC, 0664);
    if(fd < 0)
    {
        perror("open");
        return -errno;
    }
    return 0;
}
