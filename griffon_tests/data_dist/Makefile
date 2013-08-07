GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON data_dist.c -lrt -D_GFN
	rm data_dist.o
	
master: trans
	mpicc -o data_dist $(CFLAGS) plaincc_data_dist.c -lrt -lOpenCL
	
worker: trans
	workergen data_dist.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o data_dist $(CFLAGS) plaincc_data_dist.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
clean:
	@rm -rf plaincc_data_dist.c data_dist.c.kerdecl.c data_dist worker*
