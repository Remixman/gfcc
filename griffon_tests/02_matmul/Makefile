GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# maeka config
#MPI_PATH = /opt/openmpi/include/
#CL_INC_PATH = /san01/home/pisit_m/OpenCL/common/inc
#CL_LIB_PATH = /san01/home/pisit_m/OpenCL/common/lib

# yggdrasil config
MPI_PATH = /usr/lib/openmpi/include
CL_INC_PATH = /usr/include
CL_LIB_PATH = 


CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

#--pass-through    Disables preprocessing
trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -D_GRIFFON matmul.c -lrt
	

test: worker master

master:
	mpicc -o matmul $(CFLAGS) plaincc_matmul.c -lrt -lOpenCL
	
worker:
	mpicc -DGFN_WORKER -o worker $(CFLAGS) plaincc_matmul.c ../worker.c -lrt -lOpenCL  
	
	
clean:
	@rm -rf matmul worker a.out
