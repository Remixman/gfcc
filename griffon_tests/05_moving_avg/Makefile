GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON ma.c -lrt -D_GFN -lgsl -lgslcblas -lm
	rm ma.o
	
master: trans
	mpicc -o ma $(CFLAGS) plaincc_ma.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
worker: trans
	workergen ma.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm

opt: workeropt masteropt

transopt:
	plaincc --ao 2 --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON ma.c -lrt -D_GFN -lgsl -lgslcblas -lm
	rm ma.o
	
masteropt: transopt
	mpicc -o ma $(CFLAGS) plaincc_ma.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
workeropt: transopt
	workergen ma.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
modify:
	mpicc -o ma $(CFLAGS) plaincc_ma.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
hand:
	mpicc -o mpicl_ma mpicl_ma.c -lrt -lOpenCL -lm
	
clean:
	@rm -rf plaincc_ma.c ma.c.kerdecl.c ma worker* mpicl_ma
