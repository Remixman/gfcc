#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "variable_id_table.h"
#include "gfn.h"


int _gfn_rank;		/**/
int _gfn_num_proc;	/**/
cl_platform_id _gfn_platform_id;
cl_device_id _gfn_device_id;
cl_context _gfn_context;
cl_command_queue _gfn_cmd_queue;
cl_int _gfn_status;
cl_program _gfn_cl_program;

#define PATTERN_NONE     0
#define PATTERN_RANGE    1
#define PATTERN_SPECIFIC 2

#define TYPE_CHAR             1
#define TYPE_UNSIGNED_CHAR    2
#define TYPE_BYTE             3
#define TYPE_SHORT            4
#define TYPE_UNSIGNED_SHORT   5
#define TYPE_INT              6
#define TYPE_UNSIGNED         7
#define TYPE_LONG             8
#define TYPE_UNSIGNED_LONG    9
#define TYPE_FLOAT            10
#define TYPE_DOUBLE           11
#define TYPE_LONG_DOUBLE      12
#define TYPE_LONG_LONG_INT    13

// API for user
int gfn_get_num_process()
{
	return _gfn_num_proc;
}

int gfn_get_process_num()
{
	return _gfn_rank;
}

int _GfnInit(int *argc, char **argv[])
{
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;

	MPI_Init(argc, argv);
	MPI_Comm_size(MPI_COMM_WORLD, &_gfn_num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &_gfn_rank);
	MPI_Get_processor_name(processor_name, &name_len);
	
	printf("Rank %d is at %s\n", _gfn_rank, processor_name);

	_InitOpenCL();

	if (_gfn_rank == 0) _OpenWorkerMsgQueue();

	return 0;
}

int _GfnFinalize()
{
	_free_mem_and_clear_var_table();

	_FinalOpenCL();
	
	MPI_Finalize();

	if (_gfn_rank == 0) _CloseWorkerMsgQueue();

	return 0;
}

