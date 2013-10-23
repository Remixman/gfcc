#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include "variable_id_table.h"
#include "gfn.h"

typedef struct var_record {
	long long id;
	int dimension_num;
	void * host_ptr1;
	void ** host_ptr2;
	void *** host_ptr3;
	void **** host_ptr4;
	void ***** host_ptr5;
	void ****** host_ptr6;
	cl_mem device_ptr;
} var_record;

int _var_compare(const void *l, const void *r) {
	const var_record *lm = (const var_record *)l;
	const var_record *lr = (const var_record *)r;
	return lm->id - lr->id;
}

void _var_free_action(const void *nodep, const VISIT which, const int depth) {
	var_record *datap = *((var_record **)nodep);
    int dim_num = datap->dimension_num;
    
	switch(which) {
		case preorder:
		case postorder:
		case endorder:
        case leaf:

#ifdef DEBUG_AUTO_ALLOCATE
	printf("[DEBUG]: Deallocate : xxx : %p\n", datap->host_ptr1);
#endif
			if (datap->host_ptr1 == NULL) {
				/* Already deallocate host and device memory */
				return;
			}

			if (dim_num >= 6) free(datap->host_ptr6);
			if (dim_num >= 5) free(datap->host_ptr5);
			if (dim_num >= 4) free(datap->host_ptr4);
			if (dim_num >= 3) free(datap->host_ptr3);
			if (dim_num >= 2) free(datap->host_ptr2);
			free(datap->host_ptr1);

			_gfn_status = clReleaseMemObject(datap->device_ptr);
    		_GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER");

    		datap->host_ptr1 = NULL;
			break;
    }
}

void * _var_tab_root;

int _insert_to_var_table( long long id, cl_mem device_ptr, int dimension_num, 
						  void * host_ptr1, void ** host_ptr2, void *** host_ptr3,
						  void **** host_ptr4, void ***** host_ptr5, void ****** host_ptr6 ) {

#ifdef DEBUG_AUTO_ALLOCATE
	printf("[DEBUG]: Allocate : %ld : %p\n", id, host_ptr1);
#endif

	// INSERT
	// void * tsearch (const void *key, void **rootp, comparison_fn_t compar)
	var_record * new_rec = (var_record *) malloc(sizeof(var_record));
	new_rec->id = id;
	new_rec->dimension_num = dimension_num;
	new_rec->host_ptr1 = host_ptr1;
	new_rec->host_ptr2 = host_ptr2;
	new_rec->host_ptr3 = host_ptr3;
	new_rec->host_ptr4 = host_ptr4;
	new_rec->host_ptr5 = host_ptr5;
	new_rec->host_ptr6 = host_ptr6;
	new_rec->device_ptr = device_ptr;

	tsearch(new_rec, &_var_tab_root, _var_compare);

	return 0;
}

int _retieve_var_table( long long id, cl_mem device_ptr, int *dimension_num,
						void ** host_ptr, int *found ) {

	// FIND
	// void * tfind (const void *key, void *const *rootp, comparison_fn_t compar)
	void *retieved_rec = NULL;
	var_record find_rec;
	find_rec.id = id;

	retieved_rec = tfind((void *)&find_rec, &_var_tab_root, _var_compare);

	if (retieved_rec) {
		*dimension_num = (*(var_record**)retieved_rec)->dimension_num;
		*host_ptr = (*(var_record**)retieved_rec)->host_ptr1;
		device_ptr = (*(var_record**)retieved_rec)->device_ptr;
		*found = 1 /* TRUE */;
	} else {
		*found = 0 /* FALSE */;
	}

#ifdef DEBUG_AUTO_ALLOCATE
	printf("[DEBUG]: Found [%d] Retieve : %ld : %p\n", *found, id, *host_ptr);
#endif

	return 0;
}

int _free_mem_and_delete_from_var_table(long long id) {
	// FIND ,DEALLOCATE and DELETE
	// void * tfind (const void *key, void *const *rootp, comparison_fn_t compar)
	// void * tdelete (const void *key, void **rootp, comparison_fn_t compar)
	void *retieved_rec = NULL;
	var_record find_rec;
	find_rec.id = id;

	retieved_rec = tfind((void *)&find_rec, &_var_tab_root, _var_compare);

	if (retieved_rec) {
		int dim_num = (*(var_record**)retieved_rec)->dimension_num;

		if (dim_num >= 6) free((*(var_record**)retieved_rec)->host_ptr6);
		if (dim_num >= 5) free((*(var_record**)retieved_rec)->host_ptr5);
		if (dim_num >= 4) free((*(var_record**)retieved_rec)->host_ptr4);
		if (dim_num >= 3) free((*(var_record**)retieved_rec)->host_ptr3);
		if (dim_num >= 2) free((*(var_record**)retieved_rec)->host_ptr2);
		free((*(var_record**)retieved_rec)->host_ptr1);

		// device mem maybe nil, so check before release
		if ((*(var_record**)retieved_rec)->device_ptr) {
			_gfn_status = clReleaseMemObject((*(var_record**)retieved_rec)->device_ptr);
    		_GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER");
    	}

    	tdelete((void *)&find_rec, &_var_tab_root, _var_compare);
	}

	return 0;
}

int _free_mem_and_clear_var_table() {
	// WALK and DEALLOCATE WITH
	// void twalk (const void *root, __action_fn_t action)
	// tdestroy (void *vroot, __free_fn_t freefct)
	twalk(_var_tab_root, _var_free_action);
	tdestroy(_var_tab_root, free);

	//http://www.gnu.org/software/libc/manual/html_node/Tree-Search-Function.html
	//http://linux.die.net/man/3/tdestroy

	return 0;
}


int _lock_var_compare(const void *l, const void *r) {
	const long long *lm = (const long long *)l;
	const long long *lr = (const long long *)r;
	return lm - lr;
}

void * _lock_var_tab_root;

int _lock_transfer( long long id ) {

#ifdef DEBUG_VAR_TABLE
	printf("[DEBUG VTAB]: Lock transfer \"%ld\"\n", id);
#endif

	long long * id_ptr = (long long *) malloc(sizeof(long long));
	*id_ptr = id;
	tsearch(id_ptr, &_lock_var_tab_root, _lock_var_compare);

	return 0;
}

int _unlock_transfer( long long id ) {

	tdelete((void *)&id, &_lock_var_tab_root, _lock_var_compare);

	return 0;
}

int _is_lock_transfer( long long id ) {

	void *retieved_id = NULL;
	retieved_id = tfind((void *)&id, &_lock_var_tab_root, _lock_var_compare);

#ifdef DEBUG_VAR_TABLE
	printf("[DEBUG VTAB]: Check lock \"%ld\" result is ", id);
	if (retieved_id != NULL)  printf("FOUND\n");
	else                      printf("NOT FOUND\n");
#endif

	return (retieved_id != NULL);
}

int _clear_lock_table() {

	tdestroy(_lock_var_tab_root, free);

	return 0;
}
