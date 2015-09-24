2015-03-01 Xuechao Wei <troore@gmail.com>

[Bug Fixed]

1.	fft_no_pragmas.cpp	
	
	<--void assign(float target, float source) {...}
	-->void assign(float *target, float *source) {...}

	And the callers of assign should give addresses rather than values when calling assign, like

	<--assign(y[i], a[i]);
	-->assign(&y[i], &a[i]);

2.	SubblockInterleaver_lte_base.cpp

	<--float pInterMatrix[((BLOCK_SIZE + 4 + 31) / 32) * 32];
	-->float pInterMatrix[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0.0};

	Other local memory should also be initialized likewise to eliminate the post check failure of cosimulation due to 'X' on some ports, as there is uninitialized value in C design.

[Exp]

1.	Warning on Equalizer cosimulation

	Collision occured: 
	Port 0: read
	Port 1: write
	Address: 1

2.	Total != Dynamic + static

	Some kernels show this result on power estimation.

2015-09-19 Xuechao Wei <troore@gmail.com>

[Bug Fixed]

Equalizing/Equalizer.cpp

The usage of pX_tmp for pipeline optimization is not correct. But the right way to eliminate dependence is under design.
