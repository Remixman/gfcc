# maeka config
#GFN_INCLUDE_PATH = /san01/home/pisit_m/griffon/eagle/include
#GFN_LIB_PATH = /san01/home/pisit_m/griffon/eagle/lib
#MPI_PATH = /opt/openmpi/include/
#CL_INC_PATH = /san01/home/pisit_m/OpenCL/common/inc
#CL_LIB_PATH = /san01/home/pisit_m/OpenCL/common/lib

# yggdrasil config
GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib
MPI_PATH = /usr/lib/openmpi/include
CL_INC_PATH = /usr/include
CL_LIB_PATH = 


CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

#--pass-through    Disables preprocessing
trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -D_GRIFFON simple.c -lrt
	

test: worker master

master:
	mpicc -o simple $(CFLAGS) plaincc_simple.c -lrt -lOpenCL
	
worker:
	mpicc -DGFN_WORKER -o worker $(CFLAGS) plaincc_simple.c ../worker.c -lrt -lOpenCL  
	
	
clean:
	@rm -rf simple_worker.o simple worker a.out
