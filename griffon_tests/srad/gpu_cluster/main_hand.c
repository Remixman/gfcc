//====================================================================================================100
//		UPDATE
//====================================================================================================100

//    2006.03   Rob Janiczek
//        --creation of prototype version
//    2006.03   Drew Gilliam
//        --rewriting of prototype version into current version
//        --got rid of multiple function calls, all code in a  
//         single function (for speed)
//        --code cleanup & commenting
//        --code optimization efforts   
//    2006.04   Drew Gilliam
//        --added diffusion coefficent saturation on [0,1]
//		2009.12 Lukasz G. Szafaryn
//		-- reading from image, command line inputs
//		2010.01 Lukasz G. Szafaryn
//		--comments

//====================================================================================================100
//	DEFINE / INCLUDE
//====================================================================================================100

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <CL/cl.h>

#include "define.c"
#include "graphics.c"
#include "resize.c"
#include "timer.c"

void _GfnCheckCLStatus(cl_int status, const char *phase_name)
{
	if (status != CL_SUCCESS)
	{
		fprintf(stderr, "**[OpenCL] Runtime error in %s phase**\n", phase_name);

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

int round_to(int x, int r) {
	int q = ceil(x/(float)r);
	return q * r;
}

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void exp_process(__global double *image,						\n"
"							   int Nr,									\n"
"							   int Nc,									\n"
"							   int start,								\n"
"							   int end) 								\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j;															\n"
"	if (tid < end) {													\n"
"		i = tid / Nc;													\n"
"		j = tid % Nc;													\n"
"		image[i*Nc+j] = exp(image[i*Nc+j]/255);							\n"
"	}																	\n"
"}																		\n"
"																		\n"
"__kernel void reduce_process(__global double *image,					\n"
"							__global double *global_sum,				\n"
"							__local double *shared_sum,					\n"
"							__global double *global_sum2,				\n"
"							__local double *shared_sum2,				\n"
"							   int Nr,									\n"
"							   int Nc,									\n"
"							   int start,								\n"
"							   int end) 								\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j;															\n"
"	double sum = 0.0, sum2 = 0.0, tmp;									\n"
"	if (tid < end) {													\n"
"		i = tid / Nc;													\n"
"		j = tid % Nc;													\n"
"		tmp   = image[i*Nc+j];											\n"
"		sum  += tmp ;													\n"
"		sum2 += tmp*tmp;												\n"
"	}																	\n"
"	shared_sum[get_local_id(0)] = sum;									\n"
"	shared_sum2[get_local_id(0)] = sum2;								\n"
"	for (int stride = get_local_size(0)/2; stride > 0; stride/=2) {		\n"
"		barrier(CLK_LOCAL_MEM_FENCE);									\n"
"		if (get_local_id(0) < stride) {									\n"
"			shared_sum[get_local_id(0)] += shared_sum[get_local_id(0)+stride];	\n"
"			shared_sum2[get_local_id(0)] += shared_sum2[get_local_id(0)+stride];\n"
"		}																\n"
"	}																	\n"
"	barrier(CLK_LOCAL_MEM_FENCE);										\n"
"	if (get_local_id(0) == 0) {											\n"
"		global_sum[get_group_id(0)] = shared_sum[0];					\n"
"		global_sum2[get_group_id(0)] = shared_sum2[0];					\n"
"	}																	\n"
"}																		\n"
"																		\n"
"__kernel void c_process(__global double *image,						\n"
"                        __global double *c,							\n"
"                        __global double *dN,							\n"
"                        __global double *dS,							\n"
"                        __global double *dW,							\n"
"                        __global double *dE,							\n"
"							double q0sqr,								\n"
"							int Nr,										\n"
"							int Nc,										\n"
"							int start,									\n"
"							int end) 									\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	double tdN, tdS, tdW, tdE, cij, G2, Jc, L;							\n"
"	double num, den, qsqr;												\n"
"	int i, j, iN, iS, jW, jE;											\n"
"	if (tid < end) {													\n"
"		i = tid / Nc;													\n"
"		j = tid % Nc;													\n"
"		Jc = image[i*Nc+j];	  											\n"
"																		\n"                
"		iN = (i==0)? 0 : i-1;											\n"
"		iS = (i==Nr-1)? Nr-1 : i+1;										\n"
"		jW = (j==0)? 0 : j-1;											\n"
"		jE = (j==Nc-1)? Nc-1 : j+1;										\n"
"																		\n"
"		tdN = image[iN*Nc+j] - Jc;										\n"
"		tdS = image[iS*Nc+j] - Jc;										\n"
"		tdW = image[i*Nc+jW] - Jc;										\n"
"		tdE = image[i*Nc+jE] - Jc;										\n"
"																		\n"
"		G2 = (tdN*tdN + tdS*tdS	+ tdW*tdW + tdE*tdE) / (Jc*Jc);			\n"
"		L = (tdN + tdS + tdW + tdE) / Jc;								\n"
"																		\n"
"		dN[i*Nc+j] = tdN;												\n"
"		dS[i*Nc+j] = tdS;												\n"
"		dW[i*Nc+j] = tdW;												\n"
"		dE[i*Nc+j] = tdE;												\n"
"																		\n"
"		num  = (0.5*G2) - ((1.0/16.0)*(L*L));							\n"
"		den  = 1 + (.25*L);												\n"
"		qsqr = num/(den*den);											\n"
"																		\n"
"		den = (qsqr-q0sqr) / (q0sqr * (1+q0sqr)) ;						\n"
"		cij = 1.0 / (1.0+den) ;											\n"
"																		\n"
"		if (cij < 0) {c[i*Nc+j] = 0;}										\n"
"		else if (cij > 1) {c[i*Nc+j] = 1;}								\n"
"		else {c[i*Nc+j] = cij;}											\n"
"	}																	\n"
"}																		\n"
"																		\n"
"__kernel void image_process(__global double *image,					\n"
"                      		 __global double *c,						\n"
"                    	     __global double *dN,						\n"
"                       	 __global double *dS,						\n"
"                     	     __global double *dW,						\n"
"                        	 __global double *dE,						\n"
"							   int Nr,									\n"
"							   int Nc,									\n"
"							   double lambda,							\n"
"							   int start,								\n"
"							   int end) 								\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j, iS, jE;													\n"
"	double D;															\n"
"	if (tid < end) {													\n"
"		i = tid / Nc;													\n"
"		j = tid % Nc;													\n"
"		iS = (i==Nr-1)? Nr-1 : i+1;										\n"
"		jE = (j==Nc-1)? Nc-1 : j+1;										\n"
"		D = c[i*Nc+j]*dN[i*Nc+j] + c[iS*Nc+j]*dS[i*Nc+j] + c[i*Nc+j]*dW[i*Nc+j] + c[i*Nc+jE]*dE[i*Nc+j]; \n"
"		image[i*Nc+j] = image[i*Nc+j] + 0.25*lambda*D;					\n"
"	}																	\n"
"}																		\n"
"																		\n"
"__kernel void log_process(__global double *image,						\n"
"							   int Nr,									\n"
"							   int Nc,									\n"
"							   int start,								\n"
"							   int end) 								\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j;															\n"
"	if (tid < end) {													\n"
"		i = tid / Nc;													\n"
"		j = tid % Nc;													\n"
"		image[i*Nc+j] = log(image[i*Nc+j])*255;							\n"
"	}																	\n"
"}																		\n"
;

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

//====================================================================================================100
//====================================================================================================100
//	MAIN FUNCTION
//====================================================================================================100
//====================================================================================================100

int main(int argc, char *argv []){

	int rank, node_size;
	
	// initial cluster
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &node_size);

	cl_int status = CL_SUCCESS;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	cl_program program;
	cl_kernel kernel1, kernel2, exp_kernel, log_kernel, reduce_kernel;
	
	cl_mem cl_image, cl_c;
	cl_mem cl_dN, cl_dS, cl_dW, cl_dE;
	cl_mem cl_sub_img, cl_sub_c;
	cl_mem cl_sub_dN, cl_sub_dS;
	cl_mem cl_sub_dW, cl_sub_dE;
	
	cl_mem cl_h2d_lower, cl_h2d_c_lower;
	cl_mem cl_h2d_upper, cl_h2d_c_upper;
	cl_mem cl_d2h_lower, cl_d2h_c_lower;
	cl_mem cl_d2h_upper, cl_d2h_c_upper;
	
	double global_sum = 0;
	double global_sum2 = 0;
	double *host_sum_buffer, *host_sum2_buffer;
	cl_mem cl_sum_buffer;
	cl_mem cl_sum2_buffer;
	
	int local_start, local_end;
	int disp[node_size];
	int cnts[node_size];
	
	MPI_Status mstatus;
	MPI_Request send_lower_req;
	MPI_Request send_upper_req;
	MPI_Request recv_lower_req;
	MPI_Request recv_upper_req;

	//================================================================================80
	// 	VARIABLES
	//================================================================================80

	// time
	long long time0;
	long long time1;

    // inputs image, input paramenters
    fp* image_ori;																// originalinput image
	int image_ori_rows;
	int image_ori_cols;
	long image_ori_elem;

    // inputs image, input paramenters
    fp** image;															// input image
    long Nr,Nc;													// IMAGE nbr of rows/cols/elements
	long Ne;

	// algorithm parameters
    int niter;																// nbr of iterations
    fp lambda;															// update step size

    // size of IMAGE
	int r1,r2,c1,c2;												// row/col coordinates of uniform ROI
	long NeROI;														// ROI nbr of elements
    
    // ROI statistics
    fp meanROI, varROI, q0sqr;											//local region statistics

    // center pixel value
    fp Jc;

	// directional derivatives
	fp **dN,**dS,**dW,**dE;
    
    // calculation variables
    fp tmp,sum,sum2;
    fp G2,L,num,den,qsqr,D;
       
    // diffusion coefficient
    fp **c; 
	fp cN,cS,cW,cE;
    
    // counters
    int iter;   // primary loop
    long i,j;    // image row/col
    long k;      // image single index    

	// number of threads
	char *filename;
	char fileinname[100];
	char fileoutname[100];

	int iN, iS, jW, jE;
	
	//================================================================================80
	// 	GET INPUT PARAMETERS
	//================================================================================80

	if (rank == 0) {
		if(argc != 6){
			printf("ERROR: wrong number of arguments\n");
			return 0;
		}
		else{
			niter = atoi(argv[1]);
			lambda = atof(argv[2]);
			Nr = atoi(argv[3]);						// it is 502 in the original image
			Nc = atoi(argv[4]);						// it is 458 in the original image
			filename = argv[5];
			strcpy(fileinname,"../");
			strcat(fileinname, filename);
			strcpy(fileoutname, "../out_");
			strcat(fileoutname, filename);
		}
	}
	
	MPI_Bcast(&niter, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&lambda, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Nr, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Nc, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//================================================================================80
	// 	READ IMAGE (SIZE OF IMAGE HAS TO BE KNOWN)
	//================================================================================80

    // read image
	image_ori_rows = Nr;
	image_ori_cols = Nc;
	image_ori_elem = image_ori_rows * image_ori_cols;

	image_ori = (fp*)malloc(sizeof(fp) * image_ori_elem);

	if (rank == 0) {
		read_graphics(	fileinname,
								image_ori,
								image_ori_rows,
								image_ori_cols,
								1);
	}

	time0 = get_time();
	
	// initial GPU
	status = clGetPlatformIDs(1, &platform, NULL);
	_GfnCheckCLStatus(status, "clGetPlatformIDs");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	_GfnCheckCLStatus(status, "clGetDeviceIDs");	
	context = clCreateContext(0, 1, &device, NULL, NULL, &status);
	_GfnCheckCLStatus(status, "clCreateContext");
	queue = clCreateCommandQueue(context, device, /*0*/CL_QUEUE_PROFILING_ENABLE, &status);
	_GfnCheckCLStatus(status, "clCreateCommandQueue");
	
	// create kernel
	program = clCreateProgramWithSource(context, 1, &prog_src, NULL, &status);
	_GfnCheckCLStatus(status, "CREATE PROGRAM WITH SOURCE");
	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	_GfnCheckCLStatus(status, "BUILD PROGRAM");
	status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
        LOG_SIZE, param_value, &param_value_size_ret);
    if (param_value_size_ret != 2)
        printf("Message from kernel compiler : \n%s\n", param_value);
        
    exp_kernel = clCreateKernel(program, "exp_process", &status);
    _GfnCheckCLStatus(status, "CREATE EXP KERNEL");
	kernel1 = clCreateKernel(program, "c_process", &status);
	_GfnCheckCLStatus(status, "CREATE SRAD KERNEL 1");
	kernel2 = clCreateKernel(program, "image_process", &status);
	_GfnCheckCLStatus(status, "CREATE SRAD KERNEL 2");
	log_kernel = clCreateKernel(program, "log_process", &status);
	_GfnCheckCLStatus(status, "CREATE LOG KERNEL");
	reduce_kernel = clCreateKernel(program, "reduce_process", &status);
	_GfnCheckCLStatus(status, "CREATE REDUCE KERNEL");

	//================================================================================80
	// 	RESIZE IMAGE (ASSUMING COLUMN MAJOR STORAGE OF image_orig)
	//================================================================================80

	Ne = Nr*Nc;

	image = (fp**)malloc(sizeof(fp*) * Nr);
	image[0] = (fp*)malloc(sizeof(fp) * Ne);
	for (i = 1; i < Nr; ++i)
		image[i] = image[i-1] + Nc;

	resize(	image_ori,
				image_ori_rows,
				image_ori_cols,
				image[0],
				Nr,
				Nc,
				1);

	//================================================================================80
	// 	SETUP
	//================================================================================80

    r1     = 0;											// top row index of ROI
    r2     = Nr - 1;									// bottom row index of ROI
    c1     = 0;											// left column index of ROI
    c2     = Nc - 1;									// right column index of ROI

    // ROI image size    
    NeROI = (r2-r1+1)*(c2-c1+1);											// number of elements in ROI, ROI size
    
	// allocate variables for directional derivatives
	dN = (fp**)malloc(sizeof(fp*)*Nr) ;
    dN[0] = (fp*)malloc(sizeof(fp)*Ne);
    dS = (fp**)malloc(sizeof(fp*)*Nr) ;
    dS[0] = (fp*)malloc(sizeof(fp)*Ne);
    dW = (fp**)malloc(sizeof(fp*)*Nr) ;
    dW[0] = (fp*)malloc(sizeof(fp)*Ne);
    dE = (fp**)malloc(sizeof(fp*)*Nr) ;
    dE[0] = (fp*)malloc(sizeof(fp)*Ne);
    for (i = 1; i < Nr; ++i) {
    	dN[i] = dN[i-1] + Nc;
    	dS[i] = dS[i-1] + Nc;
    	dW[i] = dW[i-1] + Nc;
    	dE[i] = dE[i-1] + Nc;
    }

	// allocate variable for diffusion coefficient
    c  = (fp**)malloc(sizeof(fp*)*Nr) ;											// diffusion coefficient
    c[0] = (fp*)malloc(sizeof(fp)*Ne);
    for (i = 1; i < Nr; ++i)
    	c[i] = c[i-1] + Nc;

	// calculate counts and displacements
	for (i = 0; i < node_size; ++i)
		disp[i] = i * ceil(Nr/(double)node_size) * Nc;
	for (i = 0; i < node_size-1; ++i)
		cnts[i] = disp[i+1] - disp[i];
	cnts[node_size-1] = (Nr*Nc) - disp[node_size-1];

	// calculate local start and end
	local_start = rank * ceil(Nr/(double)node_size) * Nc;
	local_end = (rank+1) * ceil(Nr/(double)node_size) * Nc;
	if (local_end > Nr*Nc) local_end = Nr*Nc;
	
	const size_t work_group_size = 64;
	const size_t global_work_size = round_to(local_end - local_start + 1, 
	                                         work_group_size);	
	const size_t group_num = global_work_size/work_group_size;
	
	cl_image = clCreateBuffer(context, CL_MEM_READ_WRITE, Ne * sizeof(double), NULL, &status);
	cl_dN = clCreateBuffer(context, CL_MEM_READ_WRITE, Ne * sizeof(double), NULL, &status);
	cl_dS = clCreateBuffer(context, CL_MEM_READ_WRITE, Ne * sizeof(double), NULL, &status);
	cl_dW = clCreateBuffer(context, CL_MEM_READ_WRITE, Ne * sizeof(double), NULL, &status);
	cl_dE = clCreateBuffer(context, CL_MEM_READ_WRITE, Ne * sizeof(double), NULL, &status);
	cl_c = clCreateBuffer(context, CL_MEM_READ_WRITE, Ne * sizeof(double), NULL, &status);
	
	cl_sum_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, group_num * sizeof(double), NULL, &status);
	host_sum_buffer = (double*) malloc(group_num * sizeof(double));
	cl_sum2_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, group_num * sizeof(double), NULL, &status);
	host_sum2_buffer = (double*) malloc(group_num * sizeof(double));
	
	// scatter image
	MPI_Scatterv((void*)(image[0]), cnts, disp, MPI_DOUBLE,
		(void*)((image[0])+disp[rank]), cnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
	// create subbuffer
	cl_buffer_region sub_size_info;
	sub_size_info.origin = (size_t)(disp[rank] * sizeof(double));
	sub_size_info.size = (size_t)(cnts[rank] * sizeof(double));
	
	cl_sub_img = clCreateSubBuffer(cl_image, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_size_info, &status);
	cl_sub_dN = clCreateSubBuffer(cl_dN, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_size_info, &status);
	cl_sub_dS = clCreateSubBuffer(cl_dS, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_size_info, &status);
	cl_sub_dW = clCreateSubBuffer(cl_dW, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_size_info, &status);
	cl_sub_dE = clCreateSubBuffer(cl_dE, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_size_info, &status);
	cl_sub_c = clCreateSubBuffer(cl_c, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_size_info, &status);
		
	// send data to GPU
	status = clEnqueueWriteBuffer(queue, cl_sub_img, CL_FALSE, 0, cnts[rank] * sizeof(double),
		(image[0])+disp[rank], 0, NULL, NULL);

	// create download upper bound subbuffer
	cl_buffer_region d2h_upper_info;
	if (rank != 0) {
		d2h_upper_info.origin = (size_t)(disp[rank] * sizeof(double));
		d2h_upper_info.size = (size_t)(1 * Nc * sizeof(double));
		cl_d2h_upper = clCreateSubBuffer(cl_sub_img, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_upper_info, &status);
		cl_d2h_c_upper = clCreateSubBuffer(cl_sub_c, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_upper_info, &status);
	}

	// create download lower bound subbuffer
	cl_buffer_region d2h_lower_info;
	if (rank != (node_size-1)) {
		d2h_lower_info.origin = (size_t)((disp[rank+1]-(1 * Nc)) * sizeof(double));
		d2h_lower_info.size = (size_t)(1 * Nc * sizeof(double));
		cl_d2h_lower = clCreateSubBuffer(cl_sub_img, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_lower_info, &status);
		cl_d2h_c_lower = clCreateSubBuffer(cl_sub_c, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_lower_info, &status);
	}

	// create upload upper bound subbuffer
	cl_buffer_region h2d_upper_info;
	if (rank != 0) {
		h2d_upper_info.origin = (size_t)((disp[rank]-(1 * Nc)) * sizeof(double));
		h2d_upper_info.size = (size_t)(1 * Nc * sizeof(double));
		cl_h2d_upper = clCreateSubBuffer(cl_sub_img, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_upper_info, &status);
		cl_h2d_c_upper = clCreateSubBuffer(cl_sub_c, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_upper_info, &status);
	}

	// create upload lower bound subbuffer
	cl_buffer_region h2d_lower_info;
	if (rank != (node_size-1)) {
		h2d_lower_info.origin = (size_t)(disp[rank+1] * sizeof(double));
		h2d_lower_info.size = (size_t)(1 * Nc * sizeof(double));
		cl_h2d_lower = clCreateSubBuffer(cl_sub_img, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_lower_info, &status);
		cl_h2d_c_lower = clCreateSubBuffer(cl_sub_c, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_lower_info, &status);
	}

	// wait write buffer
	clFinish(queue);
	
	// set exp kernel arguments
	status = clSetKernelArg(exp_kernel, 0, sizeof(cl_mem), &cl_image);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(exp_kernel, 1, sizeof(int), (void*)&Nr);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(exp_kernel, 2, sizeof(int), (void*)&Nc);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(exp_kernel, 3, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(exp_kernel, 4, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	
	// set reduce kernel arguments
	status = clSetKernelArg(reduce_kernel, 0, sizeof(cl_mem), &cl_image);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(reduce_kernel, 1, sizeof(cl_mem), &cl_sum_buffer);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(reduce_kernel, 2, sizeof(double) * work_group_size, 0);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(reduce_kernel, 3, sizeof(cl_mem), &cl_sum2_buffer);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(reduce_kernel, 4, sizeof(double) * work_group_size, 0);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(reduce_kernel, 5, sizeof(int), (void*)&Nr);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");
	status = clSetKernelArg(reduce_kernel, 6, sizeof(int), (void*)&Nc);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 6");
	status = clSetKernelArg(reduce_kernel, 7, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 7");
	status = clSetKernelArg(reduce_kernel, 8, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 8");

	// set kernel 1 arguments
	status = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &cl_image);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel1, 1, sizeof(cl_mem), &cl_c);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel1, 2, sizeof(cl_mem), &cl_dN);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel1, 3, sizeof(cl_mem), &cl_dS);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel1, 4, sizeof(cl_mem), &cl_dW);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(kernel1, 5, sizeof(cl_mem), &cl_dE);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");
	status = clSetKernelArg(kernel1, 6, sizeof(double), (void*)&q0sqr);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 6");
	status = clSetKernelArg(kernel1, 7, sizeof(int), (void*)&Nr);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 7");
	status = clSetKernelArg(kernel1, 8, sizeof(int), (void*)&Nc);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 8");
	status = clSetKernelArg(kernel1, 9, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 9");
	status = clSetKernelArg(kernel1, 10, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 10");
	
	// set kernel 2 arguments
	status = clSetKernelArg(kernel2, 0, sizeof(cl_mem), &cl_image);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel2, 1, sizeof(cl_mem), &cl_c);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel2, 2, sizeof(cl_mem), &cl_dN);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel2, 3, sizeof(cl_mem), &cl_dS);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel2, 4, sizeof(cl_mem), &cl_dW);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(kernel2, 5, sizeof(cl_mem), &cl_dE);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");
	status = clSetKernelArg(kernel2, 6, sizeof(int), (void*)&Nr);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 6");
	status = clSetKernelArg(kernel2, 7, sizeof(int), (void*)&Nc);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 7");
	status = clSetKernelArg(kernel2, 8, sizeof(double), (void*)&lambda);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 8");
	status = clSetKernelArg(kernel2, 9, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 9");
	status = clSetKernelArg(kernel2, 10, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 10");
	
	// set log kernel arguments
	status = clSetKernelArg(log_kernel, 0, sizeof(cl_mem), &cl_image);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(log_kernel, 1, sizeof(int), (void*)&Nr);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(log_kernel, 2, sizeof(int), (void*)&Nc);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(log_kernel, 3, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(log_kernel, 4, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
{

	//================================================================================80
	// 	SCALE IMAGE DOWN FROM 0-255 TO 0-1 AND EXTRACT
	//================================================================================80

	status = clEnqueueNDRangeKernel(queue, exp_kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH EXP KERNEL");
	clFinish(queue);

	//================================================================================80
	// 	COMPUTATION
	//================================================================================80

	// printf("iterations: ");

    // primary loop
    for (iter=0; iter<niter; iter++){										// do for the number of iterations input parameter

		// printf("%d ", iter);
		// fflush(NULL);

        // ROI statistics for entire ROI (single number for ROI)
        status = clEnqueueNDRangeKernel(queue, reduce_kernel, 1, NULL, &global_work_size,
			&work_group_size, 0, NULL, NULL);
		_GfnCheckCLStatus(status, "LAUNCH REDUCE KERNEL");
		clFinish(queue);
		
		// copy back sum buffer
		clEnqueueReadBuffer(queue, cl_sum_buffer, CL_TRUE, 0, sizeof(double) * group_num, 
			host_sum_buffer, 0, NULL, NULL);
		clEnqueueReadBuffer(queue, cl_sum2_buffer, CL_TRUE, 0, sizeof(double) * group_num, 
			host_sum2_buffer, 0, NULL, NULL);
			
		// summation host_sum_buffer
		sum = sum2 = 0.0;
		for (i = 0; i < group_num; ++i) {
			sum += host_sum_buffer[i];
			sum2 += host_sum2_buffer[i];
		}
		
		MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&sum2, &global_sum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		sum = global_sum;
		sum2 = global_sum2;
		
        meanROI = sum / NeROI;												// gets mean (average) value of element in ROI
        varROI  = (sum2 / NeROI) - meanROI*meanROI;							// gets variance of ROI
        q0sqr   = varROI / (meanROI*meanROI);								// gets standard deviation of ROI

        // directional derivatives, ICOV, diffusion coefficent
        // download lower bound from GPU
		if (rank != (node_size-1)) {
			status = clEnqueueReadBuffer(queue, cl_d2h_lower, CL_TRUE, 0, 1 * Nc * sizeof(double), 
				(image[0])+(disp[rank+1]-(1 * Nc)), 0, NULL, NULL);
		}

		// download upper bound from GPU
		if (rank != 0) {
			status = clEnqueueReadBuffer(queue, cl_d2h_upper, CL_TRUE, 0, 1 * Nc * sizeof(double), 
				(image[0])+disp[rank], 0, NULL, NULL);
		}

		// exchange image bound
		if (rank != (node_size-1))
			MPI_Isend((void*)((image[0])+disp[rank+1]-(1 * Nc)), 1 * Nc, MPI_DOUBLE, rank+1, 
				0/*tag*/, MPI_COMM_WORLD, &send_lower_req);
		if (rank != 0)
			MPI_Isend((void*)((image[0])+disp[rank]), 1 * Nc, MPI_DOUBLE, rank-1, 
				1/*tag*/, MPI_COMM_WORLD, &send_upper_req);
		if (rank != (node_size-1))
			MPI_Irecv((void*)((image[0])+disp[rank+1]), 1 * Nc, MPI_DOUBLE, rank+1,
				1/*tag*/, MPI_COMM_WORLD, &recv_lower_req);
		if (rank != 0)
			MPI_Irecv((void*)((image[0])+disp[rank]-(1 * Nc)), 1 * Nc, MPI_DOUBLE, rank-1,
				0/*tag*/, MPI_COMM_WORLD, &recv_upper_req);

		if (rank != 0) MPI_Wait(&send_upper_req, &mstatus);
		if (rank != (node_size-1)) MPI_Wait(&send_lower_req, &mstatus);
		if (rank != (node_size-1)) MPI_Wait(&recv_lower_req, &mstatus);
		if (rank != 0) MPI_Wait(&recv_upper_req, &mstatus);

		// upload upper bound to GPU
		if (rank != 0) {
			status = clEnqueueWriteBuffer(queue, cl_h2d_upper, CL_TRUE, 0, 1 * Nc * sizeof(double),
				(image[0])+disp[rank]-(1 * Nc), 0, NULL, NULL);
		}

		// upload lower bound to GPU
		if (rank != (node_size-1)) {
			status = clEnqueueWriteBuffer(queue, cl_h2d_lower, CL_TRUE, 0, 1 * Nc * sizeof(double),
				(image[0])+disp[rank+1], 0, NULL, NULL);
		}
		
		status = clEnqueueNDRangeKernel(queue, kernel1, 1, NULL, &global_work_size,
			&work_group_size, 0, NULL, NULL);
		_GfnCheckCLStatus(status, "LAUNCH SRAD KERNEL 1");
		clFinish(queue);

        // divergence & image update
        // download lower bound from GPU
		if (rank != (node_size-1)) {
			status = clEnqueueReadBuffer(queue, cl_d2h_c_lower, CL_TRUE, 0, 1 * Nc * sizeof(double), 
				(c[0])+(disp[rank+1]-(1 * Nc)), 0, NULL, NULL);
		}

		// download upper bound from GPU
		if (rank != 0) {
			status = clEnqueueReadBuffer(queue, cl_d2h_c_upper, CL_TRUE, 0, 1 * Nc * sizeof(double), 
				(c[0])+disp[rank], 0, NULL, NULL);
		}

		// exchange image bound
		if (rank != (node_size-1))
			MPI_Isend((void*)((c[0])+disp[rank+1]-(1 * Nc)), 1 * Nc, MPI_DOUBLE, rank+1, 
				0/*tag*/, MPI_COMM_WORLD, &send_lower_req);
		if (rank != 0)
			MPI_Isend((void*)((c[0])+disp[rank]), 1 * Nc, MPI_DOUBLE, rank-1, 
				1/*tag*/, MPI_COMM_WORLD, &send_upper_req);
		if (rank != (node_size-1))
			MPI_Irecv((void*)((c[0])+disp[rank+1]), 1 * Nc, MPI_DOUBLE, rank+1,
				1/*tag*/, MPI_COMM_WORLD, &recv_lower_req);
		if (rank != 0)
			MPI_Irecv((void*)((c[0])+disp[rank]-(1 * Nc)), 1 * Nc, MPI_DOUBLE, rank-1,
				0/*tag*/, MPI_COMM_WORLD, &recv_upper_req);

		if (rank != 0) MPI_Wait(&send_upper_req, &mstatus);
		if (rank != (node_size-1)) MPI_Wait(&send_lower_req, &mstatus);
		if (rank != (node_size-1)) MPI_Wait(&recv_lower_req, &mstatus);
		if (rank != 0) MPI_Wait(&recv_upper_req, &mstatus);

		// upload upper bound to GPU
		if (rank != 0) {
			status = clEnqueueWriteBuffer(queue, cl_h2d_c_upper, CL_TRUE, 0, 1 * Nc * sizeof(double),
				(c[0])+disp[rank]-(1 * Nc), 0, NULL, NULL);
		}

		// upload lower bound to GPU
		if (rank != (node_size-1)) {
			status = clEnqueueWriteBuffer(queue, cl_h2d_c_lower, CL_TRUE, 0, 1 * Nc * sizeof(double),
				(c[0])+disp[rank+1], 0, NULL, NULL);
		}
		
		status = clEnqueueNDRangeKernel(queue, kernel2, 1, NULL, &global_work_size,
			&work_group_size, 0, NULL, NULL);
		_GfnCheckCLStatus(status, "LAUNCH SRAD KERNEL 2");
		clFinish(queue);

	}

	//================================================================================80
	// 	SCALE IMAGE UP FROM 0-1 TO 0-255 AND COMPRESS
	//================================================================================80

	status = clEnqueueNDRangeKernel(queue, log_kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH EXP KERNEL");
	clFinish(queue);

} /* end acc data */

	// copy back image
	status = clEnqueueReadBuffer(queue, cl_image, CL_TRUE, 0, cnts[rank] * sizeof(double), 
		(image[0])+disp[rank], 0, NULL, NULL);
	clFinish(queue);
	
	MPI_Gatherv((void*)((image[0])+disp[rank]), cnts[rank], MPI_DOUBLE,
		(void*)(image[0]), cnts, disp, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	time1 = get_time();

	//================================================================================80
	// 	WRITE IMAGE AFTER PROCESSING
	//================================================================================80

	write_graphics(	fileoutname,
								image[0],
								Nr,
								Nc,
								1,
								255);
								
	//================================================================================80
	// 	DEALLOCATE
	//================================================================================80

	sum = 0.0;
	for (i=0;i<Nr;i++) 
		for (j=0;j<Nc;j++)
			sum += image[i][j];

	free(image_ori);
	free(image[0]);
	free(image);

	free(dN[0]); free(dS[0]); free(dW[0]); free(dE[0]);
    free(dN); free(dS); free(dW); free(dE);									// deallocate directional derivative memory
    free(c[0]);
    free(c);																// deallocate diffusion coefficient memory
    
    free(host_sum_buffer);
    free(host_sum2_buffer);
    
    clReleaseMemObject(cl_image);
    clReleaseMemObject(cl_c);
    clReleaseMemObject(cl_dN);
    clReleaseMemObject(cl_dS);
    clReleaseMemObject(cl_dW);
    clReleaseMemObject(cl_dE);
    
	clReleaseMemObject(cl_sum_buffer);
	clReleaseMemObject(cl_sum2_buffer);
    
    clReleaseKernel(exp_kernel);
    clReleaseKernel(reduce_kernel);
    clReleaseKernel(kernel1);
	clReleaseKernel(kernel2);
	clReleaseKernel(log_kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	//================================================================================80
	//		DISPLAY TIMING
	//================================================================================80

	printf("final mean : %3.3lf\n", sum/Ne);
	printf("iteration : %d\n", niter);
	printf("input size : %d x %d\n", Nr, Nc);
	printf("compute time : %.12f s\n", (float)(time1-time0)/1000000);

//====================================================================================================100
//	END OF FILE
//====================================================================================================100
	MPI_Finalize();
	
	return 0;
}


