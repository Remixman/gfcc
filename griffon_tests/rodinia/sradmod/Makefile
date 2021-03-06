GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c main.c -lrt -D_GFN
	@rm main.o
	
master: trans
	mpicc -o srad $(CFLAGS) plaincc_main.c -lm -lrt -lOpenCL
	
worker: trans
	workergen main.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
opt: workeropt masteropt

transopt:
	plaincc --ao 2 --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON main.c -lrt -D_GFN -lm
	@rm main.o
	
masteropt: transopt
	mpicc -o srad $(CFLAGS) plaincc_main.c -lrt -lOpenCL -lm
	
workeropt: transopt
	workergen main.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lm

modify:
	mpicc -o srad $(CFLAGS) plaincc_main.c -lm -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
hand:
	mpicc -o srad_hand main_hand.c -lrt -lOpenCL
	
seq:
	gcc -o srad main.c -lm

clean:
	@rm *.o srad worker* plaincc_main.c main.c.kerdecl.c srad_hand

