GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -g -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON convolution.c -lrt -D_GFN
	rm convolution.o
	
master: trans
	mpicc -o convolution $(CFLAGS) plaincc_convolution.c -lrt -lOpenCL
	
worker: trans
	workergen convolution.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o convolution $(CFLAGS) plaincc_convolution.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
hand:
	mpicc -o mpicl_convolution mpicl_convolution.c -lrt -lOpenCL
	
clean:
	@rm -rf plaincc_convolution.c convolution.c.kerdecl.c convolution worker* mpicl_convolution
