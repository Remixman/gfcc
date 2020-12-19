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
typedef int wchar_t;
union wait
{
        int w_status;
        struct 
        {
                unsigned int __w_termsig : 7;
                unsigned int __w_coredump : 1;
                unsigned int __w_retcode : 8;
                unsigned int : 16;
        } __wait_terminated;
        struct 
        {
                unsigned int __w_stopval : 8;
                unsigned int __w_stopsig : 8;
                unsigned int : 16;
        } __wait_stopped;
};
typedef union 
{
        union wait *__uptr;
        int *__iptr;
} __WAIT_STATUS __attribute__((__transparent_union__));
typedef struct 
{
        int quot;
        int rem;
} div_t;
typedef struct 
{
        long int quot;
        long int rem;
} ldiv_t;
__extension__
typedef struct 
{
        long long int quot;
        long long int rem;
} lldiv_t;
extern size_t __ctype_get_mb_cur_max(void) __attribute__((__nothrow__, __leaf__));
extern double atof(__const char *__nptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__)) __attribute__((__nonnull__(1)));
extern int atoi(__const char *__nptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__)) __attribute__((__nonnull__(1)));
extern long int atol(__const char *__nptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__)) __attribute__((__nonnull__(1)));
__extension__
extern long long int atoll(__const char *__nptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__)) __attribute__((__nonnull__(1)));
extern double strtod(__const char *__restrict __nptr, char **__restrict __endptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern float strtof(__const char *__restrict __nptr, char **__restrict __endptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long double strtold(__const char *__restrict __nptr, char **__restrict __endptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long int strtol(__const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern unsigned long int strtoul(__const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
__extension__
extern long long int strtoq(__const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
__extension__
extern unsigned long long int strtouq(__const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
__extension__
extern long long int strtoll(__const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
__extension__
extern unsigned long long int strtoull(__const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern char *l64a(long int __n) __attribute__((__nothrow__, __leaf__));
extern long int a64l(__const char *__s) __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__)) __attribute__((__nonnull__(1)));
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
extern long int random(void) __attribute__((__nothrow__, __leaf__));
extern void srandom(unsigned int __seed) __attribute__((__nothrow__, __leaf__));
extern char *initstate(unsigned int __seed, char *__statebuf, size_t __statelen) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern char *setstate(char *__statebuf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
struct random_data
{
        int32_t *fptr;
        int32_t *rptr;
        int32_t *state;
        int rand_type;
        int rand_deg;
        int rand_sep;
        int32_t *end_ptr;
};
extern int random_r(struct random_data *__restrict __buf, int32_t *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int srandom_r(unsigned int __seed, struct random_data *__buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int initstate_r(unsigned int __seed, char *__restrict __statebuf, size_t __statelen, struct random_data *__restrict __buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2, 4)));
extern int setstate_r(char *__restrict __statebuf, struct random_data *__restrict __buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int rand(void) __attribute__((__nothrow__, __leaf__));
extern void srand(unsigned int __seed) __attribute__((__nothrow__, __leaf__));
extern int rand_r(unsigned int *__seed) __attribute__((__nothrow__, __leaf__));
extern double drand48(void) __attribute__((__nothrow__, __leaf__));
extern double erand48(unsigned short int __xsubi[3]) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long int lrand48(void) __attribute__((__nothrow__, __leaf__));
extern long int nrand48(unsigned short int __xsubi[3]) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long int mrand48(void) __attribute__((__nothrow__, __leaf__));
extern long int jrand48(unsigned short int __xsubi[3]) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern void srand48(long int __seedval) __attribute__((__nothrow__, __leaf__));
extern unsigned short int *seed48(unsigned short int __seed16v[3]) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern void lcong48(unsigned short int __param[7]) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
struct drand48_data
{
        unsigned short int __x[3];
        unsigned short int __old_x[3];
        unsigned short int __c;
        unsigned short int __init;
        unsigned long long int __a;
};
extern int drand48_r(struct drand48_data *__restrict __buffer, double *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int erand48_r(unsigned short int __xsubi[3], struct drand48_data *__restrict __buffer, double *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int lrand48_r(struct drand48_data *__restrict __buffer, long int *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int nrand48_r(unsigned short int __xsubi[3], struct drand48_data *__restrict __buffer, long int *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int mrand48_r(struct drand48_data *__restrict __buffer, long int *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int jrand48_r(unsigned short int __xsubi[3], struct drand48_data *__restrict __buffer, long int *__restrict __result) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int srand48_r(long int __seedval, struct drand48_data *__buffer) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int seed48_r(unsigned short int __seed16v[3], struct drand48_data *__buffer) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int lcong48_r(unsigned short int __param[7], struct drand48_data *__buffer) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern void *malloc(size_t __size) __attribute__((__nothrow__, __leaf__)) __attribute__((__malloc__));
extern void *calloc(size_t __nmemb, size_t __size) __attribute__((__nothrow__, __leaf__)) __attribute__((__malloc__));
extern void *realloc(void *__ptr, size_t __size) __attribute__((__nothrow__, __leaf__)) __attribute__((__warn_unused_result__));
extern void free(void *__ptr) __attribute__((__nothrow__, __leaf__));
extern void cfree(void *__ptr) __attribute__((__nothrow__, __leaf__));
extern void *alloca(size_t __size) __attribute__((__nothrow__, __leaf__));
extern void *valloc(size_t __size) __attribute__((__nothrow__, __leaf__)) __attribute__((__malloc__));
extern int posix_memalign(void **__memptr, size_t __alignment, size_t __size) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern void abort(void) __attribute__((__nothrow__, __leaf__)) __attribute__((__noreturn__));
extern int atexit(void (*__func)(void)) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int on_exit(void (*__func)(int __status, void *__arg), void *__arg) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern void exit(int __status) __attribute__((__nothrow__, __leaf__)) __attribute__((__noreturn__));
extern void _Exit(int __status) __attribute__((__nothrow__, __leaf__)) __attribute__((__noreturn__));
extern char *getenv(__const char *__name) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern char *__secure_getenv(__const char *__name) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int putenv(char *__string) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int setenv(__const char *__name, __const char *__value, int __replace) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int unsetenv(__const char *__name) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int clearenv(void) __attribute__((__nothrow__, __leaf__));
extern char *mktemp(char *__template) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int mkstemp(char *__template) __attribute__((__nonnull__(1)));
extern int mkstemps(char *__template, int __suffixlen) __attribute__((__nonnull__(1)));
extern char *mkdtemp(char *__template) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int system(__const char *__command);
extern char *realpath(__const char *__restrict __name, char *__restrict __resolved) __attribute__((__nothrow__, __leaf__));
typedef int (*__compar_fn_t)(__const void *, __const void *);
extern void *bsearch(__const void *__key, __const void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar) __attribute__((__nonnull__(1, 2, 5)));
extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar) __attribute__((__nonnull__(1, 4)));
extern int abs(int __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long int labs(long int __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
__extension__
extern long long int llabs(long long int __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern div_t div(int __numer, int __denom) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern ldiv_t ldiv(long int __numer, long int __denom) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
__extension__
extern lldiv_t lldiv(long long int __numer, long long int __denom) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern char *ecvt(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern char *fcvt(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern char *gcvt(double __value, int __ndigit, char *__buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3)));
extern char *qecvt(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern char *qfcvt(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern char *qgcvt(long double __value, int __ndigit, char *__buf) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3)));
extern int ecvt_r(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4, 5)));
extern int fcvt_r(double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4, 5)));
extern int qecvt_r(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4, 5)));
extern int qfcvt_r(long double __value, int __ndigit, int *__restrict __decpt, int *__restrict __sign, char *__restrict __buf, size_t __len) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4, 5)));
extern int mblen(__const char *__s, size_t __n) __attribute__((__nothrow__, __leaf__));
extern int mbtowc(wchar_t *__restrict __pwc, __const char *__restrict __s, size_t __n) __attribute__((__nothrow__, __leaf__));
extern int wctomb(char *__s, wchar_t __wchar) __attribute__((__nothrow__, __leaf__));
extern size_t mbstowcs(wchar_t *__restrict __pwcs, __const char *__restrict __s, size_t __n) __attribute__((__nothrow__, __leaf__));
extern size_t wcstombs(char *__restrict __s, __const wchar_t *__restrict __pwcs, size_t __n) __attribute__((__nothrow__, __leaf__));
extern int rpmatch(__const char *__response) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int getsubopt(char **__restrict __optionp, char *__const *__restrict __tokens, char **__restrict __valuep) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2, 3)));
extern int getloadavg(double __loadavg[], int __nelem) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
typedef long double float_t;
typedef long double double_t;
extern double acos(double __x) __attribute__((__nothrow__, __leaf__));
extern double __acos(double __x) __attribute__((__nothrow__, __leaf__));
extern double asin(double __x) __attribute__((__nothrow__, __leaf__));
extern double __asin(double __x) __attribute__((__nothrow__, __leaf__));
extern double atan(double __x) __attribute__((__nothrow__, __leaf__));
extern double __atan(double __x) __attribute__((__nothrow__, __leaf__));
extern double atan2(double __y, double __x) __attribute__((__nothrow__, __leaf__));
extern double __atan2(double __y, double __x) __attribute__((__nothrow__, __leaf__));
extern double cos(double __x) __attribute__((__nothrow__, __leaf__));
extern double __cos(double __x) __attribute__((__nothrow__, __leaf__));
extern double sin(double __x) __attribute__((__nothrow__, __leaf__));
extern double __sin(double __x) __attribute__((__nothrow__, __leaf__));
extern double tan(double __x) __attribute__((__nothrow__, __leaf__));
extern double __tan(double __x) __attribute__((__nothrow__, __leaf__));
extern double cosh(double __x) __attribute__((__nothrow__, __leaf__));
extern double __cosh(double __x) __attribute__((__nothrow__, __leaf__));
extern double sinh(double __x) __attribute__((__nothrow__, __leaf__));
extern double __sinh(double __x) __attribute__((__nothrow__, __leaf__));
extern double tanh(double __x) __attribute__((__nothrow__, __leaf__));
extern double __tanh(double __x) __attribute__((__nothrow__, __leaf__));
extern double acosh(double __x) __attribute__((__nothrow__, __leaf__));
extern double __acosh(double __x) __attribute__((__nothrow__, __leaf__));
extern double asinh(double __x) __attribute__((__nothrow__, __leaf__));
extern double __asinh(double __x) __attribute__((__nothrow__, __leaf__));
extern double atanh(double __x) __attribute__((__nothrow__, __leaf__));
extern double __atanh(double __x) __attribute__((__nothrow__, __leaf__));
extern double exp(double __x) __attribute__((__nothrow__, __leaf__));
extern double __exp(double __x) __attribute__((__nothrow__, __leaf__));
extern double frexp(double __x, int *__exponent) __attribute__((__nothrow__, __leaf__));
extern double __frexp(double __x, int *__exponent) __attribute__((__nothrow__, __leaf__));
extern double ldexp(double __x, int __exponent) __attribute__((__nothrow__, __leaf__));
extern double __ldexp(double __x, int __exponent) __attribute__((__nothrow__, __leaf__));
extern double log(double __x) __attribute__((__nothrow__, __leaf__));
extern double __log(double __x) __attribute__((__nothrow__, __leaf__));
extern double log10(double __x) __attribute__((__nothrow__, __leaf__));
extern double __log10(double __x) __attribute__((__nothrow__, __leaf__));
extern double modf(double __x, double *__iptr) __attribute__((__nothrow__, __leaf__));
extern double __modf(double __x, double *__iptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern double expm1(double __x) __attribute__((__nothrow__, __leaf__));
extern double __expm1(double __x) __attribute__((__nothrow__, __leaf__));
extern double log1p(double __x) __attribute__((__nothrow__, __leaf__));
extern double __log1p(double __x) __attribute__((__nothrow__, __leaf__));
extern double logb(double __x) __attribute__((__nothrow__, __leaf__));
extern double __logb(double __x) __attribute__((__nothrow__, __leaf__));
extern double exp2(double __x) __attribute__((__nothrow__, __leaf__));
extern double __exp2(double __x) __attribute__((__nothrow__, __leaf__));
extern double log2(double __x) __attribute__((__nothrow__, __leaf__));
extern double __log2(double __x) __attribute__((__nothrow__, __leaf__));
extern double pow(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __pow(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double sqrt(double __x) __attribute__((__nothrow__, __leaf__));
extern double __sqrt(double __x) __attribute__((__nothrow__, __leaf__));
extern double hypot(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __hypot(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double cbrt(double __x) __attribute__((__nothrow__, __leaf__));
extern double __cbrt(double __x) __attribute__((__nothrow__, __leaf__));
extern double ceil(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __ceil(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double fabs(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __fabs(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double floor(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __floor(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double fmod(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __fmod(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern int __isinf(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __finite(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int isinf(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int finite(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double drem(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __drem(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double significand(double __x) __attribute__((__nothrow__, __leaf__));
extern double __significand(double __x) __attribute__((__nothrow__, __leaf__));
extern double copysign(double __x, double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __copysign(double __x, double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double nan(__const char *__tagb) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __nan(__const char *__tagb) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __isnan(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int isnan(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double j0(double) __attribute__((__nothrow__, __leaf__));
extern double __j0(double) __attribute__((__nothrow__, __leaf__));
extern double j1(double) __attribute__((__nothrow__, __leaf__));
extern double __j1(double) __attribute__((__nothrow__, __leaf__));
extern double jn(int, double) __attribute__((__nothrow__, __leaf__));
extern double __jn(int, double) __attribute__((__nothrow__, __leaf__));
extern double y0(double) __attribute__((__nothrow__, __leaf__));
extern double __y0(double) __attribute__((__nothrow__, __leaf__));
extern double y1(double) __attribute__((__nothrow__, __leaf__));
extern double __y1(double) __attribute__((__nothrow__, __leaf__));
extern double yn(int, double) __attribute__((__nothrow__, __leaf__));
extern double __yn(int, double) __attribute__((__nothrow__, __leaf__));
extern double erf(double) __attribute__((__nothrow__, __leaf__));
extern double __erf(double) __attribute__((__nothrow__, __leaf__));
extern double erfc(double) __attribute__((__nothrow__, __leaf__));
extern double __erfc(double) __attribute__((__nothrow__, __leaf__));
extern double lgamma(double) __attribute__((__nothrow__, __leaf__));
extern double __lgamma(double) __attribute__((__nothrow__, __leaf__));
extern double tgamma(double) __attribute__((__nothrow__, __leaf__));
extern double __tgamma(double) __attribute__((__nothrow__, __leaf__));
extern double gamma(double) __attribute__((__nothrow__, __leaf__));
extern double __gamma(double) __attribute__((__nothrow__, __leaf__));
extern double lgamma_r(double, int *__signgamp) __attribute__((__nothrow__, __leaf__));
extern double __lgamma_r(double, int *__signgamp) __attribute__((__nothrow__, __leaf__));
extern double rint(double __x) __attribute__((__nothrow__, __leaf__));
extern double __rint(double __x) __attribute__((__nothrow__, __leaf__));
extern double nextafter(double __x, double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __nextafter(double __x, double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double nexttoward(double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __nexttoward(double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double remainder(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __remainder(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double scalbn(double __x, int __n) __attribute__((__nothrow__, __leaf__));
extern double __scalbn(double __x, int __n) __attribute__((__nothrow__, __leaf__));
extern int ilogb(double __x) __attribute__((__nothrow__, __leaf__));
extern int __ilogb(double __x) __attribute__((__nothrow__, __leaf__));
extern double scalbln(double __x, long int __n) __attribute__((__nothrow__, __leaf__));
extern double __scalbln(double __x, long int __n) __attribute__((__nothrow__, __leaf__));
extern double nearbyint(double __x) __attribute__((__nothrow__, __leaf__));
extern double __nearbyint(double __x) __attribute__((__nothrow__, __leaf__));
extern double round(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __round(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double trunc(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double __trunc(double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double remquo(double __x, double __y, int *__quo) __attribute__((__nothrow__, __leaf__));
extern double __remquo(double __x, double __y, int *__quo) __attribute__((__nothrow__, __leaf__));
extern long int lrint(double __x) __attribute__((__nothrow__, __leaf__));
extern long int __lrint(double __x) __attribute__((__nothrow__, __leaf__));
extern long long int llrint(double __x) __attribute__((__nothrow__, __leaf__));
extern long long int __llrint(double __x) __attribute__((__nothrow__, __leaf__));
extern long int lround(double __x) __attribute__((__nothrow__, __leaf__));
extern long int __lround(double __x) __attribute__((__nothrow__, __leaf__));
extern long long int llround(double __x) __attribute__((__nothrow__, __leaf__));
extern long long int __llround(double __x) __attribute__((__nothrow__, __leaf__));
extern double fdim(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __fdim(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double fmax(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __fmax(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double fmin(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern double __fmin(double __x, double __y) __attribute__((__nothrow__, __leaf__));
extern int __fpclassify(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __signbit(double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern double fma(double __x, double __y, double __z) __attribute__((__nothrow__, __leaf__));
extern double __fma(double __x, double __y, double __z) __attribute__((__nothrow__, __leaf__));
extern double scalb(double __x, double __n) __attribute__((__nothrow__, __leaf__));
extern double __scalb(double __x, double __n) __attribute__((__nothrow__, __leaf__));
extern float acosf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __acosf(float __x) __attribute__((__nothrow__, __leaf__));
extern float asinf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __asinf(float __x) __attribute__((__nothrow__, __leaf__));
extern float atanf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __atanf(float __x) __attribute__((__nothrow__, __leaf__));
extern float atan2f(float __y, float __x) __attribute__((__nothrow__, __leaf__));
extern float __atan2f(float __y, float __x) __attribute__((__nothrow__, __leaf__));
extern float cosf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __cosf(float __x) __attribute__((__nothrow__, __leaf__));
extern float sinf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __sinf(float __x) __attribute__((__nothrow__, __leaf__));
extern float tanf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __tanf(float __x) __attribute__((__nothrow__, __leaf__));
extern float coshf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __coshf(float __x) __attribute__((__nothrow__, __leaf__));
extern float sinhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __sinhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float tanhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __tanhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float acoshf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __acoshf(float __x) __attribute__((__nothrow__, __leaf__));
extern float asinhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __asinhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float atanhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __atanhf(float __x) __attribute__((__nothrow__, __leaf__));
extern float expf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __expf(float __x) __attribute__((__nothrow__, __leaf__));
extern float frexpf(float __x, int *__exponent) __attribute__((__nothrow__, __leaf__));
extern float __frexpf(float __x, int *__exponent) __attribute__((__nothrow__, __leaf__));
extern float ldexpf(float __x, int __exponent) __attribute__((__nothrow__, __leaf__));
extern float __ldexpf(float __x, int __exponent) __attribute__((__nothrow__, __leaf__));
extern float logf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __logf(float __x) __attribute__((__nothrow__, __leaf__));
extern float log10f(float __x) __attribute__((__nothrow__, __leaf__));
extern float __log10f(float __x) __attribute__((__nothrow__, __leaf__));
extern float modff(float __x, float *__iptr) __attribute__((__nothrow__, __leaf__));
extern float __modff(float __x, float *__iptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern float expm1f(float __x) __attribute__((__nothrow__, __leaf__));
extern float __expm1f(float __x) __attribute__((__nothrow__, __leaf__));
extern float log1pf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __log1pf(float __x) __attribute__((__nothrow__, __leaf__));
extern float logbf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __logbf(float __x) __attribute__((__nothrow__, __leaf__));
extern float exp2f(float __x) __attribute__((__nothrow__, __leaf__));
extern float __exp2f(float __x) __attribute__((__nothrow__, __leaf__));
extern float log2f(float __x) __attribute__((__nothrow__, __leaf__));
extern float __log2f(float __x) __attribute__((__nothrow__, __leaf__));
extern float powf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __powf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float sqrtf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __sqrtf(float __x) __attribute__((__nothrow__, __leaf__));
extern float hypotf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __hypotf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float cbrtf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __cbrtf(float __x) __attribute__((__nothrow__, __leaf__));
extern float ceilf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __ceilf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float fabsf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __fabsf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float floorf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __floorf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float fmodf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __fmodf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern int __isinff(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __finitef(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int isinff(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int finitef(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float dremf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __dremf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float significandf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __significandf(float __x) __attribute__((__nothrow__, __leaf__));
extern float copysignf(float __x, float __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __copysignf(float __x, float __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float nanf(__const char *__tagb) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __nanf(__const char *__tagb) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __isnanf(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int isnanf(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float j0f(float) __attribute__((__nothrow__, __leaf__));
extern float __j0f(float) __attribute__((__nothrow__, __leaf__));
extern float j1f(float) __attribute__((__nothrow__, __leaf__));
extern float __j1f(float) __attribute__((__nothrow__, __leaf__));
extern float jnf(int, float) __attribute__((__nothrow__, __leaf__));
extern float __jnf(int, float) __attribute__((__nothrow__, __leaf__));
extern float y0f(float) __attribute__((__nothrow__, __leaf__));
extern float __y0f(float) __attribute__((__nothrow__, __leaf__));
extern float y1f(float) __attribute__((__nothrow__, __leaf__));
extern float __y1f(float) __attribute__((__nothrow__, __leaf__));
extern float ynf(int, float) __attribute__((__nothrow__, __leaf__));
extern float __ynf(int, float) __attribute__((__nothrow__, __leaf__));
extern float erff(float) __attribute__((__nothrow__, __leaf__));
extern float __erff(float) __attribute__((__nothrow__, __leaf__));
extern float erfcf(float) __attribute__((__nothrow__, __leaf__));
extern float __erfcf(float) __attribute__((__nothrow__, __leaf__));
extern float lgammaf(float) __attribute__((__nothrow__, __leaf__));
extern float __lgammaf(float) __attribute__((__nothrow__, __leaf__));
extern float tgammaf(float) __attribute__((__nothrow__, __leaf__));
extern float __tgammaf(float) __attribute__((__nothrow__, __leaf__));
extern float gammaf(float) __attribute__((__nothrow__, __leaf__));
extern float __gammaf(float) __attribute__((__nothrow__, __leaf__));
extern float lgammaf_r(float, int *__signgamp) __attribute__((__nothrow__, __leaf__));
extern float __lgammaf_r(float, int *__signgamp) __attribute__((__nothrow__, __leaf__));
extern float rintf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __rintf(float __x) __attribute__((__nothrow__, __leaf__));
extern float nextafterf(float __x, float __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __nextafterf(float __x, float __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float nexttowardf(float __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __nexttowardf(float __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float remainderf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __remainderf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float scalbnf(float __x, int __n) __attribute__((__nothrow__, __leaf__));
extern float __scalbnf(float __x, int __n) __attribute__((__nothrow__, __leaf__));
extern int ilogbf(float __x) __attribute__((__nothrow__, __leaf__));
extern int __ilogbf(float __x) __attribute__((__nothrow__, __leaf__));
extern float scalblnf(float __x, long int __n) __attribute__((__nothrow__, __leaf__));
extern float __scalblnf(float __x, long int __n) __attribute__((__nothrow__, __leaf__));
extern float nearbyintf(float __x) __attribute__((__nothrow__, __leaf__));
extern float __nearbyintf(float __x) __attribute__((__nothrow__, __leaf__));
extern float roundf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __roundf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float truncf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float __truncf(float __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float remquof(float __x, float __y, int *__quo) __attribute__((__nothrow__, __leaf__));
extern float __remquof(float __x, float __y, int *__quo) __attribute__((__nothrow__, __leaf__));
extern long int lrintf(float __x) __attribute__((__nothrow__, __leaf__));
extern long int __lrintf(float __x) __attribute__((__nothrow__, __leaf__));
extern long long int llrintf(float __x) __attribute__((__nothrow__, __leaf__));
extern long long int __llrintf(float __x) __attribute__((__nothrow__, __leaf__));
extern long int lroundf(float __x) __attribute__((__nothrow__, __leaf__));
extern long int __lroundf(float __x) __attribute__((__nothrow__, __leaf__));
extern long long int llroundf(float __x) __attribute__((__nothrow__, __leaf__));
extern long long int __llroundf(float __x) __attribute__((__nothrow__, __leaf__));
extern float fdimf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __fdimf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float fmaxf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __fmaxf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float fminf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern float __fminf(float __x, float __y) __attribute__((__nothrow__, __leaf__));
extern int __fpclassifyf(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __signbitf(float __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern float fmaf(float __x, float __y, float __z) __attribute__((__nothrow__, __leaf__));
extern float __fmaf(float __x, float __y, float __z) __attribute__((__nothrow__, __leaf__));
extern float scalbf(float __x, float __n) __attribute__((__nothrow__, __leaf__));
extern float __scalbf(float __x, float __n) __attribute__((__nothrow__, __leaf__));
extern long double acosl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __acosl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double asinl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __asinl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double atanl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __atanl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double atan2l(long double __y, long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __atan2l(long double __y, long double __x) __attribute__((__nothrow__, __leaf__));
extern long double cosl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __cosl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double sinl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __sinl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double tanl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __tanl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double coshl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __coshl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double sinhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __sinhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double tanhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __tanhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double acoshl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __acoshl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double asinhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __asinhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double atanhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __atanhl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double expl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __expl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double frexpl(long double __x, int *__exponent) __attribute__((__nothrow__, __leaf__));
extern long double __frexpl(long double __x, int *__exponent) __attribute__((__nothrow__, __leaf__));
extern long double ldexpl(long double __x, int __exponent) __attribute__((__nothrow__, __leaf__));
extern long double __ldexpl(long double __x, int __exponent) __attribute__((__nothrow__, __leaf__));
extern long double logl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __logl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double log10l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __log10l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double modfl(long double __x, long double *__iptr) __attribute__((__nothrow__, __leaf__));
extern long double __modfl(long double __x, long double *__iptr) __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern long double expm1l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __expm1l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double log1pl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __log1pl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double logbl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __logbl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double exp2l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __exp2l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double log2l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __log2l(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double powl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __powl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double sqrtl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __sqrtl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double hypotl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __hypotl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double cbrtl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __cbrtl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double ceill(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __ceill(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double fabsl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __fabsl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double floorl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __floorl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double fmodl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __fmodl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern int __isinfl(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __finitel(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int isinfl(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int finitel(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double dreml(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __dreml(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double significandl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __significandl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double copysignl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __copysignl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double nanl(__const char *__tagb) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __nanl(__const char *__tagb) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __isnanl(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int isnanl(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double j0l(long double) __attribute__((__nothrow__, __leaf__));
extern long double __j0l(long double) __attribute__((__nothrow__, __leaf__));
extern long double j1l(long double) __attribute__((__nothrow__, __leaf__));
extern long double __j1l(long double) __attribute__((__nothrow__, __leaf__));
extern long double jnl(int, long double) __attribute__((__nothrow__, __leaf__));
extern long double __jnl(int, long double) __attribute__((__nothrow__, __leaf__));
extern long double y0l(long double) __attribute__((__nothrow__, __leaf__));
extern long double __y0l(long double) __attribute__((__nothrow__, __leaf__));
extern long double y1l(long double) __attribute__((__nothrow__, __leaf__));
extern long double __y1l(long double) __attribute__((__nothrow__, __leaf__));
extern long double ynl(int, long double) __attribute__((__nothrow__, __leaf__));
extern long double __ynl(int, long double) __attribute__((__nothrow__, __leaf__));
extern long double erfl(long double) __attribute__((__nothrow__, __leaf__));
extern long double __erfl(long double) __attribute__((__nothrow__, __leaf__));
extern long double erfcl(long double) __attribute__((__nothrow__, __leaf__));
extern long double __erfcl(long double) __attribute__((__nothrow__, __leaf__));
extern long double lgammal(long double) __attribute__((__nothrow__, __leaf__));
extern long double __lgammal(long double) __attribute__((__nothrow__, __leaf__));
extern long double tgammal(long double) __attribute__((__nothrow__, __leaf__));
extern long double __tgammal(long double) __attribute__((__nothrow__, __leaf__));
extern long double gammal(long double) __attribute__((__nothrow__, __leaf__));
extern long double __gammal(long double) __attribute__((__nothrow__, __leaf__));
extern long double lgammal_r(long double, int *__signgamp) __attribute__((__nothrow__, __leaf__));
extern long double __lgammal_r(long double, int *__signgamp) __attribute__((__nothrow__, __leaf__));
extern long double rintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __rintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double nextafterl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __nextafterl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double nexttowardl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __nexttowardl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double remainderl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __remainderl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double scalbnl(long double __x, int __n) __attribute__((__nothrow__, __leaf__));
extern long double __scalbnl(long double __x, int __n) __attribute__((__nothrow__, __leaf__));
extern int ilogbl(long double __x) __attribute__((__nothrow__, __leaf__));
extern int __ilogbl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double scalblnl(long double __x, long int __n) __attribute__((__nothrow__, __leaf__));
extern long double __scalblnl(long double __x, long int __n) __attribute__((__nothrow__, __leaf__));
extern long double nearbyintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double __nearbyintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double roundl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __roundl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double truncl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double __truncl(long double __x) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double remquol(long double __x, long double __y, int *__quo) __attribute__((__nothrow__, __leaf__));
extern long double __remquol(long double __x, long double __y, int *__quo) __attribute__((__nothrow__, __leaf__));
extern long int lrintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long int __lrintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long long int llrintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long long int __llrintl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long int lroundl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long int __lroundl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long long int llroundl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long long int __llroundl(long double __x) __attribute__((__nothrow__, __leaf__));
extern long double fdiml(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __fdiml(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double fmaxl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __fmaxl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double fminl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern long double __fminl(long double __x, long double __y) __attribute__((__nothrow__, __leaf__));
extern int __fpclassifyl(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int __signbitl(long double __value) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern long double fmal(long double __x, long double __y, long double __z) __attribute__((__nothrow__, __leaf__));
extern long double __fmal(long double __x, long double __y, long double __z) __attribute__((__nothrow__, __leaf__));
extern long double scalbl(long double __x, long double __n) __attribute__((__nothrow__, __leaf__));
extern long double __scalbl(long double __x, long double __n) __attribute__((__nothrow__, __leaf__));
extern int signgam;
enum 
{
    FP_NAN, 
    FP_INFINITE, 
    FP_ZERO, 
    FP_SUBNORMAL, 
    FP_NORMAL
};
typedef enum 
{
    _IEEE_ = - 1, 
    _SVID_, 
    _XOPEN_, 
    _POSIX_, 
    _ISOC_
} _LIB_VERSION_TYPE;
extern _LIB_VERSION_TYPE _LIB_VERSION;
struct exception
{
        int type;
        char *name;
        double arg1;
        double arg2;
        double retval;
};
extern int matherr(struct exception *__exc);
struct tm
{
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
        long int tm_gmtoff;
        __const char *tm_zone;
};
struct itimerspec
{
        struct timespec it_interval;
        struct timespec it_value;
};
struct sigevent;
extern clock_t clock(void) __attribute__((__nothrow__, __leaf__));
extern time_t time(time_t *__timer) __attribute__((__nothrow__, __leaf__));
extern double difftime(time_t __time1, time_t __time0) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern time_t mktime(struct tm *__tp) __attribute__((__nothrow__, __leaf__));
extern size_t strftime(char *__restrict __s, size_t __maxsize, __const char *__restrict __format, __const struct tm *__restrict __tp) __attribute__((__nothrow__, __leaf__));
typedef struct __locale_struct
{
        struct __locale_data *__locales[13];
        const unsigned short int *__ctype_b;
        const int *__ctype_tolower;
        const int *__ctype_toupper;
        const char *__names[13];
} *__locale_t;
typedef __locale_t locale_t;
extern size_t strftime_l(char *__restrict __s, size_t __maxsize, __const char *__restrict __format, __const struct tm *__restrict __tp, __locale_t __loc) __attribute__((__nothrow__, __leaf__));
extern struct tm *gmtime(__const time_t *__timer) __attribute__((__nothrow__, __leaf__));
extern struct tm *localtime(__const time_t *__timer) __attribute__((__nothrow__, __leaf__));
extern struct tm *gmtime_r(__const time_t *__restrict __timer, struct tm *__restrict __tp) __attribute__((__nothrow__, __leaf__));
extern struct tm *localtime_r(__const time_t *__restrict __timer, struct tm *__restrict __tp) __attribute__((__nothrow__, __leaf__));
extern char *asctime(__const struct tm *__tp) __attribute__((__nothrow__, __leaf__));
extern char *ctime(__const time_t *__timer) __attribute__((__nothrow__, __leaf__));
extern char *asctime_r(__const struct tm *__restrict __tp, char *__restrict __buf) __attribute__((__nothrow__, __leaf__));
extern char *ctime_r(__const time_t *__restrict __timer, char *__restrict __buf) __attribute__((__nothrow__, __leaf__));
extern char *__tzname[2];
extern int __daylight;
extern long int __timezone;
extern char *tzname[2];
extern void tzset(void) __attribute__((__nothrow__, __leaf__));
extern int daylight;
extern long int timezone;
extern int stime(__const time_t *__when) __attribute__((__nothrow__, __leaf__));
extern time_t timegm(struct tm *__tp) __attribute__((__nothrow__, __leaf__));
extern time_t timelocal(struct tm *__tp) __attribute__((__nothrow__, __leaf__));
extern int dysize(int __year) __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern int nanosleep(__const struct timespec *__requested_time, struct timespec *__remaining);
extern int clock_getres(clockid_t __clock_id, struct timespec *__res) __attribute__((__nothrow__, __leaf__));
extern int clock_gettime(clockid_t __clock_id, struct timespec *__tp) __attribute__((__nothrow__, __leaf__));
extern int clock_settime(clockid_t __clock_id, __const struct timespec *__tp) __attribute__((__nothrow__, __leaf__));
extern int clock_nanosleep(clockid_t __clock_id, int __flags, __const struct timespec *__req, struct timespec *__rem);
extern int clock_getcpuclockid(pid_t __pid, clockid_t *__clock_id) __attribute__((__nothrow__, __leaf__));
extern int timer_create(clockid_t __clock_id, struct sigevent *__restrict __evp, timer_t *__restrict __timerid) __attribute__((__nothrow__, __leaf__));
extern int timer_delete(timer_t __timerid) __attribute__((__nothrow__, __leaf__));
extern int timer_settime(timer_t __timerid, int __flags, __const struct itimerspec *__restrict __value, struct itimerspec *__restrict __ovalue) __attribute__((__nothrow__, __leaf__));
extern int timer_gettime(timer_t __timerid, struct itimerspec *__value) __attribute__((__nothrow__, __leaf__));
extern int timer_getoverrun(timer_t __timerid) __attribute__((__nothrow__, __leaf__));
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
typedef int MPI_Datatype;
typedef int MPI_Comm;
typedef int MPI_Group;
typedef int MPI_Win;
typedef struct ADIOI_FileD *MPI_File;
typedef int MPI_Op;
typedef enum MPIR_Win_flavor
{
    MPI_WIN_FLAVOR_CREATE = 1, 
    MPI_WIN_FLAVOR_ALLOCATE = 2, 
    MPI_WIN_FLAVOR_DYNAMIC = 3, 
    MPI_WIN_FLAVOR_SHARED = 4
} MPIR_Win_flavor_t;
typedef enum MPIR_Win_model
{
    MPI_WIN_SEPARATE = 1, 
    MPI_WIN_UNIFIED = 2
} MPIR_Win_model_t;
typedef enum MPIR_Topo_type
{
    MPI_GRAPH = 1, 
    MPI_CART = 2, 
    MPI_DIST_GRAPH = 3
} MPIR_Topo_type;
extern int *const MPI_UNWEIGHTED;
extern int *const MPI_WEIGHTS_EMPTY;
typedef void (MPI_Handler_function)(MPI_Comm *, int *, ...);
typedef int (MPI_Comm_copy_attr_function)(MPI_Comm, int, void *, void *, void *, int *);
typedef int (MPI_Comm_delete_attr_function)(MPI_Comm, int, void *, void *);
typedef int (MPI_Type_copy_attr_function)(MPI_Datatype, int, void *, void *, void *, int *);
typedef int (MPI_Type_delete_attr_function)(MPI_Datatype, int, void *, void *);
typedef int (MPI_Win_copy_attr_function)(MPI_Win, int, void *, void *, void *, int *);
typedef int (MPI_Win_delete_attr_function)(MPI_Win, int, void *, void *);
typedef void (MPI_Comm_errhandler_function)(MPI_Comm *, int *, ...);
typedef void (MPI_File_errhandler_function)(MPI_File *, int *, ...);
typedef void (MPI_Win_errhandler_function)(MPI_Win *, int *, ...);
typedef MPI_Comm_errhandler_function MPI_Comm_errhandler_fn;
typedef MPI_File_errhandler_function MPI_File_errhandler_fn;
typedef MPI_Win_errhandler_function MPI_Win_errhandler_fn;
typedef int MPI_Errhandler;
typedef int MPI_Request;
typedef int MPI_Message;
typedef void (MPI_User_function)(void *, void *, int *, MPI_Datatype *);
typedef int (MPI_Copy_function)(MPI_Comm, int, void *, void *, void *, int *);
typedef int (MPI_Delete_function)(MPI_Comm, int, void *, void *);
enum MPIR_Combiner_enum
{
    MPI_COMBINER_NAMED = 1, 
    MPI_COMBINER_DUP = 2, 
    MPI_COMBINER_CONTIGUOUS = 3, 
    MPI_COMBINER_VECTOR = 4, 
    MPI_COMBINER_HVECTOR_INTEGER = 5, 
    MPI_COMBINER_HVECTOR = 6, 
    MPI_COMBINER_INDEXED = 7, 
    MPI_COMBINER_HINDEXED_INTEGER = 8, 
    MPI_COMBINER_HINDEXED = 9, 
    MPI_COMBINER_INDEXED_BLOCK = 10, 
    MPI_COMBINER_STRUCT_INTEGER = 11, 
    MPI_COMBINER_STRUCT = 12, 
    MPI_COMBINER_SUBARRAY = 13, 
    MPI_COMBINER_DARRAY = 14, 
    MPI_COMBINER_F90_REAL = 15, 
    MPI_COMBINER_F90_COMPLEX = 16, 
    MPI_COMBINER_F90_INTEGER = 17, 
    MPI_COMBINER_RESIZED = 18, 
    MPI_COMBINER_HINDEXED_BLOCK = 19
};
typedef int MPI_Info;
typedef int MPI_Aint;
typedef int MPI_Fint;
typedef long long MPI_Count;
typedef long long MPI_Offset;
typedef struct MPI_Status
{
        int count_lo;
        int count_hi_and_cancelled;
        int MPI_SOURCE;
        int MPI_TAG;
        int MPI_ERROR;
} MPI_Status;
struct MPIR_T_enum_s;
struct MPIR_T_cvar_handle_s;
struct MPIR_T_pvar_handle_s;
struct MPIR_T_pvar_session_s;
typedef struct MPIR_T_enum_s *MPI_T_enum;
typedef struct MPIR_T_cvar_handle_s *MPI_T_cvar_handle;
typedef struct MPIR_T_pvar_handle_s *MPI_T_pvar_handle;
typedef struct MPIR_T_pvar_session_s *MPI_T_pvar_session;
extern struct MPIR_T_pvar_handle_s *const MPI_T_PVAR_ALL_HANDLES;
typedef enum MPIR_T_verbosity_t
{
    MPIX_T_VERBOSITY_INVALID = 0, 
    MPI_T_VERBOSITY_USER_BASIC = 221, 
    MPI_T_VERBOSITY_USER_DETAIL, 
    MPI_T_VERBOSITY_USER_ALL, 
    MPI_T_VERBOSITY_TUNER_BASIC, 
    MPI_T_VERBOSITY_TUNER_DETAIL, 
    MPI_T_VERBOSITY_TUNER_ALL, 
    MPI_T_VERBOSITY_MPIDEV_BASIC, 
    MPI_T_VERBOSITY_MPIDEV_DETAIL, 
    MPI_T_VERBOSITY_MPIDEV_ALL
} MPIR_T_verbosity_t;
typedef enum MPIR_T_bind_t
{
    MPIX_T_BIND_INVALID = 0, 
    MPI_T_BIND_NO_OBJECT = 9700, 
    MPI_T_BIND_MPI_COMM, 
    MPI_T_BIND_MPI_DATATYPE, 
    MPI_T_BIND_MPI_ERRHANDLER, 
    MPI_T_BIND_MPI_FILE, 
    MPI_T_BIND_MPI_GROUP, 
    MPI_T_BIND_MPI_OP, 
    MPI_T_BIND_MPI_REQUEST, 
    MPI_T_BIND_MPI_WIN, 
    MPI_T_BIND_MPI_MESSAGE, 
    MPI_T_BIND_MPI_INFO
} MPIR_T_bind_t;
typedef enum MPIR_T_scope_t
{
    MPIX_T_SCOPE_INVALID = 0, 
    MPI_T_SCOPE_CONSTANT = 60438, 
    MPI_T_SCOPE_READONLY, 
    MPI_T_SCOPE_LOCAL, 
    MPI_T_SCOPE_GROUP, 
    MPI_T_SCOPE_GROUP_EQ, 
    MPI_T_SCOPE_ALL, 
    MPI_T_SCOPE_ALL_EQ
} MPIR_T_scope_t;
typedef enum MPIR_T_pvar_class_t
{
    MPIX_T_PVAR_CLASS_INVALID = 0, 
    MPIR_T_PVAR_CLASS_FIRST = 240, 
    MPI_T_PVAR_CLASS_STATE = MPIR_T_PVAR_CLASS_FIRST, 
    MPI_T_PVAR_CLASS_LEVEL, 
    MPI_T_PVAR_CLASS_SIZE, 
    MPI_T_PVAR_CLASS_PERCENTAGE, 
    MPI_T_PVAR_CLASS_HIGHWATERMARK, 
    MPI_T_PVAR_CLASS_LOWWATERMARK, 
    MPI_T_PVAR_CLASS_COUNTER, 
    MPI_T_PVAR_CLASS_AGGREGATE, 
    MPI_T_PVAR_CLASS_TIMER, 
    MPI_T_PVAR_CLASS_GENERIC, 
    MPIR_T_PVAR_CLASS_LAST, 
    MPIR_T_PVAR_CLASS_NUMBER = MPIR_T_PVAR_CLASS_LAST - MPIR_T_PVAR_CLASS_FIRST
} MPIR_T_pvar_class_t;
extern MPI_Fint *MPI_F_STATUS_IGNORE;
extern MPI_Fint *MPI_F_STATUSES_IGNORE;
typedef struct 
{
        MPI_Fint count_lo;
        MPI_Fint count_hi_and_cancelled;
        MPI_Fint MPI_SOURCE;
        MPI_Fint MPI_TAG;
        MPI_Fint MPI_ERROR;
} MPI_F08_Status;
extern MPI_F08_Status MPIR_F08_MPI_STATUS_IGNORE_OBJ;
extern MPI_F08_Status MPIR_F08_MPI_STATUSES_IGNORE_OBJ[1];
extern int MPIR_F08_MPI_IN_PLACE;
extern int MPIR_F08_MPI_BOTTOM;
extern MPI_F08_Status *MPI_F08_STATUS_IGNORE;
extern MPI_F08_Status *MPI_F08_STATUSES_IGNORE;
typedef int (MPI_Grequest_cancel_function)(void *, int);
typedef int (MPI_Grequest_free_function)(void *);
typedef int (MPI_Grequest_query_function)(void *, MPI_Status *);
typedef int (MPIX_Grequest_poll_function)(void *, MPI_Status *);
typedef int (MPIX_Grequest_wait_function)(int, void **, double, MPI_Status *);
typedef int (MPI_Datarep_conversion_function)(void *, MPI_Datatype, int, void *, MPI_Offset, void *);
typedef int (MPI_Datarep_extent_function)(MPI_Datatype datatype, MPI_Aint *, void *);
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count);
int MPI_Bsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPI_Rsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int MPI_Buffer_attach(void *buffer, int size);
int MPI_Buffer_detach(void *buffer_addr, int *size);
int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Ibsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Irsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Wait(MPI_Request *request, MPI_Status *status);
int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status);
int MPI_Request_free(MPI_Request *request);
int MPI_Waitany(int count, MPI_Request array_of_requests[], int *indx, MPI_Status *status);
int MPI_Testany(int count, MPI_Request array_of_requests[], int *indx, int *flag, MPI_Status *status);
int MPI_Waitall(int count, MPI_Request array_of_requests[], MPI_Status array_of_statuses[]);
int MPI_Testall(int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[]);
int MPI_Waitsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]);
int MPI_Testsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]);
int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);
int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
int MPI_Cancel(MPI_Request *request);
int MPI_Test_cancelled(const MPI_Status *status, int *flag);
int MPI_Send_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Bsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Ssend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Rsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Start(MPI_Request *request);
int MPI_Startall(int count, MPI_Request array_of_requests[]);
int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_indexed(int count, const int *array_of_blocklengths, const int *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_hindexed(int count, const int *array_of_blocklengths, const MPI_Aint *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_struct(int count, const int *array_of_blocklengths, const MPI_Aint *array_of_displacements, const MPI_Datatype *array_of_types, MPI_Datatype *newtype);
int MPI_Address(const void *location, MPI_Aint *address);
int MPI_Type_extent(MPI_Datatype datatype, MPI_Aint *extent);
int MPI_Type_size(MPI_Datatype datatype, int *size);
int MPI_Type_lb(MPI_Datatype datatype, MPI_Aint *displacement);
int MPI_Type_ub(MPI_Datatype datatype, MPI_Aint *displacement);
int MPI_Type_commit(MPI_Datatype *datatype);
int MPI_Type_free(MPI_Datatype *datatype);
int MPI_Get_elements(const MPI_Status *status, MPI_Datatype datatype, int *count);
int MPI_Pack(const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm);
int MPI_Unpack(const void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm);
int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size);
int MPI_Barrier(MPI_Comm comm);
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Alltoallv(const void *sendbuf, const int *sendcounts, const int *sdispls, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *rdispls, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Alltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);
int MPI_Exscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int MPI_Op_create(MPI_User_function *user_fn, int commute, MPI_Op *op);
int MPI_Op_free(MPI_Op *op);
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPI_Scan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPI_Group_size(MPI_Group group, int *size);
int MPI_Group_rank(MPI_Group group, int *rank);
int MPI_Group_translate_ranks(MPI_Group group1, int n, const int ranks1[], MPI_Group group2, int ranks2[]);
int MPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result);
int MPI_Comm_group(MPI_Comm comm, MPI_Group *group);
int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
int MPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
int MPI_Group_incl(MPI_Group group, int n, const int ranks[], MPI_Group *newgroup);
int MPI_Group_excl(MPI_Group group, int n, const int ranks[], MPI_Group *newgroup);
int MPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
int MPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
int MPI_Group_free(MPI_Group *group);
int MPI_Comm_size(MPI_Comm comm, int *size);
int MPI_Comm_rank(MPI_Comm comm, int *rank);
int MPI_Comm_compare(MPI_Comm comm1, MPI_Comm comm2, int *result);
int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);
int MPI_Comm_dup_with_info(MPI_Comm comm, MPI_Info info, MPI_Comm *newcomm);
int MPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);
int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
int MPI_Comm_free(MPI_Comm *comm);
int MPI_Comm_test_inter(MPI_Comm comm, int *flag);
int MPI_Comm_remote_size(MPI_Comm comm, int *size);
int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);
int MPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader, int tag, MPI_Comm *newintercomm);
int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintracomm);
int MPI_Keyval_create(MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state);
int MPI_Keyval_free(int *keyval);
int MPI_Attr_put(MPI_Comm comm, int keyval, void *attribute_val);
int MPI_Attr_get(MPI_Comm comm, int keyval, void *attribute_val, int *flag);
int MPI_Attr_delete(MPI_Comm comm, int keyval);
int MPI_Topo_test(MPI_Comm comm, int *status);
int MPI_Cart_create(MPI_Comm comm_old, int ndims, const int dims[], const int periods[], int reorder, MPI_Comm *comm_cart);
int MPI_Dims_create(int nnodes, int ndims, int dims[]);
int MPI_Graph_create(MPI_Comm comm_old, int nnodes, const int indx[], const int edges[], int reorder, MPI_Comm *comm_graph);
int MPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);
int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int indx[], int edges[]);
int MPI_Cartdim_get(MPI_Comm comm, int *ndims);
int MPI_Cart_get(MPI_Comm comm, int maxdims, int dims[], int periods[], int coords[]);
int MPI_Cart_rank(MPI_Comm comm, const int coords[], int *rank);
int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[]);
int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);
int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int neighbors[]);
int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
int MPI_Cart_sub(MPI_Comm comm, const int remain_dims[], MPI_Comm *newcomm);
int MPI_Cart_map(MPI_Comm comm, int ndims, const int dims[], const int periods[], int *newrank);
int MPI_Graph_map(MPI_Comm comm, int nnodes, const int indx[], const int edges[], int *newrank);
int MPI_Get_processor_name(char *name, int *resultlen);
int MPI_Get_version(int *version, int *subversion);
int MPI_Get_library_version(char *version, int *resultlen);
int MPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler);
int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);
int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);
int MPI_Errhandler_free(MPI_Errhandler *errhandler);
int MPI_Error_string(int errorcode, char *string, int *resultlen);
int MPI_Error_class(int errorcode, int *errorclass);
double MPI_Wtime(void);
double MPI_Wtick(void);
int MPI_Init(int *argc, char ***argv);
int MPI_Finalize(void);
int MPI_Initialized(int *flag);
int MPI_Abort(MPI_Comm comm, int errorcode);
int MPI_Pcontrol(const int level, ...);
int MPIR_Dup_fn(MPI_Comm oldcomm, int keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag);
int MPI_Close_port(const char *port_name);
int MPI_Comm_accept(const char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
int MPI_Comm_connect(const char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
int MPI_Comm_disconnect(MPI_Comm *comm);
int MPI_Comm_get_parent(MPI_Comm *parent);
int MPI_Comm_join(int fd, MPI_Comm *intercomm);
int MPI_Comm_spawn(const char *command, char *argv[], int maxprocs, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[]);
int MPI_Comm_spawn_multiple(int count, char *array_of_commands[], char **array_of_argv[], const int array_of_maxprocs[], const MPI_Info array_of_info[], int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[]);
int MPI_Lookup_name(const char *service_name, MPI_Info info, char *port_name);
int MPI_Open_port(MPI_Info info, char *port_name);
int MPI_Publish_name(const char *service_name, MPI_Info info, const char *port_name);
int MPI_Unpublish_name(const char *service_name, MPI_Info info, const char *port_name);
int MPI_Comm_set_info(MPI_Comm comm, MPI_Info info);
int MPI_Comm_get_info(MPI_Comm comm, MPI_Info *info);
int MPI_Accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
int MPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
int MPI_Put(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
int MPI_Win_complete(MPI_Win win);
int MPI_Win_create(void *base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, MPI_Win *win);
int MPI_Win_fence(int assert, MPI_Win win);
int MPI_Win_free(MPI_Win *win);
int MPI_Win_get_group(MPI_Win win, MPI_Group *group);
int MPI_Win_lock(int lock_type, int rank, int assert, MPI_Win win);
int MPI_Win_post(MPI_Group group, int assert, MPI_Win win);
int MPI_Win_start(MPI_Group group, int assert, MPI_Win win);
int MPI_Win_test(MPI_Win win, int *flag);
int MPI_Win_unlock(int rank, MPI_Win win);
int MPI_Win_wait(MPI_Win win);
int MPI_Win_allocate(MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, void *baseptr, MPI_Win *win);
int MPI_Win_allocate_shared(MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, void *baseptr, MPI_Win *win);
int MPI_Win_shared_query(MPI_Win win, int rank, MPI_Aint *size, int *disp_unit, void *baseptr);
int MPI_Win_create_dynamic(MPI_Info info, MPI_Comm comm, MPI_Win *win);
int MPI_Win_attach(MPI_Win win, void *base, MPI_Aint size);
int MPI_Win_detach(MPI_Win win, const void *base);
int MPI_Win_get_info(MPI_Win win, MPI_Info *info_used);
int MPI_Win_set_info(MPI_Win win, MPI_Info info);
int MPI_Get_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
int MPI_Fetch_and_op(const void *origin_addr, void *result_addr, MPI_Datatype datatype, int target_rank, MPI_Aint target_disp, MPI_Op op, MPI_Win win);
int MPI_Compare_and_swap(const void *origin_addr, const void *compare_addr, void *result_addr, MPI_Datatype datatype, int target_rank, MPI_Aint target_disp, MPI_Win win);
int MPI_Rput(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request);
int MPI_Rget(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request);
int MPI_Raccumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request);
int MPI_Rget_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request);
int MPI_Win_lock_all(int assert, MPI_Win win);
int MPI_Win_unlock_all(MPI_Win win);
int MPI_Win_flush(int rank, MPI_Win win);
int MPI_Win_flush_all(MPI_Win win);
int MPI_Win_flush_local(int rank, MPI_Win win);
int MPI_Win_flush_local_all(MPI_Win win);
int MPI_Win_sync(MPI_Win win);
int MPI_Add_error_class(int *errorclass);
int MPI_Add_error_code(int errorclass, int *errorcode);
int MPI_Add_error_string(int errorcode, const char *string);
int MPI_Comm_call_errhandler(MPI_Comm comm, int errorcode);
int MPI_Comm_create_keyval(MPI_Comm_copy_attr_function *comm_copy_attr_fn, MPI_Comm_delete_attr_function *comm_delete_attr_fn, int *comm_keyval, void *extra_state);
int MPI_Comm_delete_attr(MPI_Comm comm, int comm_keyval);
int MPI_Comm_free_keyval(int *comm_keyval);
int MPI_Comm_get_attr(MPI_Comm comm, int comm_keyval, void *attribute_val, int *flag);
int MPI_Comm_get_name(MPI_Comm comm, char *comm_name, int *resultlen);
int MPI_Comm_set_attr(MPI_Comm comm, int comm_keyval, void *attribute_val);
int MPI_Comm_set_name(MPI_Comm comm, const char *comm_name);
int MPI_File_call_errhandler(MPI_File fh, int errorcode);
int MPI_Grequest_complete(MPI_Request request);
int MPI_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, void *extra_state, MPI_Request *request);
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided);
int MPI_Is_thread_main(int *flag);
int MPI_Query_thread(int *provided);
int MPI_Status_set_cancelled(MPI_Status *status, int flag);
int MPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype, int count);
int MPI_Type_create_keyval(MPI_Type_copy_attr_function *type_copy_attr_fn, MPI_Type_delete_attr_function *type_delete_attr_fn, int *type_keyval, void *extra_state);
int MPI_Type_delete_attr(MPI_Datatype datatype, int type_keyval);
int MPI_Type_dup(MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_free_keyval(int *type_keyval);
int MPI_Type_get_attr(MPI_Datatype datatype, int type_keyval, void *attribute_val, int *flag);
int MPI_Type_get_contents(MPI_Datatype datatype, int max_integers, int max_addresses, int max_datatypes, int array_of_integers[], MPI_Aint array_of_addresses[], MPI_Datatype array_of_datatypes[]);
int MPI_Type_get_envelope(MPI_Datatype datatype, int *num_integers, int *num_addresses, int *num_datatypes, int *combiner);
int MPI_Type_get_name(MPI_Datatype datatype, char *type_name, int *resultlen);
int MPI_Type_set_attr(MPI_Datatype datatype, int type_keyval, void *attribute_val);
int MPI_Type_set_name(MPI_Datatype datatype, const char *type_name);
int MPI_Type_match_size(int typeclass, int size, MPI_Datatype *datatype);
int MPI_Win_call_errhandler(MPI_Win win, int errorcode);
int MPI_Win_create_keyval(MPI_Win_copy_attr_function *win_copy_attr_fn, MPI_Win_delete_attr_function *win_delete_attr_fn, int *win_keyval, void *extra_state);
int MPI_Win_delete_attr(MPI_Win win, int win_keyval);
int MPI_Win_free_keyval(int *win_keyval);
int MPI_Win_get_attr(MPI_Win win, int win_keyval, void *attribute_val, int *flag);
int MPI_Win_get_name(MPI_Win win, char *win_name, int *resultlen);
int MPI_Win_set_attr(MPI_Win win, int win_keyval, void *attribute_val);
int MPI_Win_set_name(MPI_Win win, const char *win_name);
int MPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
int MPI_Comm_create_errhandler(MPI_Comm_errhandler_function *comm_errhandler_fn, MPI_Errhandler *errhandler);
int MPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *errhandler);
int MPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhandler);
int MPI_File_create_errhandler(MPI_File_errhandler_function *file_errhandler_fn, MPI_Errhandler *errhandler);
int MPI_File_get_errhandler(MPI_File file, MPI_Errhandler *errhandler);
int MPI_File_set_errhandler(MPI_File file, MPI_Errhandler errhandler);
int MPI_Finalized(int *flag);
int MPI_Free_mem(void *base);
int MPI_Get_address(const void *location, MPI_Aint *address);
int MPI_Info_create(MPI_Info *info);
int MPI_Info_delete(MPI_Info info, const char *key);
int MPI_Info_dup(MPI_Info info, MPI_Info *newinfo);
int MPI_Info_free(MPI_Info *info);
int MPI_Info_get(MPI_Info info, const char *key, int valuelen, char *value, int *flag);
int MPI_Info_get_nkeys(MPI_Info info, int *nkeys);
int MPI_Info_get_nthkey(MPI_Info info, int n, char *key);
int MPI_Info_get_valuelen(MPI_Info info, const char *key, int *valuelen, int *flag);
int MPI_Info_set(MPI_Info info, const char *key, const char *value);
int MPI_Pack_external(const char datarep[], const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, MPI_Aint outsize, MPI_Aint *position);
int MPI_Pack_external_size(const char datarep[], int incount, MPI_Datatype datatype, MPI_Aint *size);
int MPI_Request_get_status(MPI_Request request, int *flag, MPI_Status *status);
int MPI_Status_c2f(const MPI_Status *c_status, MPI_Fint *f_status);
int MPI_Status_f2c(const MPI_Fint *f_status, MPI_Status *c_status);
int MPI_Type_create_darray(int size, int rank, int ndims, const int array_of_gsizes[], const int array_of_distribs[], const int array_of_dargs[], const int array_of_psizes[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_create_hindexed(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_create_indexed_block(int count, int blocklength, const int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_create_hindexed_block(int count, int blocklength, const MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype);
int MPI_Type_create_struct(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], const MPI_Datatype array_of_types[], MPI_Datatype *newtype);
int MPI_Type_create_subarray(int ndims, const int array_of_sizes[], const int array_of_subsizes[], const int array_of_starts[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
int MPI_Type_get_extent(MPI_Datatype datatype, MPI_Aint *lb, MPI_Aint *extent);
int MPI_Type_get_true_extent(MPI_Datatype datatype, MPI_Aint *true_lb, MPI_Aint *true_extent);
int MPI_Unpack_external(const char datarep[], const void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype);
int MPI_Win_create_errhandler(MPI_Win_errhandler_function *win_errhandler_fn, MPI_Errhandler *errhandler);
int MPI_Win_get_errhandler(MPI_Win win, MPI_Errhandler *errhandler);
int MPI_Win_set_errhandler(MPI_Win win, MPI_Errhandler errhandler);
int MPI_Type_create_f90_integer(int range, MPI_Datatype *newtype);
int MPI_Type_create_f90_real(int precision, int range, MPI_Datatype *newtype);
int MPI_Type_create_f90_complex(int precision, int range, MPI_Datatype *newtype);
int MPI_Reduce_local(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op);
int MPI_Op_commutative(MPI_Op op, int *commute);
int MPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, int recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPI_Dist_graph_create_adjacent(MPI_Comm comm_old, int indegree, const int sources[], const int sourceweights[], int outdegree, const int destinations[], const int destweights[], MPI_Info info, int reorder, MPI_Comm *comm_dist_graph);
int MPI_Dist_graph_create(MPI_Comm comm_old, int n, const int sources[], const int degrees[], const int destinations[], const int weights[], MPI_Info info, int reorder, MPI_Comm *comm_dist_graph);
int MPI_Dist_graph_neighbors_count(MPI_Comm comm, int *indegree, int *outdegree, int *weighted);
int MPI_Dist_graph_neighbors(MPI_Comm comm, int maxindegree, int sources[], int sourceweights[], int maxoutdegree, int destinations[], int destweights[]);
int MPI_Improbe(int source, int tag, MPI_Comm comm, int *flag, MPI_Message *message, MPI_Status *status);
int MPI_Imrecv(void *buf, int count, MPI_Datatype datatype, MPI_Message *message, MPI_Request *request);
int MPI_Mprobe(int source, int tag, MPI_Comm comm, MPI_Message *message, MPI_Status *status);
int MPI_Mrecv(void *buf, int count, MPI_Datatype datatype, MPI_Message *message, MPI_Status *status);
int MPI_Comm_idup(MPI_Comm comm, MPI_Comm *newcomm, MPI_Request *request);
int MPI_Ibarrier(MPI_Comm comm, MPI_Request *request);
int MPI_Ibcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request);
int MPI_Igather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPI_Igatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPI_Iscatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPI_Iscatterv(const void *sendbuf, const int sendcounts[], const int displs[], MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int MPI_Iallgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Iallgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ialltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ialltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ialltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);
int MPI_Ireduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request *request);
int MPI_Iallreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int MPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int MPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, int recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int MPI_Iscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int MPI_Iexscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int MPI_Ineighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ineighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ineighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ineighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int MPI_Ineighbor_alltoallw(const void *sendbuf, const int sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);
int MPI_Neighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Neighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Neighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Neighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Neighbor_alltoallw(const void *sendbuf, const int sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);
int MPI_Comm_split_type(MPI_Comm comm, int split_type, int key, MPI_Info info, MPI_Comm *newcomm);
int MPI_Get_elements_x(const MPI_Status *status, MPI_Datatype datatype, MPI_Count *count);
int MPI_Status_set_elements_x(MPI_Status *status, MPI_Datatype datatype, MPI_Count count);
int MPI_Type_get_extent_x(MPI_Datatype datatype, MPI_Count *lb, MPI_Count *extent);
int MPI_Type_get_true_extent_x(MPI_Datatype datatype, MPI_Count *lb, MPI_Count *extent);
int MPI_Type_size_x(MPI_Datatype datatype, MPI_Count *size);
int MPI_Comm_create_group(MPI_Comm comm, MPI_Group group, int tag, MPI_Comm *newcomm);
int MPIX_Comm_group_failed(MPI_Comm comm, MPI_Group *failed_group);
int MPIX_Comm_remote_group_failed(MPI_Comm comm, MPI_Group *failed_group);
int MPIX_Comm_reenable_anysource(MPI_Comm comm, MPI_Group *failed_group);
int MPI_T_init_thread(int required, int *provided);
int MPI_T_finalize(void);
int MPI_T_enum_get_info(MPI_T_enum enumtype, int *num, char *name, int *name_len);
int MPI_T_enum_get_item(MPI_T_enum enumtype, int indx, int *value, char *name, int *name_len);
int MPI_T_cvar_get_num(int *num_cvar);
int MPI_T_cvar_get_info(int cvar_index, char *name, int *name_len, int *verbosity, MPI_Datatype *datatype, MPI_T_enum *enumtype, char *desc, int *desc_len, int *binding, int *scope);
int MPI_T_cvar_handle_alloc(int cvar_index, void *obj_handle, MPI_T_cvar_handle *handle, int *count);
int MPI_T_cvar_handle_free(MPI_T_cvar_handle *handle);
int MPI_T_cvar_read(MPI_T_cvar_handle handle, void *buf);
int MPI_T_cvar_write(MPI_T_cvar_handle handle, const void *buf);
int MPI_T_pvar_get_num(int *num_pvar);
int MPI_T_pvar_get_info(int pvar_index, char *name, int *name_len, int *verbosity, int *var_class, MPI_Datatype *datatype, MPI_T_enum *enumtype, char *desc, int *desc_len, int *binding, int *readonly, int *continuous, int *atomic);
int MPI_T_pvar_session_create(MPI_T_pvar_session *session);
int MPI_T_pvar_session_free(MPI_T_pvar_session *session);
int MPI_T_pvar_handle_alloc(MPI_T_pvar_session session, int pvar_index, void *obj_handle, MPI_T_pvar_handle *handle, int *count);
int MPI_T_pvar_handle_free(MPI_T_pvar_session session, MPI_T_pvar_handle *handle);
int MPI_T_pvar_start(MPI_T_pvar_session session, MPI_T_pvar_handle handle);
int MPI_T_pvar_stop(MPI_T_pvar_session session, MPI_T_pvar_handle handle);
int MPI_T_pvar_read(MPI_T_pvar_session session, MPI_T_pvar_handle handle, void *buf);
int MPI_T_pvar_write(MPI_T_pvar_session session, MPI_T_pvar_handle handle, const void *buf);
int MPI_T_pvar_reset(MPI_T_pvar_session session, MPI_T_pvar_handle handle);
int MPI_T_pvar_readreset(MPI_T_pvar_session session, MPI_T_pvar_handle handle, void *buf);
int MPI_T_category_get_num(int *num_cat);
int MPI_T_category_get_info(int cat_index, char *name, int *name_len, char *desc, int *desc_len, int *num_cvars, int *num_pvars, int *num_categories);
int MPI_T_category_get_cvars(int cat_index, int len, int indices[]);
int MPI_T_category_get_pvars(int cat_index, int len, int indices[]);
int MPI_T_category_get_categories(int cat_index, int len, int indices[]);
int MPI_T_category_changed(int *stamp);
int PMPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int PMPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
int PMPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count);
int PMPI_Bsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int PMPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int PMPI_Rsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int PMPI_Buffer_attach(void *buffer, int size);
int PMPI_Buffer_detach(void *buffer_addr, int *size);
int PMPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Ibsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Irsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Wait(MPI_Request *request, MPI_Status *status);
int PMPI_Test(MPI_Request *request, int *flag, MPI_Status *status);
int PMPI_Request_free(MPI_Request *request);
int PMPI_Waitany(int count, MPI_Request array_of_requests[], int *indx, MPI_Status *status);
int PMPI_Testany(int count, MPI_Request array_of_requests[], int *indx, int *flag, MPI_Status *status);
int PMPI_Waitall(int count, MPI_Request array_of_requests[], MPI_Status array_of_statuses[]);
int PMPI_Testall(int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[]);
int PMPI_Waitsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]);
int PMPI_Testsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]);
int PMPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);
int PMPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
int PMPI_Cancel(MPI_Request *request);
int PMPI_Test_cancelled(const MPI_Status *status, int *flag);
int PMPI_Send_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Bsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Ssend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Rsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
int PMPI_Start(MPI_Request *request);
int PMPI_Startall(int count, MPI_Request array_of_requests[]);
int PMPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
int PMPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status);
int PMPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_indexed(int count, const int *array_of_blocklengths, const int *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_hindexed(int count, const int *array_of_blocklengths, const MPI_Aint *array_of_displacements, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_struct(int count, const int *array_of_blocklengths, const MPI_Aint *array_of_displacements, const MPI_Datatype *array_of_types, MPI_Datatype *newtype);
int PMPI_Address(const void *location, MPI_Aint *address);
int PMPI_Type_extent(MPI_Datatype datatype, MPI_Aint *extent);
int PMPI_Type_size(MPI_Datatype datatype, int *size);
int PMPI_Type_lb(MPI_Datatype datatype, MPI_Aint *displacement);
int PMPI_Type_ub(MPI_Datatype datatype, MPI_Aint *displacement);
int PMPI_Type_commit(MPI_Datatype *datatype);
int PMPI_Type_free(MPI_Datatype *datatype);
int PMPI_Get_elements(const MPI_Status *status, MPI_Datatype datatype, int *count);
int PMPI_Pack(const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm);
int PMPI_Unpack(const void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm);
int PMPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size);
int PMPI_Barrier(MPI_Comm comm);
int PMPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int PMPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int PMPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm);
int PMPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int PMPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int PMPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Alltoallv(const void *sendbuf, const int *sendcounts, const int *sdispls, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *rdispls, MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Alltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);
int PMPI_Exscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int PMPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int PMPI_Op_create(MPI_User_function *user_fn, int commute, MPI_Op *op);
int PMPI_Op_free(MPI_Op *op);
int PMPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int PMPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int PMPI_Scan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int PMPI_Group_size(MPI_Group group, int *size);
int PMPI_Group_rank(MPI_Group group, int *rank);
int PMPI_Group_translate_ranks(MPI_Group group1, int n, const int ranks1[], MPI_Group group2, int ranks2[]);
int PMPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result);
int PMPI_Comm_group(MPI_Comm comm, MPI_Group *group);
int PMPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
int PMPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
int PMPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);
int PMPI_Group_incl(MPI_Group group, int n, const int ranks[], MPI_Group *newgroup);
int PMPI_Group_excl(MPI_Group group, int n, const int ranks[], MPI_Group *newgroup);
int PMPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
int PMPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);
int PMPI_Group_free(MPI_Group *group);
int PMPI_Comm_size(MPI_Comm comm, int *size);
int PMPI_Comm_rank(MPI_Comm comm, int *rank);
int PMPI_Comm_compare(MPI_Comm comm1, MPI_Comm comm2, int *result);
int PMPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);
int PMPI_Comm_dup_with_info(MPI_Comm comm, MPI_Info info, MPI_Comm *newcomm);
int PMPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);
int PMPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
int PMPI_Comm_free(MPI_Comm *comm);
int PMPI_Comm_test_inter(MPI_Comm comm, int *flag);
int PMPI_Comm_remote_size(MPI_Comm comm, int *size);
int PMPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);
int PMPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader, int tag, MPI_Comm *newintercomm);
int PMPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintracomm);
int PMPI_Keyval_create(MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state);
int PMPI_Keyval_free(int *keyval);
int PMPI_Attr_put(MPI_Comm comm, int keyval, void *attribute_val);
int PMPI_Attr_get(MPI_Comm comm, int keyval, void *attribute_val, int *flag);
int PMPI_Attr_delete(MPI_Comm comm, int keyval);
int PMPI_Topo_test(MPI_Comm comm, int *status);
int PMPI_Cart_create(MPI_Comm comm_old, int ndims, const int dims[], const int periods[], int reorder, MPI_Comm *comm_cart);
int PMPI_Dims_create(int nnodes, int ndims, int dims[]);
int PMPI_Graph_create(MPI_Comm comm_old, int nnodes, const int indx[], const int edges[], int reorder, MPI_Comm *comm_graph);
int PMPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);
int PMPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int indx[], int edges[]);
int PMPI_Cartdim_get(MPI_Comm comm, int *ndims);
int PMPI_Cart_get(MPI_Comm comm, int maxdims, int dims[], int periods[], int coords[]);
int PMPI_Cart_rank(MPI_Comm comm, const int coords[], int *rank);
int PMPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[]);
int PMPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);
int PMPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int neighbors[]);
int PMPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
int PMPI_Cart_sub(MPI_Comm comm, const int remain_dims[], MPI_Comm *newcomm);
int PMPI_Cart_map(MPI_Comm comm, int ndims, const int dims[], const int periods[], int *newrank);
int PMPI_Graph_map(MPI_Comm comm, int nnodes, const int indx[], const int edges[], int *newrank);
int PMPI_Get_processor_name(char *name, int *resultlen);
int PMPI_Get_version(int *version, int *subversion);
int PMPI_Get_library_version(char *version, int *resultlen);
int PMPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler);
int PMPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);
int PMPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);
int PMPI_Errhandler_free(MPI_Errhandler *errhandler);
int PMPI_Error_string(int errorcode, char *string, int *resultlen);
int PMPI_Error_class(int errorcode, int *errorclass);
double PMPI_Wtime(void);
double PMPI_Wtick(void);
int PMPI_Init(int *argc, char ***argv);
int PMPI_Finalize(void);
int PMPI_Initialized(int *flag);
int PMPI_Abort(MPI_Comm comm, int errorcode);
int PMPI_Pcontrol(const int level, ...);
int PMPI_Close_port(const char *port_name);
int PMPI_Comm_accept(const char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
int PMPI_Comm_connect(const char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm);
int PMPI_Comm_disconnect(MPI_Comm *comm);
int PMPI_Comm_get_parent(MPI_Comm *parent);
int PMPI_Comm_join(int fd, MPI_Comm *intercomm);
int PMPI_Comm_spawn(const char *command, char *argv[], int maxprocs, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[]);
int PMPI_Comm_spawn_multiple(int count, char *array_of_commands[], char **array_of_argv[], const int array_of_maxprocs[], const MPI_Info array_of_info[], int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[]);
int PMPI_Lookup_name(const char *service_name, MPI_Info info, char *port_name);
int PMPI_Open_port(MPI_Info info, char *port_name);
int PMPI_Publish_name(const char *service_name, MPI_Info info, const char *port_name);
int PMPI_Unpublish_name(const char *service_name, MPI_Info info, const char *port_name);
int PMPI_Comm_set_info(MPI_Comm comm, MPI_Info info);
int PMPI_Comm_get_info(MPI_Comm comm, MPI_Info *info);
int PMPI_Accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
int PMPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
int PMPI_Put(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win);
int PMPI_Win_complete(MPI_Win win);
int PMPI_Win_create(void *base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, MPI_Win *win);
int PMPI_Win_fence(int assert, MPI_Win win);
int PMPI_Win_free(MPI_Win *win);
int PMPI_Win_get_group(MPI_Win win, MPI_Group *group);
int PMPI_Win_lock(int lock_type, int rank, int assert, MPI_Win win);
int PMPI_Win_post(MPI_Group group, int assert, MPI_Win win);
int PMPI_Win_start(MPI_Group group, int assert, MPI_Win win);
int PMPI_Win_test(MPI_Win win, int *flag);
int PMPI_Win_unlock(int rank, MPI_Win win);
int PMPI_Win_wait(MPI_Win win);
int PMPI_Win_allocate(MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, void *baseptr, MPI_Win *win);
int PMPI_Win_allocate_shared(MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, void *baseptr, MPI_Win *win);
int PMPI_Win_shared_query(MPI_Win win, int rank, MPI_Aint *size, int *disp_unit, void *baseptr);
int PMPI_Win_create_dynamic(MPI_Info info, MPI_Comm comm, MPI_Win *win);
int PMPI_Win_attach(MPI_Win win, void *base, MPI_Aint size);
int PMPI_Win_detach(MPI_Win win, const void *base);
int PMPI_Win_get_info(MPI_Win win, MPI_Info *info_used);
int PMPI_Win_set_info(MPI_Win win, MPI_Info info);
int PMPI_Get_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win);
int PMPI_Fetch_and_op(const void *origin_addr, void *result_addr, MPI_Datatype datatype, int target_rank, MPI_Aint target_disp, MPI_Op op, MPI_Win win);
int PMPI_Compare_and_swap(const void *origin_addr, const void *compare_addr, void *result_addr, MPI_Datatype datatype, int target_rank, MPI_Aint target_disp, MPI_Win win);
int PMPI_Rput(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request);
int PMPI_Rget(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request);
int PMPI_Raccumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request);
int PMPI_Rget_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request);
int PMPI_Win_lock_all(int assert, MPI_Win win);
int PMPI_Win_unlock_all(MPI_Win win);
int PMPI_Win_flush(int rank, MPI_Win win);
int PMPI_Win_flush_all(MPI_Win win);
int PMPI_Win_flush_local(int rank, MPI_Win win);
int PMPI_Win_flush_local_all(MPI_Win win);
int PMPI_Win_sync(MPI_Win win);
int PMPI_Add_error_class(int *errorclass);
int PMPI_Add_error_code(int errorclass, int *errorcode);
int PMPI_Add_error_string(int errorcode, const char *string);
int PMPI_Comm_call_errhandler(MPI_Comm comm, int errorcode);
int PMPI_Comm_create_keyval(MPI_Comm_copy_attr_function *comm_copy_attr_fn, MPI_Comm_delete_attr_function *comm_delete_attr_fn, int *comm_keyval, void *extra_state);
int PMPI_Comm_delete_attr(MPI_Comm comm, int comm_keyval);
int PMPI_Comm_free_keyval(int *comm_keyval);
int PMPI_Comm_get_attr(MPI_Comm comm, int comm_keyval, void *attribute_val, int *flag);
int PMPI_Comm_get_name(MPI_Comm comm, char *comm_name, int *resultlen);
int PMPI_Comm_set_attr(MPI_Comm comm, int comm_keyval, void *attribute_val);
int PMPI_Comm_set_name(MPI_Comm comm, const char *comm_name);
int PMPI_File_call_errhandler(MPI_File fh, int errorcode);
int PMPI_Grequest_complete(MPI_Request request);
int PMPI_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, void *extra_state, MPI_Request *request);
int PMPI_Init_thread(int *argc, char ***argv, int required, int *provided);
int PMPI_Is_thread_main(int *flag);
int PMPI_Query_thread(int *provided);
int PMPI_Status_set_cancelled(MPI_Status *status, int flag);
int PMPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype, int count);
int PMPI_Type_create_keyval(MPI_Type_copy_attr_function *type_copy_attr_fn, MPI_Type_delete_attr_function *type_delete_attr_fn, int *type_keyval, void *extra_state);
int PMPI_Type_delete_attr(MPI_Datatype datatype, int type_keyval);
int PMPI_Type_dup(MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_free_keyval(int *type_keyval);
int PMPI_Type_get_attr(MPI_Datatype datatype, int type_keyval, void *attribute_val, int *flag);
int PMPI_Type_get_contents(MPI_Datatype datatype, int max_integers, int max_addresses, int max_datatypes, int array_of_integers[], MPI_Aint array_of_addresses[], MPI_Datatype array_of_datatypes[]);
int PMPI_Type_get_envelope(MPI_Datatype datatype, int *num_integers, int *num_addresses, int *num_datatypes, int *combiner);
int PMPI_Type_get_name(MPI_Datatype datatype, char *type_name, int *resultlen);
int PMPI_Type_set_attr(MPI_Datatype datatype, int type_keyval, void *attribute_val);
int PMPI_Type_set_name(MPI_Datatype datatype, const char *type_name);
int PMPI_Type_match_size(int typeclass, int size, MPI_Datatype *datatype);
int PMPI_Win_call_errhandler(MPI_Win win, int errorcode);
int PMPI_Win_create_keyval(MPI_Win_copy_attr_function *win_copy_attr_fn, MPI_Win_delete_attr_function *win_delete_attr_fn, int *win_keyval, void *extra_state);
int PMPI_Win_delete_attr(MPI_Win win, int win_keyval);
int PMPI_Win_free_keyval(int *win_keyval);
int PMPI_Win_get_attr(MPI_Win win, int win_keyval, void *attribute_val, int *flag);
int PMPI_Win_get_name(MPI_Win win, char *win_name, int *resultlen);
int PMPI_Win_set_attr(MPI_Win win, int win_keyval, void *attribute_val);
int PMPI_Win_set_name(MPI_Win win, const char *win_name);
int PMPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr);
int PMPI_Comm_create_errhandler(MPI_Comm_errhandler_function *comm_errhandler_fn, MPI_Errhandler *errhandler);
int PMPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *errhandler);
int PMPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhandler);
int PMPI_File_create_errhandler(MPI_File_errhandler_function *file_errhandler_fn, MPI_Errhandler *errhandler);
int PMPI_File_get_errhandler(MPI_File file, MPI_Errhandler *errhandler);
int PMPI_File_set_errhandler(MPI_File file, MPI_Errhandler errhandler);
int PMPI_Finalized(int *flag);
int PMPI_Free_mem(void *base);
int PMPI_Get_address(const void *location, MPI_Aint *address);
int PMPI_Info_create(MPI_Info *info);
int PMPI_Info_delete(MPI_Info info, const char *key);
int PMPI_Info_dup(MPI_Info info, MPI_Info *newinfo);
int PMPI_Info_free(MPI_Info *info);
int PMPI_Info_get(MPI_Info info, const char *key, int valuelen, char *value, int *flag);
int PMPI_Info_get_nkeys(MPI_Info info, int *nkeys);
int PMPI_Info_get_nthkey(MPI_Info info, int n, char *key);
int PMPI_Info_get_valuelen(MPI_Info info, const char *key, int *valuelen, int *flag);
int PMPI_Info_set(MPI_Info info, const char *key, const char *value);
int PMPI_Pack_external(const char datarep[], const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, MPI_Aint outsize, MPI_Aint *position);
int PMPI_Pack_external_size(const char datarep[], int incount, MPI_Datatype datatype, MPI_Aint *size);
int PMPI_Request_get_status(MPI_Request request, int *flag, MPI_Status *status);
int PMPI_Status_c2f(const MPI_Status *c_status, MPI_Fint *f_status);
int PMPI_Status_f2c(const MPI_Fint *f_status, MPI_Status *c_status);
int PMPI_Type_create_darray(int size, int rank, int ndims, const int array_of_gsizes[], const int array_of_distribs[], const int array_of_dargs[], const int array_of_psizes[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_create_hindexed(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_create_indexed_block(int count, int blocklength, const int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_create_hindexed_block(int count, int blocklength, const MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype);
int PMPI_Type_create_struct(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], const MPI_Datatype array_of_types[], MPI_Datatype *newtype);
int PMPI_Type_create_subarray(int ndims, const int array_of_sizes[], const int array_of_subsizes[], const int array_of_starts[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
int PMPI_Type_get_extent(MPI_Datatype datatype, MPI_Aint *lb, MPI_Aint *extent);
int PMPI_Type_get_true_extent(MPI_Datatype datatype, MPI_Aint *true_lb, MPI_Aint *true_extent);
int PMPI_Unpack_external(const char datarep[], const void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype);
int PMPI_Win_create_errhandler(MPI_Win_errhandler_function *win_errhandler_fn, MPI_Errhandler *errhandler);
int PMPI_Win_get_errhandler(MPI_Win win, MPI_Errhandler *errhandler);
int PMPI_Win_set_errhandler(MPI_Win win, MPI_Errhandler errhandler);
int PMPI_Type_create_f90_integer(int r, MPI_Datatype *newtype);
int PMPI_Type_create_f90_real(int p, int r, MPI_Datatype *newtype);
int PMPI_Type_create_f90_complex(int p, int r, MPI_Datatype *newtype);
int PMPI_Reduce_local(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op);
int PMPI_Op_commutative(MPI_Op op, int *commute);
int PMPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, int recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int PMPI_Dist_graph_create_adjacent(MPI_Comm comm_old, int indegree, const int sources[], const int sourceweights[], int outdegree, const int destinations[], const int destweights[], MPI_Info info, int reorder, MPI_Comm *comm_dist_graph);
int PMPI_Dist_graph_create(MPI_Comm comm_old, int n, const int sources[], const int degrees[], const int destinations[], const int weights[], MPI_Info info, int reorder, MPI_Comm *comm_dist_graph);
int PMPI_Dist_graph_neighbors_count(MPI_Comm comm, int *indegree, int *outdegree, int *weighted);
int PMPI_Dist_graph_neighbors(MPI_Comm comm, int maxindegree, int sources[], int sourceweights[], int maxoutdegree, int destinations[], int destweights[]);
int PMPI_Improbe(int source, int tag, MPI_Comm comm, int *flag, MPI_Message *message, MPI_Status *status);
int PMPI_Imrecv(void *buf, int count, MPI_Datatype datatype, MPI_Message *message, MPI_Request *request);
int PMPI_Mprobe(int source, int tag, MPI_Comm comm, MPI_Message *message, MPI_Status *status);
int PMPI_Mrecv(void *buf, int count, MPI_Datatype datatype, MPI_Message *message, MPI_Status *status);
int PMPI_Comm_idup(MPI_Comm comm, MPI_Comm *newcomm, MPI_Request *request);
int PMPI_Ibarrier(MPI_Comm comm, MPI_Request *request);
int PMPI_Ibcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request);
int PMPI_Igather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int PMPI_Igatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int PMPI_Iscatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int PMPI_Iscatterv(const void *sendbuf, const int sendcounts[], const int displs[], MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request);
int PMPI_Iallgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Iallgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ialltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ialltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ialltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);
int PMPI_Ireduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request *request);
int PMPI_Iallreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int PMPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int PMPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, int recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int PMPI_Iscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int PMPI_Iexscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request);
int PMPI_Ineighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ineighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ineighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ineighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request);
int PMPI_Ineighbor_alltoallw(const void *sendbuf, const int sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request);
int PMPI_Neighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Neighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Neighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Neighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm);
int PMPI_Neighbor_alltoallw(const void *sendbuf, const int sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm);
int PMPI_Comm_split_type(MPI_Comm comm, int split_type, int key, MPI_Info info, MPI_Comm *newcomm);
int PMPI_Comm_create_group(MPI_Comm comm, MPI_Group group, int tag, MPI_Comm *newcomm);
int PMPI_Get_elements_x(const MPI_Status *status, MPI_Datatype datatype, MPI_Count *count);
int PMPI_Status_set_elements_x(MPI_Status *status, MPI_Datatype datatype, MPI_Count count);
int PMPI_Type_get_extent_x(MPI_Datatype datatype, MPI_Count *lb, MPI_Count *extent);
int PMPI_Type_get_true_extent_x(MPI_Datatype datatype, MPI_Count *lb, MPI_Count *extent);
int PMPI_Type_size_x(MPI_Datatype datatype, MPI_Count *size);
int PMPIX_Comm_group_failed(MPI_Comm comm, MPI_Group *failed_group);
int PMPIX_Comm_remote_group_failed(MPI_Comm comm, MPI_Group *failed_group);
int PMPIX_Comm_reenable_anysource(MPI_Comm comm, MPI_Group *failed_group);
int PMPI_T_init_thread(int required, int *provided);
int PMPI_T_finalize(void);
int PMPI_T_enum_get_info(MPI_T_enum enumtype, int *num, char *name, int *name_len);
int PMPI_T_enum_get_item(MPI_T_enum enumtype, int indx, int *value, char *name, int *name_len);
int PMPI_T_cvar_get_num(int *num_cvar);
int PMPI_T_cvar_get_info(int cvar_index, char *name, int *name_len, int *verbosity, MPI_Datatype *datatype, MPI_T_enum *enumtype, char *desc, int *desc_len, int *binding, int *scope);
int PMPI_T_cvar_handle_alloc(int cvar_index, void *obj_handle, MPI_T_cvar_handle *handle, int *count);
int PMPI_T_cvar_handle_free(MPI_T_cvar_handle *handle);
int PMPI_T_cvar_read(MPI_T_cvar_handle handle, void *buf);
int PMPI_T_cvar_write(MPI_T_cvar_handle handle, const void *buf);
int PMPI_T_pvar_get_num(int *num_pvar);
int PMPI_T_pvar_get_info(int pvar_index, char *name, int *name_len, int *verbosity, int *var_class, MPI_Datatype *datatype, MPI_T_enum *enumtype, char *desc, int *desc_len, int *binding, int *readonly, int *continuous, int *atomic);
int PMPI_T_pvar_session_create(MPI_T_pvar_session *session);
int PMPI_T_pvar_session_free(MPI_T_pvar_session *session);
int PMPI_T_pvar_handle_alloc(MPI_T_pvar_session session, int pvar_index, void *obj_handle, MPI_T_pvar_handle *handle, int *count);
int PMPI_T_pvar_handle_free(MPI_T_pvar_session session, MPI_T_pvar_handle *handle);
int PMPI_T_pvar_start(MPI_T_pvar_session session, MPI_T_pvar_handle handle);
int PMPI_T_pvar_stop(MPI_T_pvar_session session, MPI_T_pvar_handle handle);
int PMPI_T_pvar_read(MPI_T_pvar_session session, MPI_T_pvar_handle handle, void *buf);
int PMPI_T_pvar_write(MPI_T_pvar_session session, MPI_T_pvar_handle handle, const void *buf);
int PMPI_T_pvar_reset(MPI_T_pvar_session session, MPI_T_pvar_handle handle);
int PMPI_T_pvar_readreset(MPI_T_pvar_session session, MPI_T_pvar_handle handle, void *buf);
int PMPI_T_category_get_num(int *num_cat);
int PMPI_T_category_get_info(int cat_index, char *name, int *name_len, char *desc, int *desc_len, int *num_cvars, int *num_pvars, int *num_categories);
int PMPI_T_category_get_cvars(int cat_index, int len, int indices[]);
int PMPI_T_category_get_pvars(int cat_index, int len, int indices[]);
int PMPI_T_category_get_categories(int cat_index, int len, int indices[]);
int PMPI_T_category_changed(int *stamp);
int MPI_File_open(MPI_Comm comm, const char *filename, int amode, MPI_Info info, MPI_File *fh);
int MPI_File_close(MPI_File *fh);
int MPI_File_delete(const char *filename, MPI_Info info);
int MPI_File_set_size(MPI_File fh, MPI_Offset size);
int MPI_File_preallocate(MPI_File fh, MPI_Offset size);
int MPI_File_get_size(MPI_File fh, MPI_Offset *size);
int MPI_File_get_group(MPI_File fh, MPI_Group *group);
int MPI_File_get_amode(MPI_File fh, int *amode);
int MPI_File_set_info(MPI_File fh, MPI_Info info);
int MPI_File_get_info(MPI_File fh, MPI_Info *info_used);
int MPI_File_set_view(MPI_File fh, MPI_Offset disp, MPI_Datatype etype, MPI_Datatype filetype, const char *datarep, MPI_Info info);
int MPI_File_get_view(MPI_File fh, MPI_Offset *disp, MPI_Datatype *etype, MPI_Datatype *filetype, char *datarep);
int MPI_File_read_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_write_at(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_write_at_all(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_iread_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
int MPI_File_iwrite_at(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
int MPI_File_read(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_read_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_write(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_write_all(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_iread(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
int MPI_File_iwrite(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
int MPI_File_seek(MPI_File fh, MPI_Offset offset, int whence);
int MPI_File_get_position(MPI_File fh, MPI_Offset *offset);
int MPI_File_get_byte_offset(MPI_File fh, MPI_Offset offset, MPI_Offset *disp);
int MPI_File_read_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_write_shared(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_iread_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
int MPI_File_iwrite_shared(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request);
int MPI_File_read_ordered(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_write_ordered(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int MPI_File_seek_shared(MPI_File fh, MPI_Offset offset, int whence);
int MPI_File_get_position_shared(MPI_File fh, MPI_Offset *offset);
int MPI_File_read_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype);
int MPI_File_read_at_all_end(MPI_File fh, void *buf, MPI_Status *status);
int MPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype);
int MPI_File_write_at_all_end(MPI_File fh, const void *buf, MPI_Status *status);
int MPI_File_read_all_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
int MPI_File_read_all_end(MPI_File fh, void *buf, MPI_Status *status);
int MPI_File_write_all_begin(MPI_File fh, const void *buf, int count, MPI_Datatype datatype);
int MPI_File_write_all_end(MPI_File fh, const void *buf, MPI_Status *status);
int MPI_File_read_ordered_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
int MPI_File_read_ordered_end(MPI_File fh, void *buf, MPI_Status *status);
int MPI_File_write_ordered_begin(MPI_File fh, const void *buf, int count, MPI_Datatype datatype);
int MPI_File_write_ordered_end(MPI_File fh, const void *buf, MPI_Status *status);
int MPI_File_get_type_extent(MPI_File fh, MPI_Datatype datatype, MPI_Aint *extent);
int MPI_Register_datarep(const char *datarep, MPI_Datarep_conversion_function *read_conversion_fn, MPI_Datarep_conversion_function *write_conversion_fn, MPI_Datarep_extent_function *dtype_file_extent_fn, void *extra_state);
int MPI_File_set_atomicity(MPI_File fh, int flag);
int MPI_File_get_atomicity(MPI_File fh, int *flag);
int MPI_File_sync(MPI_File fh);
MPI_File MPI_File_f2c(MPI_Fint file);
MPI_Fint MPI_File_c2f(MPI_File file);
int PMPI_File_open(MPI_Comm, const char *, int, MPI_Info, MPI_File *);
int PMPI_File_close(MPI_File *);
int PMPI_File_delete(const char *, MPI_Info);
int PMPI_File_set_size(MPI_File, MPI_Offset);
int PMPI_File_preallocate(MPI_File, MPI_Offset);
int PMPI_File_get_size(MPI_File, MPI_Offset *);
int PMPI_File_get_group(MPI_File, MPI_Group *);
int PMPI_File_get_amode(MPI_File, int *);
int PMPI_File_set_info(MPI_File, MPI_Info);
int PMPI_File_get_info(MPI_File, MPI_Info *);
int PMPI_File_set_view(MPI_File, MPI_Offset, MPI_Datatype, MPI_Datatype, const char *, MPI_Info);
int PMPI_File_get_view(MPI_File, MPI_Offset *, MPI_Datatype *, MPI_Datatype *, char *);
int PMPI_File_read_at(MPI_File, MPI_Offset, void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_read_at_all(MPI_File, MPI_Offset, void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_write_at(MPI_File, MPI_Offset, const void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_write_at_all(MPI_File, MPI_Offset, const void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_iread_at(MPI_File, MPI_Offset, void *, int, MPI_Datatype, MPI_Request *);
int PMPI_File_iwrite_at(MPI_File, MPI_Offset, const void *, int, MPI_Datatype, MPI_Request *);
int PMPI_File_read(MPI_File, void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_read_all(MPI_File, void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_write(MPI_File, const void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_write_all(MPI_File, const void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_iread(MPI_File, void *, int, MPI_Datatype, MPI_Request *);
int PMPI_File_iwrite(MPI_File, const void *, int, MPI_Datatype, MPI_Request *);
int PMPI_File_seek(MPI_File, MPI_Offset, int);
int PMPI_File_get_position(MPI_File, MPI_Offset *);
int PMPI_File_get_byte_offset(MPI_File, MPI_Offset, MPI_Offset *);
int PMPI_File_read_shared(MPI_File, void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_write_shared(MPI_File, const void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_iread_shared(MPI_File, void *, int, MPI_Datatype, MPI_Request *);
int PMPI_File_iwrite_shared(MPI_File, const void *, int, MPI_Datatype, MPI_Request *);
int PMPI_File_read_ordered(MPI_File, void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_write_ordered(MPI_File, const void *, int, MPI_Datatype, MPI_Status *);
int PMPI_File_seek_shared(MPI_File, MPI_Offset, int);
int PMPI_File_get_position_shared(MPI_File, MPI_Offset *);
int PMPI_File_read_at_all_begin(MPI_File, MPI_Offset, void *, int, MPI_Datatype);
int PMPI_File_read_at_all_end(MPI_File, void *, MPI_Status *);
int PMPI_File_write_at_all_begin(MPI_File, MPI_Offset, const void *, int, MPI_Datatype);
int PMPI_File_write_at_all_end(MPI_File, const void *, MPI_Status *);
int PMPI_File_read_all_begin(MPI_File, void *, int, MPI_Datatype);
int PMPI_File_read_all_end(MPI_File, void *, MPI_Status *);
int PMPI_File_write_all_begin(MPI_File, const void *, int, MPI_Datatype);
int PMPI_File_write_all_end(MPI_File, const void *, MPI_Status *);
int PMPI_File_read_ordered_begin(MPI_File, void *, int, MPI_Datatype);
int PMPI_File_read_ordered_end(MPI_File, void *, MPI_Status *);
int PMPI_File_write_ordered_begin(MPI_File, const void *, int, MPI_Datatype);
int PMPI_File_write_ordered_end(MPI_File, const void *, MPI_Status *);
int PMPI_File_get_type_extent(MPI_File, MPI_Datatype, MPI_Aint *);
int PMPI_Register_datarep(const char *, MPI_Datarep_conversion_function *, MPI_Datarep_conversion_function *, MPI_Datarep_extent_function *, void *);
int PMPI_File_set_atomicity(MPI_File, int);
int PMPI_File_get_atomicity(MPI_File, int *);
int PMPI_File_sync(MPI_File);
MPI_File PMPI_File_f2c(MPI_Fint);
MPI_Fint PMPI_File_c2f(MPI_File);
typedef int MPIX_Grequest_class;
int MPIX_Grequest_class_create(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, MPIX_Grequest_poll_function *poll_fn, MPIX_Grequest_wait_function *wait_fn, MPIX_Grequest_class *greq_class);
int MPIX_Grequest_class_allocate(MPIX_Grequest_class greq_class, void *extra_state, MPI_Request *request);
int MPIX_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, MPIX_Grequest_poll_function *poll_fn, MPIX_Grequest_wait_function *wait_fn, void *extra_state, MPI_Request *request);
struct mpixi_mutex_s;
typedef struct mpixi_mutex_s *MPIX_Mutex;
int MPIX_Mutex_create(int count, MPI_Comm comm, MPIX_Mutex *hdl);
int MPIX_Mutex_free(MPIX_Mutex *hdl);
int MPIX_Mutex_lock(MPIX_Mutex hdl, int mutex, int proc);
int MPIX_Mutex_unlock(MPIX_Mutex hdl, int mutex, int proc);
int PMPIX_Grequest_class_create(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, MPIX_Grequest_poll_function *poll_fn, MPIX_Grequest_wait_function *wait_fn, MPIX_Grequest_class *greq_class);
int PMPIX_Grequest_class_allocate(MPIX_Grequest_class greq_class, void *extra_state, MPI_Request *request);
int PMPIX_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, MPIX_Grequest_poll_function *poll_fn, MPIX_Grequest_wait_function *wait_fn, void *extra_state, MPI_Request *request);
int PMPIX_Mutex_create(int count, MPI_Comm comm, MPIX_Mutex *hdl);
int PMPIX_Mutex_free(MPIX_Mutex *hdl);
int PMPIX_Mutex_lock(MPIX_Mutex hdl, int mutex, int proc);
int PMPIX_Mutex_unlock(MPIX_Mutex hdl, int mutex, int proc);
typedef struct 
{
        MPI_Comm comm;
        int ordering_tag;
} Ibcast_syshandle;
typedef struct 
{
        MPI_Request *req_array;
        MPI_Status *stat_array;
        int num_sends;
        int num_recvs;
} Ibcast_handle;
int Ibcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, Ibcast_handle **handle_out);
int Ibcast_wait(Ibcast_handle **handle_out);
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
typedef int ptrdiff_t;
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
extern int _gfn_opt_level;
extern int _gfn_rank;
extern int _gfn_num_proc;
extern cl_platform_id _gfn_platform_id;
extern cl_device_id _gfn_device_id;
extern cl_context _gfn_context;
extern cl_command_queue _gfn_cmd_queue;
extern cl_int _gfn_status;
extern cl_program _gfn_cl_program;
extern int _debug_stream_seq;
extern long long _gfn_last_kernel_time;
extern char current_kernel_name[50];
extern Ibcast_handle *send_scalar_handles[20];
extern int send_scalar_curr_idx;
struct _data_information
{
        long long unique_id;
        void *ptr;
        cl_mem cl_ptr;
        cl_mem_flags mem_type;
        int type_id;
        size_t dim_n;
        size_t dim_size[7];
        size_t elem_num;
        size_t block_size;
        int loop_start;
        int loop_end;
        int loop_step;
        int pattern_type;
        int pattern_array[40];
        int pattern_num;
        int last_gather_cnts[8];
        int last_gather_disp[8];
        int last_download_cnts[8];
        int last_download_disp[8];
        int last_exec_cnts[8];
        int last_exec_disp[8];
        int last_upload_cnts[8];
        int last_upload_disp[8];
        int last_partition_cnts[8];
        int last_partition_disp[8];
        int end_disp[8];
        int has_upload_evt;
        cl_event last_upload_evt;
        int has_download_evt;
        cl_event last_download_evt;
        int has_iscatter_req;
        MPI_Request last_iscatter_req;
        int has_igather_req;
        MPI_Request last_igather_req;
};
void _set_data_info_variable(struct _data_information *data_info, long long unique_id, void *ptr, cl_mem cl_ptr, cl_map_flags mem_type, int type_id);
void _set_data_info_loop(struct _data_information *data_info, int loop_start, int loop_end, int loop_step);
void _set_data_info_pattern(struct _data_information *data_info, int pattern_type, int pattern_num, int *pattern_array);
struct _app_profile
{
        int upload_data_size;
        double scatter_latency;
        double scatter_bandwidth;
        double upload_latency;
        double upload_bandwidth;
        int download_data_size;
        double download_latency;
        double download_bandwidth;
        double gather_latency;
        double gather_bandwidth;
        int loop_size;
};
struct _kernel_information
{
        long long kernel_id;
        int local_start;
        int local_end;
        int loop_start;
        int loop_end;
        int loop_step;
        int curr_sequence_id;
        int stream_seq_start_idx;
        int stream_seq_end_idx;
        int is_complete;
        int last_gather_seq_start;
        int last_gather_seq_end;
        int last_gather_partition_size;
        int last_download_seq_start;
        int last_download_seq_end;
        int last_download_partition_size;
        int save_exec_seq_start;
        int save_exec_seq_end;
        int last_exec_partition_size;
        int last_upload_seq_start;
        int last_upload_seq_end;
        int last_upload_partition_size;
        int last_partition_seq_start;
        int last_partition_seq_end;
        int last_partition_partition_size;
        cl_event exec_evt;
        int has_exec_evt;
        size_t bcast_var_num;
        struct _data_information *bcast_var_datas[15];
        size_t scatter_var_num;
        struct _data_information *scatter_var_datas[15];
        size_t gather_var_num;
        struct _data_information *gather_var_datas[15];
        struct _app_profile app_profile;
};
void _update_kernel_info_seq(struct _kernel_information *ker_info);
void _add_scatter_var_id(struct _kernel_information *ker_info, struct _data_information *var_data);
void _get_scatter_var_ids(struct _kernel_information *ker_info, struct _data_information ***var_datas, size_t *var_num);
void _add_gather_var_id(struct _kernel_information *ker_info, struct _data_information *var_data);
void _get_gather_var_ids(struct _kernel_information *ker_info, struct _data_information ***var_datas, size_t *var_num);
int gfn_get_num_process();
int gfn_get_process_num();
int _GfnInit(int *argc, char **argv[]);
int _GfnFinalize();
int _GfnMallocReduceScalar(void *ptr, cl_mem *cl_ptr, int type_id, int group_num, int level1_cond, int level2_cond);
int _GfnFreeReduceScalar(cl_mem cl_ptr, int level1_cond, int level2_cond);
int _GfnEnqueueBroadcastScalar(void *ptr, int type_id);
int _GfnFinishBroadcastScalar();
int _GfnEnqueueReduceScalar(void *ptr, cl_mem cl_ptr, int type_id, MPI_Op op_id, int group_num, int level1_cond, int level2_cond);
int _GfnFinishReduceScalar();
int _GfnMalloc1D(void **ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc2D(void ***ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc3D(void ****ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc4D(void *****ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc5D(void ******ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc6D(void *******ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnFree(long long unique_id, int level1_cond, int level2_cond);
int _GfnEnqueueBroadcastND(void *ptr, cl_mem cl_ptr, long long unique_id, int type_id, int level1_cond, int level2_cond, int dim_n, ...);
int _GfnEnqueueScatterND(void *ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnFinishDistributeArray();
int _GfnEnqueueGatherND(void *ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnFinishGatherArray();
int _GfnLockTransfer(long long id);
int _GfnUnlockTransfer(long long id);
int _GfnStreamSeqKernelRegister(long long kernel_id, int local_start, int local_end, int loop_start, int loop_end, int loop_step, struct _kernel_information **out_ker_info);
int _GfnStreamSeqKernelCleanUp(struct _kernel_information *ker_info);
int _GfnStreamSeqKernelGetNextSequence(struct _kernel_information *ker_info, int *seq_id, size_t *stream_global_item_num, size_t *stream_work_group_item_num);
int _GfnStreamSeqKernelFinishSequence(struct _kernel_information *ker_info);
int _GfnStreamSeqEnqueueScatterND(struct _kernel_information *ker_info, void *ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnStreamSeqFinishDistributeArray();
int _GfnStreamSeqEnqueueGatherND(struct _kernel_information *ker_info, void *ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnStreamSeqFinishGatherND(struct _kernel_information *ker_info, void *ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnStreamSeqFinishGatherArray();
int _GfnStreamSeqExec(int start, int end);
void _GfnBarrier();
void _GfnCheckCLStatus(cl_int status, const char *phase_name);
void _GfnCheckCLStatus2(cl_int status, const char *phase_name, int arg);
size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num);
int _GfnCalcLocalDataStart(int data_start, int data_end);
int _GfnCalcLocalDataEnd(int data_start, int data_end);
int _GfnCalcLocalLoopStart(int loop_start, int loop_end, int loop_step);
int _GfnCalcLocalLoopEnd(int loop_start, int loop_end, int loop_step);
int _GfnCalcLocalLoopStartCore(int loop_start, int loop_end, int loop_step, int num_proc, int rank);
int _GfnCalcLocalLoopEndCore(int loop_start, int loop_end, int loop_step, int num_proc, int rank);
void _GfnMasterInit();
void _GfnMasterFinish();
void _SyncWorker();
void _SyncMaster();
int _CalcLoopSize(int start, int end, int incre);
int _CalcSubSize(int size, int num_proc, int rank, int block_size);
int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcOffset(int size, int num_proc, int rank);
void _CalcCnts(int size, int num_proc, int *cnts, int block_size);
void _CalcDisp(int size, int num_proc, int *disp, int block_size);
void _CalcCnts2(int size, int num_proc, int *cnts, int block_size, int offset);
void _CalcDisp2(int size, int num_proc, int *disp, int block_size, int offset);
size_t _CalcTypeSize(int type_id);
void _CalcPartitionInfo(int size, int block_size, int loop_start, int loop_end, int loop_step, int *pattern_array, int pattern_array_size, int pattern_type, int *cnts, int *disp);
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
MPI_Datatype _GetMpiDatatype(int type_id);
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
void _GfnLaunchKernel(cl_kernel kernel, const size_t *global_size, const size_t *local_size, struct _kernel_information *ker_info);
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
long long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, ((void *) 0));
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
int main(int argc, char *argv[])
{
    double *vec1, *vec2;
    int it, iteration = 100, i, n = 51200;
    int passed = 1;
    long long time0, time1;
    if (argc >= 2)
        n = atoi(argv[1]);
    if (argc >= 3)
        iteration = atoi(argv[2]);
    vec1 = (double *) malloc(sizeof(double) * n);
    vec2 = (double *) malloc(sizeof(double) * n);
    for (i = 0;
        i < n;
        ++i)
        vec1[i] = rand() % 10000;
    time0 = get_time();
    for (it = 0;
        it < iteration;
        ++it)
    {
        /* Send call function message */
        _SendCallFuncMsg(269511);
        _SendInputMsg((void *) &n, sizeof(int));
        _SendConstInputMsg((long long) &(vec2[0]));
        _SendConstInputMsg((long long) &(vec1[0]));
        _SendInputNDMsg(&(vec1[0]), _GFN_TYPE_DOUBLE(), 0, (n) - 1, 1, 0, 1, 1, 2, n, - 1, 1);
        _RecvOutputNDMsg(&(vec2[0]), _GFN_TYPE_DOUBLE(), 0, (n) - 1, 1, 0, 0, 1, 0, n);
    }
    time1 = get_time();
    for (i = 1;
        i < n - 1;
        ++i)
    {
        if (fabs(vec2[i] - (vec1[i - 1] + vec1[i] + vec1[i + 1]) / 3.0) > 0.00001)
        {
            printf("At %d : get %.4lf expected %.4lf\n", i, vec2[i], (vec1[i - 1] + vec1[i] + vec1[i + 1]) / 3.0);
            passed = 0;
            break;
        }
    }
    free(vec1);
    free(vec2);
    if (passed)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    printf("Average time = %f sec.\n", ((float) (time1 - time0) / 1000000) / iteration);
    return 0;
}
