CFLAGS = -g

all:
	mpicc $(CFLAGS) -c -o ./lib/var_table.o ./src/variable_id_table.c -I./include -lOpenCL
	mpicc $(CFLAGS) -c -o ./lib/kernel_table.o ./src/kernel_id_table.c -I./include -lOpenCL
	mpicc $(CFLAGS) -c -o ./lib/ipc.o ./src/myipc_socket.c -I./include -lrt
	mpicc $(CFLAGS) -c -o ./lib/gfn_api.o ./src/gfn.c -I./include -lrt -lOpenCL 
	ld -r -o ./lib/gfn.o ./lib/ipc.o ./lib/gfn_api.o ./lib/var_table.o ./lib/kernel_table.o
	rm ./lib/ipc.o ./lib/gfn_api.o ./lib/var_table.o
	
test: all
	mpicc $(CFLAGS) -o ./lib/eagle_test ./src/eagle_test.c ./lib/gfn.o -I./include -lOpenCL
	./lib/eagle_test
	@rm ./lib/eagle_test

clean:
	rm lib/*
