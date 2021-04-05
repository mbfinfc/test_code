#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int
main(int argc, char **argv)
{
    int fd;
    int i;
    int count;
    off_t offset;
    char buf[8192] = {'a'};

    if(argc != 3)
    {
        fprintf(stderr,"Usage %s size file\n", argv[0]);
        exit(-1);
    }

    int size = atoi (argv[1]);

    fd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC | O_SYNC , 0600);
    if(fd < 0)
    {
        perror("open");
        exit(-errno);
    }

    offset = 0;
    for(i = 0; i < 1024 * 100; ++i)
    {
        count = write(fd, buf, size);
        if(count != size)
        {
            perror("write");
            exit(-errno);
        }
    }

    return 0;
}

