CFLAGS = -I/home/remixman/Desktop/griffon_runtime_library/ipc/ -I/usr/include \
 -I/usr/lib/openmpi/include -I/home/remixman/Desktop/griffon_runtime_library \
 /home/remixman/Desktop/griffon_runtime_library/ipc/myipc.o

#--pass-through    Disables preprocessing
trans:
	plaincc --cuda --gfn --keep-files $(CFLAGS) -D_GRIFFON pi_num.c -lrt
	

test: worker master

gfnlib:
	mpicc -c $(CFLAGS) -o gfn.o /home/remixman/Desktop/griffon_runtime_library/gfn.c -lrt -lOpenCL  

master: gfnlib
	gcc -o pi_num $(CFLAGS) plaincc_pi_num.c -lrt
	
worker: gfnlib
	mpicc -DGFN_WORKER -c -o pi_num_worker.o $(CFLAGS) plaincc_pi_num.c -lrt -lOpenCL
	mpicc -DGFN_WORKER -o worker $(CFLAGS) gfn.o pi_num_worker.o ../worker.c -lrt -lOpenCL
	
	
clean:
	@rm -rf gfn.o pi_num_worker.o pi_num worker a.out
