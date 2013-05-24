all:
	mpicc -c -o ./lib/ipc.o ./src/myipc_socket.c -I./include -lrt
	mpicc -c -o ./lib/gfn_api.o ./src/gfn.c -I./include -lrt -lOpenCL 
	ld -r -o ./lib/gfn.o ./lib/ipc.o ./lib/gfn_api.o
	rm ./lib/ipc.o ./lib/gfn_api.o
	
clean:
	rm lib/*
