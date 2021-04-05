
#include "table.h"
#include <stdio.h>

phash_table_t
htCreate(size_t size)
{
    phash_table_t pTable;

    if((0 >= size) || (MAX_TABLE_SIZE < size))
    {
        fprintf(stderr,"Invalid size: %d\n", size);
        return NULL;
    }
    pTable = (phash_table_t) calloc(sizeof(hash_table_t), size);

    return pTable;
}

hashKeyComp_t
htKeyComp(hash_key_t *pKey1, hash_key_t *pKey2)
{
    return(memcmp(pKey1, pKey2, sizeof(hash_key_t)));
}

hashFunc_t
htGetHash(hash_key_t *pKey)
{
    if(NULL == pKey)
    {
        fprintf(stderr,"Invalid key\n");
        return -1;
    }
    pGuid = (GUID *) pKey;
    return(int) (pKey->Data1                      ^
                (pKey->Data2 << 16 | pKey->Data3) ^
                 (pKey->Data4[0] << 24 |
                  pKey->Data4[1] << 16 |
                  pKey->Data4[2] << 8  |
                  pKey->Data4[3])                 ^
                 (pKey->Data4[4] << 24 |
                  pKey->Data4[5] << 16 |
                  pKey->Data4[6] << 8  |
                  pKey->Data4[7]));
}

