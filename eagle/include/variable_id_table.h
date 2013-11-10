#include <CL/cl.h>

int _insert_to_var_table( long long id, cl_mem device_ptr, int dimension_num, 
						  void * host_ptr1, void ** host_ptr2, void *** host_ptr3,
						  void **** host_ptr4, void ***** host_ptr5, void ****** host_ptr6 );
int _retieve_var_table( long long id, cl_mem *device_ptr, int *dimension_num,
						void ** host_ptr, int *found );
int _free_mem_and_delete_from_var_table(long long id);
int _free_mem_and_clear_var_table();


int _lock_transfer( long long id );
int _unlock_transfer( long long id );
int _is_lock_transfer( long long id );
int _clear_lock_table();
