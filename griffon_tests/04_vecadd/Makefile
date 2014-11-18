GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -g -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON vecadd.c -lrt -D_GFN -lgsl -lgslcblas -lm
	rm vecadd.o
	
master: trans
	mpicc -o vecadd $(CFLAGS) plaincc_vecadd.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
worker: trans
	workergen vecadd.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm

opt: workeropt masteropt

transopt:
	plaincc --ao 2 --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON vecadd.c -lrt -D_GFN -lgsl -lgslcblas -lm
	rm vecadd.o
	
masteropt: transopt
	mpicc -o vecadd $(CFLAGS) plaincc_vecadd.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
workeropt: transopt
	workergen vecadd.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
modify:
	mpicc -o vecadd $(CFLAGS) plaincc_vecadd.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
hand:
	mpicc -o mpicl_vecadd mpicl_vecadd.c -lrt -lOpenCL -lm
	
clean:
	@rm -rf plaincc_vecadd.c vecadd.c.kerdecl.c vecadd worker* mpicl_vecadd
