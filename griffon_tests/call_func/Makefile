GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON call_func.c -lrt -D_GFN
	rm call_func.o
	
master: trans
	mpicc -o call_func $(CFLAGS) plaincc_call_func.c -lrt -lOpenCL
	
worker: trans
	workergen call_func.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o call_func $(CFLAGS) plaincc_call_func.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
clean:
	@rm -rf plaincc_call_func.c call_func.c.kerdecl.c call_func worker*
