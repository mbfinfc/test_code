#include <sys/types.h>
#include <stdio.h>

typedef char bool;


typedef struct smb_vol {
        char *username;
        char *password;
        char *domainname;
        char *UNC;
        char *iocharset;  /* local code page for mapping to and from Unicode */
        char source_rfc1001_name[128]; /* clnt nb name */
        char target_rfc1001_name[128]; /* srvr nb name */
        uid_t cred_uid;
        uid_t linux_uid;
        gid_t linux_gid;
        uid_t backupuid;
        gid_t backupgid;
        bool sign; /* was signing requested via mnt opts? */
        bool retry:1;
        bool intr:1;
        bool setuids:1;
        bool setuidfromacl:1;
        bool override_uid:1;
        bool override_gid:1;
        bool dynperm:1;
        bool noperm:1;
        bool no_psx_acl:1; /* set if posix acl support should be disabled */
        bool cifs_acl:1;
        bool backupuid_specified; /* mount option  backupuid  is specified */
        bool backupgid_specified; /* mount option  backupgid  is specified */
        bool no_xattr:1;   /* set if xattr (EA) support should be disabled*/
        bool server_ino:1; /* use inode numbers from server ie UniqueId */
        bool direct_io:1;
        bool strict_io:1; /* strict cache behavior */
        bool remap:1;      /* set to remap seven reserved chars in filenames */
        bool sfu_remap:1;  /* remap seven reserved chars ala SFU */
        bool posix_paths:1; /* unset to not ask for posix pathnames. */
        bool no_linux_ext:1;
        bool sfu_emul:1;
        bool nullauth:1;   /* attempt to authenticate with null user */
        bool nocase:1;     /* request case insensitive filenames */
        bool nobrl:1;      /* disable sending byte range locks to srv */
        bool mand_lock:1;  /* send mandatory not posix byte range lock reqs */
        bool seal:1;       /* request transport encryption on share */
        bool nodfs:1;      /* Do not request DFS, even if available */
        bool local_lease:1; /* check leases only on local system, not remote */
        bool noblocksnd:1;
        bool noautotune:1;
        bool nostrictsync:1; /* do not force expensive SMBflush on every sync */
        bool fsc:1;     /* enable fscache */
        bool mfsymlinks:1; /* use Minshall+French Symlinks */
        bool multiuser:1;
        bool rwpidforward:1; /* pid forward for read/write operations */
        bool nosharesock:1;
        bool persistent:1;
        bool nopersistent:1;
        bool resilient:1; /* noresilient not required since not fored for CA */
        bool domainauto:1;
        bool rdma:1;
        unsigned int rsize;
        unsigned int wsize;
        bool sockopt_tcp_nodelay:1;
}smb_vol_t;

int
main(int argc, char **argv)
{
    smb_vol_t vol;

    printf("vol addr is %p\n", &vol);
    return 0;
}