int _GfnEnqueueBoardcastScalar(void *ptr, int type_id)
{
	// TOD0: MPI pack and bcast at _GfnFinishBoardcastScalar

#define SWITCH_BCAST(mpi_type) \
do { \
	MPI_Bcast(ptr, 1, mpi_type, 0, MPI_COMM_WORLD); \
} while(0)

	if (_gfn_rank == 0) _RecvInputMsg(ptr, _CalcTypeSize(type_id));
    
	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_BCAST(MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_BCAST(MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_BCAST(MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_BCAST(MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_BCAST(MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_BCAST(MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_BCAST(MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_BCAST(MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_BCAST(MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_BCAST(MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_BCAST(MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_BCAST(MPI_LONG_LONG_INT); break;
	}

	return 0;
}

int _GfnFinishBoardcastScalar()
{
	return 0;
}

int _GfnEnqueueReduceScalar(void *ptr, int type_id, MPI_Op op_id)
{
	// TOD0: MPI pack and bcast at _GfnFinishReduceScalar

#define SWITCH_REDUCE(mpi_type) \
do { \
	MPI_Reduce(ptr, ptr, 1, mpi_type, op_id, 0 /* root */, MPI_COMM_WORLD); \
} while(0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_REDUCE(MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_REDUCE(MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_REDUCE(MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_REDUCE(MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_REDUCE(MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_REDUCE(MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_REDUCE(MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_REDUCE(MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_REDUCE(MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_REDUCE(MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_REDUCE(MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_REDUCE(MPI_LONG_LONG_INT); break;
	}

	if (_gfn_rank == 0) _SendOutputMsg(ptr, _CalcTypeSize(type_id));
	return 0;
}

int _GfnFinishReduceScalar()
{ 
	return 0;
}

int _GfnMalloc1D(void ** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc)
{

#define SWITCH_MALLOC_1D(type,size1) \
do { \
	type * tmp_ptr; \
	tmp_ptr = (type *) malloc(sizeof(type) * size1); \
	*ptr = (void *) tmp_ptr; \
	if (level2_malloc) { \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1, 0, &_gfn_status); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
    } \
    _insert_to_var_table(unique_id, *cl_ptr, 1, (void *)tmp_ptr, NULL, NULL, NULL, NULL, NULL); \
} while (0)

	
	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_1D(char,dim1_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_1D(unsigned char,dim1_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_1D(short,dim1_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_1D(unsigned short,dim1_size); break;
	case TYPE_INT:            SWITCH_MALLOC_1D(int,dim1_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_1D(unsigned,dim1_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_1D(long,dim1_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_1D(unsigned long,dim1_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_1D(float,dim1_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_1D(double,dim1_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_1D(long double,dim1_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_1D(long long int,dim1_size); break;
	}
	
	return 0;
}

int _GfnMalloc2D(void *** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc)
{
	int i;

#define SWITCH_MALLOC_2D(type,size1,size2) \
do { \
	type ** tmp_ptr; \
	tmp_ptr = (type **) malloc(sizeof(type*) * size1); \
	tmp_ptr[0] = (type *) malloc(sizeof(type) * size1 * size2); \
	for (i = 1; i < size1; ++i) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	*ptr = (void **) tmp_ptr; \
	if (level2_malloc) { \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2, 0, &_gfn_status); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	_insert_to_var_table(unique_id, *cl_ptr, 1, (void *)tmp_ptr[0], (void **)tmp_ptr, NULL, NULL, NULL, NULL); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_2D(char,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_2D(unsigned char,dim1_size,dim2_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_2D(short,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_2D(unsigned short,dim1_size,dim2_size); break;
	case TYPE_INT:            SWITCH_MALLOC_2D(int,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_2D(unsigned,dim1_size,dim2_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_2D(long,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_2D(unsigned long,dim1_size,dim2_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_2D(float,dim1_size,dim2_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_2D(double,dim1_size,dim2_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_2D(long double,dim1_size,dim2_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_2D(long long int,dim1_size,dim2_size); break;
	}
	
	return 0;
}

int _GfnMalloc3D(void **** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc)
{
	int i;

#define SWITCH_MALLOC_3D(type,size1,size2,size3) \
do { \
	type *** tmp_ptr; \
	tmp_ptr = (type ***) malloc(sizeof(type**) * size1); \
	tmp_ptr[0] = (type **) malloc(sizeof(type*) * size1 * size2); \
	for (i = 1; i < size1; i++) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	tmp_ptr[0][0] = (type *) malloc(sizeof(type) * size1 * size2 * size3); \
	for (i = 1; i < size1 * size2; i++) tmp_ptr[0][i] = tmp_ptr[0][i-1] + size3; \
	*ptr = (void ***) tmp_ptr; \
	if (level2_malloc) { \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2 * size3, 0, &_gfn_status); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	_insert_to_var_table(unique_id, *cl_ptr, 1, (void *)tmp_ptr[0][0], (void **)tmp_ptr[0], (void ***)tmp_ptr, NULL, NULL, NULL); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_3D(char,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_3D(unsigned char,dim1_size,dim2_size,dim3_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_3D(short,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_3D(unsigned short,dim1_size,dim2_size,dim3_size); break;
	case TYPE_INT:            SWITCH_MALLOC_3D(int,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_3D(unsigned,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_3D(long,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_3D(unsigned long,dim1_size,dim2_size,dim3_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_3D(float,dim1_size,dim2_size,dim3_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_3D(double,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_3D(long double,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_3D(long long int,dim1_size,dim2_size,dim3_size); break;
	}

	return 0;
}

int _GfnMalloc4D(void ***** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc)
{
	int i;

#define SWITCH_MALLOC_4D(type,size1,size2,size3,size4) \
do { \
	type **** tmp_ptr; \
	tmp_ptr = (type ****) malloc(sizeof(type***) * size1); \
	tmp_ptr[0] = (type ***) malloc(sizeof(type**) * size1 * size2); \
	for (i = 1; i < size1; i++) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	tmp_ptr[0][0] = (type **) malloc(sizeof(type*) * size1 * size2 * size3); \
	for (i = 1; i < size1 * size2; i++) tmp_ptr[0][i] = tmp_ptr[0][i-1] + size3; \
	tmp_ptr[0][0][0] = (type *) malloc(sizeof(type) * size1 * size2 * size3 * size4); \
	for (i = 1; i < size1 * size2 * size3; i++) tmp_ptr[0][0][i] = tmp_ptr[0][0][i-1] + size4; \
	*ptr = (void ****) tmp_ptr; \
	if (level2_malloc) { \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2 * size3 * size4, 0, &_gfn_status); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_4D(char,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_4D(unsigned char,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_4D(short,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_4D(unsigned short,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_INT:            SWITCH_MALLOC_4D(int,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_4D(unsigned,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_4D(long,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_4D(unsigned long,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_4D(float,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_4D(double,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_4D(long double,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_4D(long long int,dim1_size,dim2_size,dim3_size,dim4_size); break;
	}

	return 0;
}

int _GfnMalloc5D(void ****** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc)
{ 
	return 0;
}

int _GfnMalloc6D(void ******* ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc)
{ 
	return 0;
}

int _GfnFree(long long unique_id, int level1_malloc, int level2_malloc)
{
	_free_mem_and_delete_from_var_table(unique_id);

	return 0;
}

int _GfnEnqueueBoardcast1D(void ** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, int level1_transfer, int level2_transfer)
{
	// TODO: make queue and boardcast out-of-order
	// TODO: level 2 transfer only used partition
#define SWITCH_BCAST_1D(type,mpi_type,size1) \
do { \
	type * tmp_ptr = (type *) (*ptr); \
	if (_gfn_rank == 0) _RecvInputMsg((void *)(tmp_ptr), sizeof(type) * size1); \
	MPI_Bcast((void *)(tmp_ptr), size1, mpi_type, 0, MPI_COMM_WORLD); \
	if (level2_transfer) { \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * size1, tmp_ptr, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           
		SWITCH_BCAST_1D(char,MPI_CHAR,dim1_size); break;
	case TYPE_UNSIGNED_CHAR:  
		SWITCH_BCAST_1D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size); break;
	case TYPE_SHORT:          
		SWITCH_BCAST_1D(short,MPI_SHORT,dim1_size); break;
	case TYPE_UNSIGNED_SHORT: 
		SWITCH_BCAST_1D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size); break;
	case TYPE_INT:            
		SWITCH_BCAST_1D(int,MPI_INT,dim1_size); break;
	case TYPE_UNSIGNED:       
		SWITCH_BCAST_1D(unsigned,MPI_UNSIGNED,dim1_size); break;
	case TYPE_LONG:           
		SWITCH_BCAST_1D(long,MPI_LONG,dim1_size); break;
	case TYPE_UNSIGNED_LONG:  
		SWITCH_BCAST_1D(unsigned long,MPI_UNSIGNED_LONG,dim1_size); break;
	case TYPE_FLOAT:          
		SWITCH_BCAST_1D(float,MPI_FLOAT,dim1_size); break;
	case TYPE_DOUBLE:         
		SWITCH_BCAST_1D(double,MPI_DOUBLE,dim1_size); break;
	case TYPE_LONG_DOUBLE:    
		SWITCH_BCAST_1D(long double,MPI_LONG_DOUBLE,dim1_size); break;
	case TYPE_LONG_LONG_INT:  
		SWITCH_BCAST_1D(long long int,MPI_LONG_LONG_INT,dim1_size); break;
	}

	return 0;
}

int _GfnEnqueueBoardcast2D(void *** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, int level1_transfer, int level2_transfer)
{
	// TODO: make queue and boardcast out-of-order
	// TODO: level 2 transfer only used partition
#define SWITCH_BCAST_2D(type,mpi_type,size1,size2) \
do { \
	type ** tmp_ptr = (type **) (*ptr); \
	if (_gfn_rank == 0) _RecvInputMsg(tmp_ptr[0], sizeof(type) * size1 * size2); \
	MPI_Bcast(tmp_ptr[0], size1 * size2, mpi_type, 0, MPI_COMM_WORLD); \
	if (level2_transfer) { \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * size1 * size2, tmp_ptr[0], 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           
		SWITCH_BCAST_2D(char,MPI_CHAR,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_CHAR:  
		SWITCH_BCAST_2D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size,dim2_size); break;
	case TYPE_SHORT:          
		SWITCH_BCAST_2D(short,MPI_SHORT,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_SHORT: 
		SWITCH_BCAST_2D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size,dim2_size); break;
	case TYPE_INT:            
		SWITCH_BCAST_2D(int,MPI_INT,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED:       
		SWITCH_BCAST_2D(unsigned,MPI_UNSIGNED,dim1_size,dim2_size); break;
	case TYPE_LONG:           
		SWITCH_BCAST_2D(long,MPI_LONG,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_LONG:  
		SWITCH_BCAST_2D(unsigned long,MPI_UNSIGNED_LONG,dim1_size,dim2_size); break;
	case TYPE_FLOAT:          
		SWITCH_BCAST_2D(float,MPI_FLOAT,dim1_size,dim2_size); break;
	case TYPE_DOUBLE:         
		SWITCH_BCAST_2D(double,MPI_DOUBLE,dim1_size,dim2_size); break;
	case TYPE_LONG_DOUBLE:    
		SWITCH_BCAST_2D(long double,MPI_LONG_DOUBLE,dim1_size,dim2_size); break;
	case TYPE_LONG_LONG_INT:  
		SWITCH_BCAST_2D(long long int,MPI_LONG_LONG_INT,dim1_size,dim2_size); break;
	}

	return 0;
}

int _GfnEnqueueBoardcast3D(void **** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, int level1_transfer, int level2_transfer)
{ 
#define SWITCH_BCAST_3D(type,mpi_type,size1,size2,size3) \
do { \
	type *** tmp_ptr = (type ***) (*ptr); \
	if (_gfn_rank == 0) _RecvInputMsg(tmp_ptr[0][0], sizeof(type) * size1 * size2 * size3); \
	MPI_Bcast(tmp_ptr[0][0], size1 * size2 * size3, mpi_type, 0, MPI_COMM_WORLD); \
	if (level2_transfer) { \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * size1 * size2 * size3, tmp_ptr[0][0], 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           
		SWITCH_BCAST_3D(char,MPI_CHAR,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_CHAR:  
		SWITCH_BCAST_3D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size,dim2_size,dim3_size); break;
	case TYPE_SHORT:          
		SWITCH_BCAST_3D(short,MPI_SHORT,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_SHORT: 
		SWITCH_BCAST_3D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size,dim2_size,dim3_size); break;
	case TYPE_INT:            
		SWITCH_BCAST_3D(int,MPI_INT,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED:       
		SWITCH_BCAST_3D(unsigned,MPI_UNSIGNED,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG:           
		SWITCH_BCAST_3D(long,MPI_LONG,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_LONG:  
		SWITCH_BCAST_3D(unsigned long,MPI_UNSIGNED_LONG,dim1_size,dim2_size,dim3_size); break;
	case TYPE_FLOAT:          
		SWITCH_BCAST_3D(float,MPI_FLOAT,dim1_size,dim2_size,dim3_size); break;
	case TYPE_DOUBLE:         
		SWITCH_BCAST_3D(double,MPI_DOUBLE,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG_DOUBLE:    
		SWITCH_BCAST_3D(long double,MPI_LONG_DOUBLE,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG_LONG_INT:  
		SWITCH_BCAST_3D(long long int,MPI_LONG_LONG_INT,dim1_size,dim2_size,dim3_size); break;
	}

	return 0;
}

int _GfnEnqueueBoardcast4D(void ***** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueBoardcast5D(void ****** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueBoardcast6D(void ******* ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, int level1_transfer, int level2_transfer)
{ return 0; }

int _GfnEnqueueScatter1D(void ** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{
	// TODO: level 2 transfer only used partition

	int cnts[_gfn_num_proc];
    int disp[_gfn_num_proc];
    int sub_size, recv_elem_offset;

    _CalcPartitionInfo(dim1_size, 1, pattern_array, pattern_array_size, pattern_type, cnts, disp, &sub_size, &recv_elem_offset);

#define SWITCH_SCATTER_1D(type,mpi_type,size1) \
do { \
	type * tmp_ptr = (type *) (*ptr); \
	if (_gfn_rank == 0) _RecvInputMsg(tmp_ptr, sizeof(type) * size1); \
	MPI_Scatterv(tmp_ptr, cnts, disp, mpi_type, tmp_ptr + recv_elem_offset, sub_size, mpi_type, 0, MPI_COMM_WORLD); \
	if (level2_transfer) { \
		cl_buffer_region info; \
		info.origin = (size_t)(recv_elem_offset * sizeof(type)); \
		info.size = (size_t)(sub_size * sizeof(type)); \
		cl_mem subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
		_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + recv_elem_offset, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
		_gfn_status = clReleaseMemObject(subbuf); \
		_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           
		SWITCH_SCATTER_1D(char,MPI_CHAR,dim1_size); break;
	case TYPE_UNSIGNED_CHAR:  
		SWITCH_SCATTER_1D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size); break;
	case TYPE_SHORT:          
		SWITCH_SCATTER_1D(short,MPI_SHORT,dim1_size); break;
	case TYPE_UNSIGNED_SHORT: 
		SWITCH_SCATTER_1D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size); break;
	case TYPE_INT:            
		SWITCH_SCATTER_1D(int,MPI_INT,dim1_size); break;
	case TYPE_UNSIGNED:       
		SWITCH_SCATTER_1D(unsigned,MPI_UNSIGNED,dim1_size); break;
	case TYPE_LONG:           
		SWITCH_SCATTER_1D(long,MPI_LONG,dim1_size); break;
	case TYPE_UNSIGNED_LONG:  
		SWITCH_SCATTER_1D(unsigned long,MPI_UNSIGNED_LONG,dim1_size); break;
	case TYPE_FLOAT:          
		SWITCH_SCATTER_1D(float,MPI_FLOAT,dim1_size); break;
	case TYPE_DOUBLE:         
		SWITCH_SCATTER_1D(double,MPI_DOUBLE,dim1_size); break;
	case TYPE_LONG_DOUBLE:    
		SWITCH_SCATTER_1D(long double,MPI_LONG_DOUBLE,dim1_size); break;
	case TYPE_LONG_LONG_INT:  
		SWITCH_SCATTER_1D(long long int,MPI_LONG_LONG_INT,dim1_size); break;
	}

	return 0;
}
int _GfnEnqueueScatter2D(void *** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{
	// TODO: level 2 transfer only used partition
	// TODO: clCreateSubBuffer 1.1 spec for point to subbuffer

#define SWITCH_SCATTER_2D(type,mpi_type,size1,size2) \
do { \
	type ** tmp_ptr = (type **) (*ptr); \
	if (_gfn_rank == 0) _RecvInputMsg(tmp_ptr[0], sizeof(type) * size1 * size2); \
	MPI_Scatterv(tmp_ptr[0], cnts, disp, mpi_type, tmp_ptr[0] + recv_elem_offset, sub_size, mpi_type, 0, MPI_COMM_WORLD); \
	if (level2_transfer) { \
		cl_buffer_region info; \
		info.origin = (size_t)(recv_elem_offset * sizeof(type)); \
		info.size = (size_t)(sub_size * sizeof(type)); \
		cl_mem subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
		_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr[0] + recv_elem_offset, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
		_gfn_status = clReleaseMemObject(subbuf); \
		_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
	} \
} while (0)

#if 0
#define SWITCH_SCATTER_2D_2(type,mpi_type,size1,size2) \
do { \
	type ** tmp_ptr = (type **) (*ptr); \
	if (_gfn_rank == 0) _RecvInputMsg(tmp_ptr[0], sizeof(type) * size1 * size2); \
	row_offset = 0; \
	for (r = 0; r < size1; ++r) { \
		MPI_Scatterv(tmp_ptr[0]+row_offset, cnts, disp, mpi_type, tmp_ptr[0]+row_offset+recv_elem_offset, sub_size, mpi_type, 0, MPI_COMM_WORLD); \
		if (level2_transfer) { \
			cl_buffer_region info;
			info.origin = (size_t)0; // in bytes
			info.size = (size_t)8;//in bytes 
			
			cl_mem subbuf = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status);
			_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr+row_offset, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr[0]+row_offset, 0, 0, 0); \
			_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
		} \
		row_offset += size2; \
	} \
} while (0)
#endif

	int cnts[_gfn_num_proc];
    int disp[_gfn_num_proc];
    int sub_size, recv_elem_offset;
    int r, row_offset;

    switch(partitioned_dim)
    {
    case 1: 
    	_CalcPartitionInfo(dim1_size, dim2_size, pattern_array, pattern_array_size, pattern_type, cnts, disp, &sub_size, &recv_elem_offset);
    	switch(type_id)
		{
		case TYPE_CHAR:           
			SWITCH_SCATTER_2D(char,MPI_CHAR,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED_CHAR:  
			SWITCH_SCATTER_2D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size,dim2_size); break;
		case TYPE_SHORT:          
			SWITCH_SCATTER_2D(short,MPI_SHORT,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED_SHORT: 
			SWITCH_SCATTER_2D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size,dim2_size); break;
		case TYPE_INT:            
			SWITCH_SCATTER_2D(int,MPI_INT,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED:       
			SWITCH_SCATTER_2D(unsigned,MPI_UNSIGNED,dim1_size,dim2_size); break;
		case TYPE_LONG:           
			SWITCH_SCATTER_2D(long,MPI_LONG,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED_LONG:  
			SWITCH_SCATTER_2D(unsigned long,MPI_UNSIGNED_LONG,dim1_size,dim2_size); break;
		case TYPE_FLOAT:
			SWITCH_SCATTER_2D(float,MPI_FLOAT,dim1_size,dim2_size); break;          
		case TYPE_DOUBLE:         
			SWITCH_SCATTER_2D(double,MPI_DOUBLE,dim1_size,dim2_size); break;
		case TYPE_LONG_DOUBLE:    
			SWITCH_SCATTER_2D(long double,MPI_LONG_DOUBLE,dim1_size,dim2_size); break;
		case TYPE_LONG_LONG_INT:  
			SWITCH_SCATTER_2D(long long int,MPI_LONG_LONG_INT,dim1_size,dim2_size); break;
		}
    	break;
    case 2:
#if 0
    	_CalcPartitionInfo(dim2_size, 1, pattern_array, pattern_array_size, pattern_type, cnts, disp, &sub_size, &recv_elem_offset);
    	switch(type_id)
		{
		case TYPE_CHAR:
		{
			char ** tmp_ptr = (char **) (*ptr);
	if (_gfn_rank == 0) _RecvInputMsg(tmp_ptr[0], sizeof(char) * dim1_size * dim2_size);
	row_offset = 0;
	for (r = 0; r < dim1_size; ++r) {
		MPI_Scatterv(tmp_ptr[0]+row_offset, cnts, disp, MPI_CHAR, tmp_ptr[0]+row_offset+recv_elem_offset, sub_size, MPI_CHAR, 0, MPI_COMM_WORLD);
		if (level2_transfer) {
			_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr+(cl_mem)row_offset, CL_TRUE, 0, sizeof(char) * sub_size, tmp_ptr[0]+row_offset, 0, 0, 0);
			_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER");
		}
		row_offset += dim2_size;
	}
		}
		break;
			//SWITCH_SCATTER_2D_2(char,MPI_CHAR,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED_CHAR:  
			SWITCH_SCATTER_2D_2(unsigned char,MPI_UNSIGNED_CHAR,dim1_size,dim2_size); break;
		case TYPE_SHORT:          
			SWITCH_SCATTER_2D_2(short,MPI_SHORT,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED_SHORT: 
			SWITCH_SCATTER_2D_2(unsigned short,MPI_UNSIGNED_SHORT,dim1_size,dim2_size); break;
		case TYPE_INT:            
			SWITCH_SCATTER_2D_2(int,MPI_INT,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED:       
			SWITCH_SCATTER_2D_2(unsigned,MPI_UNSIGNED,dim1_size,dim2_size); break;
		case TYPE_LONG:           
			SWITCH_SCATTER_2D_2(long,MPI_LONG,dim1_size,dim2_size); break;
		case TYPE_UNSIGNED_LONG:  
			SWITCH_SCATTER_2D_2(unsigned long,MPI_UNSIGNED_LONG,dim1_size,dim2_size); break;
		case TYPE_FLOAT:
			SWITCH_SCATTER_2D_2(float,MPI_FLOAT,dim1_size,dim2_size); break;          
		case TYPE_DOUBLE:         
			SWITCH_SCATTER_2D_2(double,MPI_DOUBLE,dim1_size,dim2_size); break;
		case TYPE_LONG_DOUBLE:    
			SWITCH_SCATTER_2D_2(long double,MPI_LONG_DOUBLE,dim1_size,dim2_size); break;
		case TYPE_LONG_LONG_INT:  
			SWITCH_SCATTER_2D_2(long long int,MPI_LONG_LONG_INT,dim1_size,dim2_size); break;
		}
#endif
    	break;
    }

	return 0;
}

int _GfnEnqueueScatter3D(void **** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueScatter4D(void ***** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueScatter5D(void ****** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueScatter6D(void ******* ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnFinishDistributeArray()
{ return 0; }

int _GfnEnqueueGather1D(void ** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{
	int cnts[_gfn_num_proc];
    int disp[_gfn_num_proc];
    int sub_size, send_elem_offset;

    _CalcPartitionInfo(dim1_size, 1, pattern_array, pattern_array_size, pattern_type, cnts, disp, &sub_size, &send_elem_offset);

#define SWITCH_GATHER_1D(type,mpi_type,size1) \
do { \
	type * tmp_ptr = (type *) (*ptr); \
	if (level2_transfer) { \
		cl_buffer_region info; \
		info.origin = (size_t)(send_elem_offset * sizeof(type)); \
		info.size = (size_t)(sub_size * sizeof(type)); \
		cl_mem subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
		_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
		_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + send_elem_offset, 0, 0, 0); \
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER"); \
        _gfn_status = clReleaseMemObject(subbuf); \
		_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
	} \
	MPI_Gatherv(tmp_ptr + send_elem_offset, sub_size, mpi_type, tmp_ptr, cnts, disp, mpi_type, 0, MPI_COMM_WORLD); \
	if (_gfn_rank == 0) _SendOutputMsg(tmp_ptr, sizeof(type) * size1); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           
		SWITCH_GATHER_1D(char,MPI_CHAR,dim1_size); break;
	case TYPE_UNSIGNED_CHAR:  
		SWITCH_GATHER_1D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size); break;
	case TYPE_SHORT:          
		SWITCH_GATHER_1D(short,MPI_SHORT,dim1_size); break;
	case TYPE_UNSIGNED_SHORT: 
		SWITCH_GATHER_1D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size); break;
	case TYPE_INT:            
		SWITCH_GATHER_1D(int,MPI_INT,dim1_size); break;
	case TYPE_UNSIGNED:       
		SWITCH_GATHER_1D(unsigned,MPI_UNSIGNED,dim1_size); break;
	case TYPE_LONG:           
		SWITCH_GATHER_1D(long,MPI_LONG,dim1_size); break;
	case TYPE_UNSIGNED_LONG:  
		SWITCH_GATHER_1D(unsigned long,MPI_UNSIGNED_LONG,dim1_size); break;
	case TYPE_FLOAT:          
		SWITCH_GATHER_1D(float,MPI_FLOAT,dim1_size); break;
	case TYPE_DOUBLE:         
		SWITCH_GATHER_1D(double,MPI_DOUBLE,dim1_size); break;
	case TYPE_LONG_DOUBLE:    
		SWITCH_GATHER_1D(long double,MPI_LONG_DOUBLE,dim1_size); break;
	case TYPE_LONG_LONG_INT:  
		SWITCH_GATHER_1D(long long int,MPI_LONG_LONG_INT,dim1_size); break;
	}

	return 0;
}

int _GfnEnqueueGather2D(void *** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{
	int cnts[_gfn_num_proc];
    int disp[_gfn_num_proc];
    int sub_size, send_elem_offset;

    switch(partitioned_dim)
    {
    case 1: 
    	_CalcPartitionInfo(dim1_size, dim2_size, pattern_array, pattern_array_size, pattern_type, cnts, disp, &sub_size, &send_elem_offset);
    	break;
    case 2: 
    	// TODO:
    	break;
    }

#define SWITCH_GATHER_2D(type,mpi_type,size1,size2) \
do { \
	type ** tmp_ptr = (type **) (*ptr); \
	if (level2_transfer) { \
		cl_buffer_region info; \
		info.origin = (size_t)(send_elem_offset * sizeof(type)); \
		info.size = (size_t)(sub_size * sizeof(type)); \
		cl_mem subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
		_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
		_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr[0] + send_elem_offset, 0, 0, 0); \
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER"); \
        _gfn_status = clReleaseMemObject(subbuf); \
		_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
	} \
	MPI_Gatherv(tmp_ptr[0] + send_elem_offset, sub_size, mpi_type, tmp_ptr[0], cnts, disp, mpi_type, 0, MPI_COMM_WORLD); \
	if (_gfn_rank == 0) _SendOutputMsg(tmp_ptr[0], sizeof(type) * size1 * size2); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           
		SWITCH_GATHER_2D(char,MPI_CHAR,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_CHAR:  
		SWITCH_GATHER_2D(unsigned char,MPI_UNSIGNED_CHAR,dim1_size,dim2_size); break;
	case TYPE_SHORT:          
		SWITCH_GATHER_2D(short,MPI_SHORT,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_SHORT: 
		SWITCH_GATHER_2D(unsigned short,MPI_UNSIGNED_SHORT,dim1_size,dim2_size); break;
	case TYPE_INT:            
		SWITCH_GATHER_2D(int,MPI_INT,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED:       
		SWITCH_GATHER_2D(unsigned,MPI_UNSIGNED,dim1_size,dim2_size); break;
	case TYPE_LONG:           
		SWITCH_GATHER_2D(long,MPI_LONG,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_LONG:  
		SWITCH_GATHER_2D(unsigned long,MPI_UNSIGNED_LONG,dim1_size,dim2_size); break;
	case TYPE_FLOAT:          
		SWITCH_GATHER_2D(float,MPI_FLOAT,dim1_size,dim2_size); break;
	case TYPE_DOUBLE:         
		SWITCH_GATHER_2D(double,MPI_DOUBLE,dim1_size,dim2_size); break;
	case TYPE_LONG_DOUBLE:    
		SWITCH_GATHER_2D(long double,MPI_LONG_DOUBLE,dim1_size,dim2_size); break;
	case TYPE_LONG_LONG_INT:  
		SWITCH_GATHER_2D(long long int,MPI_LONG_LONG_INT,dim1_size,dim2_size); break;
	}

	return 0;
}

int _GfnEnqueueGather3D(void **** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueGather4D(void ***** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueGather5D(void ****** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnEnqueueGather6D(void ******* ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int * pattern_array, int pattern_array_size, int pattern_type, int level1_transfer, int level2_transfer)
{ return 0; }
int _GfnFinishGatherArray()
{ return 0; }


// Higher Level function
void _GfnBarrier()
{
	MPI_Barrier(MPI_COMM_WORLD);
}

void _GfnCheckCLStatus(cl_int status, const char *phase_name)
{
	if (status != CL_SUCCESS)
	{
		fprintf(stderr, "**[OpenCL] Rank #%d Runtime error in %s phase**\n", 
			_gfn_rank, phase_name);

		switch(status) {
		// program is not a valid program object
		case CL_INVALID_PROGRAM:
		    fprintf(stderr, "Error : CL_INVALID_PROGRAM\n"); break;
		// there is a failure to build the program executable
		case CL_INVALID_ARG_INDEX:
			fprintf(stderr, "Error : CL_INVALID_ARG_INDEX\n"); break;
		case CL_INVALID_ARG_VALUE:
			fprintf(stderr, "Error : CL_INVALID_ARG_VALUE\n"); break;
		case CL_INVALID_SAMPLER:
			fprintf(stderr, "Error : CL_INVALID_SAMPLER\n"); break;
		case CL_INVALID_ARG_SIZE:
			fprintf(stderr, "Error : CL_INVALID_ARG_SIZE\n"); break;
		case CL_BUILD_PROGRAM_FAILURE:
		    fprintf(stderr, "Error : CL_BUILD_PROGRAM_FAILURE\n"); break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			fprintf(stderr, "Error : CL_INVALID_PROGRAM_EXECUTABLE\n"); break;
		case CL_INVALID_BINARY:
		    fprintf(stderr, "Error : CL_INVALID_BINARY\n"); break;
		case CL_INVALID_BUILD_OPTIONS:
		    fprintf(stderr, "Error : CL_INVALID_BUILD_OPTIONS\n"); break;
		case CL_INVALID_COMMAND_QUEUE:
			fprintf(stderr, "Error : CL_INVALID_COMMAND_QUEUE\n"); break;
		case CL_INVALID_KERNEL:
			fprintf(stderr, "Error : CL_INVALID_KERNEL\n"); break;
		case CL_INVALID_KERNEL_NAME:
		    fprintf(stderr, "Error : CL_INVALID_KERNEL_NAME\n"); break;
		case CL_INVALID_KERNEL_DEFINITION:
		    fprintf(stderr, "Error : CL_INVALID_KERNEL_DEFINITION\n"); break;
		case CL_INVALID_CONTEXT:
			fprintf(stderr, "Error : CL_INVALID_CONTEXT\n"); break;
		case CL_INVALID_KERNEL_ARGS:
			fprintf(stderr, "Error : CL_INVALID_KERNEL_ARGS\n"); break;
		case CL_INVALID_WORK_DIMENSION:
			fprintf(stderr, "Error : CL_INVALID_WORK_DIMENSION\n"); break;
		case CL_INVALID_WORK_GROUP_SIZE:
			fprintf(stderr, "Error : CL_INVALID_WORK_GROUP_SIZE\n"); break;
		case CL_INVALID_WORK_ITEM_SIZE:
			fprintf(stderr, "Error : CL_INVALID_WORK_ITEM_SIZE\n"); break;
		case CL_INVALID_GLOBAL_OFFSET:
			fprintf(stderr, "Error : CL_INVALID_GLOBAL_OFFSET\n"); break;
		case CL_INVALID_MEM_OBJECT:
			fprintf(stderr, "Error : CL_INVALID_MEM_OBJECT\n"); break;
		case CL_INVALID_VALUE:
			fprintf(stderr, "Error : CL_INVALID_VALUE\n"); break;
		case CL_INVALID_DEVICE:
		    fprintf(stderr, "Error : CL_INVALID_DEVICE\n"); break;
		case CL_INVALID_OPERATION:
		    fprintf(stderr, "Error : CL_INVALID_OPERATION\n"); break;
		case CL_OUT_OF_RESOURCES:
			fprintf(stderr, "Error : CL_OUT_OF_RESOURCES\n"); break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			fprintf(stderr, "Error : CL_MEM_OBJECT_ALLOCATION_FAILURE\n"); break;
		case CL_INVALID_EVENT_WAIT_LIST:
			fprintf(stderr, "Error : CL_INVALID_EVENT_WAIT_LIST\n"); break;
		case CL_OUT_OF_HOST_MEMORY:
			fprintf(stderr, "Error : CL_OUT_OF_HOST_MEMORY\n"); break;
		case CL_COMPILER_NOT_AVAILABLE:
		    fprintf(stderr, "Error : CL_COMPILER_NOT_AVAILABLE\n"); break;
		default:
		    fprintf(stderr, "Error : CL unknown error value = %d\n", status); break;
		}
	}
}

size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num)
{
	size_t div_group_num = ((work_item_num + work_group_item_num - 1) / work_group_item_num);
	return (work_group_item_num * div_group_num);
}


int _CalcLoopSize(int start, int end, int incre)
{
	return ceil((float)(end - start + 1) / (float)incre);
}

int _CalcSubSize(int size, int num_proc, int rank, int block_size)
{
	int result = 0;
	if (rank < 0) return 0; // for start index of rank 0
	result = (size/num_proc) + (int)(rank < (size%num_proc));
	result *= block_size;
	return result;
}

int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank)
{
	int range_size = old_end - old_start + 1;
	return old_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank);
}

int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank)
{
	int range_size = old_end - old_start + 1;
	return old_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank + 1);
}

int _CalcOffset(int size, int num_proc, int rank)
{
	int offset = (size/num_proc)*(rank);
	if (rank <= size%num_proc)
		offset += rank;
	else
		offset += size%num_proc;
	return offset;
}

void _CalcCnts(int size, int num_proc, int *cnts /* OUTS */, int block_size)
{
	int i;
	for (i = 0; i < num_proc; ++i) {
		cnts[i] = _CalcSubSize(size, num_proc, i, 1) * block_size;
#ifdef API_DEBUG
		printf("Rank #%d , count[%d] : %d\n", _gfn_rank, i, cnts[i]);
#endif // API_DEBUG
	}
}

void _CalcDisp(int size, int num_proc, int *disp /* OUTS */, int block_size)
{
	int i;
	for (i = 0; i < num_proc; ++i) {
		disp[i] = _CalcOffset(size, num_proc, i) * block_size;
#ifdef API_DEBUG
		printf("Rank #%d , disp[%d] : %d\n", _gfn_rank, i, disp[i]);
#endif // API_DEBUG
	}
}

size_t _CalcTypeSize(int type_id)
{
	int tid = (int) type_id;

	switch(tid)
	{
		case TYPE_CHAR:           return sizeof(char);
		case TYPE_UNSIGNED_CHAR:  return sizeof(unsigned char);
		case TYPE_SHORT:          return sizeof(short);
		case TYPE_UNSIGNED_SHORT: return sizeof(unsigned short);
		case TYPE_INT:            return sizeof(int);
		case TYPE_UNSIGNED:       return sizeof(unsigned);
		case TYPE_LONG:           return sizeof(long);
		case TYPE_UNSIGNED_LONG:  return sizeof(unsigned long);
		case TYPE_FLOAT:          return sizeof(float);
		case TYPE_DOUBLE:         return sizeof(double);
		case TYPE_LONG_DOUBLE:    return sizeof(long double);
		case TYPE_LONG_LONG_INT:  return sizeof(long long int);
	}
}

void _CalcPartitionInfo(int size, int block_size, int * pattern_array, int pattern_array_size, int pattern_type,
	                    int *cnts /* OUTS */, int *disp /* OUTS */, 
	                    int *sub_size /* OUTS */, int *elem_offset /* OUTS */)
{
	int i;
	int lower_bound;
    int upper_bound;

	_CalcCnts(size, _gfn_num_proc, cnts, block_size);
    _CalcDisp(size, _gfn_num_proc, disp, block_size);
    switch(pattern_type)
    {
    case PATTERN_NONE: break;
    case PATTERN_RANGE:
    	lower_bound = pattern_array[0];
    	upper_bound = pattern_array[1];
    	for (i = 0; i < _gfn_num_proc; ++i) {
    		if (i != 0) {
    			cnts[i] += (lower_bound * block_size);
    			disp[i] -= (lower_bound * block_size);
    		}
    		if (i != _gfn_num_proc-1) {
    			cnts[i] += (lower_bound * block_size);
    		}
    	}
    	break;
    case PATTERN_SPECIFIC: /* TODO */ break;
    }
    *sub_size = cnts[_gfn_rank];
    *elem_offset = disp[_gfn_rank];
}


int _GFN_TYPE_CHAR()           { return TYPE_CHAR; }
int _GFN_TYPE_UNSIGNED_CHAR()  { return TYPE_UNSIGNED_CHAR; }
int _GFN_TYPE_SHORT()          { return TYPE_SHORT; }
int _GFN_TYPE_UNSIGNED_SHORT() { return TYPE_UNSIGNED_SHORT; }
int _GFN_TYPE_INT()            { return TYPE_INT; }
int _GFN_TYPE_UNSIGNED()       { return TYPE_UNSIGNED; }
int _GFN_TYPE_LONG()           { return TYPE_LONG; }
int _GFN_TYPE_UNSIGNED_LONG()  { return TYPE_UNSIGNED_LONG; }
int _GFN_TYPE_FLOAT()          { return TYPE_FLOAT; }
int _GFN_TYPE_DOUBLE()         { return TYPE_DOUBLE; }
int _GFN_TYPE_LONG_DOUBLE()    { return TYPE_LONG_DOUBLE; }
int _GFN_TYPE_LONG_LONG_INT()  { return TYPE_LONG_LONG_INT; }

MPI_Op _GFN_OP_MAX()              { return MPI_MAX; }
MPI_Op _GFN_OP_MIN()              { return MPI_MIN; }
MPI_Op _GFN_OP_SUM()              { return MPI_SUM; }
MPI_Op _GFN_OP_PROD()             { return MPI_PROD; }
MPI_Op _GFN_OP_LAND()             { return MPI_LAND; }
MPI_Op _GFN_OP_BAND()             { return MPI_BAND; }
MPI_Op _GFN_OP_LOR()              { return MPI_LOR; }
MPI_Op _GFN_OP_BOR()              { return MPI_BOR; }
MPI_Op _GFN_OP_LXOR()             { return MPI_LXOR; }
MPI_Op _GFN_OP_BXOR()             { return MPI_BXOR; }
MPI_Op _GFN_OP_MINLOC()           { return MPI_MINLOC; }
MPI_Op _GFN_OP_MAXLOC()           { return MPI_MAXLOC; }

cl_bool _GFN_TRUE()                     { return CL_TRUE; }
cl_bool _GFN_FALSE()                    { return CL_FALSE; }

cl_mem_flags _GFN_MEM_READ_WRITE()      { return CL_MEM_READ_WRITE; }
cl_mem_flags _GFN_MEM_WRITE_ONLY()      { return CL_MEM_WRITE_ONLY; }
cl_mem_flags _GFN_MEM_READ_ONLY()       { return CL_MEM_READ_ONLY; }
cl_mem_flags _GFN_MEM_USE_HOST_PTR()    { return CL_MEM_USE_HOST_PTR; }
cl_mem_flags _GFN_MEM_ALLOC_HOST_PTR()  { return CL_MEM_ALLOC_HOST_PTR; }


// Function for OpenCL
void _InitOpenCL()
{
	cl_uint /*num_platforms, */num_devices;
	
	_gfn_status = clGetPlatformIDs(1, &_gfn_platform_id, NULL/*&num_platforms*/);
	_GfnCheckCLStatus(_gfn_status, "clGetPlatformIDs");

	_gfn_status = clGetDeviceIDs(_gfn_platform_id, CL_DEVICE_TYPE_DEFAULT/*CL_DEVICE_TYPE_GPU*/, 1,
		&_gfn_device_id, &num_devices);
	_GfnCheckCLStatus(_gfn_status, "clGetDeviceIDs");

	_gfn_context = clCreateContext(NULL, 1, &_gfn_device_id,
		NULL, NULL, &_gfn_status);
	_GfnCheckCLStatus(_gfn_status, "clCreateContext");

	_gfn_cmd_queue = clCreateCommandQueue(_gfn_context, _gfn_device_id, 0, &_gfn_status);
	_GfnCheckCLStatus(_gfn_status, "clCreateCommandQueue");
}

cl_platform_id _gfn_platform_id;
cl_device_id _gfn_device_id;
cl_context _gfn_context;
cl_command_queue _gfn_cmd_queue;
cl_int _gfn_status;

void _FinalOpenCL()
{
	// TODO: move to before send data back
	// Block until all commands in queue have completed
	//_gfn_status = clFlush(_gfn_cmd_queue);
	//_GfnCheckCLStatus(_gfn_status, "clFlush");

	// Block until all commands in queue have been removed from the queue
    //_gfn_status = clFinish(_gfn_cmd_queue);
    //_GfnCheckCLStatus(_gfn_status, "clFinish");

	_gfn_status = clReleaseContext(_gfn_context);
	_GfnCheckCLStatus(_gfn_status, "clReleaseContext");
}

cl_kernel _GfnCreateKernel(const char *name, const char *src, 
                           cl_context context, cl_device_id device_id)
{
	// TODO: save kernel, so dont need to recompile
	cl_int status;
	cl_kernel kernel = NULL;
	size_t param_value_size = 1024 * 1024;
    size_t param_value_size_ret;
    char param_value[param_value_size];

    _gfn_cl_program = clCreateProgramWithSource(context, 1, 
            (const char **)&src, NULL, &status);
    _GfnCheckCLStatus(status, "CREATE PROGRAM WITH SOURCE");
    status = clBuildProgram(_gfn_cl_program, 1, &device_id, NULL, NULL, NULL);
    _GfnCheckCLStatus(status, "BUILD PROGRAM");

    /* Get kernel compiler messages */
    status = clGetProgramBuildInfo(_gfn_cl_program,
                                   device_id,
                                   CL_PROGRAM_BUILD_LOG,
                                   param_value_size,
                                   param_value,
                                   &param_value_size_ret);

    // TODO: why length 2 for success build
    if (param_value_size_ret != 2) {
        printf("Message from kernel compiler : \n%s\n", param_value);
    }

    kernel = clCreateKernel(_gfn_cl_program, name, &status);
    _GfnCheckCLStatus(status, "CREATE KERNEL");
    
    return kernel;
}

void _GfnClearKernel(cl_kernel kernel)
{
	_gfn_status = clReleaseKernel(kernel);
	_GfnCheckCLStatus(_gfn_status, "clReleaseKernel");

	_gfn_status = clReleaseProgram(_gfn_cl_program);
	_GfnCheckCLStatus(_gfn_status, "clReleaseProgram");
}
