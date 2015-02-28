#include <stdio.h>
#include "Scrambler.h"

void Scrambling(/*LTE_PHY_PARAMS *lte_phy_params, */int pInpSeq[N_SCRAMB_IN_MAX], int pOutSeq[N_SCRAMB_OUT_MAX], int n)
{
//	int n_inp;
	int scramb_seq_int[N_SCRAMB_IN_MAX];

	int i;

//	n_inp = lte_phy_params->scramb_in_buf_sz;

	printf("%d\n", n);
//	GenScrambInt(scramb_seq_int, 6144);
	GenScrambInt(scramb_seq_int, n);

	////////////////////////Scrambling////////////////////////////
	Scrambling_label1:for (i = 0; i < n; i++)
	{
		pOutSeq[i] = (pInpSeq[i] + scramb_seq_int[i]) % 2;
	}
	////////////////////////END Scrambling////////////////////////////
}


void Descrambling(/*LTE_PHY_PARAMS *lte_phy_params, */float pInpSeq[N_SCRAMB_IN_MAX], float pOutSeq[N_SCRAMB_OUT_MAX], int n)
{
//	int n_inp;
	float scramb_seq_float[N_SCRAMB_IN_MAX];
	int scramb_seq_int[N_SCRAMB_IN_MAX];

	int i;

//	n_inp = lte_phy_params->scramb_in_buf_sz;
	// Generate integer scrambling sequence
	GenScrambInt(scramb_seq_int, n);

	/*
	for (i = 0; i < n_inp; i++)
	{
		if (1 == scramb_seq_int[i])
		{
			scramb_seq_float[i] = -1.0;
		}
		else
		{
			scramb_seq_float[i] = +1.0;
		}
	}
	for (i = 0; i < n_inp; i++)
		pOutSeq[i] = pInpSeq[i] * scramb_seq_float[i];
	*/


	for (i = 0; i < n; i++)
	{
		pOutSeq[i] = (pInpSeq[i] * (scramb_seq_int[i] * (-2.0) + 1.0));
	}
}

/*
void GenScramLoop1(int px1[], int px2[]){
	int n_init[31] = { 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0};
	int i;

	GenScramLoop1_label:for (i = 0; i < 31; i++)
		{
#pragma HLS PIPELINE
			px1[i] = 0;
			px2[i] = n_init[i];
		}
		px1[0] = 1;
}

void GenScramLoop2(int array1[], int array2[], int n){
	int i;
	GenScramLoop2_label:for (i = 0; i < 7200 + 1600 - 31; i++)
		{
#pragma HLS PIPELINE
			if(i >= n + 1600 -31)
				break;
			array1[i + 31] =(array1[i + 3] + array1[i]) % 2;
			array2[i + 31] = (array2[i + 3] + array2[i + 2] + array2[i + 1] + array2[i]) % 2;
		}
}

void GenScramLoop3(int array1[], int array2[], int arrayout[], int n){
	int i;
	GenScramLoop3_label:for (i = 0; i < 7200; i++)
		{
#pragma HLS PIPELINE
			if(i >= n)
				break;
			arrayout[i] = (array1[i + 1600] + array2[i + 1600]) % 2;
		}
}
*/

void GenScrambInt(int pScrambInt[N_SCRAMB_IN_MAX], int n)
{
	int i;
	int N_c = 1600;
	
	int n_init[31] = { 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0};

	/////////////////////Generate ScrambSeq///////////////////////
	int px1[N_SCRAMB_IN_MAX + 1600];
	int px2[N_SCRAMB_IN_MAX + 1600];


	for (i = 0; i < 31; i++)
	{
		px1[i] = 0;
		px2[i] = n_init[i];
	}
	px1[0] = 1;


	GenScrambInt_label2:for (i = 0; i < n + N_c - 31; i++)
	{
		px1[i + 31] =(px1[i + 3] + px1[i]) % 2;
		px2[i + 31] = (px2[i + 3] + px2[i + 2] + px2[i + 1] + px2[i]) % 2;
	}

	GenScrambInt_label3:for (i = 0; i < n; i++)
	{
		pScrambInt[i] = (px1[i + N_c] + px2[i + N_c]) % 2;
	}

	/////////////////////END Generate ScrambSeq///////////////////////
}

