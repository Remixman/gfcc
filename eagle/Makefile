all:
	mpicc -c -o ./lib/var_table.o ./src/variable_id_table.c -I./include -lOpenCL
	mpicc -c -o ./lib/ipc.o ./src/myipc_socket.c -I./include -lrt
	mpicc -c -o ./lib/ibcast.o ./src/ibcast.c -I./include -lrt
	mpicc -c -o ./lib/gfn_api.o ./src/gfn.c -I./include -lrt -lOpenCL 
	ld -r -o ./lib/gfn.o ./lib/ibcast.o ./lib/ipc.o ./lib/gfn_api.o ./lib/var_table.o
	rm ./lib/ipc.o ./lib/gfn_api.o ./lib/var_table.o
	
test: all
	mpicc -o ./lib/eagle_test ./src/eagle_test.c ./lib/gfn.o -I./include -lOpenCL
	./lib/eagle_test
	@rm ./lib/eagle_test

clean:
	rm lib/*
