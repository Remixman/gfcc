GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON small_bt.c -lrt
	rm small_bt.o
	
master: trans
	mpicc -o small_bt $(CFLAGS) plaincc_small_bt.c -lrt -lOpenCL
	
worker: trans
	workergen small_bt.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
clean:
	@rm -rf plaincc_small_bt.c small_bt.c.kerdecl.c small_bt worker* a.out
