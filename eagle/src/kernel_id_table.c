#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_id_table.h"
#include "gfn.h"

typedef struct kernel_record {
	long long id;
	struct _kernel_information *ker_info;
} kernel_record;

int _kernel_compare(const void *l, const void *r) {
	const kernel_record *lm = (const kernel_record *)l;
	const kernel_record *lr = (const kernel_record *)r;
	return lm->id - lr->id;
}

void * _kernel_tab_root;

int _insert_to_kernel_table( long long id, struct _kernel_information *ker_info ) {
	kernel_record * new_rec = (kernel_record*) malloc(sizeof(kernel_record));
	
	new_rec->id = id;
	new_rec->ker_info = ker_info;
	
	tsearch(new_rec, &_kernel_tab_root, _kernel_compare);
	
	return 0;
}

int _retieve_kernel_table( long long id, struct _kernel_information *ker_info, int *found ) {
	void *retieved_rec = NULL;
	kernel_record find_rec;
	find_rec.id = id;
	
	retieved_rec = tfind((void *)&find_rec, &_kernel_tab_root, _kernel_compare);
	
	if (retieved_rec) {
		ker_info = (*(kernel_record**)retieved_rec)->ker_info;
		*found = 1 /* TRUE */;
	} else {
		*found = 0 /* FALSE */;
	}
	
	return 0;
}

int _free_mem_and_clear_kernel_table() {
	tdestroy(_kernel_tab_root, free);
	
	return 0;
}
