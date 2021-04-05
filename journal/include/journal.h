#ifndef _JOURNAL_H_INCLUDED_
#define _JOURNAL_H_INCLUDED_

#include <sys/types.h>

/*
** I would use an enum here but I want to specify the exact size
*/
typedef char jstate_t;
#define JOURNAL_INVALID       0x00
#define JOURNAL_ACTIVE        0x01
#define JOURNAL_COMPLETE      0x02
#define JOURNAL_COMPACTING    0x04

typedef char jlock_t;
#define JOURNAL_LOCK_INVALID  0x00
#define JOURNAL_LOCKED        0x01
#define JOURNAL_UNLOCKED      0x02

typedef struct _journalHeader
{
    union
    {
        struct
        {
            u_int64_t  _jh_startOffset;
            jstate_t   _jh_state;
            jlock_t    _jh_lock;
            u_int64_t  _jh_reserved[4];
        } __journal_header;
        char pad[512];
    } __journal_header_pad;
} journalHeader_t;

#define jh_startOffset   __journal_header_pad.__journal_header._jh_startOffset
#define jh_state         __journal_header_pad.__journal_header._jh_state
#define jh_lock          __journal_header_pad.__journal_header._jh_lock

#endif /* _JOURNAL_H_INCLUDED_ */
