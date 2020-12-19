# maeka config
#MPI_PATH = /opt/openmpi/include/
#CL_INC_PATH = /san01/home/pisit_m/OpenCL/common/inc
#CL_LIB_PATH = /san01/home/pisit_m/OpenCL/common/lib

# yggdrasil config
MPI_PATH = /usr/lib/openmpi/include
CL_INC_PATH = /usr/include
CL_LIB_PATH = 


CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) -O2

all:
	mpicc $(CFLAGS) -o platform_explorer platform_explorer.c -lOpenCL
	
clean:
	rm platform_explorer
