
#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdbool.h>
#include <sys/types.h>

typedef struct _GUID {
    u_int32_t Data1;
    u_int16_t Data2;
    u_int16_t Data3;
    u_char_t  Data4[8];
} GUID, *PGUID;

typedef GUID hash_key_t;
typedef void hash_element_t;
typedef int  (*hashFunc_t)    (hash_key_t *key);
typedef bool (*hashKeyComp_t) (hash_key_t *k1, hash_key_t *k2);

#define MAX_TABLE_SIZE    (1024 * 64)

typedef struct _hBucket
{
    hash_key_t        *hb_pKey;
    struct _hBucket   *hb_pNext;
    struct _hBucket   *hb_pPrev;
    hash_element_t    *hb_pElement;
} hash_bucket_t, *phash_bucket_t;

typedef struct _hTable
{
    u_int32_t              ht_entryCount;
    struct hash_bucket_t  *ht_pBucketList; /* my favorite name... */
} hash_table_t, *phash_table_t;

phash_table_t htCreate(size_t size);

hashKeyComp_t htKeyComp;
hashFunc_t    htGetHash;

#endif /* __TABLE_H__ */
