GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -O3 -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c matmul.c -lrt -D_GFN -lgsl -lgslcblas -lm
	@rm matmul.o
	
master: trans
	mpicc -o matmul $(CFLAGS) plaincc_matmul.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
worker: trans
	workergen matmul.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
modify:
	mpicc -o matmul $(CFLAGS) plaincc_matmul.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
hand:
	mpicc -O3 -o matmul_hand matmul_hand.c -lrt -lOpenCL -lgsl -lgslcblas -lm

seq:
	gcc -O3 -o matmul matmul.c 
	
block:
	gcc -O3 -o matmul_block matmul_block.c

clean:
	@rm -rf plaincc_matmul.c matmul.c.kerdecl.c matmul worker* matmul_hand

