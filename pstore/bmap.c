#include <sys/types.h>
#include <stdio.h>


// #define MAX_LIST_SIZE 2097151
// #define MAX_LIST_SIZE 1048575
#define MAX_LIST_SIZE 0x4000000 // 1 GB

unsigned char theList[2097151] = {0};

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID, *PGUID;


unsigned long
SrvPhsGuidHash(GUID *pGuid)
{
    unsigned long hash = (pGuid->Data1                       ^
                (pGuid->Data2 << 16 | pGuid->Data3) ^
//                 (pGuid->Data4[0] << 24 |
//                  pGuid->Data4[1] << 16 |
//                  pGuid->Data4[2] << 8  |
//                  pGuid->Data4[3])                  ^
                 (pGuid->Data4[4] << 24 |
                  pGuid->Data4[5] << 16 |
                  pGuid->Data4[6] << 8  |
                  pGuid->Data4[7]));

    return (hash % MAX_LIST_SIZE);
}

int
parseGuid(GUID *pGuid, char *gStr)
{
    int count;
    unsigned short *sp1, *sp2;
    unsigned int *ip;

    sp1 = (unsigned short *)  pGuid->Data4;
    sp2 = (unsigned short *) &pGuid->Data4[2];
    ip  = (unsigned int *)   &pGuid->Data4[4];

    /*
    ** uuidgen ouput looks like this: 9bd6ed79-a004-41de-a307-5abc8a61a71b
    ** This maps onto the GUID like this...
    **                        Data1: 9bd6ed79
    **                        Data2: a004
    **                        Data3: 41de
    **                        Data4[0-1]: a307
    **                        Data4[2-3]: 5abc
    **                        Data4[4-7]: 8a61a71b
    **
    ** The '%*c' directives ignore the '-' characters. The last
    ** 8 bytes are read into the 8 byte Data4 array as two shorts
    ** and an int.
    */
    count = sscanf(gStr,"%x%*c%hx%*c%hx%*c%hx%*c%4hx%x",
        &pGuid->Data1,
        &pGuid->Data2,
        &pGuid->Data3,
        sp1,
        sp2,
        ip);

    if(6 != count)
    {
        fprintf(stderr, "Failed buffer scan! count == %d\n", count);
        exit(-1);
    }

    return count;
}

int
main(int argc, char **argv)
{
    int i;
    char buf[1024];
    GUID guid;
    unsigned long hash;

    while(gets(buf) != NULL)
    {
        parseGuid(&guid, buf);
        hash = SrvPhsGuidHash(&guid);
        printf("%ul\n", hash);
    }
    return;
}
