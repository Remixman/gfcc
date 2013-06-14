all:
	g++ -c -g -o ./lib/var_table.o ./src/variable_id_table.cpp -I./include -lOpenCL
	mpicc -c -g -o ./lib/ipc.o ./src/myipc_socket.c -I./include -lrt
	mpicc -c -g -o ./lib/gfn_api.o ./src/gfn.c -I./include -lrt -lOpenCL 
	ld -r -o ./lib/gfn.o ./lib/ipc.o ./lib/gfn_api.o ./lib/var_table.o
	rm ./lib/ipc.o ./lib/gfn_api.o ./lib/var_table.o
	
clean:
	rm lib/*
