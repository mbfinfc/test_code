#ifndef _PHE_H_INCLUDED_
#define _PHE_H_INCLUDED_

#include <sys/types.h>

typedef enum
{
    PHS_FIELD_FID = 0,
    PHS_FIELD_GUID,
    PHS_FIELD_APP_INST_ID,
    PHS_FIELD_CON_STATE,
    PHS_FIELD_DISC_TIME,
    PHS_FIELD_OPLOCK_INFO,
    PHS_FIELD_LEASE_INFO,
    PHS_FIELD_BRL_LIST_ADD,
    PHS_FIELD_BRL_LIST_DEL,
    PHS_FIELD_OWNER_ID,
    PHS_FIELD_LEASE_EPOCH_INFO

} SRV_PH_FIELD;

typedef struct __SMB2_FID
{
    unsigned long ullPersistentId;
    unsigned long ullVolatileId;
} __attribute__((__packed__)) SMB2_FID, *PSMB2_FID;

typedef struct _GUID {
    u_int32_t Data1;
    u_int16_t Data2;
    u_int16_t Data3;
    u_int8_t  Data4[8];
} GUID, *PGUID;

typedef struct _SRV_OPLOCK_INFO_PHS
{
    u_int8_t  oplockRequest;
    u_int8_t  oplockLevel;
    u_int32_t leaseState;
} SRV_OPLOCK_INFO_PHS, *PSRV_OPLOCK_INFO_PHS;

typedef struct __SMB2_LOCK_PHS
{
    u_int64_t ullFileOffset;
    u_int64_t ullByteRange;
    u_int32_t ulFlags;
    u_int32_t ulSequence;
    u_int32_t ulReserved;
} __attribute__((__packed__)) SMB2_LOCK_PHS, *PSMB2_LOCK_PHS;


typedef struct __SMB2_CREATE_LEASE_PHS
{
    GUID Key;
    u_int32_t State;
    u_int32_t Flags;
    u_int64_t Duration;
    u_int16_t LeaseEpoch;
    GUID ParentKey;
} __attribute__((__packed__)) SMB2_CREATE_LEASE_PHS, *PSMB2_CREATE_LEASE_PHS;

typedef struct _OPERATIONAL_BUCKET
{
    u_int32_t  ulSequenceNumber;
    u_int8_t   bIsInUse;
} OPERATIONAL_BUCKET, *POPERATIONAL_BUCKET;

typedef union  _SRV_PH_FIELD_VAL
{
    SMB2_FID                fid;
    GUID                    CreateGuid;
    u_int64_t               AppInstanceID[2];
    u_int32_t               ConnectionState;
    u_int64_t               DisconnectTime;
    SRV_OPLOCK_INFO_PHS     OplockInfo;
    SMB2_CREATE_LEASE_PHS   LeaseInfo;
    SMB2_LOCK_PHS           PhBrl;
    u_int32_t               OwnerId;
    u_int16_t               LeaseEpoch;
}SRV_PH_FIELD_VAL, *PSRV_PH_FIELD_VAL;

typedef struct _LW_LIST_ENTRY {
    /** @brief Next entry */
    struct _LW_LIST_ENTRY* Next;
    /** @brief Previous entry */
    struct _LW_LIST_ENTRY* Prev;
} LW_LIST_ENTRY, *PLW_LIST_ENTRY;


typedef struct _SRV_PH_OBJ
{
    // Note: This size is the structure + the size of the name string
    int32_t                   size;
    // This is the offset at which the file name starts, basically, the size of structure
    int32_t                   NameOffset;
    u_int32_t                 NodeId;
    int32_t                   refcount;        // Not used currently
    // IDs
    SMB2_FID                  fid;             //We may only want half of this.  persistent half.
    u_int32_t                 ShareAccess;     //SRV_OBJECT_FLAGS??
    GUID                      CreateGuid;
    u_int32_t                 CreateOptions;
    u_int32_t                 CreateDisposition;
    u_int32_t                 DesiredAccess;
    u_int32_t                 FileAttributes;

    unsigned long             AppInstanceID[2];
    u_int32_t                 ConnectionState;

    //Time
    unsigned long             CreateTime;
    unsigned long             DisconnectTime;

    // Locks
    SRV_OPLOCK_INFO_PHS       OplockInfo;
    SMB2_CREATE_LEASE_PHS     LeaseInfo;
    LW_LIST_ENTRY             BrlList;
    u_int64_t                 LockEpoch;

    //0th place is reserved for seq# 0x00
    OPERATIONAL_BUCKET        OperationalBucket[65];
    u_int8_t                  ClientGUID[16];
    u_int64_t                 Reserved[4];
}phobj_t, SRV_PH_OBJ, *PSRV_PH_OBJ;

typedef struct _SRV_PH_UPDATE_INFO
{
    SMB2_FID                fid;
    u_int32_t               size;
    u_int32_t               NameOffset;
    SRV_PH_FIELD            PhsField;
    SRV_PH_FIELD_VAL        PhsFiledVal;
} SRV_PH_UPDATE_INFO, *PSRV_PH_UPDATE_INFO;


#endif /* _PHE_H_INCLUDED_ */
