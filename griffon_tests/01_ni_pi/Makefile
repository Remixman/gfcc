GFN_INCLUDE_PATH = ../../eagle/include
GFN_LIB_PATH = ../../eagle/lib

# should set MPI_PATH , CL_INC_PATH , CL_LIB_PATH in ~/.bashrc

CFLAGS = -I$(CL_INC_PATH) -L$(CL_LIB_PATH) -I$(MPI_PATH) \
-I$(GFN_INCLUDE_PATH) $(GFN_LIB_PATH)/gfn.o

all: worker master

trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -c -D_GRIFFON pi_num.c -lrt -D_GRIFFON
	rm pi_num.o
	
master: trans
	mpicc -o pi_num $(CFLAGS) plaincc_pi_num.c -lrt -lOpenCL
	
worker: trans
	workergen pi_num.c.kerdecl.c 
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
modify:
	mpicc -o pi_num $(CFLAGS) plaincc_pi_num.c -lrt -lOpenCL
	mpicc -o worker $(CFLAGS) worker.c -lrt -lOpenCL  
	
	
hand:
	mpicc -o mpicl_pi_num mpicl_pi_num.c -lrt -lOpenCL
	
clean:
	@rm -rf plaincc_pi_num.c pi_num.c.kerdecl.c pi_num worker* mpicl_pi_num
