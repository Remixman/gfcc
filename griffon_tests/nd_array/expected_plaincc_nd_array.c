typedef long unsigned int size_t;
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
typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;
typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct 
{
        int __val[2];
} __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef int __daddr_t;
typedef long int __swblk_t;
typedef int __key_t;
typedef int __clockid_t;
typedef void *__timer_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;
typedef long int __ssize_t;
typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;
typedef long int __intptr_t;
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
typedef long int ptrdiff_t;
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
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_real16;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_complex8;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_complex16;
__attribute__((visibility("default"))) extern struct ompi_predefined_datatype_t ompi_mpi_complex32;
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
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long int uint_least64_t;
typedef signed char int_fast8_t;
typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long int uint_fast64_t;
typedef long int intptr_t;
typedef unsigned long int uintptr_t;
typedef long int intmax_t;
typedef unsigned long int uintmax_t;
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
typedef int __m64 __attribute__((__vector_size__(8), __may_alias__));
typedef int __v2si __attribute__((__vector_size__(8)));
typedef short __v4hi __attribute__((__vector_size__(8)));
typedef char __v8qi __attribute__((__vector_size__(8)));
typedef long long __v1di __attribute__((__vector_size__(8)));
typedef float __v2sf __attribute__((__vector_size__(8)));
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_empty(void)
{
    __builtin_ia32_emms();
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_empty(void)
{
    _mm_empty();
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi32_si64(int __i)
{
    return (__m64) __builtin_ia32_vec_init_v2si(__i, 0);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_from_int(int __i)
{
    return _mm_cvtsi32_si64(__i);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_from_int64(long long __i)
{
    return (__m64) __i;
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64_m64(long long __i)
{
    return (__m64) __i;
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64x_si64(long long __i)
{
    return (__m64) __i;
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_pi64x(long long __i)
{
    return (__m64) __i;
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64_si32(__m64 __i)
{
    return __builtin_ia32_vec_ext_v2si((__v2si) __i, 0);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_to_int(__m64 __i)
{
    return _mm_cvtsi64_si32(__i);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_to_int64(__m64 __i)
{
    return (long long) __i;
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtm64_si64(__m64 __i)
{
    return (long long) __i;
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64_si64x(__m64 __i)
{
    return (long long) __i;
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_packs_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_packsswb((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_packsswb(__m64 __m1, __m64 __m2)
{
    return _mm_packs_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_packs_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_packssdw((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_packssdw(__m64 __m1, __m64 __m2)
{
    return _mm_packs_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_packs_pu16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_packuswb((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_packuswb(__m64 __m1, __m64 __m2)
{
    return _mm_packs_pu16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_punpckhbw((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_punpckhbw(__m64 __m1, __m64 __m2)
{
    return _mm_unpackhi_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_punpckhwd((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_punpckhwd(__m64 __m1, __m64 __m2)
{
    return _mm_unpackhi_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_punpckhdq((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_punpckhdq(__m64 __m1, __m64 __m2)
{
    return _mm_unpackhi_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_punpcklbw((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_punpcklbw(__m64 __m1, __m64 __m2)
{
    return _mm_unpacklo_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_punpcklwd((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_punpcklwd(__m64 __m1, __m64 __m2)
{
    return _mm_unpacklo_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_punpckldq((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_punpckldq(__m64 __m1, __m64 __m2)
{
    return _mm_unpacklo_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddb(__m64 __m1, __m64 __m2)
{
    return _mm_add_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddw(__m64 __m1, __m64 __m2)
{
    return _mm_add_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddd((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddd(__m64 __m1, __m64 __m2)
{
    return _mm_add_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_si64(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddq((__v1di) __m1, (__v1di) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddsb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddsb(__m64 __m1, __m64 __m2)
{
    return _mm_adds_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddsw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddsw(__m64 __m1, __m64 __m2)
{
    return _mm_adds_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_pu8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddusb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddusb(__m64 __m1, __m64 __m2)
{
    return _mm_adds_pu8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_pu16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_paddusw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_paddusw(__m64 __m1, __m64 __m2)
{
    return _mm_adds_pu16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubb(__m64 __m1, __m64 __m2)
{
    return _mm_sub_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubw(__m64 __m1, __m64 __m2)
{
    return _mm_sub_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubd((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubd(__m64 __m1, __m64 __m2)
{
    return _mm_sub_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_si64(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubq((__v1di) __m1, (__v1di) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubsb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubsb(__m64 __m1, __m64 __m2)
{
    return _mm_subs_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubsw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubsw(__m64 __m1, __m64 __m2)
{
    return _mm_subs_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_pu8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubusb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubusb(__m64 __m1, __m64 __m2)
{
    return _mm_subs_pu8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_pu16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_psubusw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psubusw(__m64 __m1, __m64 __m2)
{
    return _mm_subs_pu16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_madd_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pmaddwd((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmaddwd(__m64 __m1, __m64 __m2)
{
    return _mm_madd_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mulhi_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pmulhw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmulhw(__m64 __m1, __m64 __m2)
{
    return _mm_mulhi_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mullo_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pmullw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmullw(__m64 __m1, __m64 __m2)
{
    return _mm_mullo_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sll_pi16(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psllw((__v4hi) __m, (__v4hi) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psllw(__m64 __m, __m64 __count)
{
    return _mm_sll_pi16(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_slli_pi16(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psllwi((__v4hi) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psllwi(__m64 __m, int __count)
{
    return _mm_slli_pi16(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sll_pi32(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_pslld((__v2si) __m, (__v2si) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pslld(__m64 __m, __m64 __count)
{
    return _mm_sll_pi32(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_slli_pi32(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_pslldi((__v2si) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pslldi(__m64 __m, int __count)
{
    return _mm_slli_pi32(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sll_si64(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psllq((__v1di) __m, (__v1di) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psllq(__m64 __m, __m64 __count)
{
    return _mm_sll_si64(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_slli_si64(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psllqi((__v1di) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psllqi(__m64 __m, int __count)
{
    return _mm_slli_si64(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sra_pi16(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psraw((__v4hi) __m, (__v4hi) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psraw(__m64 __m, __m64 __count)
{
    return _mm_sra_pi16(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srai_pi16(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psrawi((__v4hi) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrawi(__m64 __m, int __count)
{
    return _mm_srai_pi16(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sra_pi32(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psrad((__v2si) __m, (__v2si) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrad(__m64 __m, __m64 __count)
{
    return _mm_sra_pi32(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srai_pi32(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psradi((__v2si) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psradi(__m64 __m, int __count)
{
    return _mm_srai_pi32(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srl_pi16(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psrlw((__v4hi) __m, (__v4hi) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrlw(__m64 __m, __m64 __count)
{
    return _mm_srl_pi16(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srli_pi16(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psrlwi((__v4hi) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrlwi(__m64 __m, int __count)
{
    return _mm_srli_pi16(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srl_pi32(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psrld((__v2si) __m, (__v2si) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrld(__m64 __m, __m64 __count)
{
    return _mm_srl_pi32(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srli_pi32(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psrldi((__v2si) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrldi(__m64 __m, int __count)
{
    return _mm_srli_pi32(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srl_si64(__m64 __m, __m64 __count)
{
    return (__m64) __builtin_ia32_psrlq((__v1di) __m, (__v1di) __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrlq(__m64 __m, __m64 __count)
{
    return _mm_srl_si64(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srli_si64(__m64 __m, int __count)
{
    return (__m64) __builtin_ia32_psrlqi((__v1di) __m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psrlqi(__m64 __m, int __count)
{
    return _mm_srli_si64(__m, __count);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_and_si64(__m64 __m1, __m64 __m2)
{
    return __builtin_ia32_pand(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pand(__m64 __m1, __m64 __m2)
{
    return _mm_and_si64(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_andnot_si64(__m64 __m1, __m64 __m2)
{
    return __builtin_ia32_pandn(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pandn(__m64 __m1, __m64 __m2)
{
    return _mm_andnot_si64(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_or_si64(__m64 __m1, __m64 __m2)
{
    return __builtin_ia32_por(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_por(__m64 __m1, __m64 __m2)
{
    return _mm_or_si64(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_xor_si64(__m64 __m1, __m64 __m2)
{
    return __builtin_ia32_pxor(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pxor(__m64 __m1, __m64 __m2)
{
    return _mm_xor_si64(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pcmpeqb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pcmpeqb(__m64 __m1, __m64 __m2)
{
    return _mm_cmpeq_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_pi8(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pcmpgtb((__v8qi) __m1, (__v8qi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pcmpgtb(__m64 __m1, __m64 __m2)
{
    return _mm_cmpgt_pi8(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pcmpeqw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pcmpeqw(__m64 __m1, __m64 __m2)
{
    return _mm_cmpeq_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_pi16(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pcmpgtw((__v4hi) __m1, (__v4hi) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pcmpgtw(__m64 __m1, __m64 __m2)
{
    return _mm_cmpgt_pi16(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pcmpeqd((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pcmpeqd(__m64 __m1, __m64 __m2)
{
    return _mm_cmpeq_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_pi32(__m64 __m1, __m64 __m2)
{
    return (__m64) __builtin_ia32_pcmpgtd((__v2si) __m1, (__v2si) __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pcmpgtd(__m64 __m1, __m64 __m2)
{
    return _mm_cmpgt_pi32(__m1, __m2);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setzero_si64(void)
{
    return (__m64) 0LL;
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_pi32(int __i1, int __i0)
{
    return (__m64) __builtin_ia32_vec_init_v2si(__i0, __i1);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_pi16(short __w3, short __w2, short __w1, short __w0)
{
    return (__m64) __builtin_ia32_vec_init_v4hi(__w0, __w1, __w2, __w3);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_pi8(char __b7, char __b6, char __b5, char __b4, char __b3, char __b2, char __b1, char __b0)
{
    return (__m64) __builtin_ia32_vec_init_v8qi(__b0, __b1, __b2, __b3, __b4, __b5, __b6, __b7);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_pi32(int __i0, int __i1)
{
    return _mm_set_pi32(__i1, __i0);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_pi16(short __w0, short __w1, short __w2, short __w3)
{
    return _mm_set_pi16(__w3, __w2, __w1, __w0);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_pi8(char __b0, char __b1, char __b2, char __b3, char __b4, char __b5, char __b6, char __b7)
{
    return _mm_set_pi8(__b7, __b6, __b5, __b4, __b3, __b2, __b1, __b0);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_pi32(int __i)
{
    return _mm_set_pi32(__i, __i);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_pi16(short __w)
{
    return _mm_set_pi16(__w, __w, __w, __w);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_pi8(char __b)
{
    return _mm_set_pi8(__b, __b, __b, __b, __b, __b, __b, __b);
}
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
        char __size[56];
        long int __align;
} pthread_attr_t;
typedef struct __pthread_internal_list
{
        struct __pthread_internal_list *__prev;
        struct __pthread_internal_list *__next;
} __pthread_list_t;
typedef union 
{
        struct __pthread_mutex_s
        {
                int __lock;
                unsigned int __count;
                int __owner;
                unsigned int __nusers;
                int __kind;
                int __spins;
                __pthread_list_t __list;
        } __data;
        char __size[40];
        long int __align;
} pthread_mutex_t;
typedef union 
{
        char __size[4];
        int __align;
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
        int __align;
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
                int __writer;
                int __shared;
                unsigned long int __pad1;
                unsigned long int __pad2;
                unsigned int __flags;
        } __data;
        char __size[56];
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
        char __size[32];
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
extern int posix_memalign(void **, size_t, size_t);
static __inline void *_mm_malloc(size_t size, size_t alignment)
{
    void *ptr;
    if (alignment == 1)
        return malloc(size);
    if (alignment == 2 || (sizeof(void *) == 8 && alignment == 4))
        alignment = sizeof(void *);
    if (posix_memalign(&ptr, alignment, size) == 0)
        return ptr;
    else
        return ((void *) 0);
}
static __inline void _mm_free(void *ptr)
{
    free(ptr);
}
typedef float __m128 __attribute__((__vector_size__(16), __may_alias__));
typedef float __v4sf __attribute__((__vector_size__(16)));
enum _mm_hint
{
    _MM_HINT_T0 = 3, 
    _MM_HINT_T1 = 2, 
    _MM_HINT_T2 = 1, 
    _MM_HINT_NTA = 0
};
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setzero_ps(void)
{
    return __extension__ (__m128){0.0f, 0.0f, 0.0f, 0.0f};
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_addss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_subss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mul_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_mulss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_div_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_divss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sqrt_ss(__m128 __A)
{
    return (__m128) __builtin_ia32_sqrtss((__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_rcp_ss(__m128 __A)
{
    return (__m128) __builtin_ia32_rcpss((__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_rsqrt_ss(__m128 __A)
{
    return (__m128) __builtin_ia32_rsqrtss((__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_minss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_maxss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_addps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_subps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mul_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_mulps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_div_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_divps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sqrt_ps(__m128 __A)
{
    return (__m128) __builtin_ia32_sqrtps((__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_rcp_ps(__m128 __A)
{
    return (__m128) __builtin_ia32_rcpps((__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_rsqrt_ps(__m128 __A)
{
    return (__m128) __builtin_ia32_rsqrtps((__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_minps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_maxps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_and_ps(__m128 __A, __m128 __B)
{
    return __builtin_ia32_andps(__A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_andnot_ps(__m128 __A, __m128 __B)
{
    return __builtin_ia32_andnps(__A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_or_ps(__m128 __A, __m128 __B)
{
    return __builtin_ia32_orps(__A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_xor_ps(__m128 __A, __m128 __B)
{
    return __builtin_ia32_xorps(__A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpeqss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpltss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmple_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpless((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movss((__v4sf) __A, (__v4sf) __builtin_ia32_cmpltss((__v4sf) __B, (__v4sf) __A));
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpge_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movss((__v4sf) __A, (__v4sf) __builtin_ia32_cmpless((__v4sf) __B, (__v4sf) __A));
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpneq_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpneqss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnlt_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpnltss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnle_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpnless((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpngt_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movss((__v4sf) __A, (__v4sf) __builtin_ia32_cmpnltss((__v4sf) __B, (__v4sf) __A));
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnge_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movss((__v4sf) __A, (__v4sf) __builtin_ia32_cmpnless((__v4sf) __B, (__v4sf) __A));
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpord_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpordss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpunord_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpunordss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpeqps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpltps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmple_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpleps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpgtps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpge_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpgeps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpneq_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpneqps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnlt_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpnltps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnle_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpnleps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpngt_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpngtps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnge_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpngeps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpord_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpordps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpunord_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_cmpunordps((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comieq_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_comieq((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comilt_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_comilt((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comile_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_comile((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comigt_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_comigt((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comige_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_comige((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comineq_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_comineq((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomieq_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_ucomieq((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomilt_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_ucomilt((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomile_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_ucomile((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomigt_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_ucomigt((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomige_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_ucomige((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomineq_ss(__m128 __A, __m128 __B)
{
    return __builtin_ia32_ucomineq((__v4sf) __A, (__v4sf) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtss_si32(__m128 __A)
{
    return __builtin_ia32_cvtss2si((__v4sf) __A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvt_ss2si(__m128 __A)
{
    return _mm_cvtss_si32(__A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtss_si64(__m128 __A)
{
    return __builtin_ia32_cvtss2si64((__v4sf) __A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtss_si64x(__m128 __A)
{
    return __builtin_ia32_cvtss2si64((__v4sf) __A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtps_pi32(__m128 __A)
{
    return (__m64) __builtin_ia32_cvtps2pi((__v4sf) __A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvt_ps2pi(__m128 __A)
{
    return _mm_cvtps_pi32(__A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttss_si32(__m128 __A)
{
    return __builtin_ia32_cvttss2si((__v4sf) __A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtt_ss2si(__m128 __A)
{
    return _mm_cvttss_si32(__A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttss_si64(__m128 __A)
{
    return __builtin_ia32_cvttss2si64((__v4sf) __A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttss_si64x(__m128 __A)
{
    return __builtin_ia32_cvttss2si64((__v4sf) __A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttps_pi32(__m128 __A)
{
    return (__m64) __builtin_ia32_cvttps2pi((__v4sf) __A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtt_ps2pi(__m128 __A)
{
    return _mm_cvttps_pi32(__A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi32_ss(__m128 __A, int __B)
{
    return (__m128) __builtin_ia32_cvtsi2ss((__v4sf) __A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvt_si2ss(__m128 __A, int __B)
{
    return _mm_cvtsi32_ss(__A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64_ss(__m128 __A, long long __B)
{
    return (__m128) __builtin_ia32_cvtsi642ss((__v4sf) __A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64x_ss(__m128 __A, long long __B)
{
    return (__m128) __builtin_ia32_cvtsi642ss((__v4sf) __A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpi32_ps(__m128 __A, __m64 __B)
{
    return (__m128) __builtin_ia32_cvtpi2ps((__v4sf) __A, (__v2si) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvt_pi2ps(__m128 __A, __m64 __B)
{
    return _mm_cvtpi32_ps(__A, __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpi16_ps(__m64 __A)
{
    __v4hi __sign;
    __v2si __hisi, __losi;
    __v4sf __zero, __ra, __rb;
    __sign = __builtin_ia32_pcmpgtw((__v4hi) 0LL, (__v4hi) __A);
    __losi = (__v2si) __builtin_ia32_punpcklwd((__v4hi) __A, __sign);
    __hisi = (__v2si) __builtin_ia32_punpckhwd((__v4hi) __A, __sign);
    __zero = (__v4sf) _mm_setzero_ps();
    __ra = __builtin_ia32_cvtpi2ps(__zero, __losi);
    __rb = __builtin_ia32_cvtpi2ps(__ra, __hisi);
    return (__m128) __builtin_ia32_movlhps(__ra, __rb);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpu16_ps(__m64 __A)
{
    __v2si __hisi, __losi;
    __v4sf __zero, __ra, __rb;
    __losi = (__v2si) __builtin_ia32_punpcklwd((__v4hi) __A, (__v4hi) 0LL);
    __hisi = (__v2si) __builtin_ia32_punpckhwd((__v4hi) __A, (__v4hi) 0LL);
    __zero = (__v4sf) _mm_setzero_ps();
    __ra = __builtin_ia32_cvtpi2ps(__zero, __losi);
    __rb = __builtin_ia32_cvtpi2ps(__ra, __hisi);
    return (__m128) __builtin_ia32_movlhps(__ra, __rb);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpi8_ps(__m64 __A)
{
    __v8qi __sign;
    __sign = __builtin_ia32_pcmpgtb((__v8qi) 0LL, (__v8qi) __A);
    __A = (__m64) __builtin_ia32_punpcklbw((__v8qi) __A, __sign);
    return _mm_cvtpi16_ps(__A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpu8_ps(__m64 __A)
{
    __A = (__m64) __builtin_ia32_punpcklbw((__v8qi) __A, (__v8qi) 0LL);
    return _mm_cvtpu16_ps(__A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpi32x2_ps(__m64 __A, __m64 __B)
{
    __v4sf __zero = (__v4sf) _mm_setzero_ps();
    __v4sf __sfa = __builtin_ia32_cvtpi2ps(__zero, (__v2si) __A);
    __v4sf __sfb = __builtin_ia32_cvtpi2ps(__sfa, (__v2si) __B);
    return (__m128) __builtin_ia32_movlhps(__sfa, __sfb);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtps_pi16(__m128 __A)
{
    __v4sf __hisf = (__v4sf) __A;
    __v4sf __losf = __builtin_ia32_movhlps(__hisf, __hisf);
    __v2si __hisi = __builtin_ia32_cvtps2pi(__hisf);
    __v2si __losi = __builtin_ia32_cvtps2pi(__losf);
    return (__m64) __builtin_ia32_packssdw(__hisi, __losi);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtps_pi8(__m128 __A)
{
    __v4hi __tmp = (__v4hi) _mm_cvtps_pi16(__A);
    return (__m64) __builtin_ia32_packsswb(__tmp, (__v4hi) 0LL);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_unpckhps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_unpcklps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadh_pi(__m128 __A, __m64 const *__P)
{
    return (__m128) __builtin_ia32_loadhps((__v4sf) __A, (const __v2sf *) __P);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storeh_pi(__m64 *__P, __m128 __A)
{
    __builtin_ia32_storehps((__v2sf *) __P, (__v4sf) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movehl_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movhlps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movelh_ps(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movlhps((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadl_pi(__m128 __A, __m64 const *__P)
{
    return (__m128) __builtin_ia32_loadlps((__v4sf) __A, (const __v2sf *) __P);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storel_pi(__m64 *__P, __m128 __A)
{
    __builtin_ia32_storelps((__v2sf *) __P, (__v4sf) __A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movemask_ps(__m128 __A)
{
    return __builtin_ia32_movmskps((__v4sf) __A);
}
extern __inline unsigned int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_getcsr(void)
{
    return __builtin_ia32_stmxcsr();
}
extern __inline unsigned int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_GET_EXCEPTION_STATE(void)
{
    return _mm_getcsr() & 0x003f;
}
extern __inline unsigned int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_GET_EXCEPTION_MASK(void)
{
    return _mm_getcsr() & 0x1f80;
}
extern __inline unsigned int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_GET_ROUNDING_MODE(void)
{
    return _mm_getcsr() & 0x6000;
}
extern __inline unsigned int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_GET_FLUSH_ZERO_MODE(void)
{
    return _mm_getcsr() & 0x8000;
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setcsr(unsigned int __I)
{
    __builtin_ia32_ldmxcsr(__I);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_SET_EXCEPTION_STATE(unsigned int __mask)
{
    _mm_setcsr((_mm_getcsr() & ~0x003f) | __mask);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_SET_EXCEPTION_MASK(unsigned int __mask)
{
    _mm_setcsr((_mm_getcsr() & ~0x1f80) | __mask);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_SET_ROUNDING_MODE(unsigned int __mode)
{
    _mm_setcsr((_mm_getcsr() & ~0x6000) | __mode);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _MM_SET_FLUSH_ZERO_MODE(unsigned int __mode)
{
    _mm_setcsr((_mm_getcsr() & ~0x8000) | __mode);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_ss(float __F)
{
    return __extension__ (__m128) (__v4sf){__F, 0.0f, 0.0f, 0.0f};
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_ps(float __F)
{
    return __extension__ (__m128) (__v4sf){__F, __F, __F, __F};
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_ps1(float __F)
{
    return _mm_set1_ps(__F);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_ss(float const *__P)
{
    return _mm_set_ss(*__P);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load1_ps(float const *__P)
{
    return _mm_set1_ps(*__P);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_ps1(float const *__P)
{
    return _mm_load1_ps(__P);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_ps(float const *__P)
{
    return (__m128) *(__v4sf *) __P;
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadu_ps(float const *__P)
{
    return (__m128) __builtin_ia32_loadups(__P);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadr_ps(float const *__P)
{
    __v4sf __tmp = *(__v4sf *) __P;
    return (__m128) __builtin_ia32_shufps(__tmp, __tmp, (((0) << 6) | ((1) << 4) | ((2) << 2) | (3)));
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_ps(const float __Z, const float __Y, const float __X, const float __W)
{
    return __extension__ (__m128) (__v4sf){__W, __X, __Y, __Z};
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_ps(float __Z, float __Y, float __X, float __W)
{
    return __extension__ (__m128) (__v4sf){__Z, __Y, __X, __W};
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_ss(float *__P, __m128 __A)
{
    *__P = __builtin_ia32_vec_ext_v4sf((__v4sf) __A, 0);
}
extern __inline float __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtss_f32(__m128 __A)
{
    return __builtin_ia32_vec_ext_v4sf((__v4sf) __A, 0);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_ps(float *__P, __m128 __A)
{
    *(__v4sf *) __P = (__v4sf) __A;
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storeu_ps(float *__P, __m128 __A)
{
    __builtin_ia32_storeups(__P, (__v4sf) __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store1_ps(float *__P, __m128 __A)
{
    __v4sf __va = (__v4sf) __A;
    __v4sf __tmp = __builtin_ia32_shufps(__va, __va, (((0) << 6) | ((0) << 4) | ((0) << 2) | (0)));
    _mm_storeu_ps(__P, __tmp);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_ps1(float *__P, __m128 __A)
{
    _mm_store1_ps(__P, __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storer_ps(float *__P, __m128 __A)
{
    __v4sf __va = (__v4sf) __A;
    __v4sf __tmp = __builtin_ia32_shufps(__va, __va, (((0) << 6) | ((1) << 4) | ((2) << 2) | (3)));
    _mm_store_ps(__P, __tmp);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_move_ss(__m128 __A, __m128 __B)
{
    return (__m128) __builtin_ia32_movss((__v4sf) __A, (__v4sf) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_pi16(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pmaxsw((__v4hi) __A, (__v4hi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmaxsw(__m64 __A, __m64 __B)
{
    return _mm_max_pi16(__A, __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_pu8(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pmaxub((__v8qi) __A, (__v8qi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmaxub(__m64 __A, __m64 __B)
{
    return _mm_max_pu8(__A, __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_pi16(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pminsw((__v4hi) __A, (__v4hi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pminsw(__m64 __A, __m64 __B)
{
    return _mm_min_pi16(__A, __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_pu8(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pminub((__v8qi) __A, (__v8qi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pminub(__m64 __A, __m64 __B)
{
    return _mm_min_pu8(__A, __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movemask_pi8(__m64 __A)
{
    return __builtin_ia32_pmovmskb((__v8qi) __A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmovmskb(__m64 __A)
{
    return _mm_movemask_pi8(__A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mulhi_pu16(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pmulhuw((__v4hi) __A, (__v4hi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pmulhuw(__m64 __A, __m64 __B)
{
    return _mm_mulhi_pu16(__A, __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_maskmove_si64(__m64 __A, __m64 __N, char *__P)
{
    __builtin_ia32_maskmovq((__v8qi) __A, (__v8qi) __N, __P);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_maskmovq(__m64 __A, __m64 __N, char *__P)
{
    _mm_maskmove_si64(__A, __N, __P);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_avg_pu8(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pavgb((__v8qi) __A, (__v8qi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pavgb(__m64 __A, __m64 __B)
{
    return _mm_avg_pu8(__A, __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_avg_pu16(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pavgw((__v4hi) __A, (__v4hi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_pavgw(__m64 __A, __m64 __B)
{
    return _mm_avg_pu16(__A, __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sad_pu8(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_psadbw((__v8qi) __A, (__v8qi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _m_psadbw(__m64 __A, __m64 __B)
{
    return _mm_sad_pu8(__A, __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_stream_pi(__m64 *__P, __m64 __A)
{
    __builtin_ia32_movntq((unsigned long long *) __P, (unsigned long long) __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_stream_ps(float *__P, __m128 __A)
{
    __builtin_ia32_movntps(__P, (__v4sf) __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sfence(void)
{
    __builtin_ia32_sfence();
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_pause(void)
{
    __asm__ __volatile__("rep; nop": : );
}
typedef double __v2df __attribute__((__vector_size__(16)));
typedef long long __v2di __attribute__((__vector_size__(16)));
typedef int __v4si __attribute__((__vector_size__(16)));
typedef short __v8hi __attribute__((__vector_size__(16)));
typedef char __v16qi __attribute__((__vector_size__(16)));
typedef long long __m128i __attribute__((__vector_size__(16), __may_alias__));
typedef double __m128d __attribute__((__vector_size__(16), __may_alias__));
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_sd(double __F)
{
    return __extension__ (__m128d){__F, 0.0};
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_pd(double __F)
{
    return __extension__ (__m128d){__F, __F};
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_pd1(double __F)
{
    return _mm_set1_pd(__F);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_pd(double __W, double __X)
{
    return __extension__ (__m128d){__X, __W};
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_pd(double __W, double __X)
{
    return __extension__ (__m128d){__W, __X};
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setzero_pd(void)
{
    return __extension__ (__m128d){0.0, 0.0};
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_move_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_movsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_pd(double const *__P)
{
    return *(__m128d *) __P;
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadu_pd(double const *__P)
{
    return __builtin_ia32_loadupd(__P);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load1_pd(double const *__P)
{
    return _mm_set1_pd(*__P);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_sd(double const *__P)
{
    return _mm_set_sd(*__P);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_pd1(double const *__P)
{
    return _mm_load1_pd(__P);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadr_pd(double const *__P)
{
    __m128d __tmp = _mm_load_pd(__P);
    return __builtin_ia32_shufpd(__tmp, __tmp, (((0) << 1) | (1)));
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_pd(double *__P, __m128d __A)
{
    *(__m128d *) __P = __A;
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storeu_pd(double *__P, __m128d __A)
{
    __builtin_ia32_storeupd(__P, __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_sd(double *__P, __m128d __A)
{
    *__P = __builtin_ia32_vec_ext_v2df(__A, 0);
}
extern __inline double __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsd_f64(__m128d __A)
{
    return __builtin_ia32_vec_ext_v2df(__A, 0);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storel_pd(double *__P, __m128d __A)
{
    _mm_store_sd(__P, __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storeh_pd(double *__P, __m128d __A)
{
    *__P = __builtin_ia32_vec_ext_v2df(__A, 1);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store1_pd(double *__P, __m128d __A)
{
    _mm_store_pd(__P, __builtin_ia32_shufpd(__A, __A, (((0) << 1) | (0))));
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_pd1(double *__P, __m128d __A)
{
    _mm_store1_pd(__P, __A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storer_pd(double *__P, __m128d __A)
{
    _mm_store_pd(__P, __builtin_ia32_shufpd(__A, __A, (((0) << 1) | (1))));
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi128_si32(__m128i __A)
{
    return __builtin_ia32_vec_ext_v4si((__v4si) __A, 0);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi128_si64(__m128i __A)
{
    return __builtin_ia32_vec_ext_v2di((__v2di) __A, 0);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi128_si64x(__m128i __A)
{
    return __builtin_ia32_vec_ext_v2di((__v2di) __A, 0);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_addpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_addsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_subpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_subsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mul_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_mulpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mul_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_mulsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_div_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_divpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_div_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_divsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sqrt_pd(__m128d __A)
{
    return (__m128d) __builtin_ia32_sqrtpd((__v2df) __A);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sqrt_sd(__m128d __A, __m128d __B)
{
    __v2df __tmp = __builtin_ia32_movsd((__v2df) __A, (__v2df) __B);
    return (__m128d) __builtin_ia32_sqrtsd((__v2df) __tmp);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_minpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_minsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_maxpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_maxsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_and_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_andpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_andnot_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_andnpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_or_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_orpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_xor_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_xorpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpeqpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpltpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmple_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmplepd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpgtpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpge_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpgepd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpneq_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpneqpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnlt_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpnltpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnle_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpnlepd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpngt_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpngtpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnge_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpngepd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpord_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpordpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpunord_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpunordpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpeqsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpltsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmple_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmplesd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_movsd((__v2df) __A, (__v2df) __builtin_ia32_cmpltsd((__v2df) __B, (__v2df) __A));
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpge_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_movsd((__v2df) __A, (__v2df) __builtin_ia32_cmplesd((__v2df) __B, (__v2df) __A));
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpneq_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpneqsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnlt_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpnltsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnle_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpnlesd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpngt_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_movsd((__v2df) __A, (__v2df) __builtin_ia32_cmpnltsd((__v2df) __B, (__v2df) __A));
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpnge_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_movsd((__v2df) __A, (__v2df) __builtin_ia32_cmpnlesd((__v2df) __B, (__v2df) __A));
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpord_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpordsd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpunord_sd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_cmpunordsd((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comieq_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_comisdeq((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comilt_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_comisdlt((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comile_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_comisdle((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comigt_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_comisdgt((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comige_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_comisdge((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_comineq_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_comisdneq((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomieq_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_ucomisdeq((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomilt_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_ucomisdlt((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomile_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_ucomisdle((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomigt_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_ucomisdgt((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomige_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_ucomisdge((__v2df) __A, (__v2df) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_ucomineq_sd(__m128d __A, __m128d __B)
{
    return __builtin_ia32_ucomisdneq((__v2df) __A, (__v2df) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_epi64x(long long __q1, long long __q0)
{
    return __extension__ (__m128i) (__v2di){__q0, __q1};
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_epi64(__m64 __q1, __m64 __q0)
{
    return _mm_set_epi64x((long long) __q1, (long long) __q0);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_epi32(int __q3, int __q2, int __q1, int __q0)
{
    return __extension__ (__m128i) (__v4si){__q0, __q1, __q2, __q3};
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_epi16(short __q7, short __q6, short __q5, short __q4, short __q3, short __q2, short __q1, short __q0)
{
    return __extension__ (__m128i) (__v8hi){__q0, __q1, __q2, __q3, __q4, __q5, __q6, __q7};
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set_epi8(char __q15, char __q14, char __q13, char __q12, char __q11, char __q10, char __q09, char __q08, char __q07, char __q06, char __q05, char __q04, char __q03, char __q02, char __q01, char __q00)
{
    return __extension__ (__m128i) (__v16qi){__q00, __q01, __q02, __q03, __q04, __q05, __q06, __q07, __q08, __q09, __q10, __q11, __q12, __q13, __q14, __q15};
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_epi64x(long long __A)
{
    return _mm_set_epi64x(__A, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_epi64(__m64 __A)
{
    return _mm_set_epi64(__A, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_epi32(int __A)
{
    return _mm_set_epi32(__A, __A, __A, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_epi16(short __A)
{
    return _mm_set_epi16(__A, __A, __A, __A, __A, __A, __A, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_set1_epi8(char __A)
{
    return _mm_set_epi8(__A, __A, __A, __A, __A, __A, __A, __A, __A, __A, __A, __A, __A, __A, __A, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_epi64(__m64 __q0, __m64 __q1)
{
    return _mm_set_epi64(__q1, __q0);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_epi32(int __q0, int __q1, int __q2, int __q3)
{
    return _mm_set_epi32(__q3, __q2, __q1, __q0);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_epi16(short __q0, short __q1, short __q2, short __q3, short __q4, short __q5, short __q6, short __q7)
{
    return _mm_set_epi16(__q7, __q6, __q5, __q4, __q3, __q2, __q1, __q0);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setr_epi8(char __q00, char __q01, char __q02, char __q03, char __q04, char __q05, char __q06, char __q07, char __q08, char __q09, char __q10, char __q11, char __q12, char __q13, char __q14, char __q15)
{
    return _mm_set_epi8(__q15, __q14, __q13, __q12, __q11, __q10, __q09, __q08, __q07, __q06, __q05, __q04, __q03, __q02, __q01, __q00);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_load_si128(__m128i const *__P)
{
    return *__P;
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadu_si128(__m128i const *__P)
{
    return (__m128i) __builtin_ia32_loaddqu((char const *) __P);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadl_epi64(__m128i const *__P)
{
    return _mm_set_epi64((__m64) 0LL, *(__m64 *) __P);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_store_si128(__m128i *__P, __m128i __B)
{
    *__P = __B;
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storeu_si128(__m128i *__P, __m128i __B)
{
    __builtin_ia32_storedqu((char *) __P, (__v16qi) __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_storel_epi64(__m128i *__P, __m128i __B)
{
    *(long long *) __P = __builtin_ia32_vec_ext_v2di((__v2di) __B, 0);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movepi64_pi64(__m128i __B)
{
    return (__m64) __builtin_ia32_vec_ext_v2di((__v2di) __B, 0);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movpi64_epi64(__m64 __A)
{
    return _mm_set_epi64((__m64) 0LL, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_move_epi64(__m128i __A)
{
    return (__m128i) __builtin_ia32_movq128((__v2di) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_setzero_si128(void)
{
    return __extension__ (__m128i) (__v4si){0, 0, 0, 0};
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtepi32_pd(__m128i __A)
{
    return (__m128d) __builtin_ia32_cvtdq2pd((__v4si) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtepi32_ps(__m128i __A)
{
    return (__m128) __builtin_ia32_cvtdq2ps((__v4si) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpd_epi32(__m128d __A)
{
    return (__m128i) __builtin_ia32_cvtpd2dq((__v2df) __A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpd_pi32(__m128d __A)
{
    return (__m64) __builtin_ia32_cvtpd2pi((__v2df) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpd_ps(__m128d __A)
{
    return (__m128) __builtin_ia32_cvtpd2ps((__v2df) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttpd_epi32(__m128d __A)
{
    return (__m128i) __builtin_ia32_cvttpd2dq((__v2df) __A);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttpd_pi32(__m128d __A)
{
    return (__m64) __builtin_ia32_cvttpd2pi((__v2df) __A);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtpi32_pd(__m64 __A)
{
    return (__m128d) __builtin_ia32_cvtpi2pd((__v2si) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtps_epi32(__m128 __A)
{
    return (__m128i) __builtin_ia32_cvtps2dq((__v4sf) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttps_epi32(__m128 __A)
{
    return (__m128i) __builtin_ia32_cvttps2dq((__v4sf) __A);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtps_pd(__m128 __A)
{
    return (__m128d) __builtin_ia32_cvtps2pd((__v4sf) __A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsd_si32(__m128d __A)
{
    return __builtin_ia32_cvtsd2si((__v2df) __A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsd_si64(__m128d __A)
{
    return __builtin_ia32_cvtsd2si64((__v2df) __A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsd_si64x(__m128d __A)
{
    return __builtin_ia32_cvtsd2si64((__v2df) __A);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttsd_si32(__m128d __A)
{
    return __builtin_ia32_cvttsd2si((__v2df) __A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttsd_si64(__m128d __A)
{
    return __builtin_ia32_cvttsd2si64((__v2df) __A);
}
extern __inline long long __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvttsd_si64x(__m128d __A)
{
    return __builtin_ia32_cvttsd2si64((__v2df) __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsd_ss(__m128 __A, __m128d __B)
{
    return (__m128) __builtin_ia32_cvtsd2ss((__v4sf) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi32_sd(__m128d __A, int __B)
{
    return (__m128d) __builtin_ia32_cvtsi2sd((__v2df) __A, __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64_sd(__m128d __A, long long __B)
{
    return (__m128d) __builtin_ia32_cvtsi642sd((__v2df) __A, __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64x_sd(__m128d __A, long long __B)
{
    return (__m128d) __builtin_ia32_cvtsi642sd((__v2df) __A, __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtss_sd(__m128d __A, __m128 __B)
{
    return (__m128d) __builtin_ia32_cvtss2sd((__v2df) __A, (__v4sf) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_unpckhpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_pd(__m128d __A, __m128d __B)
{
    return (__m128d) __builtin_ia32_unpcklpd((__v2df) __A, (__v2df) __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadh_pd(__m128d __A, double const *__B)
{
    return (__m128d) __builtin_ia32_loadhpd((__v2df) __A, __B);
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_loadl_pd(__m128d __A, double const *__B)
{
    return (__m128d) __builtin_ia32_loadlpd((__v2df) __A, __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movemask_pd(__m128d __A)
{
    return __builtin_ia32_movmskpd((__v2df) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_packs_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_packsswb128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_packs_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_packssdw128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_packus_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_packuswb128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpckhbw128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpckhwd128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpckhdq128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpackhi_epi64(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpckhqdq128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpcklbw128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpcklwd128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpckldq128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_unpacklo_epi64(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_punpcklqdq128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddd128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_add_epi64(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddq128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddsb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddsw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_epu8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddusb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_adds_epu16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_paddusw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubd128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sub_epi64(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubq128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubsb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubsw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_epu8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubusb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_subs_epu16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psubusw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_madd_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmaddwd128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mulhi_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmulhw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mullo_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmullw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mul_su32(__m64 __A, __m64 __B)
{
    return (__m64) __builtin_ia32_pmuludq((__v2si) __A, (__v2si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mul_epu32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmuludq128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_slli_epi16(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psllwi128((__v8hi) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_slli_epi32(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_pslldi128((__v4si) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_slli_epi64(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psllqi128((__v2di) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srai_epi16(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psrawi128((__v8hi) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srai_epi32(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psradi128((__v4si) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srli_epi16(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psrlwi128((__v8hi) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srli_epi32(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psrldi128((__v4si) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srli_epi64(__m128i __A, int __B)
{
    return (__m128i) __builtin_ia32_psrlqi128((__v2di) __A, __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sll_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psllw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sll_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pslld128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sll_epi64(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psllq128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sra_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psraw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sra_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psrad128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srl_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psrlw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srl_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psrld128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_srl_epi64(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psrlq128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_and_si128(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pand128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_andnot_si128(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pandn128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_or_si128(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_por128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_xor_si128(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pxor128((__v2di) __A, (__v2di) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpeqb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpeqw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpeq_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpeqd128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpgtb128((__v16qi) __B, (__v16qi) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpgtw128((__v8hi) __B, (__v8hi) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmplt_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpgtd128((__v4si) __B, (__v4si) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_epi8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpgtb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpgtw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cmpgt_epi32(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pcmpgtd128((__v4si) __A, (__v4si) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmaxsw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_max_epu8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmaxub128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_epi16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pminsw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_min_epu8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pminub128((__v16qi) __A, (__v16qi) __B);
}
extern __inline int __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_movemask_epi8(__m128i __A)
{
    return __builtin_ia32_pmovmskb128((__v16qi) __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mulhi_epu16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pmulhuw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_maskmoveu_si128(__m128i __A, __m128i __B, char *__C)
{
    __builtin_ia32_maskmovdqu((__v16qi) __A, (__v16qi) __B, __C);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_avg_epu8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pavgb128((__v16qi) __A, (__v16qi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_avg_epu16(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_pavgw128((__v8hi) __A, (__v8hi) __B);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_sad_epu8(__m128i __A, __m128i __B)
{
    return (__m128i) __builtin_ia32_psadbw128((__v16qi) __A, (__v16qi) __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_stream_si32(int *__A, int __B)
{
    __builtin_ia32_movnti(__A, __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_stream_si64(long long int *__A, long long int __B)
{
    __builtin_ia32_movnti64(__A, __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_stream_si128(__m128i *__A, __m128i __B)
{
    __builtin_ia32_movntdq((__v2di *) __A, (__v2di) __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_stream_pd(double *__A, __m128d __B)
{
    __builtin_ia32_movntpd(__A, (__v2df) __B);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_clflush(void const *__A)
{
    __builtin_ia32_clflush(__A);
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_lfence(void)
{
    __builtin_ia32_lfence();
}
extern __inline void __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_mfence(void)
{
    __builtin_ia32_mfence();
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi32_si128(int __A)
{
    return _mm_set_epi32(0, 0, 0, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64_si128(long long __A)
{
    return _mm_set_epi64x(0, __A);
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_cvtsi64x_si128(long long __A)
{
    return _mm_set_epi64x(0, __A);
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_castpd_ps(__m128d __A)
{
    return (__m128) __A;
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_castpd_si128(__m128d __A)
{
    return (__m128i) __A;
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_castps_pd(__m128 __A)
{
    return (__m128d) __A;
}
extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_castps_si128(__m128 __A)
{
    return (__m128i) __A;
}
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_castsi128_ps(__m128i __A)
{
    return (__m128) __A;
}
extern __inline __m128d __attribute__((__gnu_inline__, __always_inline__, __artificial__)) _mm_castsi128_pd(__m128i __A)
{
    return (__m128d) __A;
}
typedef float __cl_float4 __attribute__((vector_size(16)));
typedef cl_uchar __cl_uchar16 __attribute__((vector_size(16)));
typedef cl_char __cl_char16 __attribute__((vector_size(16)));
typedef cl_ushort __cl_ushort8 __attribute__((vector_size(16)));
typedef cl_short __cl_short8 __attribute__((vector_size(16)));
typedef cl_uint __cl_uint4 __attribute__((vector_size(16)));
typedef cl_int __cl_int4 __attribute__((vector_size(16)));
typedef cl_ulong __cl_ulong2 __attribute__((vector_size(16)));
typedef cl_long __cl_long2 __attribute__((vector_size(16)));
typedef cl_double __cl_double2 __attribute__((vector_size(16)));
typedef cl_uchar __cl_uchar8 __attribute__((vector_size(8)));
typedef cl_char __cl_char8 __attribute__((vector_size(8)));
typedef cl_ushort __cl_ushort4 __attribute__((vector_size(8)));
typedef cl_short __cl_short4 __attribute__((vector_size(8)));
typedef cl_uint __cl_uint2 __attribute__((vector_size(8)));
typedef cl_int __cl_int2 __attribute__((vector_size(8)));
typedef cl_ulong __cl_ulong1 __attribute__((vector_size(8)));
typedef cl_long __cl_long1 __attribute__((vector_size(8)));
typedef cl_float __cl_float2 __attribute__((vector_size(8)));
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
        __cl_char8 v8;
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
        __cl_char8 v8[2];
        __cl_char16 v16;
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
        __cl_uchar8 v8;
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
        __cl_uchar8 v8[2];
        __cl_uchar16 v16;
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
        __cl_short4 v4;
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
        __cl_short4 v4[2];
        __cl_short8 v8;
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
        __cl_short4 v4[4];
        __cl_short8 v8[2];
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
        __cl_ushort4 v4;
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
        __cl_ushort4 v4[2];
        __cl_ushort8 v8;
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
        __cl_ushort4 v4[4];
        __cl_ushort8 v8[2];
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
        __cl_int2 v2;
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
        __cl_int2 v2[2];
        __cl_int4 v4;
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
        __cl_int2 v2[4];
        __cl_int4 v4[2];
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
        __cl_int2 v2[8];
        __cl_int4 v4[4];
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
        __cl_uint2 v2;
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
        __cl_uint2 v2[2];
        __cl_uint4 v4;
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
        __cl_uint2 v2[4];
        __cl_uint4 v4[2];
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
        __cl_uint2 v2[8];
        __cl_uint4 v4[4];
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
        __cl_long2 v2;
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
        __cl_long2 v2[2];
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
        __cl_long2 v2[4];
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
        __cl_long2 v2[8];
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
        __cl_ulong2 v2;
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
        __cl_ulong2 v2[2];
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
        __cl_ulong2 v2[4];
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
        __cl_ulong2 v2[8];
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
        __cl_float2 v2;
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
        __cl_float2 v2[2];
        __cl_float4 v4;
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
        __cl_float2 v2[4];
        __cl_float4 v4[2];
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
        __cl_float2 v2[8];
        __cl_float4 v4[4];
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
        __cl_double2 v2;
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
        __cl_double2 v2[2];
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
        __cl_double2 v2[4];
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
        __cl_double2 v2[8];
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
extern cl_int clEnqueueNativeKernel(cl_command_queue, void (*)(void *), void *, size_t, cl_uint, const cl_mem *, const void **, cl_uint, const cl_event *, cl_event *);
extern cl_int clEnqueueMarker(cl_command_queue, cl_event *);
extern cl_int clEnqueueWaitForEvents(cl_command_queue, cl_uint, const cl_event *);
extern cl_int clEnqueueBarrier(cl_command_queue);
extern void *clGetExtensionFunctionAddress(const char *);
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
        __ino_t st_ino;
        __nlink_t st_nlink;
        __mode_t st_mode;
        __uid_t st_uid;
        __gid_t st_gid;
        int __pad0;
        __dev_t st_rdev;
        __off_t st_size;
        __blksize_t st_blksize;
        __blkcnt_t st_blocks;
        struct timespec st_atim;
        struct timespec st_mtim;
        struct timespec st_ctim;
        long int __unused[3];
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
                int _pad[((64 / sizeof(int)) - 4)];
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
void _SendInputMsg(void *buffer, size_t size);
void _RecvOutputMsg(void *buffer, size_t size);
void _RecvInputMsg(void *buffer, size_t size);
void _SendOutputMsg(void *buffer, size_t size);
void _SendCallFuncMsg(int func_code);
void _RecvCallFuncMsg(int *func_code);
void _SendMasterMsg(void *buffer, size_t size);
void _RecvMasterMsg(void *buffer, size_t size);
void _SendWorkerMsg(void *buffer, size_t size);
void _RecvWorkerMsg(void *buffer, size_t size);
int gfn_get_num_process();
int gfn_get_process_num();
void _GfnBarrier();
void _GfnCheckCLStatus(cl_int status, const char *phase_name);
size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num);
extern int _gfn_rank;
extern int _gfn_num_proc;
int _CalcLoopSize(int start, int end, int incre);
int _CalcSubSize(int size, int num_proc, int rank, int block_size);
int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcOffset(int size, int num_proc, int rank);
void _CalcCnts(int size, int num_proc, int *cnts, int block_size);
void _CalcDisp(int size, int num_proc, int *disp, int block_size);
MPI_Datatype _get_mpi_char();
MPI_Datatype _get_mpi_unsigned_char();
MPI_Datatype _get_mpi_byte();
MPI_Datatype _get_mpi_short();
MPI_Datatype _get_mpi_unsigned_short();
MPI_Datatype _get_mpi_int();
MPI_Datatype _get_mpi_unsigned();
MPI_Datatype _get_mpi_long();
MPI_Datatype _get_mpi_unsigned_long();
MPI_Datatype _get_mpi_float();
MPI_Datatype _get_mpi_double();
MPI_Datatype _get_mpi_long_double();
MPI_Datatype _get_mpi_long_long_int();
MPI_Op _get_mpi_max();
MPI_Op _get_mpi_min();
MPI_Op _get_mpi_sum();
MPI_Op _get_mpi_prod();
MPI_Op _get_mpi_land();
MPI_Op _get_mpi_band();
MPI_Op _get_mpi_lor();
MPI_Op _get_mpi_bor();
MPI_Op _get_mpi_lxor();
MPI_Op _get_mpi_bxor();
MPI_Op _get_mpi_minloc();
MPI_Op _get_mpi_maxloc();
MPI_Comm _get_mpi_comm_world();
extern cl_platform_id _gfn_platform_id;
extern cl_device_id _gfn_device_id;
extern cl_context _gfn_context;
extern cl_command_queue _gfn_cmd_queue;
extern cl_int _gfn_status;
void _InitOpenCL();
void _FinalOpenCL();
cl_kernel _CreateKernelFromSource(const char *name, const char *src, cl_context context, cl_device_id device_id);
cl_bool _get_cl_true();
cl_bool _get_cl_false();
cl_mem_flags _get_cl_mem_read_write();
cl_mem_flags _get_cl_mem_write_only();
cl_mem_flags _get_cl_mem_read_only();
cl_mem_flags _get_cl_mem_use_host_ptr();
cl_mem_flags _get_cl_mem_alloc_host_ptr();
/* */ #ifdef GFN_WORKER /* */
/* void _GfnBarrier() { */
/*     barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE); */
/* } */
/*  */
/* int _GfnAtomicAddInt(__global int* const address, const int value) { */
/*     return atomic_add(address, value); */
/* } */
/*  */
/* float _GfnAtomicAddFloat(__global float* const address, const float value) { */
/*     uint oldval, newval, readback; */
/*     *(float*)&oldval = *address; */
/*     *(float*)&newval = (*(float*)&oldval + value); */
/*     while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) { */
/*         oldval = readback; */
/*         *(float*)&newval = (*(float*)&oldval + value); */
/*     } */
/*     return *(float*)&oldval; */
/* } */
/*  */
/* __kernel void _kernel_1(__global double * _local_buffer_A,int _local_i_start,int _local_i_end,int _loop_step) { */
/* int j; */
/* int k; */
/* double * A = ((void *)_local_buffer_A) - (_local_i_start * sizeof(double)); */
/* int _loop_size = (_local_i_end - _local_i_start) / _loop_step; */
/* int i = (get_global_id(0) * _loop_step) + _local_i_start; */
/* if (i <= (4) - 1) { */
/* for (j = 0; */
/*     j < 20; */
/*     j++) */
/*     for (k = 0; */
/*         k < 100; */
/*         k++) */
/*         A[((i) * 100 * 20) + ((j) * 100) + (k)] = 123; */
/* } */
/* } */
const char *_kernel_1_src = "void _GfnBarrier() {""    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);""}""""int _GfnAtomicAddInt(__global int* const address, const int value) {""    return atomic_add(address, value);""}""""float _GfnAtomicAddFloat(__global float* const address, const float value) {""    uint oldval, newval, readback;""    *(float*)&oldval = *address;""    *(float*)&newval = (*(float*)&oldval + value);""    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {""        oldval = readback;""        *(float*)&newval = (*(float*)&oldval + value);""    }""    return *(float*)&oldval;""}""""__kernel void _kernel_1(__global double * _local_buffer_A,int _local_i_start,int _local_i_end,int _loop_step) {""int j;""int k;""double * A = ((void *)_local_buffer_A) - (_local_i_start * sizeof(double));""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;""int i = (get_global_id(0) * _loop_step) + _local_i_start;""if (i <= (4) - 1) {""for (j = 0;""    j < 20;""    j++)""    for (k = 0;""        k < 100;""        k++)""        A[((i) * 100 * 20) + ((j) * 100) + (k)] = 123;""}""}";
void _Function_1()
{
    int i;
    int j;
    int k;
    double *A;
    cl_mem _cl_mem_A;
    cl_kernel _kernel;
    /* Generated variable */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    double *_local_buffer_A;
    double *_buffer_A;
    int _sub_size_A;
    int _A_cnts[_gfn_num_proc];
    int _A_disp[_gfn_num_proc];
    if (_gfn_rank == 0)
    {
        _buffer_A = (double *) malloc((sizeof(double) * ((4) * (20) * (100))));
    }
    /* Init generated variable */
    _local_i_start = _CalcLocalStartIndex(0, (4) - 1, _gfn_num_proc, _gfn_rank + 1);
    _local_i_end = _CalcLocalEndIndex(0, (4) - 1, _gfn_num_proc, _gfn_rank + 1);
    _loop_size = _CalcLoopSize(0, (4) - 1, 1);
    _loop_step = 1;
    _sub_size_A = _CalcSubSize((4), _gfn_num_proc, _gfn_rank, ((20) * (100)));
    _local_buffer_A = (double *) malloc(sizeof(double) * _sub_size_A);
    A = ((void *) _local_buffer_A) - ((_local_i_start * ((20) * (100))) * sizeof(double));
    _CalcCnts((4), _gfn_num_proc, _A_cnts, ((20) * (100)));
    _CalcDisp((4), _gfn_num_proc, _A_disp, ((20) * (100)));
    _cl_mem_A = clCreateBuffer(_gfn_context, _get_cl_mem_write_only(), sizeof(double) * _sub_size_A, 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    /* Send data to all process */
    /* Compute work-load */
    if (1)
    {
        size_t _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
        size_t _work_group_item_num = 64;
        size_t _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
        cl_int _local_cl_i_start = _local_i_start;
        cl_int _local_cl_i_end = _local_i_end;
        cl_int _cl_loop_step = _loop_step;
        /* TODO: Overlap node data transfer and device data transfer */
        _kernel = _CreateKernelFromSource("_kernel_1", _kernel_1_src, _gfn_context, _gfn_device_id);
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(cl_int), (void *) &_local_cl_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_int), (void *) &_local_cl_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(cl_int), (void *) &_cl_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
        _gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, _cl_mem_A, _get_cl_true(), 0, sizeof(double) * _sub_size_A, _local_buffer_A, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER _local_buffer_A");
    }
    else
    {
        for (i = _local_i_start;
            i < _local_i_end;
            i++)
            for (j = 0;
                j < 20;
                j++)
                for (k = 0;
                    k < 100;
                    k++)
                    A[((i) * 100 * 20) + ((j) * 100) + (k)] = 123;
    }
    /* Gather data from all process */
    MPI_Gatherv(_local_buffer_A, _sub_size_A, _get_mpi_double(), _buffer_A, _A_cnts, _A_disp, _get_mpi_double(), 0, _get_mpi_comm_world());
    if (_gfn_rank == 0)
    {
        _SendOutputMsg((void *) _buffer_A, (sizeof(double) * ((4) * (20) * (100))));
    }
    free(_local_buffer_A);
    if (_gfn_rank == 0)
    {
        free(_buffer_A);
    }
    _gfn_status = clReleaseMemObject(_cl_mem_A);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_A");
}
/* */ #endif /* */

#ifndef GFN_WORKER
int main()
{
    int i, j, k;
    double A[4][20][100];
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    /* Send call function message */
    _SendCallFuncMsg(1);
    _RecvOutputMsg((void *) A, (sizeof(double) * ((4) * (20) * (100))));
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    for (i = 0;
        i < 4;
        i++)
    {
        for (j = 0;
            j < 20;
            j++)
        {
            for (k = 0;
                k < 100;
                k++)
            {
                if (A[i][j][k] != 123)
                {
                    printf("Fail at i = %d , j = %d , k = %d, A[i][j][k] = %lf\n", i, j, k, A[i][j][k]);
                    ((0) ? (void) (0) : __assert_fail("0", "nd_array.c", 30, __PRETTY_FUNCTION__));
                }
            }
        }
    }
    printf("TEST PASS!\n");
    return 0;
}
#endif

