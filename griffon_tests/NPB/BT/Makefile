GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o -I../common

DEP_OBJ = c_print_results.o c_timers.o wtime.o c_randdp.o

all: worker master

dep:
	mpicc -c $(CFLAGS) ../common/c_print_results.c  ../common/c_randdp.c  ../common/c_timers.c ../common/wtime.c

trans: dep
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON bt.c -lrt
	rm bt.o
	
master: trans
	mpicc -o bt $(CFLAGS) $(DEP_OBJ) plaincc_bt.c -lrt -lOpenCL
	
worker: trans
	workergen bt.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL
	
	
clean:
	@rm -rf plaincc_bt.c bt.c.kerdecl.c bt worker* *.o
