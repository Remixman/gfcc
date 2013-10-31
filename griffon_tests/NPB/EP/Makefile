GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib
NPB_INCLUDE_APTH = ../common
NPB_LIB_PATH = ../common
OBJS = ../common/c_print_results.o ../common/c_randdp.o ../common/c_timers.o ../common/c_wtime.o

#include ../config/make.def

#OBJS = ep.o ${COMMON}/c_print_results.o ${COMMON}/c_${RAND}.o \
#       ${COMMON}/c_timers.o ${COMMON}/c_wtime.o

#include ../sys/make.common

#${PROGRAM}: config ${OBJS}
#	${CLINK} ${CLINKFLAGS} -o ${PROGRAM} ${OBJS} ${C_LIB}


#clean:
#	- rm -f *.o *~ 
#	- rm -f npbparams.h core

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(NPB_INCLUDE_APTH) -L$(NPB_LIB_PATH) -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: libs worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON ep.c -lrt -D_GRIFFON
	rm ep.o
	
master: trans
	mpicc -o ep $(CFLAGS) $(OBJS) plaincc_ep.c -lrt -lOpenCL
	
worker: trans
	workergen ep.c.kerdecl.c 
	mpicc -o worker $(CFLAGS) $(OBJS) worker.c -lrt -lOpenCL  
	
libs:
	mpicc -c -o ../common/c_print_results.o ../common/c_print_results.c -lrt
	mpicc -c -o ../common/c_randdp.o ../common/c_randdp.c -lrt
	mpicc -c -o ../common/c_timers.o ../common/c_timers.c -lrt
	mpicc -c -o ../common/c_wtime.o ../common/wtime.c -lrt
	
modify:
	mpicc -o pi_num $(CFLAGS) plaincc_ep.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
clean:
	@rm -rf plaincc_ep.c ep.c.kerdecl.c ep worker*
