GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -O3 -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c main.c -lrt -D_GFN -lgsl -lgslcblas -lm
	@rm main.o
	
master: trans
	mpicc -o srad $(CFLAGS) plaincc_main.c -lm -lrt -lOpenCL -lgsl -lgslcblas -lm
	
worker: trans
	workergen main.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
modify:
	mpicc -o srad $(CFLAGS) plaincc_main.c -lm -lrt -lOpenCL -lgsl -lgslcblas -lm
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
hand:
	mpicc -O3 -o srad_hand main_hand.c -lrt -lOpenCL
	
seq:
	gcc -O3 -o srad main.c -lm
	
block:
	gcc -O3 -o srad_block main_block.c

clean:
	@rm *.o srad worker* plaincc_main.c main.c.kerdecl.c srad_hand

