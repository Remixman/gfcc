GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --gfn --keep-files $(CFLAGS) -c hotspot.c -lrt -D_GFN
	@rm hotspot.o
	
master: trans
	mpicc -o hotspot $(CFLAGS) plaincc_hotspot.c -lm -lrt -lOpenCL
	
worker: trans
	workergen hotspot.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o hotspot $(CFLAGS) plaincc_hotspot.c -lm -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
clean:
	@rm *.o hotspot result.txt worker* plaincc_hotspot.c hotspot.c.kerdecl.c

test:
	diff 100ite_result.txt result.txt
