GFN_INCLUDE_PATH = ../../../eagle/include
GFN_LIB_PATH = ../../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c gaussblur.c -lrt -D_GFN
	@rm gaussblur.o
	
master: trans
	mpicc -o gaussblur $(CFLAGS) plaincc_gaussblur.c -lrt -lOpenCL
	
worker: trans
	workergen gaussblur.c.kerdecl.c
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o gaussblur $(CFLAGS) plaincc_gaussblur.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
hand:
	mpicc -o gaussblur_hand gaussblur_hand.c -lrt -lOpenCL

seq:
	gcc -o gaussblur gaussblur.c 

clean:
	@rm -rf plaincc_gaussblur.c gaussblur.c.kerdecl.c gaussblur worker* gaussblur_hand

