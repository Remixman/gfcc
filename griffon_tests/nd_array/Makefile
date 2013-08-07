GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON nd_array.c -lrt -D_GFN
	rm nd_array.o
	
master: trans
	mpicc -o nd_array $(CFLAGS) plaincc_nd_array.c -lrt -lOpenCL
	
worker: trans
	workergen nd_array.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o nd_array $(CFLAGS) plaincc_nd_array.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
clean:
	@rm -rf plaincc_nd_array.c nd_array.c.kerdecl.c nd_array worker*
