GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON simple.c -lrt -D_GFN
	rm simple.o
	
master: trans
	mpicc -o simple $(CFLAGS) plaincc_simple.c -lrt -lOpenCL
	
worker: trans
	workergen simple.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
clean:
	@rm -rf plaincc_simple.c simple.c.kerdecl.c simple worker*
