typedef unsigned int size_t;
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
__extension__
typedef signed long long int __int64_t;
__extension__
typedef unsigned long long int __uint64_t;
__extension__
typedef long long int __quad_t;
__extension__
typedef unsigned long long int __u_quad_t;
__extension__
typedef __u_quad_t __dev_t;
__extension__
typedef unsigned int __uid_t;
__extension__
typedef unsigned int __gid_t;
__extension__
typedef unsigned long int __ino_t;
__extension__
typedef __u_quad_t __ino64_t;
__extension__
typedef unsigned int __mode_t;
__extension__
typedef unsigned int __nlink_t;
__extension__
typedef long int __off_t;
__extension__
typedef __quad_t __off64_t;
__extension__
typedef int __pid_t;
__extension__
typedef struct 
{
        int __val[2];
} __fsid_t;
__extension__
typedef long int __clock_t;
__extension__
typedef unsigned long int __rlim_t;
__extension__
typedef __u_quad_t __rlim64_t;
__extension__
typedef unsigned int __id_t;
__extension__
typedef long int __time_t;
__extension__
typedef unsigned int __useconds_t;
__extension__
typedef long int __suseconds_t;
__extension__
typedef int __daddr_t;
__extension__
typedef long int __swblk_t;
__extension__
typedef int __key_t;
__extension__
typedef int __clockid_t;
__extension__
typedef void *__timer_t;
__extension__
typedef long int __blksize_t;
__extension__
typedef long int __blkcnt_t;
__extension__
typedef __quad_t __blkcnt64_t;
__extension__
typedef unsigned long int __fsblkcnt_t;
__extension__
typedef __u_quad_t __fsblkcnt64_t;
__extension__
typedef unsigned long int __fsfilcnt_t;
__extension__
typedef __u_quad_t __fsfilcnt64_t;
__extension__
typedef int __ssize_t;
typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;
__extension__
typedef int __intptr_t;
__extension__
typedef unsigned int __socklen_t;
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef struct _IO_FILE __FILE;
typedef struct 
{
        int __count;
        union 
        {
                unsigned int __wch;
                char __wchb[4];
        } __value;
} __mbstate_t;
typedef struct 
{
        __off_t __pos;
        __mbstate_t __state;
} _G_fpos_t;
typedef struct 
{
        __off64_t __pos;
        __mbstate_t __state;
} _G_fpos64_t;
typedef int _G_int16_t __attribute__((__mode__(__HI__)));
typedef int _G_int32_t __attribute__((__mode__(__SI__)));
typedef unsigned int _G_uint16_t __attribute__((__mode__(__HI__)));
typedef unsigned int _G_uint32_t __attribute__((__mode__(__SI__)));
typedef __builtin_va_list __gnuc_va_list;
struct _IO_jump_t;
struct _IO_FILE;
typedef void _IO_lock_t;
struct _IO_marker
{
        struct _IO_marker *_next;
        struct _IO_FILE *_sbuf;
        int _pos;
};
enum __codecvt_result
{
    __codecvt_ok, 
    __codecvt_partial, 
    __codecvt_error, 
    __codecvt_noconv
};
struct _IO_FILE
{
        int _flags;
        char *_IO_read_ptr;
        char *_IO_read_end;
        char *_IO_read_base;
        char *_IO_write_base;
        char *_IO_write_ptr;
        char *_IO_write_end;
        char *_IO_buf_base;
        char *_IO_buf_end;
        char *_IO_save_base;
        char *_IO_backup_base;
        char *_IO_save_end;
        struct _IO_marker *_markers;
        struct _IO_FILE *_chain;
        int _fileno;
        int _flags2;
        __off_t _old_offset;
        unsigned short _cur_column;
        signed char _vtable_offset;
        char _shortbuf[1];
        _IO_lock_t *_lock;
        __off64_t _offset;
        void *__pad1;
        void *__pad2;
        void *__pad3;
        void *__pad4;
        size_t __pad5;
        int _mode;
        char _unused2[15 * sizeof(int) - 4 * sizeof(void *) - sizeof(size_t)];
};
typedef struct _IO_FILE _IO_FILE;
struct _IO_FILE_plus;
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
typedef __ssize_t __io_read_fn(void *__cookie, char *__buf, size_t __nbytes);
typedef __ssize_t __io_write_fn(void *__cookie, __const char *__buf, size_t __n);
typedef int __io_seek_fn(void *__cookie, __off64_t *__pos, int __w);
typedef int __io_close_fn(void *__cookie);
extern int __underflow(_IO_FILE *);
extern int __uflow(_IO_FILE *);
extern int __overflow(_IO_FILE *, int);
extern int _IO_getc(_IO_FILE *__fp);
extern int _IO_putc(int __c, _IO_FILE *__fp);
extern int _IO_feof(_IO_FILE *__fp) __attribute__((__nothrow__, __leaf__));
extern int _IO_ferror(_IO_FILE *__fp) __attribute__((__nothrow__, __leaf__));
extern int _IO_peekc_locked(_IO_FILE *__fp);
extern void _IO_flockfile(_IO_FILE *) __attribute__((__nothrow__, __leaf__));
extern void _IO_funlockfile(_IO_FILE *) __attribute__((__nothrow__, __leaf__));
extern int _IO_ftrylockfile(_IO_FILE *) __attribute__((__nothrow__, __leaf__));
extern int _IO_vfscanf(_IO_FILE *__restrict , const char *__restrict , __gnuc_va_list, int *__restrict );
extern int _IO_vfprintf(_IO_FILE *__restrict , const char *__restrict , __gnuc_va_list);
extern __ssize_t _IO_padn(_IO_FILE *, int, __ssize_t);
extern size_t _IO_sgetn(_IO_FILE *, void *, size_t);
extern __off64_t _IO_seekoff(_IO_FILE *, __off64_t, int, int);
extern __off64_t _IO_seekpos(_IO_FILE *, __off64_t, int);
extern void _IO_free_backup_area(_IO_FILE *) __attribute__((__nothrow__, __leaf__));
typedef __gnuc_va_list va_list;
typedef __off_t off_t;
typedef __ssize_t ssize_t;
typedef _G_fpos_t fpos_t;
extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;
extern int remove(__const char *__filename) __attribute__((__nothrow__, __leaf__));
extern int rename(__const char *__old, __const char *__new) __attribute__((__nothrow__, __leaf__));
extern int renameat(int __oldfd, __const char *__old, int __newfd, __const char *__new) __attribute__((__nothrow__, __leaf__));
extern FILE *tmpfile(void);
extern char *tmpnam(char *__s) __attribute__((__nothrow__, __leaf__));
extern char *tmpnam_r(char *__s) __attribute__((__nothrow__, __leaf__));
extern char *tempnam(__const char *__dir, __const char *__pfx) __attribute__((__nothrow__, __leaf__)) __attribute__((__malloc__));
extern int fclose(FILE *__stream);
extern int fflush(FILE *__stream);
extern int fflush_unlocked(FILE *__stream);
extern FILE *fopen(__const char *__restrict __filename, __const char *__restrict __modes);
extern FILE *freopen(__const char *__restrict __filename, __const char *__restrict __modes, FILE *__restrict __stream);
extern FILE *fdopen(int __fd, __const char *__modes) __attribute__((__nothrow__, __leaf__));
extern FILE *fmemopen(void *__s, size_t __len, __const char *__modes) __attribute__((__nothrow__, __leaf__));
extern FILE *open_memstream(char **__bufloc, size_t *__sizeloc) __attribute__((__nothrow__, __leaf__));
extern void setbuf(FILE *__restrict __stream, char *__restrict __buf) __attribute__((__nothrow__, __leaf__));
extern int setvbuf(FILE *__restrict __stream, char *__restrict __buf, int __modes, size_t __n) __attribute__((__nothrow__, __leaf__));
extern void setbuffer(FILE *__restrict __stream, char *__restrict __buf, size_t __size) __attribute__((__nothrow__, __leaf__));
extern void setlinebuf(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int fprintf(FILE *__restrict __stream, __const char *__restrict __format, ...);
extern int printf(__const char *__restrict __format, ...);
extern int sprintf(char *__restrict __s, __const char *__restrict __format, ...) __attribute__((__nothrow__));
extern int vfprintf(FILE *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg);
extern int vprintf(__const char *__restrict __format, __gnuc_va_list __arg);
extern int vsprintf(char *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __attribute__((__nothrow__));
extern int snprintf(char *__restrict __s, size_t __maxlen, __const char *__restrict __format, ...) __attribute__((__nothrow__)) __attribute__((__format__(__printf__, 3, 4)));
extern int vsnprintf(char *__restrict __s, size_t __maxlen, __const char *__restrict __format, __gnuc_va_list __arg) __attribute__((__nothrow__)) __attribute__((__format__(__printf__, 3, 0)));
extern int vdprintf(int __fd, __const char *__restrict __fmt, __gnuc_va_list __arg) __attribute__((__format__(__printf__, 2, 0)));
extern int dprintf(int __fd, __const char *__restrict __fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
extern int fscanf(FILE *__restrict __stream, __const char *__restrict __format, ...);
extern int scanf(__const char *__restrict __format, ...);
extern int sscanf(__const char *__restrict __s, __const char *__restrict __format, ...) __attribute__((__nothrow__, __leaf__));
extern int fscanf(FILE *__restrict __stream, __const char *__restrict __format, ...) __asm__ ("""__isoc99_fscanf");
extern int scanf(__const char *__restrict __format, ...) __asm__ ("""__isoc99_scanf");
extern int sscanf(__const char *__restrict __s, __const char *__restrict __format, ...) __asm__ ("""__isoc99_sscanf") __attribute__((__nothrow__, __leaf__));
extern int vfscanf(FILE *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __attribute__((__format__(__scanf__, 2, 0)));
extern int vscanf(__const char *__restrict __format, __gnuc_va_list __arg) __attribute__((__format__(__scanf__, 1, 0)));
extern int vsscanf(__const char *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __attribute__((__nothrow__, __leaf__)) __attribute__((__format__(__scanf__, 2, 0)));
extern int vfscanf(FILE *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("""__isoc99_vfscanf") __attribute__((__format__(__scanf__, 2, 0)));
extern int vscanf(__const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("""__isoc99_vscanf") __attribute__((__format__(__scanf__, 1, 0)));
extern int vsscanf(__const char *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("""__isoc99_vsscanf") __attribute__((__nothrow__, __leaf__)) __attribute__((__format__(__scanf__, 2, 0)));
extern int fgetc(FILE *__stream);
extern int getc(FILE *__stream);
extern int getchar(void);
extern int getc_unlocked(FILE *__stream);
extern int getchar_unlocked(void);
extern int fgetc_unlocked(FILE *__stream);
extern int fputc(int __c, FILE *__stream);
extern int putc(int __c, FILE *__stream);
extern int putchar(int __c);
extern int fputc_unlocked(int __c, FILE *__stream);
extern int putc_unlocked(int __c, FILE *__stream);
extern int putchar_unlocked(int __c);
extern int getw(FILE *__stream);
extern int putw(int __w, FILE *__stream);
extern char *fgets(char *__restrict __s, int __n, FILE *__restrict __stream);
extern char *gets(char *__s);
extern __ssize_t __getdelim(char **__restrict __lineptr, size_t *__restrict __n, int __delimiter, FILE *__restrict __stream);
extern __ssize_t getdelim(char **__restrict __lineptr, size_t *__restrict __n, int __delimiter, FILE *__restrict __stream);
extern __ssize_t getline(char **__restrict __lineptr, size_t *__restrict __n, FILE *__restrict __stream);
extern int fputs(__const char *__restrict __s, FILE *__restrict __stream);
extern int puts(__const char *__s);
extern int ungetc(int __c, FILE *__stream);
extern size_t fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);
extern size_t fwrite(__const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __s);
extern size_t fread_unlocked(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);
extern size_t fwrite_unlocked(__const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);
extern int fseek(FILE *__stream, long int __off, int __whence);
extern long int ftell(FILE *__stream);
extern void rewind(FILE *__stream);
extern int fseeko(FILE *__stream, __off_t __off, int __whence);
extern __off_t ftello(FILE *__stream);
extern int fgetpos(FILE *__restrict __stream, fpos_t *__restrict __pos);
extern int fsetpos(FILE *__stream, __const fpos_t *__pos);
extern void clearerr(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int feof(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int ferror(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern void clearerr_unlocked(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int feof_unlocked(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int ferror_unlocked(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern void perror(__const char *__s);
extern int sys_nerr;
extern __const char *__const sys_errlist[];
extern int fileno(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int fileno_unlocked(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern FILE *popen(__const char *__command, __const char *__modes);
extern int pclose(FILE *__stream);
extern char *ctermid(char *__s) __attribute__((__nothrow__, __leaf__));
extern void flockfile(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern int ftrylockfile(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern void funlockfile(FILE *__stream) __attribute__((__nothrow__, __leaf__));
extern void __assert_fail(__const char *__assertion, __const char *__file, unsigned int __line, __const char *__function) __attribute__((__nothrow__, __leaf__)) __attribute__((__noreturn__));
extern void __assert_perror_fail(int __errnum, __const char *__file, unsigned int __line, __const char *__function) __attribute__((__nothrow__, __leaf__)) __attribute__((__noreturn__));
extern void __assert(const char *__assertion, const char *__file, int __line) __attribute__((__nothrow__, __leaf__)) __attribute__((__noreturn__));
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
typedef __loff_t loff_t;
typedef __ino_t ino_t;
typedef __dev_t dev_t;
typedef __gid_t gid_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __uid_t uid_t;
typedef __pid_t pid_t;
typedef __id_t id_t;
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
typedef __key_t key_t;
typedef __clock_t clock_t;
typedef __time_t time_t;
typedef __clockid_t clockid_t;
typedef __timer_t timer_t;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef int int8_t __attribute__((__mode__(__QI__)));
typedef int int16_t __attribute__((__mode__(__HI__)));
typedef int int32_t __attribute__((__mode__(__SI__)));
typedef int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int u_int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int u_int16_t __attribute__((__mode__(__HI__)));
typedef unsigned int u_int32_t __attribute__((__mode__(__SI__)));
typedef unsigned int u_int64_t __attribute__((__mode__(__DI__)));
typedef int register_t __attribute__((__mode__(__word__)));
typedef int __sig_atomic_t;
typedef struct 
{
        unsigned long int __val[(1024 / (8 * sizeof(unsigned long int)))];
} __sigset_t;
typedef __sigset_t sigset_t;
struct timespec
{
        __time_t tv_sec;
        long int tv_nsec;
};
struct timeval
{
        __time_t tv_sec;
        __suseconds_t tv_usec;
};
typedef __suseconds_t suseconds_t;
typedef long int __fd_mask;
typedef struct 
{
        __fd_mask __fds_bits[1024 / (8 * (int) sizeof(__fd_mask))];
} fd_set;
typedef __fd_mask fd_mask;
extern int select(int __nfds, fd_set *__restrict __readfds, fd_set *__restrict __writefds, fd_set *__restrict __exceptfds, struct timeval *__restrict __timeout);
extern int pselect(int __nfds, fd_set *__restrict __readfds, fd_set *__restrict __writefds, fd_set *__restrict __exceptfds, const struct timespec *__restrict __timeout, const __sigset_t *__restrict __sigmask);
__extension__
extern unsigned int gnu_dev_major(unsigned long long int __dev) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
__extension__
extern unsigned int gnu_dev_minor(unsigned long long int __dev) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
__extension__
extern unsigned long long int gnu_dev_makedev(unsigned int __major, unsigned int __minor) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
typedef __blksize_t blksize_t;
typedef __blkcnt_t blkcnt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
typedef unsigned long int pthread_t;
typedef union 
{
        char __size[36];
        long int __align;
} pthread_attr_t;
typedef struct __pthread_internal_slist
{
        struct __pthread_internal_slist *__next;
} __pthread_slist_t;
typedef union 
{
        struct __pthread_mutex_s
        {
                int __lock;
                unsigned int __count;
                int __owner;
                int __kind;
                unsigned int __nusers;
                __extension__
                union 
                {
                        int __spins;
                        __pthread_slist_t __list;
                };
        } __data;
        char __size[24];
        long int __align;
} pthread_mutex_t;
typedef union 
{
        char __size[4];
        long int __align;
} pthread_mutexattr_t;
typedef union 
{
        struct 
        {
                int __lock;
                unsigned int __futex;
                __extension__
                unsigned long long int __total_seq;
                __extension__
                unsigned long long int __wakeup_seq;
                __extension__
                unsigned long long int __woken_seq;
                void *__mutex;
                unsigned int __nwaiters;
                unsigned int __broadcast_seq;
        } __data;
        char __size[48];
        __extension__
        long long int __align;
} pthread_cond_t;
typedef union 
{
        char __size[4];
        long int __align;
} pthread_condattr_t;
typedef unsigned int pthread_key_t;
typedef int pthread_once_t;
typedef union 
{
        struct 
        {
                int __lock;
                unsigned int __nr_readers;
                unsigned int __readers_wakeup;
                unsigned int __writer_wakeup;
                unsigned int __nr_readers_queued;
                unsigned int __nr_writers_queued;
                unsigned char __flags;
                unsigned char __shared;
                unsigned char __pad1;
                unsigned char __pad2;
                int __writer;
        } __data;
        char __size[32];
        long int __align;
} pthread_rwlock_t;
typedef union 
{
        char __size[8];
        long int __align;
} pthread_rwlockattr_t;
typedef volatile int pthread_spinlock_t;
typedef union 
{
        char __size[20];
        long int __align;
} pthread_barrier_t;
typedef union 
{
        char __size[4];
        int __align;
} pthread_barrierattr_t;
struct flock
{
        short int l_type;
        short int l_whence;
        __off_t l_start;
        __off_t l_len;
        __pid_t l_pid;
};
struct stat
{
        __dev_t st_dev;
        unsigned short int __pad1;
        __ino_t st_ino;
        __mode_t st_mode;
        __nlink_t st_nlink;
        __uid_t st_uid;
        __gid_t st_gid;
        __dev_t st_rdev;
        unsigned short int __pad2;
        __off_t st_size;
        __blksize_t st_blksize;
        __blkcnt_t st_blocks;
        struct timespec st_atim;
        struct timespec st_mtim;
        struct timespec st_ctim;
        unsigned long int __unused4;
        unsigned long int __unused5;
};
extern int fcntl(int __fd, int __cmd, ...);
extern int open(__const char *__file, int __oflag, ...) __attribute__((__nonnull__(1)));
extern int openat(int __fd, __const char *__file, int __oflag, ...) __attribute__((__nonnull__(2)));
extern int creat(__const char *__file, __mode_t __mode) __attribute__((__nonnull__(1)));
extern int lockf(int __fd, int __cmd, __off_t __len);
extern int posix_fadvise(int __fd, __off_t __offset, __off_t __len, int __advise) __attribute__((__nothrow__, __leaf__));
extern int posix_fallocate(int __fd, __off_t __offset, __off_t __len);
typedef union sigval
{
        int sival_int;
        void *sival_ptr;
} sigval_t;
typedef struct sigevent
{
        sigval_t sigev_value;
        int sigev_signo;
        int sigev_notify;
        union 
        {
                int _pad[((64 / sizeof(int)) - 3)];
                __pid_t _tid;
                struct 
                {
                        void (*_function)(sigval_t);
                        void *_attribute;
                } _sigev_thread;
        } _sigev_un;
} sigevent_t;
enum 
{
    SIGEV_SIGNAL = 0, 
    SIGEV_NONE, 
    SIGEV_THREAD, 
    SIGEV_THREAD_ID = 4
};
typedef int mqd_t;
struct mq_attr
{
        long int mq_flags;
        long int mq_maxmsg;
        long int mq_msgsize;
        long int mq_curmsgs;
        long int __pad[4];
};
extern mqd_t mq_open(__const char *__name, int __oflag, ...) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int mq_close(mqd_t __mqdes) __attribute__((__nothrow__, __leaf__));
extern int mq_getattr(mqd_t __mqdes, struct mq_attr *__mqstat) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int mq_setattr(mqd_t __mqdes, __const struct mq_attr *__restrict __mqstat, struct mq_attr *__restrict __omqstat) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int mq_unlink(__const char *__name) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int mq_notify(mqd_t __mqdes, __const struct sigevent *__notification) __attribute__((__nothrow__, __leaf__));
extern ssize_t mq_receive(mqd_t __mqdes, char *__msg_ptr, size_t __msg_len, unsigned int *__msg_prio) __attribute__((__nonnull__(2)));
extern int mq_send(mqd_t __mqdes, __const char *__msg_ptr, size_t __msg_len, unsigned int __msg_prio) __attribute__((__nonnull__(2)));
extern ssize_t mq_timedreceive(mqd_t __mqdes, char *__restrict __msg_ptr, size_t __msg_len, unsigned int *__restrict __msg_prio, __const struct timespec *__restrict __abs_timeout) __attribute__((__nonnull__(2, 5)));
extern int mq_timedsend(mqd_t __mqdes, __const char *__msg_ptr, size_t __msg_len, unsigned int __msg_prio, __const struct timespec *__abs_timeout) __attribute__((__nonnull__(2, 5)));
extern int stat(__const char *__restrict __file, struct stat *__restrict __buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int fstat(int __fd, struct stat *__buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int fstatat(int __fd, __const char *__restrict __file, struct stat *__restrict __buf, int __flag) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2, 3)));
extern int lstat(__const char *__restrict __file, struct stat *__restrict __buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int chmod(__const char *__file, __mode_t __mode) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int lchmod(__const char *__file, __mode_t __mode) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int fchmod(int __fd, __mode_t __mode) __attribute__((__nothrow__, __leaf__));
extern int fchmodat(int __fd, __const char *__file, __mode_t __mode, int __flag) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern __mode_t umask(__mode_t __mask) __attribute__((__nothrow__, __leaf__));
extern int mkdir(__const char *__path, __mode_t __mode) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int mkdirat(int __fd, __const char *__path, __mode_t __mode) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int mknod(__const char *__path, __mode_t __mode, __dev_t __dev) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int mknodat(int __fd, __const char *__path, __mode_t __mode, __dev_t __dev) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int mkfifo(__const char *__path, __mode_t __mode) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int mkfifoat(int __fd, __const char *__path, __mode_t __mode) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int utimensat(int __fd, __const char *__path, __const struct timespec __times[2], int __flags) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int futimens(int __fd, __const struct timespec __times[2]) __attribute__((__nothrow__, __leaf__));
extern int __fxstat(int __ver, int __fildes, struct stat *__stat_buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3)));
extern int __xstat(int __ver, __const char *__filename, struct stat *__stat_buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2, 3)));
extern int __lxstat(int __ver, __const char *__filename, struct stat *__stat_buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2, 3)));
extern int __fxstatat(int __ver, int __fildes, __const char *__filename, struct stat *__stat_buf, int __flag) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern int __xmknod(int __ver, __const char *__path, __mode_t __mode, __dev_t *__dev) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2, 4)));
extern int __xmknodat(int __ver, int __fd, __const char *__path, __mode_t __mode, __dev_t *__dev) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 5)));
typedef int ptrdiff_t;
typedef int wchar_t;
typedef ptrdiff_t MPI_Aint;
typedef long long MPI_Offset;
typedef struct ompi_communicator_t *MPI_Comm;
typedef struct ompi_datatype_t *MPI_Datatype;
typedef struct ompi_errhandler_t *MPI_Errhandler;
typedef struct ompi_file_t *MPI_File;
typedef struct ompi_group_t *MPI_Group;
typedef struct ompi_info_t *MPI_Info;
typedef struct ompi_op_t *MPI_Op;
typedef struct ompi_request_t *MPI_Request;
typedef struct ompi_status_public_t MPI_Status;
typedef struct ompi_win_t *MPI_Win;
struct ompi_status_public_t
{
        int MPI_SOURCE;
        int MPI_TAG;
        int MPI_ERROR;
        int _count;
        int _cancelled;
};
typedef struct ompi_status_public_t ompi_status_public_t;
typedef int (MPI_Copy_function)(MPI_Comm, int, void *, void *, void *, int *);
typedef int (MPI_Delete_function)(MPI_Comm, int, void *, void *);
typedef int (MPI_Datarep_extent_function)(MPI_Datatype, MPI_Aint *, void *);
typedef int (MPI_Datarep_conversion_function)(void *, MPI_Datatype, int, void *, MPI_Offset, void *);
typedef void (MPI_Comm_errhandler_fn)(MPI_Comm *, int *, ...);
typedef void (ompi_file_errhandler_fn)(MPI_File *, int *, ...);
typedef ompi_file_errhandler_fn MPI_File_errhandler_fn;
typedef void (MPI_Win_errhandler_fn)(MPI_Win *, int *, ...);
typedef void (MPI_Handler_function)(MPI_Comm *, int *, ...);
typedef void (MPI_User_function)(void *, void *, int *, MPI_Datatype *);
typedef int (MPI_Comm_copy_attr_function)(MPI_Comm, int, void *, void *, void *, int *);
typedef int (MPI_Comm_delete_attr_function)(MPI_Comm, int, void *, void *);
typedef int (MPI_Type_copy_attr_function)(MPI_Datatype, int, void *, void *, void *, int *);
typedef int (MPI_Type_delete_attr_function)(MPI_Datatype, int, void *, void *);
typedef int (MPI_Win_copy_attr_function)(MPI_Win, int, void *, void *, void *, int *);
typedef int (MPI_Win_delete_attr_function)(MPI_Win, int, void *, void *);
typedef int (MPI_Grequest_query_function)(void *, MPI_Status *);
typedef int (MPI_Grequest_free_function)(void *);
typedef int (MPI_Grequest_cancel_function)(void *, int);
enum 
{
    MPI_TAG_UB, 
    MPI_HOST, 
    MPI_IO, 
    MPI_WTIME_IS_GLOBAL, 
    MPI_APPNUM, 
    MPI_LASTUSEDCODE, 
    MPI_UNIVERSE_SIZE, 
    MPI_WIN_BASE, 
    MPI_WIN_SIZE, 
    MPI_WIN_DISP_UNIT, 
    IMPI_CLIENT_SIZE, 
    IMPI_CLIENT_COLOR, 
    IMPI_HOST_SIZE, 
    IMPI_HOST_COLOR
};
enum 
{
    MPI_IDENT, 
    MPI_CONGRUENT, 
    MPI_SIMILAR, 
    MPI_UNEQUAL
};
enum 
{
    MPI_THREAD_SINGLE, 
    MPI_THREAD_FUNNELED, 
    MPI_THREAD_SERIALIZED, 
    MPI_THREAD_MULTIPLE
};
enum 
{
    MPI_COMBINER_NAMED, 
    MPI_COMBINER_DUP, 
    MPI_COMBINER_CONTIGUOUS, 
    MPI_COMBINER_VECTOR, 
    MPI_COMBINER_HVECTOR_INTEGER, 
    MPI_COMBINER_HVECTOR, 
    MPI_COMBINER_INDEXED, 
    MPI_COMBINER_HINDEXED_INTEGER, 
    MPI_COMBINER_HINDEXED, 
    MPI_COMBINER_INDEXED_BLOCK, 
    MPI_COMBINER_STRUCT_INTEGER, 
    MPI_COMBINER_STRUCT, 
    MPI_COMBINER_SUBARRAY, 
    MPI_COMBINER_DARRAY, 
    MPI_COMBINER_F90_REAL, 
    MPI_COMBINER_F90_COMPLEX, 
    MPI_COMBINER_F90_INTEGER, 
    MPI_COMBINER_RESIZED
};
__attribute__((visibility("default"))) int OMPI_C_MPI_TYPE_NULL_DELETE_FN(MPI_Datatype datatype, int type_keyval, void *attribute_val_out, void *extra_state);
__attribute__((visibility("default"))) int OMPI_C_MPI_TYPE_NULL_COPY_FN(MPI_Datatype datatype, int type_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_TYPE_DUP_FN(MPI_Datatype datatype, int type_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_COMM_NULL_DELETE_FN(MPI_Comm comm, int comm_keyval, void *attribute_val_out, void *extra_state);
__attribute__((visibility("default"))) int OMPI_C_MPI_COMM_NULL_COPY_FN(MPI_Comm comm, int comm_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_COMM_DUP_FN(MPI_Comm comm, int comm_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_NULL_DELETE_FN(MPI_Comm comm, int comm_keyval, void *attribute_val_out, void *extra_state);
__attribute__((visibility("default"))) int OMPI_C_MPI_NULL_COPY_FN(MPI_Comm comm, int comm_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_DUP_FN(MPI_Comm comm, int comm_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_WIN_NULL_DELETE_FN(MPI_Win window, int win_keyval, void *attribute_val_out, void *extra_state);
__attribute__((visibility("default"))) int OMPI_C_MPI_WIN_NULL_COPY_FN(MPI_Win window, int win_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) int OMPI_C_MPI_WIN_DUP_FN(MPI_Win window, int win_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
__attribute__((visibility("default"))) extern struct ompi_predefined_communicator_t ompi_mpi_comm_world;
__attribute__((visibility("default"))) extern struct ompi_predefined_communicator_t ompi_mpi_comm_self;
__attribute__((visibility("default"))) extern struct ompi_predefined_communicator_t ompi_mpi_comm_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_group_t ompi_mpi_group_empty;
__attribute__((visibility("default"))) extern struct ompi_predefined_group_t ompi_mpi_group_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_request_t ompi_request_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_max, ompi_mpi_op_min;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_sum;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_prod;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_land;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_band;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_lor, ompi_mpi_op_bor;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_lxor;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_bxor;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_maxloc;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_minloc;
__attribute__((visibility("default"))) extern struct ompi_predefined_op_t ompi_mpi_op_replace;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_char, ompi_mpi_byte;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_int, ompi_mpi_logic;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_short, ompi_mpi_long;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_float, ompi_mpi_double;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_long_double;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_cplex, ompi_mpi_packed;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_signed_char;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_unsigned_char;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_unsigned_short;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_unsigned, ompi_mpi_datatype_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_unsigned_long, ompi_mpi_ldblcplex;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_ub, ompi_mpi_lb;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_float_int, ompi_mpi_double_int;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_long_int, ompi_mpi_2int;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_short_int, ompi_mpi_dblcplex;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_integer, ompi_mpi_real;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_dblprec, ompi_mpi_character;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_2real, ompi_mpi_2dblprec;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_2integer, ompi_mpi_longdbl_int;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_wchar, ompi_mpi_long_long_int;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_unsigned_long_long;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_cxx_cplex, ompi_mpi_cxx_dblcplex;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_cxx_ldblcplex;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_cxx_bool;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_2cplex, ompi_mpi_2dblcplex;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_logical1;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_logical2;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_logical4;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_logical8;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_integer1;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_integer2;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_integer4;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_integer8;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_real4;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_real8;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_complex8;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_complex16;
__attribute__((visibility("default"))) extern struct ompi_predefined_errhandler_t ompi_mpi_errhandler_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_errhandler_t ompi_mpi_errors_are_fatal;
__attribute__((visibility("default"))) extern struct ompi_predefined_errhandler_t ompi_mpi_errors_return;
__attribute__((visibility("default"))) extern struct ompi_predefined_win_t ompi_mpi_win_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_file_t ompi_mpi_file_null;
__attribute__((visibility("default"))) extern struct ompi_predefined_info_t ompi_mpi_info_null;
__attribute__((visibility("default"))) extern int *MPI_F_STATUS_IGNORE;
__attribute__((visibility("default"))) extern int *MPI_F_STATUSES_IGNORE;
__attribute__((visibility("default"))) int MPI_Abort(MPI_Comm comm, int errorcode);
__attribute__((visibility("default"))) int MPI_Accumulate(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Add_error_class(int *errorclass);
__attribute__((visibility("default"))) int MPI_Add_error_code(int errorclass, int *errorcode);
__attribute__((visibility("default"))) int MPI_Add_error_string(int errorcode, char *string);
__attribute__((visibility("default"))) int MPI_Address(void *location, MPI_Aint *address);
__attribute__((visibility("default"))) int MPI_Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Allgatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
__attribute__((visibility("default"))) int MPI_Allreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Alltoall(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Alltoallv(void *sendbuf, int *sendcounts, int *sdispls, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *rdispls, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Alltoallw(void *sendbuf, int *sendcounts, int *sdispls, MPI_Datatype *sendtypes, void *recvbuf, int *recvcounts, int *rdispls, MPI_Datatype *recvtypes, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Attr_delete(MPI_Comm comm, int keyval);
__attribute__((visibility("default"))) int MPI_Attr_get(MPI_Comm comm, int keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int MPI_Attr_put(MPI_Comm comm, int keyval, void *attribute_val);
__attribute__((visibility("default"))) int MPI_Barrier(MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Bsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Bsend_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Buffer_attach(void *buffer, int size);
__attribute__((visibility("default"))) int MPI_Buffer_detach(void *buffer, int *size);
__attribute__((visibility("default"))) int MPI_Cancel(MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int *coords);
__attribute__((visibility("default"))) int MPI_Cart_create(MPI_Comm old_comm, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart);
__attribute__((visibility("default"))) int MPI_Cart_get(MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords);
__attribute__((visibility("default"))) int MPI_Cart_map(MPI_Comm comm, int ndims, int *dims, int *periods, int *newrank);
__attribute__((visibility("default"))) int MPI_Cart_rank(MPI_Comm comm, int *coords, int *rank);
__attribute__((visibility("default"))) int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
__attribute__((visibility("default"))) int MPI_Cart_sub(MPI_Comm comm, int *remain_dims, MPI_Comm *new_comm);
__attribute__((visibility("default"))) int MPI_Cartdim_get(MPI_Comm comm, int *ndims);
__attribute__((visibility("default"))) int MPI_Close_port(char *port_name);
__attribute__((visibility("default"))) int MPI_Comm_accept(char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int MPI_Comm_c2f(MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Comm_call_errhandler(MPI_Comm comm, int errorcode);
__attribute__((visibility("default"))) int MPI_Comm_compare(MPI_Comm comm1, MPI_Comm comm2, int *result);
__attribute__((visibility("default"))) int MPI_Comm_connect(char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int MPI_Comm_create_errhandler(MPI_Comm_errhandler_fn *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_Comm_create_keyval(MPI_Comm_copy_attr_function *comm_copy_attr_fn, MPI_Comm_delete_attr_function *comm_delete_attr_fn, int *comm_keyval, void *extra_state);
__attribute__((visibility("default"))) int MPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int MPI_Comm_delete_attr(MPI_Comm comm, int comm_keyval);
__attribute__((visibility("default"))) int MPI_Comm_disconnect(MPI_Comm *comm);
__attribute__((visibility("default"))) int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);
__attribute__((visibility("default"))) MPI_Comm MPI_Comm_f2c(int comm);
__attribute__((visibility("default"))) int MPI_Comm_free_keyval(int *comm_keyval);
__attribute__((visibility("default"))) int MPI_Comm_free(MPI_Comm *comm);
__attribute__((visibility("default"))) int MPI_Comm_get_attr(MPI_Comm comm, int comm_keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int MPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *erhandler);
__attribute__((visibility("default"))) int MPI_Comm_get_name(MPI_Comm comm, char *comm_name, int *resultlen);
__attribute__((visibility("default"))) int MPI_Comm_get_parent(MPI_Comm *parent);
__attribute__((visibility("default"))) int MPI_Comm_group(MPI_Comm comm, MPI_Group *group);
__attribute__((visibility("default"))) int MPI_Comm_join(int fd, MPI_Comm *intercomm);
__attribute__((visibility("default"))) int MPI_Comm_rank(MPI_Comm comm, int *rank);
__attribute__((visibility("default"))) int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);
__attribute__((visibility("default"))) int MPI_Comm_remote_size(MPI_Comm comm, int *size);
__attribute__((visibility("default"))) int MPI_Comm_set_attr(MPI_Comm comm, int comm_keyval, void *attribute_val);
__attribute__((visibility("default"))) int MPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int MPI_Comm_set_name(MPI_Comm comm, char *comm_name);
__attribute__((visibility("default"))) int MPI_Comm_size(MPI_Comm comm, int *size);
__attribute__((visibility("default"))) int MPI_Comm_spawn(char *command, char **argv, int maxprocs, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *intercomm, int *array_of_errcodes);
__attribute__((visibility("default"))) int MPI_Comm_spawn_multiple(int count, char **array_of_commands, char ***array_of_argv, int *array_of_maxprocs, MPI_Info *array_of_info, int root, MPI_Comm comm, MPI_Comm *intercomm, int *array_of_errcodes);
__attribute__((visibility("default"))) int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int MPI_Comm_test_inter(MPI_Comm comm, int *flag);
__attribute__((visibility("default"))) int MPI_Dims_create(int nnodes, int ndims, int *dims);
__attribute__((visibility("default"))) int MPI_Errhandler_c2f(MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int MPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) MPI_Errhandler MPI_Errhandler_f2c(int errhandler);
__attribute__((visibility("default"))) int MPI_Errhandler_free(MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int MPI_Error_class(int errorcode, int *errorclass);
__attribute__((visibility("default"))) int MPI_Error_string(int errorcode, char *string, int *resultlen);
__attribute__((visibility("default"))) int MPI_Exscan(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_File_c2f(MPI_File file);
__attribute__((visibility("default"))) MPI_File MPI_File_f2c(int file);
__attribute__((visibility("default"))) int MPI_File_call_errhandler(MPI_File fh, int errorcode);
__attribute__((visibility("default"))) int MPI_File_create_errhandler(MPI_File_errhandler_fn *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_File_set_errhandler(MPI_File file, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int MPI_File_get_errhandler(MPI_File file, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_File_open(MPI_Comm comm, char *filename, int amode, MPI_Info info, MPI_File *fh);
__attribute__((visibility("default"))) int MPI_File_close(MPI_File *fh);
__attribute__((visibility("default"))) int MPI_File_delete(char *filename, MPI_Info info);
__attribute__((visibility("default"))) int MPI_File_set_size(MPI_File fh, MPI_Offset size);
__attribute__((visibility("default"))) int MPI_File_preallocate(MPI_File fh, MPI_Offset size);
__attribute__((visibility("default"))) int MPI_File_get_size(MPI_File fh, MPI_Offset *size);
__attribute__((visibility("default"))) int MPI_File_get_group(MPI_File fh, MPI_Group *group);
__attribute__((visibility("default"))) int MPI_File_get_amode(MPI_File fh, int *amode);
__attribute__((visibility("default"))) int MPI_File_set_info(MPI_File fh, MPI_Info info);
__attribute__((visibility("default"))) int MPI_File_get_info(MPI_File fh, MPI_Info *info_used);
__attribute__((visibility("default"))) int MPI_File_set_view(MPI_File fh, MPI_Offset disp, MPI_Datatype etype, MPI_Datatype filetype, char *datarep, MPI_Info info);
__attribute__((visibility("default"))) int MPI_File_get_view(MPI_File fh, MPI_Offset *disp, MPI_Datatype *etype, MPI_Datatype *filetype, char *datarep);
__attribute__((visibility("default"))) int MPI_File_read_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_iread_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_File_iwrite_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_File_read(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_read_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_iread(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_File_iwrite(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_File_seek(MPI_File fh, MPI_Offset offset, int whence);
__attribute__((visibility("default"))) int MPI_File_get_position(MPI_File fh, MPI_Offset *offset);
__attribute__((visibility("default"))) int MPI_File_get_byte_offset(MPI_File fh, MPI_Offset offset, MPI_Offset *disp);
__attribute__((visibility("default"))) int MPI_File_read_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_iread_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_File_iwrite_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_File_read_ordered(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_ordered(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_seek_shared(MPI_File fh, MPI_Offset offset, int whence);
__attribute__((visibility("default"))) int MPI_File_get_position_shared(MPI_File fh, MPI_Offset *offset);
__attribute__((visibility("default"))) int MPI_File_read_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_File_read_at_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_File_write_at_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_read_all_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_File_read_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_all_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_File_write_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_read_ordered_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_File_read_ordered_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_write_ordered_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_File_write_ordered_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_File_get_type_extent(MPI_File fh, MPI_Datatype datatype, MPI_Aint *extent);
__attribute__((visibility("default"))) int MPI_File_set_atomicity(MPI_File fh, int flag);
__attribute__((visibility("default"))) int MPI_File_get_atomicity(MPI_File fh, int *flag);
__attribute__((visibility("default"))) int MPI_File_sync(MPI_File fh);
__attribute__((visibility("default"))) int MPI_Finalize(void);
__attribute__((visibility("default"))) int MPI_Finalized(int *flag);
__attribute__((visibility("default"))) int MPI_Free_mem(void *base);
__attribute__((visibility("default"))) int MPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Gatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Get_address(void *location, MPI_Aint *address);
__attribute__((visibility("default"))) int MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);
__attribute__((visibility("default"))) int MPI_Get_elements(MPI_Status *status, MPI_Datatype datatype, int *count);
__attribute__((visibility("default"))) int MPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Get_processor_name(char *name, int *resultlen);
__attribute__((visibility("default"))) int MPI_Get_version(int *version, int *subversion);
__attribute__((visibility("default"))) int MPI_Graph_create(MPI_Comm comm_old, int nnodes, int *index, int *edges, int reorder, MPI_Comm *comm_graph);
__attribute__((visibility("default"))) int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges);
__attribute__((visibility("default"))) int MPI_Graph_map(MPI_Comm comm, int nnodes, int *index, int *edges, int *newrank);
__attribute__((visibility("default"))) int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);
__attribute__((visibility("default"))) int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int *neighbors);
__attribute__((visibility("default"))) int MPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);
__attribute__((visibility("default"))) int MPI_Grequest_complete(MPI_Request request);
__attribute__((visibility("default"))) int MPI_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, void *extra_state, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Group_c2f(MPI_Group group);
__attribute__((visibility("default"))) int MPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result);
__attribute__((visibility("default"))) int MPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
__attribute__((visibility("default"))) int MPI_Group_excl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);
__attribute__((visibility("default"))) MPI_Group MPI_Group_f2c(int group);
__attribute__((visibility("default"))) int MPI_Group_free(MPI_Group *group);
__attribute__((visibility("default"))) int MPI_Group_incl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);
__attribute__((visibility("default"))) int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
__attribute__((visibility("default"))) int MPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
__attribute__((visibility("default"))) int MPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
__attribute__((visibility("default"))) int MPI_Group_rank(MPI_Group group, int *rank);
__attribute__((visibility("default"))) int MPI_Group_size(MPI_Group group, int *size);
__attribute__((visibility("default"))) int MPI_Group_translate_ranks(MPI_Group group1, int n, int *ranks1, MPI_Group group2, int *ranks2);
__attribute__((visibility("default"))) int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
__attribute__((visibility("default"))) int MPI_Ibsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Info_c2f(MPI_Info info);
__attribute__((visibility("default"))) int MPI_Info_create(MPI_Info *info);
__attribute__((visibility("default"))) int MPI_Info_delete(MPI_Info info, char *key);
__attribute__((visibility("default"))) int MPI_Info_dup(MPI_Info info, MPI_Info *newinfo);
__attribute__((visibility("default"))) MPI_Info MPI_Info_f2c(int info);
__attribute__((visibility("default"))) int MPI_Info_free(MPI_Info *info);
__attribute__((visibility("default"))) int MPI_Info_get(MPI_Info info, char *key, int valuelen, char *value, int *flag);
__attribute__((visibility("default"))) int MPI_Info_get_nkeys(MPI_Info info, int *nkeys);
__attribute__((visibility("default"))) int MPI_Info_get_nthkey(MPI_Info info, int n, char *key);
__attribute__((visibility("default"))) int MPI_Info_get_valuelen(MPI_Info info, char *key, int *valuelen, int *flag);
__attribute__((visibility("default"))) int MPI_Info_set(MPI_Info info, char *key, char *value);
__attribute__((visibility("default"))) int MPI_Init(int *argc, char ***argv);
__attribute__((visibility("default"))) int MPI_Initialized(int *flag);
__attribute__((visibility("default"))) int MPI_Init_thread(int *argc, char ***argv, int required, int *provided);
__attribute__((visibility("default"))) int MPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm bridge_comm, int remote_leader, int tag, MPI_Comm *newintercomm);
__attribute__((visibility("default"))) int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintercomm);
__attribute__((visibility("default"))) int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Irsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Issend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Is_thread_main(int *flag);
__attribute__((visibility("default"))) int MPI_Keyval_create(MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state);
__attribute__((visibility("default"))) int MPI_Keyval_free(int *keyval);
__attribute__((visibility("default"))) int MPI_Lookup_name(char *service_name, MPI_Info info, char *port_name);
__attribute__((visibility("default"))) int MPI_Op_c2f(MPI_Op op);
__attribute__((visibility("default"))) int MPI_Op_create(MPI_User_function *function, int commute, MPI_Op *op);
__attribute__((visibility("default"))) int MPI_Open_port(MPI_Info info, char *port_name);
__attribute__((visibility("default"))) MPI_Op MPI_Op_f2c(int op);
__attribute__((visibility("default"))) int MPI_Op_free(MPI_Op *op);
__attribute__((visibility("default"))) int MPI_Pack_external(char *datarep, void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, MPI_Aint outsize, MPI_Aint *position);
__attribute__((visibility("default"))) int MPI_Pack_external_size(char *datarep, int incount, MPI_Datatype datatype, MPI_Aint *size);
__attribute__((visibility("default"))) int MPI_Pack(void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size);
__attribute__((visibility("default"))) int MPI_Pcontrol(const int level, ...);
__attribute__((visibility("default"))) int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Publish_name(char *service_name, MPI_Info info, char *port_name);
__attribute__((visibility("default"))) int MPI_Put(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Query_thread(int *provided);
__attribute__((visibility("default"))) int MPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Reduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Reduce_scatter(void *sendbuf, void *recvbuf, int *recvcounts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Register_datarep(char *datarep, MPI_Datarep_conversion_function *read_conversion_fn, MPI_Datarep_conversion_function *write_conversion_fn, MPI_Datarep_extent_function *dtype_file_extent_fn, void *extra_state);
__attribute__((visibility("default"))) int MPI_Request_c2f(MPI_Request request);
__attribute__((visibility("default"))) MPI_Request MPI_Request_f2c(int request);
__attribute__((visibility("default"))) int MPI_Request_free(MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Request_get_status(MPI_Request request, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Rsend(void *ibuf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Rsend_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Scan(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Scatterv(void *sendbuf, int *sendcounts, int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Send_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Ssend_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Ssend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Start(MPI_Request *request);
__attribute__((visibility("default"))) int MPI_Startall(int count, MPI_Request *array_of_requests);
__attribute__((visibility("default"))) int MPI_Status_c2f(MPI_Status *c_status, int *f_status);
__attribute__((visibility("default"))) int MPI_Status_f2c(int *f_status, MPI_Status *c_status);
__attribute__((visibility("default"))) int MPI_Status_set_cancelled(MPI_Status *status, int flag);
__attribute__((visibility("default"))) int MPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype, int count);
__attribute__((visibility("default"))) int MPI_Testall(int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[]);
__attribute__((visibility("default"))) int MPI_Testany(int count, MPI_Request array_of_requests[], int *index, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Test_cancelled(MPI_Status *status, int *flag);
__attribute__((visibility("default"))) int MPI_Testsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]);
__attribute__((visibility("default"))) int MPI_Topo_test(MPI_Comm comm, int *status);
__attribute__((visibility("default"))) int MPI_Type_c2f(MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_Type_commit(MPI_Datatype *type);
__attribute__((visibility("default"))) int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_darray(int size, int rank, int ndims, int gsize_array[], int distrib_array[], int darg_array[], int psize_array[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_f90_complex(int p, int r, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_f90_integer(int r, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_f90_real(int p, int r, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_hindexed(int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_keyval(MPI_Type_copy_attr_function *type_copy_attr_fn, MPI_Type_delete_attr_function *type_delete_attr_fn, int *type_keyval, void *extra_state);
__attribute__((visibility("default"))) int MPI_Type_create_indexed_block(int count, int blocklength, int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_struct(int count, int array_of_block_lengths[], MPI_Aint array_of_displacements[], MPI_Datatype array_of_types[], MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_subarray(int ndims, int size_array[], int subsize_array[], int start_array[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_delete_attr(MPI_Datatype type, int type_keyval);
__attribute__((visibility("default"))) int MPI_Type_dup(MPI_Datatype type, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_extent(MPI_Datatype type, MPI_Aint *extent);
__attribute__((visibility("default"))) int MPI_Type_free(MPI_Datatype *type);
__attribute__((visibility("default"))) int MPI_Type_free_keyval(int *type_keyval);
__attribute__((visibility("default"))) MPI_Datatype MPI_Type_f2c(int datatype);
__attribute__((visibility("default"))) int MPI_Type_get_attr(MPI_Datatype type, int type_keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int MPI_Type_get_contents(MPI_Datatype mtype, int max_integers, int max_addresses, int max_datatypes, int array_of_integers[], MPI_Aint array_of_addresses[], MPI_Datatype array_of_datatypes[]);
__attribute__((visibility("default"))) int MPI_Type_get_envelope(MPI_Datatype type, int *num_integers, int *num_addresses, int *num_datatypes, int *combiner);
__attribute__((visibility("default"))) int MPI_Type_get_extent(MPI_Datatype type, MPI_Aint *lb, MPI_Aint *extent);
__attribute__((visibility("default"))) int MPI_Type_get_name(MPI_Datatype type, char *type_name, int *resultlen);
__attribute__((visibility("default"))) int MPI_Type_get_true_extent(MPI_Datatype datatype, MPI_Aint *true_lb, MPI_Aint *true_extent);
__attribute__((visibility("default"))) int MPI_Type_hindexed(int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_indexed(int count, int array_of_blocklengths[], int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_lb(MPI_Datatype type, MPI_Aint *lb);
__attribute__((visibility("default"))) int MPI_Type_match_size(int typeclass, int size, MPI_Datatype *type);
__attribute__((visibility("default"))) int MPI_Type_set_attr(MPI_Datatype type, int type_keyval, void *attr_val);
__attribute__((visibility("default"))) int MPI_Type_set_name(MPI_Datatype type, char *type_name);
__attribute__((visibility("default"))) int MPI_Type_size(MPI_Datatype type, int *size);
__attribute__((visibility("default"))) int MPI_Type_struct(int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype array_of_types[], MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Type_ub(MPI_Datatype mtype, MPI_Aint *ub);
__attribute__((visibility("default"))) int MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int MPI_Unpack(void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm);
__attribute__((visibility("default"))) int MPI_Unpublish_name(char *service_name, MPI_Info info, char *port_name);
__attribute__((visibility("default"))) int MPI_Unpack_external(char *datarep, void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype);
__attribute__((visibility("default"))) int MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses);
__attribute__((visibility("default"))) int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Wait(MPI_Request *request, MPI_Status *status);
__attribute__((visibility("default"))) int MPI_Waitsome(int incount, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses);
__attribute__((visibility("default"))) int MPI_Win_c2f(MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_call_errhandler(MPI_Win win, int errorcode);
__attribute__((visibility("default"))) int MPI_Win_complete(MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_create(void *base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, MPI_Win *win);
__attribute__((visibility("default"))) int MPI_Win_create_errhandler(MPI_Win_errhandler_fn *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_Win_create_keyval(MPI_Win_copy_attr_function *win_copy_attr_fn, MPI_Win_delete_attr_function *win_delete_attr_fn, int *win_keyval, void *extra_state);
__attribute__((visibility("default"))) int MPI_Win_delete_attr(MPI_Win win, int win_keyval);
__attribute__((visibility("default"))) MPI_Win MPI_Win_f2c(int win);
__attribute__((visibility("default"))) int MPI_Win_fence(int assert, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_free(MPI_Win *win);
__attribute__((visibility("default"))) int MPI_Win_free_keyval(int *win_keyval);
__attribute__((visibility("default"))) int MPI_Win_get_attr(MPI_Win win, int win_keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int MPI_Win_get_errhandler(MPI_Win win, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int MPI_Win_get_group(MPI_Win win, MPI_Group *group);
__attribute__((visibility("default"))) int MPI_Win_get_name(MPI_Win win, char *win_name, int *resultlen);
__attribute__((visibility("default"))) int MPI_Win_lock(int lock_type, int rank, int assert, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_post(MPI_Group group, int assert, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_set_attr(MPI_Win win, int win_keyval, void *attribute_val);
__attribute__((visibility("default"))) int MPI_Win_set_errhandler(MPI_Win win, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int MPI_Win_set_name(MPI_Win win, char *win_name);
__attribute__((visibility("default"))) int MPI_Win_start(MPI_Group group, int assert, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_test(MPI_Win win, int *flag);
__attribute__((visibility("default"))) int MPI_Win_unlock(int rank, MPI_Win win);
__attribute__((visibility("default"))) int MPI_Win_wait(MPI_Win win);
__attribute__((visibility("default"))) double MPI_Wtick(void);
__attribute__((visibility("default"))) double MPI_Wtime(void);
__attribute__((visibility("default"))) int PMPI_Abort(MPI_Comm comm, int errorcode);
__attribute__((visibility("default"))) int PMPI_Accumulate(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Add_error_class(int *errorclass);
__attribute__((visibility("default"))) int PMPI_Add_error_code(int errorclass, int *errorcode);
__attribute__((visibility("default"))) int PMPI_Add_error_string(int errorcode, char *string);
__attribute__((visibility("default"))) int PMPI_Address(void *location, MPI_Aint *address);
__attribute__((visibility("default"))) int PMPI_Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Allgatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
__attribute__((visibility("default"))) int PMPI_Allreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Alltoall(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Alltoallv(void *sendbuf, int *sendcounts, int *sdispls, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *rdispls, MPI_Datatype recvtype, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Alltoallw(void *sendbuf, int *sendcounts, int *sdispls, MPI_Datatype *sendtypes, void *recvbuf, int *recvcounts, int *rdispls, MPI_Datatype *recvtypes, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Attr_delete(MPI_Comm comm, int keyval);
__attribute__((visibility("default"))) int PMPI_Attr_get(MPI_Comm comm, int keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int PMPI_Attr_put(MPI_Comm comm, int keyval, void *attribute_val);
__attribute__((visibility("default"))) int PMPI_Barrier(MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Bsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Bsend_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Buffer_attach(void *buffer, int size);
__attribute__((visibility("default"))) int PMPI_Buffer_detach(void *buffer, int *size);
__attribute__((visibility("default"))) int PMPI_Cancel(MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int *coords);
__attribute__((visibility("default"))) int PMPI_Cart_create(MPI_Comm old_comm, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart);
__attribute__((visibility("default"))) int PMPI_Cart_get(MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords);
__attribute__((visibility("default"))) int PMPI_Cart_map(MPI_Comm comm, int ndims, int *dims, int *periods, int *newrank);
__attribute__((visibility("default"))) int PMPI_Cart_rank(MPI_Comm comm, int *coords, int *rank);
__attribute__((visibility("default"))) int PMPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
__attribute__((visibility("default"))) int PMPI_Cart_sub(MPI_Comm comm, int *remain_dims, MPI_Comm *new_comm);
__attribute__((visibility("default"))) int PMPI_Cartdim_get(MPI_Comm comm, int *ndims);
__attribute__((visibility("default"))) int PMPI_Close_port(char *port_name);
__attribute__((visibility("default"))) int PMPI_Comm_accept(char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int PMPI_Comm_c2f(MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Comm_call_errhandler(MPI_Comm comm, int errorcode);
__attribute__((visibility("default"))) int PMPI_Comm_compare(MPI_Comm comm1, MPI_Comm comm2, int *result);
__attribute__((visibility("default"))) int PMPI_Comm_connect(char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int PMPI_Comm_create_errhandler(MPI_Comm_errhandler_fn *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_Comm_create_keyval(MPI_Comm_copy_attr_function *comm_copy_attr_fn, MPI_Comm_delete_attr_function *comm_delete_attr_fn, int *comm_keyval, void *extra_state);
__attribute__((visibility("default"))) int PMPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int PMPI_Comm_delete_attr(MPI_Comm comm, int comm_keyval);
__attribute__((visibility("default"))) int PMPI_Comm_disconnect(MPI_Comm *comm);
__attribute__((visibility("default"))) int PMPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);
__attribute__((visibility("default"))) MPI_Comm PMPI_Comm_f2c(int comm);
__attribute__((visibility("default"))) int PMPI_Comm_free_keyval(int *comm_keyval);
__attribute__((visibility("default"))) int PMPI_Comm_free(MPI_Comm *comm);
__attribute__((visibility("default"))) int PMPI_Comm_get_attr(MPI_Comm comm, int comm_keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int PMPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *erhandler);
__attribute__((visibility("default"))) int PMPI_Comm_get_name(MPI_Comm comm, char *comm_name, int *resultlen);
__attribute__((visibility("default"))) int PMPI_Comm_get_parent(MPI_Comm *parent);
__attribute__((visibility("default"))) int PMPI_Comm_group(MPI_Comm comm, MPI_Group *group);
__attribute__((visibility("default"))) int PMPI_Comm_join(int fd, MPI_Comm *intercomm);
__attribute__((visibility("default"))) int PMPI_Comm_rank(MPI_Comm comm, int *rank);
__attribute__((visibility("default"))) int PMPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);
__attribute__((visibility("default"))) int PMPI_Comm_remote_size(MPI_Comm comm, int *size);
__attribute__((visibility("default"))) int PMPI_Comm_set_attr(MPI_Comm comm, int comm_keyval, void *attribute_val);
__attribute__((visibility("default"))) int PMPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int PMPI_Comm_set_name(MPI_Comm comm, char *comm_name);
__attribute__((visibility("default"))) int PMPI_Comm_size(MPI_Comm comm, int *size);
__attribute__((visibility("default"))) int PMPI_Comm_spawn(char *command, char **argv, int maxprocs, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *intercomm, int *array_of_errcodes);
__attribute__((visibility("default"))) int PMPI_Comm_spawn_multiple(int count, char **array_of_commands, char ***array_of_argv, int *array_of_maxprocs, MPI_Info *array_of_info, int root, MPI_Comm comm, MPI_Comm *intercomm, int *array_of_errcodes);
__attribute__((visibility("default"))) int PMPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
__attribute__((visibility("default"))) int PMPI_Comm_test_inter(MPI_Comm comm, int *flag);
__attribute__((visibility("default"))) int PMPI_Dims_create(int nnodes, int ndims, int *dims);
__attribute__((visibility("default"))) int PMPI_Errhandler_c2f(MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int PMPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) MPI_Errhandler PMPI_Errhandler_f2c(int errhandler);
__attribute__((visibility("default"))) int PMPI_Errhandler_free(MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int PMPI_Error_class(int errorcode, int *errorclass);
__attribute__((visibility("default"))) int PMPI_Error_string(int errorcode, char *string, int *resultlen);
__attribute__((visibility("default"))) int PMPI_Exscan(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_File_c2f(MPI_File file);
__attribute__((visibility("default"))) MPI_File PMPI_File_f2c(int file);
__attribute__((visibility("default"))) int PMPI_File_call_errhandler(MPI_File fh, int errorcode);
__attribute__((visibility("default"))) int PMPI_File_create_errhandler(MPI_File_errhandler_fn *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_File_set_errhandler(MPI_File file, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int PMPI_File_get_errhandler(MPI_File file, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_File_open(MPI_Comm comm, char *filename, int amode, MPI_Info info, MPI_File *fh);
__attribute__((visibility("default"))) int PMPI_File_close(MPI_File *fh);
__attribute__((visibility("default"))) int PMPI_File_delete(char *filename, MPI_Info info);
__attribute__((visibility("default"))) int PMPI_File_set_size(MPI_File fh, MPI_Offset size);
__attribute__((visibility("default"))) int PMPI_File_preallocate(MPI_File fh, MPI_Offset size);
__attribute__((visibility("default"))) int PMPI_File_get_size(MPI_File fh, MPI_Offset *size);
__attribute__((visibility("default"))) int PMPI_File_get_group(MPI_File fh, MPI_Group *group);
__attribute__((visibility("default"))) int PMPI_File_get_amode(MPI_File fh, int *amode);
__attribute__((visibility("default"))) int PMPI_File_set_info(MPI_File fh, MPI_Info info);
__attribute__((visibility("default"))) int PMPI_File_get_info(MPI_File fh, MPI_Info *info_used);
__attribute__((visibility("default"))) int PMPI_File_set_view(MPI_File fh, MPI_Offset disp, MPI_Datatype etype, MPI_Datatype filetype, char *datarep, MPI_Info info);
__attribute__((visibility("default"))) int PMPI_File_get_view(MPI_File fh, MPI_Offset *disp, MPI_Datatype *etype, MPI_Datatype *filetype, char *datarep);
__attribute__((visibility("default"))) int PMPI_File_read_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_iread_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_File_iwrite_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_File_read(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_read_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_iread(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_File_iwrite(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_File_seek(MPI_File fh, MPI_Offset offset, int whence);
__attribute__((visibility("default"))) int PMPI_File_get_position(MPI_File fh, MPI_Offset *offset);
__attribute__((visibility("default"))) int PMPI_File_get_byte_offset(MPI_File fh, MPI_Offset offset, MPI_Offset *disp);
__attribute__((visibility("default"))) int PMPI_File_read_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_iread_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_File_iwrite_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_File_read_ordered(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_ordered(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_seek_shared(MPI_File fh, MPI_Offset offset, int whence);
__attribute__((visibility("default"))) int PMPI_File_get_position_shared(MPI_File fh, MPI_Offset *offset);
__attribute__((visibility("default"))) int PMPI_File_read_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_File_read_at_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_File_write_at_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_read_all_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_File_read_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_all_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_File_write_all_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_read_ordered_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_File_read_ordered_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_write_ordered_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_File_write_ordered_end(MPI_File fh, void *buf, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_File_get_type_extent(MPI_File fh, MPI_Datatype datatype, MPI_Aint *extent);
__attribute__((visibility("default"))) int PMPI_File_set_atomicity(MPI_File fh, int flag);
__attribute__((visibility("default"))) int PMPI_File_get_atomicity(MPI_File fh, int *flag);
__attribute__((visibility("default"))) int PMPI_File_sync(MPI_File fh);
__attribute__((visibility("default"))) int PMPI_Finalize(void);
__attribute__((visibility("default"))) int PMPI_Finalized(int *flag);
__attribute__((visibility("default"))) int PMPI_Free_mem(void *base);
__attribute__((visibility("default"))) int PMPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Gatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Get_address(void *location, MPI_Aint *address);
__attribute__((visibility("default"))) int PMPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);
__attribute__((visibility("default"))) int PMPI_Get_elements(MPI_Status *status, MPI_Datatype datatype, int *count);
__attribute__((visibility("default"))) int PMPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Get_processor_name(char *name, int *resultlen);
__attribute__((visibility("default"))) int PMPI_Get_version(int *version, int *subversion);
__attribute__((visibility("default"))) int PMPI_Graph_create(MPI_Comm comm_old, int nnodes, int *index, int *edges, int reorder, MPI_Comm *comm_graph);
__attribute__((visibility("default"))) int PMPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges);
__attribute__((visibility("default"))) int PMPI_Graph_map(MPI_Comm comm, int nnodes, int *index, int *edges, int *newrank);
__attribute__((visibility("default"))) int PMPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);
__attribute__((visibility("default"))) int PMPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int *neighbors);
__attribute__((visibility("default"))) int PMPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);
__attribute__((visibility("default"))) int PMPI_Grequest_complete(MPI_Request request);
__attribute__((visibility("default"))) int PMPI_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, void *extra_state, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Group_c2f(MPI_Group group);
__attribute__((visibility("default"))) int PMPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result);
__attribute__((visibility("default"))) int PMPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
__attribute__((visibility("default"))) int PMPI_Group_excl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);
__attribute__((visibility("default"))) MPI_Group PMPI_Group_f2c(int group);
__attribute__((visibility("default"))) int PMPI_Group_free(MPI_Group *group);
__attribute__((visibility("default"))) int PMPI_Group_incl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);
__attribute__((visibility("default"))) int PMPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
__attribute__((visibility("default"))) int PMPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
__attribute__((visibility("default"))) int PMPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
__attribute__((visibility("default"))) int PMPI_Group_rank(MPI_Group group, int *rank);
__attribute__((visibility("default"))) int PMPI_Group_size(MPI_Group group, int *size);
__attribute__((visibility("default"))) int PMPI_Group_translate_ranks(MPI_Group group1, int n, int *ranks1, MPI_Group group2, int *ranks2);
__attribute__((visibility("default"))) int PMPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
__attribute__((visibility("default"))) int PMPI_Ibsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Info_c2f(MPI_Info info);
__attribute__((visibility("default"))) int PMPI_Info_create(MPI_Info *info);
__attribute__((visibility("default"))) int PMPI_Info_delete(MPI_Info info, char *key);
__attribute__((visibility("default"))) int PMPI_Info_dup(MPI_Info info, MPI_Info *newinfo);
__attribute__((visibility("default"))) MPI_Info PMPI_Info_f2c(int info);
__attribute__((visibility("default"))) int PMPI_Info_free(MPI_Info *info);
__attribute__((visibility("default"))) int PMPI_Info_get(MPI_Info info, char *key, int valuelen, char *value, int *flag);
__attribute__((visibility("default"))) int PMPI_Info_get_nkeys(MPI_Info info, int *nkeys);
__attribute__((visibility("default"))) int PMPI_Info_get_nthkey(MPI_Info info, int n, char *key);
__attribute__((visibility("default"))) int PMPI_Info_get_valuelen(MPI_Info info, char *key, int *valuelen, int *flag);
__attribute__((visibility("default"))) int PMPI_Info_set(MPI_Info info, char *key, char *value);
__attribute__((visibility("default"))) int PMPI_Init(int *argc, char ***argv);
__attribute__((visibility("default"))) int PMPI_Initialized(int *flag);
__attribute__((visibility("default"))) int PMPI_Init_thread(int *argc, char ***argv, int required, int *provided);
__attribute__((visibility("default"))) int PMPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm bridge_comm, int remote_leader, int tag, MPI_Comm *newintercomm);
__attribute__((visibility("default"))) int PMPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintercomm);
__attribute__((visibility("default"))) int PMPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Irsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Issend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Is_thread_main(int *flag);
__attribute__((visibility("default"))) int PMPI_Keyval_create(MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state);
__attribute__((visibility("default"))) int PMPI_Keyval_free(int *keyval);
__attribute__((visibility("default"))) int PMPI_Lookup_name(char *service_name, MPI_Info info, char *port_name);
__attribute__((visibility("default"))) int PMPI_Op_c2f(MPI_Op op);
__attribute__((visibility("default"))) int PMPI_Op_create(MPI_User_function *function, int commute, MPI_Op *op);
__attribute__((visibility("default"))) int PMPI_Open_port(MPI_Info info, char *port_name);
__attribute__((visibility("default"))) MPI_Op PMPI_Op_f2c(int op);
__attribute__((visibility("default"))) int PMPI_Op_free(MPI_Op *op);
__attribute__((visibility("default"))) int PMPI_Pack_external(char *datarep, void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, MPI_Aint outsize, MPI_Aint *position);
__attribute__((visibility("default"))) int PMPI_Pack_external_size(char *datarep, int incount, MPI_Datatype datatype, MPI_Aint *size);
__attribute__((visibility("default"))) int PMPI_Pack(void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size);
__attribute__((visibility("default"))) int PMPI_Pcontrol(const int level, ...);
__attribute__((visibility("default"))) int PMPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Publish_name(char *service_name, MPI_Info info, char *port_name);
__attribute__((visibility("default"))) int PMPI_Put(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Query_thread(int *provided);
__attribute__((visibility("default"))) int PMPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Reduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Reduce_scatter(void *sendbuf, void *recvbuf, int *recvcounts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Register_datarep(char *datarep, MPI_Datarep_conversion_function *read_conversion_fn, MPI_Datarep_conversion_function *write_conversion_fn, MPI_Datarep_extent_function *dtype_file_extent_fn, void *extra_state);
__attribute__((visibility("default"))) int PMPI_Request_c2f(MPI_Request request);
__attribute__((visibility("default"))) MPI_Request PMPI_Request_f2c(int request);
__attribute__((visibility("default"))) int PMPI_Request_free(MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Request_get_status(MPI_Request request, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Rsend(void *ibuf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Rsend_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Scan(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Scatterv(void *sendbuf, int *sendcounts, int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Send_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Ssend_init(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Ssend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Start(MPI_Request *request);
__attribute__((visibility("default"))) int PMPI_Startall(int count, MPI_Request *array_of_requests);
__attribute__((visibility("default"))) int PMPI_Status_c2f(MPI_Status *c_status, int *f_status);
__attribute__((visibility("default"))) int PMPI_Status_f2c(int *f_status, MPI_Status *c_status);
__attribute__((visibility("default"))) int PMPI_Status_set_cancelled(MPI_Status *status, int flag);
__attribute__((visibility("default"))) int PMPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype, int count);
__attribute__((visibility("default"))) int PMPI_Testall(int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[]);
__attribute__((visibility("default"))) int PMPI_Testany(int count, MPI_Request array_of_requests[], int *index, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Test(MPI_Request *request, int *flag, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Test_cancelled(MPI_Status *status, int *flag);
__attribute__((visibility("default"))) int PMPI_Testsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]);
__attribute__((visibility("default"))) int PMPI_Topo_test(MPI_Comm comm, int *status);
__attribute__((visibility("default"))) int PMPI_Type_c2f(MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_Type_commit(MPI_Datatype *type);
__attribute__((visibility("default"))) int PMPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_darray(int size, int rank, int ndims, int gsize_array[], int distrib_array[], int darg_array[], int psize_array[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_f90_complex(int p, int r, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_f90_integer(int r, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_f90_real(int p, int r, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_hindexed(int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_keyval(MPI_Type_copy_attr_function *type_copy_attr_fn, MPI_Type_delete_attr_function *type_delete_attr_fn, int *type_keyval, void *extra_state);
__attribute__((visibility("default"))) int PMPI_Type_create_indexed_block(int count, int blocklength, int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_struct(int count, int array_of_block_lengths[], MPI_Aint array_of_displacements[], MPI_Datatype array_of_types[], MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_subarray(int ndims, int size_array[], int subsize_array[], int start_array[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_delete_attr(MPI_Datatype type, int type_keyval);
__attribute__((visibility("default"))) int PMPI_Type_dup(MPI_Datatype type, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_extent(MPI_Datatype type, MPI_Aint *extent);
__attribute__((visibility("default"))) int PMPI_Type_free(MPI_Datatype *type);
__attribute__((visibility("default"))) int PMPI_Type_free_keyval(int *type_keyval);
__attribute__((visibility("default"))) MPI_Datatype PMPI_Type_f2c(int datatype);
__attribute__((visibility("default"))) int PMPI_Type_get_attr(MPI_Datatype type, int type_keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int PMPI_Type_get_contents(MPI_Datatype mtype, int max_integers, int max_addresses, int max_datatypes, int array_of_integers[], MPI_Aint array_of_addresses[], MPI_Datatype array_of_datatypes[]);
__attribute__((visibility("default"))) int PMPI_Type_get_envelope(MPI_Datatype type, int *num_integers, int *num_addresses, int *num_datatypes, int *combiner);
__attribute__((visibility("default"))) int PMPI_Type_get_extent(MPI_Datatype type, MPI_Aint *lb, MPI_Aint *extent);
__attribute__((visibility("default"))) int PMPI_Type_get_name(MPI_Datatype type, char *type_name, int *resultlen);
__attribute__((visibility("default"))) int PMPI_Type_get_true_extent(MPI_Datatype datatype, MPI_Aint *true_lb, MPI_Aint *true_extent);
__attribute__((visibility("default"))) int PMPI_Type_hindexed(int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_indexed(int count, int array_of_blocklengths[], int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_lb(MPI_Datatype type, MPI_Aint *lb);
__attribute__((visibility("default"))) int PMPI_Type_match_size(int typeclass, int size, MPI_Datatype *type);
__attribute__((visibility("default"))) int PMPI_Type_set_attr(MPI_Datatype type, int type_keyval, void *attr_val);
__attribute__((visibility("default"))) int PMPI_Type_set_name(MPI_Datatype type, char *type_name);
__attribute__((visibility("default"))) int PMPI_Type_size(MPI_Datatype type, int *size);
__attribute__((visibility("default"))) int PMPI_Type_struct(int count, int array_of_blocklengths[], MPI_Aint array_of_displacements[], MPI_Datatype array_of_types[], MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Type_ub(MPI_Datatype mtype, MPI_Aint *ub);
__attribute__((visibility("default"))) int PMPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
__attribute__((visibility("default"))) int PMPI_Unpack(void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm);
__attribute__((visibility("default"))) int PMPI_Unpublish_name(char *service_name, MPI_Info info, char *port_name);
__attribute__((visibility("default"))) int PMPI_Unpack_external(char *datarep, void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype);
__attribute__((visibility("default"))) int PMPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses);
__attribute__((visibility("default"))) int PMPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Wait(MPI_Request *request, MPI_Status *status);
__attribute__((visibility("default"))) int PMPI_Waitsome(int incount, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses);
__attribute__((visibility("default"))) int PMPI_Win_c2f(MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_call_errhandler(MPI_Win win, int errorcode);
__attribute__((visibility("default"))) int PMPI_Win_complete(MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_create(void *base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, MPI_Win *win);
__attribute__((visibility("default"))) int PMPI_Win_create_errhandler(MPI_Win_errhandler_fn *function, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_Win_create_keyval(MPI_Win_copy_attr_function *win_copy_attr_fn, MPI_Win_delete_attr_function *win_delete_attr_fn, int *win_keyval, void *extra_state);
__attribute__((visibility("default"))) int PMPI_Win_delete_attr(MPI_Win win, int win_keyval);
__attribute__((visibility("default"))) MPI_Win PMPI_Win_f2c(int win);
__attribute__((visibility("default"))) int PMPI_Win_fence(int assert, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_free(MPI_Win *win);
__attribute__((visibility("default"))) int PMPI_Win_free_keyval(int *win_keyval);
__attribute__((visibility("default"))) int PMPI_Win_get_attr(MPI_Win win, int win_keyval, void *attribute_val, int *flag);
__attribute__((visibility("default"))) int PMPI_Win_get_errhandler(MPI_Win win, MPI_Errhandler *errhandler);
__attribute__((visibility("default"))) int PMPI_Win_get_group(MPI_Win win, MPI_Group *group);
__attribute__((visibility("default"))) int PMPI_Win_get_name(MPI_Win win, char *win_name, int *resultlen);
__attribute__((visibility("default"))) int PMPI_Win_lock(int lock_type, int rank, int assert, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_post(MPI_Group group, int assert, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_set_attr(MPI_Win win, int win_keyval, void *attribute_val);
__attribute__((visibility("default"))) int PMPI_Win_set_errhandler(MPI_Win win, MPI_Errhandler errhandler);
__attribute__((visibility("default"))) int PMPI_Win_set_name(MPI_Win win, char *win_name);
__attribute__((visibility("default"))) int PMPI_Win_start(MPI_Group group, int assert, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_test(MPI_Win win, int *flag);
__attribute__((visibility("default"))) int PMPI_Win_unlock(int rank, MPI_Win win);
__attribute__((visibility("default"))) int PMPI_Win_wait(MPI_Win win);
__attribute__((visibility("default"))) double PMPI_Wtick(void);
__attribute__((visibility("default"))) double PMPI_Wtime(void);
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
__extension__
typedef unsigned long long int uint64_t;
typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
__extension__
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
__extension__
typedef unsigned long long int uint_least64_t;
typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
typedef int8_t cl_char;
typedef uint8_t cl_uchar;
typedef int16_t cl_short __attribute__((aligned(2)));
typedef uint16_t cl_ushort __attribute__((aligned(2)));
typedef int32_t cl_int __attribute__((aligned(4)));
typedef uint32_t cl_uint __attribute__((aligned(4)));
typedef int64_t cl_long __attribute__((aligned(8)));
typedef uint64_t cl_ulong __attribute__((aligned(8)));
typedef uint16_t cl_half __attribute__((aligned(2)));
typedef float cl_float __attribute__((aligned(4)));
typedef double cl_double __attribute__((aligned(8)));
typedef unsigned int cl_GLuint;
typedef int cl_GLint;
typedef unsigned int cl_GLenum;
typedef union 
{
        cl_char __attribute__((aligned(2))) s[2];
        __extension__
        struct 
        {
                cl_char x, y;
        };
        __extension__
        struct 
        {
                cl_char s0, s1;
        };
        __extension__
        struct 
        {
                cl_char lo, hi;
        };
} cl_char2;
typedef union 
{
        cl_char __attribute__((aligned(4))) s[4];
        __extension__
        struct 
        {
                cl_char x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_char s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_char2 lo, hi;
        };
} cl_char4;
typedef cl_char4 cl_char3;
typedef union 
{
        cl_char __attribute__((aligned(8))) s[8];
        __extension__
        struct 
        {
                cl_char x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_char s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_char4 lo, hi;
        };
} cl_char8;
typedef union 
{
        cl_char __attribute__((aligned(16))) s[16];
        __extension__
        struct 
        {
                cl_char x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_char s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_char8 lo, hi;
        };
} cl_char16;
typedef union 
{
        cl_uchar __attribute__((aligned(2))) s[2];
        __extension__
        struct 
        {
                cl_uchar x, y;
        };
        __extension__
        struct 
        {
                cl_uchar s0, s1;
        };
        __extension__
        struct 
        {
                cl_uchar lo, hi;
        };
} cl_uchar2;
typedef union 
{
        cl_uchar __attribute__((aligned(4))) s[4];
        __extension__
        struct 
        {
                cl_uchar x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_uchar s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_uchar2 lo, hi;
        };
} cl_uchar4;
typedef cl_uchar4 cl_uchar3;
typedef union 
{
        cl_uchar __attribute__((aligned(8))) s[8];
        __extension__
        struct 
        {
                cl_uchar x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_uchar s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_uchar4 lo, hi;
        };
} cl_uchar8;
typedef union 
{
        cl_uchar __attribute__((aligned(16))) s[16];
        __extension__
        struct 
        {
                cl_uchar x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_uchar s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_uchar8 lo, hi;
        };
} cl_uchar16;
typedef union 
{
        cl_short __attribute__((aligned(4))) s[2];
        __extension__
        struct 
        {
                cl_short x, y;
        };
        __extension__
        struct 
        {
                cl_short s0, s1;
        };
        __extension__
        struct 
        {
                cl_short lo, hi;
        };
} cl_short2;
typedef union 
{
        cl_short __attribute__((aligned(8))) s[4];
        __extension__
        struct 
        {
                cl_short x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_short s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_short2 lo, hi;
        };
} cl_short4;
typedef cl_short4 cl_short3;
typedef union 
{
        cl_short __attribute__((aligned(16))) s[8];
        __extension__
        struct 
        {
                cl_short x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_short s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_short4 lo, hi;
        };
} cl_short8;
typedef union 
{
        cl_short __attribute__((aligned(32))) s[16];
        __extension__
        struct 
        {
                cl_short x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_short s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_short8 lo, hi;
        };
} cl_short16;
typedef union 
{
        cl_ushort __attribute__((aligned(4))) s[2];
        __extension__
        struct 
        {
                cl_ushort x, y;
        };
        __extension__
        struct 
        {
                cl_ushort s0, s1;
        };
        __extension__
        struct 
        {
                cl_ushort lo, hi;
        };
} cl_ushort2;
typedef union 
{
        cl_ushort __attribute__((aligned(8))) s[4];
        __extension__
        struct 
        {
                cl_ushort x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_ushort s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_ushort2 lo, hi;
        };
} cl_ushort4;
typedef cl_ushort4 cl_ushort3;
typedef union 
{
        cl_ushort __attribute__((aligned(16))) s[8];
        __extension__
        struct 
        {
                cl_ushort x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_ushort s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_ushort4 lo, hi;
        };
} cl_ushort8;
typedef union 
{
        cl_ushort __attribute__((aligned(32))) s[16];
        __extension__
        struct 
        {
                cl_ushort x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_ushort s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_ushort8 lo, hi;
        };
} cl_ushort16;
typedef union 
{
        cl_int __attribute__((aligned(8))) s[2];
        __extension__
        struct 
        {
                cl_int x, y;
        };
        __extension__
        struct 
        {
                cl_int s0, s1;
        };
        __extension__
        struct 
        {
                cl_int lo, hi;
        };
} cl_int2;
typedef union 
{
        cl_int __attribute__((aligned(16))) s[4];
        __extension__
        struct 
        {
                cl_int x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_int s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_int2 lo, hi;
        };
} cl_int4;
typedef cl_int4 cl_int3;
typedef union 
{
        cl_int __attribute__((aligned(32))) s[8];
        __extension__
        struct 
        {
                cl_int x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_int s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_int4 lo, hi;
        };
} cl_int8;
typedef union 
{
        cl_int __attribute__((aligned(64))) s[16];
        __extension__
        struct 
        {
                cl_int x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_int s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_int8 lo, hi;
        };
} cl_int16;
typedef union 
{
        cl_uint __attribute__((aligned(8))) s[2];
        __extension__
        struct 
        {
                cl_uint x, y;
        };
        __extension__
        struct 
        {
                cl_uint s0, s1;
        };
        __extension__
        struct 
        {
                cl_uint lo, hi;
        };
} cl_uint2;
typedef union 
{
        cl_uint __attribute__((aligned(16))) s[4];
        __extension__
        struct 
        {
                cl_uint x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_uint s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_uint2 lo, hi;
        };
} cl_uint4;
typedef cl_uint4 cl_uint3;
typedef union 
{
        cl_uint __attribute__((aligned(32))) s[8];
        __extension__
        struct 
        {
                cl_uint x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_uint s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_uint4 lo, hi;
        };
} cl_uint8;
typedef union 
{
        cl_uint __attribute__((aligned(64))) s[16];
        __extension__
        struct 
        {
                cl_uint x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_uint s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_uint8 lo, hi;
        };
} cl_uint16;
typedef union 
{
        cl_long __attribute__((aligned(16))) s[2];
        __extension__
        struct 
        {
                cl_long x, y;
        };
        __extension__
        struct 
        {
                cl_long s0, s1;
        };
        __extension__
        struct 
        {
                cl_long lo, hi;
        };
} cl_long2;
typedef union 
{
        cl_long __attribute__((aligned(32))) s[4];
        __extension__
        struct 
        {
                cl_long x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_long s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_long2 lo, hi;
        };
} cl_long4;
typedef cl_long4 cl_long3;
typedef union 
{
        cl_long __attribute__((aligned(64))) s[8];
        __extension__
        struct 
        {
                cl_long x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_long s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_long4 lo, hi;
        };
} cl_long8;
typedef union 
{
        cl_long __attribute__((aligned(128))) s[16];
        __extension__
        struct 
        {
                cl_long x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_long s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_long8 lo, hi;
        };
} cl_long16;
typedef union 
{
        cl_ulong __attribute__((aligned(16))) s[2];
        __extension__
        struct 
        {
                cl_ulong x, y;
        };
        __extension__
        struct 
        {
                cl_ulong s0, s1;
        };
        __extension__
        struct 
        {
                cl_ulong lo, hi;
        };
} cl_ulong2;
typedef union 
{
        cl_ulong __attribute__((aligned(32))) s[4];
        __extension__
        struct 
        {
                cl_ulong x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_ulong s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_ulong2 lo, hi;
        };
} cl_ulong4;
typedef cl_ulong4 cl_ulong3;
typedef union 
{
        cl_ulong __attribute__((aligned(64))) s[8];
        __extension__
        struct 
        {
                cl_ulong x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_ulong s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_ulong4 lo, hi;
        };
} cl_ulong8;
typedef union 
{
        cl_ulong __attribute__((aligned(128))) s[16];
        __extension__
        struct 
        {
                cl_ulong x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_ulong s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_ulong8 lo, hi;
        };
} cl_ulong16;
typedef union 
{
        cl_float __attribute__((aligned(8))) s[2];
        __extension__
        struct 
        {
                cl_float x, y;
        };
        __extension__
        struct 
        {
                cl_float s0, s1;
        };
        __extension__
        struct 
        {
                cl_float lo, hi;
        };
} cl_float2;
typedef union 
{
        cl_float __attribute__((aligned(16))) s[4];
        __extension__
        struct 
        {
                cl_float x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_float s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_float2 lo, hi;
        };
} cl_float4;
typedef cl_float4 cl_float3;
typedef union 
{
        cl_float __attribute__((aligned(32))) s[8];
        __extension__
        struct 
        {
                cl_float x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_float s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_float4 lo, hi;
        };
} cl_float8;
typedef union 
{
        cl_float __attribute__((aligned(64))) s[16];
        __extension__
        struct 
        {
                cl_float x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_float s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_float8 lo, hi;
        };
} cl_float16;
typedef union 
{
        cl_double __attribute__((aligned(16))) s[2];
        __extension__
        struct 
        {
                cl_double x, y;
        };
        __extension__
        struct 
        {
                cl_double s0, s1;
        };
        __extension__
        struct 
        {
                cl_double lo, hi;
        };
} cl_double2;
typedef union 
{
        cl_double __attribute__((aligned(32))) s[4];
        __extension__
        struct 
        {
                cl_double x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_double s0, s1, s2, s3;
        };
        __extension__
        struct 
        {
                cl_double2 lo, hi;
        };
} cl_double4;
typedef cl_double4 cl_double3;
typedef union 
{
        cl_double __attribute__((aligned(64))) s[8];
        __extension__
        struct 
        {
                cl_double x, y, z, w;
        };
        __extension__
        struct 
        {
                cl_double s0, s1, s2, s3, s4, s5, s6, s7;
        };
        __extension__
        struct 
        {
                cl_double4 lo, hi;
        };
} cl_double8;
typedef union 
{
        cl_double __attribute__((aligned(128))) s[16];
        __extension__
        struct 
        {
                cl_double x, y, z, w, __spacer4, __spacer5, __spacer6, __spacer7, __spacer8, __spacer9, sa, sb, sc, sd, se, sf;
        };
        __extension__
        struct 
        {
                cl_double s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF;
        };
        __extension__
        struct 
        {
                cl_double8 lo, hi;
        };
} cl_double16;
typedef struct _cl_platform_id *cl_platform_id;
typedef struct _cl_device_id *cl_device_id;
typedef struct _cl_context *cl_context;
typedef struct _cl_command_queue *cl_command_queue;
typedef struct _cl_mem *cl_mem;
typedef struct _cl_program *cl_program;
typedef struct _cl_kernel *cl_kernel;
typedef struct _cl_event *cl_event;
typedef struct _cl_sampler *cl_sampler;
typedef cl_uint cl_bool;
typedef cl_ulong cl_bitfield;
typedef cl_bitfield cl_device_type;
typedef cl_uint cl_platform_info;
typedef cl_uint cl_device_info;
typedef cl_bitfield cl_device_address_info;
typedef cl_bitfield cl_device_fp_config;
typedef cl_uint cl_device_mem_cache_type;
typedef cl_uint cl_device_local_mem_type;
typedef cl_bitfield cl_device_exec_capabilities;
typedef cl_bitfield cl_command_queue_properties;
typedef intptr_t cl_context_properties;
typedef cl_uint cl_context_info;
typedef cl_uint cl_command_queue_info;
typedef cl_uint cl_channel_order;
typedef cl_uint cl_channel_type;
typedef cl_bitfield cl_mem_flags;
typedef cl_uint cl_mem_object_type;
typedef cl_uint cl_mem_info;
typedef cl_uint cl_image_info;
typedef cl_uint cl_buffer_create_type;
typedef cl_uint cl_addressing_mode;
typedef cl_uint cl_filter_mode;
typedef cl_uint cl_sampler_info;
typedef cl_bitfield cl_map_flags;
typedef cl_uint cl_program_info;
typedef cl_uint cl_program_build_info;
typedef cl_int cl_build_status;
typedef cl_uint cl_kernel_info;
typedef cl_uint cl_kernel_work_group_info;
typedef cl_uint cl_event_info;
typedef cl_uint cl_command_type;
typedef cl_uint cl_profiling_info;
typedef struct _cl_image_format
{
        cl_channel_order image_channel_order;
        cl_channel_type image_channel_data_type;
} cl_image_format;
typedef struct _cl_buffer_region
{
        size_t origin;
        size_t size;
} cl_buffer_region;
extern cl_int clGetPlatformIDs(cl_uint, cl_platform_id *, cl_uint *);
extern cl_int clGetPlatformInfo(cl_platform_id, cl_platform_info, size_t, void *, size_t *);
extern cl_int clGetDeviceIDs(cl_platform_id, cl_device_type, cl_uint, cl_device_id *, cl_uint *);
extern cl_int clGetDeviceInfo(cl_device_id, cl_device_info, size_t, void *, size_t *);
extern cl_context clCreateContext(const cl_context_properties *, cl_uint, const cl_device_id *, void (*)(const char *, const void *, size_t, void *), void *, cl_int *);
extern cl_context clCreateContextFromType(const cl_context_properties *, cl_device_type, void (*)(const char *, const void *, size_t, void *), void *, cl_int *);
extern cl_int clRetainContext(cl_context);
extern cl_int clReleaseContext(cl_context);
extern cl_int clGetContextInfo(cl_context, cl_context_info, size_t, void *, size_t *);
extern cl_command_queue clCreateCommandQueue(cl_context, cl_device_id, cl_command_queue_properties, cl_int *);
extern cl_int clRetainCommandQueue(cl_command_queue);
extern cl_int clReleaseCommandQueue(cl_command_queue);
extern cl_int clGetCommandQueueInfo(cl_command_queue, cl_command_queue_info, size_t, void *, size_t *);
extern cl_mem clCreateBuffer(cl_context, cl_mem_flags, size_t, void *, cl_int *);
extern cl_mem clCreateSubBuffer(cl_mem, cl_mem_flags, cl_buffer_create_type, const void *, cl_int *);
extern cl_mem clCreateImage2D(cl_context, cl_mem_flags, const cl_image_format *, size_t, size_t, size_t, void *, cl_int *);
extern cl_mem clCreateImage3D(cl_context, cl_mem_flags, const cl_image_format *, size_t, size_t, size_t, size_t, size_t, void *, cl_int *);
extern cl_int clRetainMemObject(cl_mem);
extern cl_int clReleaseMemObject(cl_mem);
extern cl_int clGetSupportedImageFormats(cl_context, cl_mem_flags, cl_mem_object_type, cl_uint, cl_image_format *, cl_uint *);
extern cl_int clGetMemObjectInfo(cl_mem, cl_mem_info, size_t, void *, size_t *);
extern cl_int clGetImageInfo(cl_mem, cl_image_info, size_t, void *, size_t *);
extern cl_int clSetMemObjectDestructorCallback(cl_mem, void (*)(cl_mem, void *), void *);
extern cl_sampler clCreateSampler(cl_context, cl_bool, cl_addressing_mode, cl_filter_mode, cl_int *);
extern cl_int clRetainSampler(cl_sampler);
extern cl_int clReleaseSampler(cl_sampler);
extern cl_int clGetSamplerInfo(cl_sampler, cl_sampler_info, size_t, void *, size_t *);
extern cl_program clCreateProgramWithSource(cl_context, cl_uint, const char **, const size_t *, cl_int *);
extern cl_program clCreateProgramWithBinary(cl_context, cl_uint, const cl_device_id *, const size_t *, const unsigned char **, cl_int *, cl_int *);
extern cl_int clRetainProgram(cl_program);
extern cl_int clReleaseProgram(cl_program);
extern cl_int clBuildProgram(cl_program, cl_uint, const cl_device_id *, const char *, void (*)(cl_program, void *), void *);
extern cl_int clUnloadCompiler(void);
extern cl_int clGetProgramInfo(cl_program, cl_program_info, size_t, void *, size_t *);
extern cl_int clGetProgramBuildInfo(cl_program, cl_device_id, cl_program_build_info, size_t, void *, size_t *);
extern cl_kernel clCreateKernel(cl_program, const char *, cl_int *);
extern cl_int clCreateKernelsInProgram(cl_program, cl_uint, cl_kernel *, cl_uint *);
extern cl_int clRetainKernel(cl_kernel);
extern cl_int clReleaseKernel(cl_kernel);
extern cl_int clSetKernelArg(cl_kernel, cl_uint, size_t, const void *);
extern cl_int clGetKernelInfo(cl_kernel, cl_kernel_info, size_t, void *, size_t *);
extern cl_int clGetKernelWorkGroupInfo(cl_kernel, cl_device_id, cl_kernel_work_group_info, size_t, void *, size_t *);
extern cl_int clWaitForEvents(cl_uint, const cl_event *);
extern cl_int clGetEventInfo(cl_event, cl_event_info, size_t, void *, size_t *);
extern cl_event clCreateUserEvent(cl_context, cl_int *);
extern cl_int clRetainEvent(cl_event);
extern cl_int clReleaseEvent(cl_event);
extern cl_int clSetUserEventStatus(cl_event, cl_int);
extern cl_int clSetEventCallback(cl_event, cl_int, void (*)(cl_event, cl_int, void *), void *);
extern cl_int clGetEventProfilingInfo(cl_event, cl_profiling_info, size_t, void *, size_t *);
extern cl_int clFlush(cl_command_queue);
extern cl_int clFinish(cl_command_queue);
extern cl_int clEnqueueReadBuffer(cl_command_queue, cl_mem, cl_bool, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueReadBufferRect(cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, const size_t *, size_t, size_t, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueWriteBuffer(cl_command_queue, cl_mem, cl_bool, size_t, size_t, const void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueWriteBufferRect(cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, const size_t *, size_t, size_t, size_t, size_t, const void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueCopyBuffer(cl_command_queue, cl_mem, cl_mem, size_t, size_t, size_t, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueCopyBufferRect(cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *, const size_t *, size_t, size_t, size_t, size_t, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueReadImage(cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueWriteImage(cl_command_queue, cl_mem, cl_bool, const size_t *, const size_t *, size_t, size_t, const void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueCopyImage(cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueCopyImageToBuffer(cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *, size_t, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueCopyBufferToImage(cl_command_queue, cl_mem, cl_mem, size_t, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);
extern void *clEnqueueMapBuffer(cl_command_queue, cl_mem, cl_bool, cl_map_flags, size_t, size_t, cl_uint, const cl_event *, cl_event *, cl_int *);
extern void *clEnqueueMapImage(cl_command_queue, cl_mem, cl_bool, cl_map_flags, const size_t *, const size_t *, size_t *, size_t *, cl_uint, const cl_event *, cl_event *, cl_int *);
extern cl_int clEnqueueUnmapMemObject(cl_command_queue, cl_mem, void *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueNDRangeKernel(cl_command_queue, cl_kernel, cl_uint, const size_t *, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueTask(cl_command_queue, cl_kernel, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueNativeKernel(cl_command_queue, void (*user_func)(void *), void *, size_t, cl_uint, const cl_mem *, const void **, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueMarker(cl_command_queue, cl_event *);
extern cl_int clEnqueueWaitForEvents(cl_command_queue, cl_uint, const cl_event *);
extern cl_int clEnqueueBarrier(cl_command_queue);
extern void *clGetExtensionFunctionAddress(const char *);
extern int _gfn_rank;
extern int _gfn_num_proc;
extern cl_platform_id _gfn_platform_id;
extern cl_device_id _gfn_device_id;
extern cl_context _gfn_context;
extern cl_command_queue _gfn_cmd_queue;
extern cl_int _gfn_status;
extern cl_program _gfn_cl_program;
int gfn_get_num_process();
int gfn_get_process_num();
int _GfnInit(int *argc, char **argv[]);
int _GfnFinalize();
int _GfnMallocReduceScalar(void *ptr, cl_mem *cl_ptr, int type_id, int group_num, int level1_cond, int level2_cond);
int _GfnFreeReduceScalar(cl_mem cl_ptr, int level1_cond, int level2_cond);
int _GfnEnqueueBoardcastScalar(void *ptr, int type_id);
int _GfnFinishBoardcastScalar();
int _GfnEnqueueReduceScalar(void *ptr, cl_mem cl_ptr, int type_id, MPI_Op op_id, int group_num, int level1_cond, int level2_cond);
int _GfnFinishReduceScalar();
int _GfnMalloc1D(void **ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc2D(void ***ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc3D(void ****ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc4D(void *****ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc5D(void ******ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc6D(void *******ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnFree(long long unique_id, int level1_cond, int level2_cond);
int _GfnEnqueueBoardcastND(void *ptr, cl_mem cl_ptr, int type_id, int level1_cond, int level2_cond, int dim_n, ...);
int _GfnEnqueueScatterND(void *ptr, cl_mem cl_ptr, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnFinishDistributeArray();
int _GfnEnqueueGatherND(void *ptr, cl_mem cl_ptr, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnFinishGatherArray();
int _GfnLockTransfer(void *ptr);
int _GfnUnlockTransfer(void *ptr);
void _GfnBarrier();
void _GfnCheckCLStatus(cl_int status, const char *phase_name);
size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num);
int _GfnCalcLocalDataStart(int data_start, int data_end);
int _GfnCalcLocalDataEnd(int data_start, int data_end);
int _GfnCalcLocalLoopStart(int local_data_start, int loop_start, int loop_step);
int _GfnCalcLocalLoopEnd(int local_data_end, int loop_end);
int _CalcLoopSize(int start, int end, int incre);
int _CalcSubSize(int size, int num_proc, int rank, int block_size);
int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcOffset(int size, int num_proc, int rank);
void _CalcCnts(int size, int num_proc, int *cnts, int block_size);
void _CalcDisp(int size, int num_proc, int *disp, int block_size);
size_t _CalcTypeSize(int type_id);
void _CalcPartitionInfo(int size, int block_size, int loop_start, int loop_end, int loop_step, int *pattern_array, int pattern_array_size, int pattern_type, int *cnts, int *disp, int *sub_size, int *elem_offset);
int _GFN_TYPE_CHAR();
int _GFN_TYPE_UNSIGNED_CHAR();
int _GFN_TYPE_SHORT();
int _GFN_TYPE_UNSIGEND_SHORT();
int _GFN_TYPE_INT();
int _GFN_TYPE_UNSIGNED();
int _GFN_TYPE_LONG();
int _GFN_TYPE_UNSIGNED_LONG();
int _GFN_TYPE_FLOAT();
int _GFN_TYPE_DOUBLE();
int _GFN_TYPE_LONG_DOUBLE();
int _GFN_TYPE_LONG_LONG_INT();
MPI_Op _GFN_OP_MAX();
MPI_Op _GFN_OP_MIN();
MPI_Op _GFN_OP_SUM();
MPI_Op _GFN_OP_PROD();
MPI_Op _GFN_OP_LAND();
MPI_Op _GFN_OP_BAND();
MPI_Op _GFN_OP_LOR();
MPI_Op _GFN_OP_BOR();
MPI_Op _GFN_OP_LXOR();
MPI_Op _GFN_OP_BXOR();
MPI_Op _GFN_OP_MINLOC();
MPI_Op _GFN_OP_MAXLOC();
cl_bool _GFN_TRUE();
cl_bool _GFN_FALSE();
cl_mem_flags _GFN_MEM_READ_WRITE();
cl_mem_flags _GFN_MEM_WRITE_ONLY();
cl_mem_flags _GFN_MEM_READ_ONLY();
cl_mem_flags _GFN_MEM_USE_HOST_PTR();
cl_mem_flags _GFN_MEM_ALLOC_HOST_PTR();
void _InitOpenCL();
void _FinalOpenCL();
void _GfnCreateProgram(const char *src);
void _GfnClearProgram();
cl_kernel _GfnCreateKernel(const char *name);
void _GfnClearKernel(cl_kernel kernel);
void _GfnSetKernelArg(cl_kernel kernel, int arg_num, size_t size, void *ptr);
void _GfnLaunchKernel(cl_kernel kernel, const size_t *global_size, const size_t *local_size);
extern mqd_t _master_send_qid;
extern mqd_t _master_recv_qid;
extern mqd_t _worker_send_qid;
extern mqd_t _worker_recv_qid;
extern int pid;
void _LaunchWorker();
void _StopWorker();
void _OpenMasterMsgQueue();
void _CloseMasterMsgQueue();
void _OpenWorkerMsgQueue();
void _CloseWorkerMsgQueue();
int _SendConstInputMsg(long long c);
int _SendInputMsg(void *ptr, size_t size);
int _RecvOutputMsg(void *ptr, size_t size);
int _RecvInputMsg(void *ptr, size_t size);
int _SendOutputMsg(void *ptr, size_t size);
int _SendInputNDMsg(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, ...);
int _SendInputNDMsgCore(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, int *size_array, int *pattern_array);
int _RecvOutputNDMsg(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, ...);
int _RecvOutputNDMsgCore(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, int *size_array, int *pattern_array);
int _RecvInputNDMsg(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, ...);
int _RecvInputNDMsgCore(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, int *size_array, int *pattern_array);
int _SendOutputNDMsg(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, ...);
int _SendOutputNDMsgCore(void *ptr, int type_id, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int size_n, int pattern_n, int *size_array, int *pattern_array);
void _SendCallFuncMsg(int func_code);
void _RecvCallFuncMsg(int *func_code);
void _SendMasterMsg(void *buffer, size_t size);
void _RecvMasterMsg(void *buffer, size_t size);
void _SendWorkerMsg(void *buffer, size_t size);
void _RecvWorkerMsg(void *buffer, size_t size);
int b(int x)
{
    return x * x;
}
int c(int x)
{
    return x + x + 1;
}
int a(int x, int y)
{
    return b(x) + c(y);
}
int main(int argc, char *argv[])
{
    int i, A[500], B[500];
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    for (i = 0;
        i < 500;
        i++)
    {
        B[i] = i;
    }
    /* Send call send function message */
    _SendCallFuncMsg(775346);
    _SendInputMsg((void *) B, (sizeof(int) * ((500))));
    _GfnLockTransfer((void *) B);
    {
        /* Send call function message */
        _SendCallFuncMsg(474613);
        _SendConstInputMsg((long long) &A);
        _SendConstInputMsg((long long) &B);
        _SendInputNDMsg((void *) B, _GFN_TYPE_INT(), 0, (500) - 1, 1, 0, 0, 1, 0, 500);
        _RecvOutputNDMsg(&(A[0]), _GFN_TYPE_INT(), 0, (500) - 1, 1, 0, 0, 1, 0, 500);
    }
    /* Send call recieve function message */
    _SendCallFuncMsg(532217);
    _GfnUnlockTransfer((void *) A);
    _RecvOutputMsg((void *) A, (sizeof(int) * ((500))));
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    for (i = 0;
        i < 500;
        i++)
    {
        if (A[i] != a(B[i], 8))
        {
            printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, a(B[i], 8), A[i]);
            ((0) ? (void) (0) : __assert_fail("0", "call_func.c", 46, __PRETTY_FUNCTION__));
        }
    }
    printf("TEST PASS!\n");
    return 0;
}
