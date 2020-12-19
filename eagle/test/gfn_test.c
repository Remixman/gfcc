#include <gfn.h>

void Test_CalcPartitionInfo() 
{
	int passed = 0;

#if 0
	_CalcPartitionInfo(int size, int block_size, int loop_start, int loop_end, int loop_step,
						int *pattern_array, int pattern_array_size, int pattern_type,
	                    int *cnts /* OUTS */, int *disp /* OUTS */, 
	                    int *sub_size /* OUTS */, int *elem_offset /* OUTS */)
#endif

	// simple 1D test
	_CalcPartitionInfo(	100,	/* size */ 
						1, 		/* block size */
						0, 		/* loop start */
						100, 	/* loop end */
						1,		/* loop step */
						
						)

	printf("\tTest CalcPartitionInfo : %s\n", (passed)? "PASSED" : "FAILED");
}

int main() 
{
	printf("Run all tests\n");

	Test_CalcPartitionInfo();
}