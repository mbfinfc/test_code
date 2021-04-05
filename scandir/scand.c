
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define EXPECTED_FILE_BASE "phsStore"

int
dirFilter(const struct dirent *pDir)
{
    return !strncmp(EXPECTED_FILE_BASE, pDir->d_name, strlen(EXPECTED_FILE_BASE));
}
    

int
main(int argc, char **argv)
{
    int retVal;
    int i, fd;
    int j;
    struct dirent **nameList = NULL;
    char buf[1024];

    for(i = 1; i < argc; ++i)
    {
        retVal = scandir(argv[i], &nameList, dirFilter, versionsort);
        if(retVal > 0)
        {
            j = 0;
            do
            {
                printf("entry %d: %s\n",j, nameList[j]->d_name);
                sprintf(buf,"%s/%s",argv[i], nameList[j]->d_name);
                fd = open(buf, O_RDWR, 0600);
                if(fd < 0)
                {
                    perror("open");
                }
                else
                    close(fd);
                free(nameList[j++]);
            } while(j < retVal);
            free(nameList);
        }
        else
        {
            perror("Bad directory");
        }
    }
    return 0;
}
            
