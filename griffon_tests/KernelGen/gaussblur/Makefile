GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -O3 -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c gaussblur.c -lrt -D_GFN -lgsl -lgslcblas -lm
	@rm gaussblur.o
	
master: trans
	mpicc -o gaussblur $(CFLAGS) plaincc_gaussblur.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
worker: trans
	workergen gaussblur.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
modify:
	mpicc -o gaussblur $(CFLAGS) plaincc_gaussblur.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL -lgsl -lgslcblas -lm
	
	
hand:
	mpicc -O3 -o gaussblur_hand gaussblur_hand.c -lrt -lOpenCL

seq:
	gcc -O3 -o gaussblur gaussblur.c 

clean:
	@rm -rf plaincc_gaussblur.c gaussblur.c.kerdecl.c gaussblur worker* gaussblur_hand

