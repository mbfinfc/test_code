#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../include/phe.h"
#include "../include/journal.h"

const int PHS_HASH_STRKEY_MULTIPLIER = 31;

#undef THIRTY_TWO_BIT_KEY_HASH

#ifdef THIRTY_TWO_BIT_KEY_HASH

const u_int32_t FNV_prime = 16777619;
const u_int32_t FNV_offset_basis = 0x811c9dc5;
u_int32_t KeyHashResult = 0;

#else

const u_int64_t FNV_prime = 1099511628211;
const u_int64_t FNV_offset_basis = 0xcbf29ce484222325;
u_int64_t KeyHashResult = 0;

#endif

const int HASH_BUCKET_COUNT = 4096;

size_t
SrvPhsTableHashKey(void* KeyString)
{
    char * pszPathname = (char *)KeyString;
    char * pszChar = NULL;

#if 0
    KeyHashResult = FNV_offset_basis;

    for (pszChar=pszPathname; pszChar && *pszChar; pszChar++)
    {
        KeyHashResult = KeyHashResult * FNV_prime;
        KeyHashResult = KeyHashResult ^ *pszChar;
    }
#else
    for (pszChar=pszPathname; pszChar && *pszChar; pszChar++)
    {
        KeyHashResult = (PHS_HASH_STRKEY_MULTIPLIER * KeyHashResult) ^ (size_t)*pszChar;
    }
#endif

    return KeyHashResult % HASH_BUCKET_COUNT;
}

phobj_t *
initPHObj(const char *pKey)
{
    size_t objSize = 0;
    phobj_t *pPhobj = NULL;
    const char *command = "/usr/bin/uuidgen";
    FILE *commandFp = NULL;
    int count;
    char buf1[1024];
    char buf2[1024];

    unsigned short *sp1;
    unsigned short *sp2;
    unsigned int   *ip;

    objSize = sizeof(phobj_t) + strlen(pKey) + 1;

    pPhobj = (phobj_t *)malloc(objSize);

    if(NULL == pPhobj)
    {
        perror("initPHObj object allocation");
        return NULL;
    }

    pPhobj->size = objSize;
    pPhobj->NameOffset = sizeof(phobj_t);
    strcpy((char *)((char *)(pPhobj) + pPhobj->NameOffset), pKey);

    commandFp = popen(command, "r");
    if(NULL == commandFp)
    {
        fprintf(stderr, "popen failed!\n");
        exit(-1);
    }

    count = fscanf(commandFp, "%s", buf1);
    pclose(commandFp);

    if(1 != count)
    {
        fprintf(stderr, "Bad popen read! count == %d\n", count);
        exit(-1);
    }

    // Data4 is a uint8_t[8]
    sp1 = (unsigned short *)  pPhobj->CreateGuid.Data4;
    sp2 = (unsigned short *) &pPhobj->CreateGuid.Data4[2];
    ip  = (unsigned int *)   &pPhobj->CreateGuid.Data4[4];

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
    count = sscanf(buf1,"%x%*c%hx%*c%hx%*c%hx%*c%4hx%x",
        &pPhobj->CreateGuid.Data1,
        &pPhobj->CreateGuid.Data2,
        &pPhobj->CreateGuid.Data3,
        sp1,
        sp2,
        ip);

    if(6 != count)
    {
        fprintf(stderr, "Failed buffer scan! count == %d\n", count);
        exit(-1);
    }

    // Sanity checking
    sprintf(buf2,"%8.8x-%4.4hx-%4.4hx-%4.4hx-%4.4hx%8.8x",
        pPhobj->CreateGuid.Data1,
        pPhobj->CreateGuid.Data2,
        pPhobj->CreateGuid.Data3,
        *sp1,
        *sp2,
        *ip);

    if(strcmp(buf1, buf2))
    {
        printf("bad poppen scan/parse\n");
        printf("\tbuf1: %s\n\tbuf2: %s\n", buf1, buf2);
        exit(-1);
    }

    return pPhobj;
}

void
usage(const char *prog)
{
    fprintf(stderr,"Usage: %s journal_file offset\n", prog);
    return;
}

int
main(int argc, char **argv)
{
    int fd;
    int i, j;
    char ch1, ch2, ch3;
    phobj_t phobj = {0};
    journalHeader_t jHeader = {0};
    static char key[1024];

    char *cp;

    if(argc != 3)
    {
        usage(argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDONLY, 0664);
    if(fd < 0)
    {
        perror("open");
        return -errno;
    }

    i = sscanf(argv[2], "%llu", &jHeader.jh_startOffset);

    if(lseek64(fd, jHeader.jh_startOffset, SEEK_SET) < 0)
    {
        perror("lseek: ");
        return -errno;
    }

    i = 0;
    do
    {
        i = read(fd, &phobj, sizeof(phobj));
        printf("Size of phobj is %d\n", phobj.size);
        printf("read %d bytes into phobj\n",i);

        i = read(fd, key, phobj.size - sizeof(phobj));

        printf("Key: %s\tNodeId\t%d\n", key, phobj.NodeId);
        memset(&phobj, 0, sizeof(phobj));
    }while(i > 0);
    return 0;
}
