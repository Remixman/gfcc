#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gfn.h>

int main(int argc, char *argv[]) {

	int expected_val;
	int actual_val;

	/* for(i = 0; i < 1000; ++i) , rank = 4 */
	actual_val = _GfnCalcLocalLoopStartCore(0, 999, 1, 4, 1);
	expected_val = 250;
	assert((actual_val == expected_val));

	actual_val = _GfnCalcLocalLoopEndCore(0, 999, 1, 4, 1);
	expected_val = 499;
	assert((actual_val == expected_val));

	actual_val = _GfnCalcLocalLoopStartCore(0, 999, 1, 4, 3);
	expected_val = 750;
	assert((actual_val == expected_val));

	actual_val = _GfnCalcLocalLoopEndCore(0, 999, 1, 4, 3);
	expected_val = 999;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopStart(
		250 /*local_start*/, 499 /*local_end*/, 1 /*loop_step*/, 
		100 /*stream_size*/, 0 /*stream_no*/, 1 /*block_size*/);
	expected_val = 250;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopEnd(
		250 /*local_start*/, 499 /*local_end*/, 1 /*loop_step*/, 
		100 /*stream_size*/, 0 /*stream_no*/, 1 /*block_size*/);
	expected_val = 349;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopStart(
		250 /*local_start*/, 499 /*local_end*/, 1 /*loop_step*/, 
		100 /*stream_size*/, 2 /*stream_no*/, 1 /*block_size*/);
	expected_val = 450;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopEnd(
		250 /*local_start*/, 499 /*local_end*/, 1 /*loop_step*/, 
		100 /*stream_size*/, 2 /*stream_no*/, 1 /*block_size*/);
	expected_val = 499;
	assert((actual_val == expected_val));

	/* for(i = 0; i < 1000; i+=2) , rank = 4 */
	actual_val = _GfnStreamSeqLocalLoopStart(
		250 /*local_start*/, 499 /*local_end*/, 2 /*loop_step*/, 
		105 /*stream_size*/, 0 /*stream_no*/, 1 /*block_size*/);
	expected_val = 250;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopEnd(
		250 /*local_start*/, 499 /*local_end*/, 2 /*loop_step*/, 
		105 /*stream_size*/, 0 /*stream_no*/, 1 /*block_size*/);
	expected_val = 354;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopStart(
		250 /*local_start*/, 499 /*local_end*/, 2 /*loop_step*/, 
		105 /*stream_size*/, 1 /*stream_no*/, 1 /*block_size*/);
	expected_val = 356;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopEnd(
		250 /*local_start*/, 499 /*local_end*/, 2 /*loop_step*/, 
		105 /*stream_size*/, 1 /*stream_no*/, 1 /*block_size*/);
	expected_val = 459;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopStart(
		250 /*local_start*/, 499 /*local_end*/, 2 /*loop_step*/, 
		105 /*stream_size*/, 2 /*stream_no*/, 1 /*block_size*/);
	expected_val = 460;
	assert((actual_val == expected_val));

	actual_val = _GfnStreamSeqLocalLoopEnd(
		250 /*local_start*/, 499 /*local_end*/, 2 /*loop_step*/, 
		105 /*stream_size*/, 2 /*stream_no*/, 1 /*block_size*/);
	expected_val = 499;
	assert((actual_val == expected_val));

	printf("TEST PASSED\n");

	return 0;
}
