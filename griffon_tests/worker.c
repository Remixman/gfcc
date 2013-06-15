#include <mpi.h>
#include <stdio.h>
#include <gfn.h>
#include "myipc.h"

//#include "type_dec.def"
void _Function_1();
void _Function_2();
void _Function_3();


void RecvCommand(int *func_code) {
	if (_gfn_rank == 0) {
		_RecvCallFuncMsg(func_code);
		printf("Rank 0 receive command code : %d\n", *func_code);
	}
	MPI_Bcast( func_code, 1, MPI_INT, 0, MPI_COMM_WORLD );
	//printf("After bcast rank %d receive command code %d\n", _gfn_rank, *func_code);
}

int main(int argc, char *argv[]) {
	
	int exit_f = 0, error_f = 0;

	_GfnInit(&argc, &argv);
	
	while (1) {
		int func_code;
		RecvCommand(&func_code);
		
		switch (func_code) {
			case 1:
				_Function_1();
				break;
			
			case 0:
				if (_gfn_rank == 0) {
					printf("Get exit code!\n");
				}
				exit_f = 1;
				break;
			
			case -1:
				exit_f = error_f = 1;
				break;
			
			default:
				// TODO: show error unknown command
				if (_gfn_rank == 0) {
					printf("Get Error Code : %d\n", func_code);
				}
				exit_f = 1;
				break;
		}
		
		if (exit_f)
			break;
	}
	
	if (error_f)
		printf("Exit with error");
		
	_GfnFinalize();

	return 0;
}

