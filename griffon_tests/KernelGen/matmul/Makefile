GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -g -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --ao=1 --keep-files $(CFLAGS) -c matmul.c -lrt -D_GFN -lm
	@rm matmul.o
	
master: trans
	mpicc -o matmul $(CFLAGS) plaincc_matmul.c -lrt -lOpenCL -lm
	
worker: trans
	workergen matmul.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lm
	
	
modify:
	mpicc -o matmul $(CFLAGS) plaincc_matmul.c -lrt -lOpenCL -lm
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lm
	
	
hand:
	mpicc -o matmul_hand matmul_hand.c -lrt -lOpenCL -lm

seq:
	gcc -o matmul matmul.c 

clean:
	@rm -rf plaincc_matmul.c matmul.c.kerdecl.c matmul worker* matmul_hand

