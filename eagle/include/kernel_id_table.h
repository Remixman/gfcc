#include <CL/cl.h>

struct _data_information;
struct _kernel_information;

int _insert_to_kernel_table( long long id, struct _kernel_information *ker_info );
int _retieve_kernel_table( long long id, struct _kernel_information *ker_info, int *found );

int _free_mem_and_clear_kernel_table();
