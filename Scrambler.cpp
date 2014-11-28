
#include "Scrambler.h"

void Scrambling(/*LTE_PHY_PARAMS *lte_phy_params, */int pInpSeq[N_SCRAMB_IN_MAX], int pOutSeq[N_SCRAMB_OUT_MAX], int n)
{
//	int n_inp;
	int scramb_seq_int[N_SCRAMB_IN_MAX];

	int i;

//	n_inp = lte_phy_params->scramb_in_buf_sz;

	GenScrambInt(scramb_seq_int, n);

	////////////////////////Scrambling////////////////////////////
	for (i = 0; i < n; i++)
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
	
	for (i = 0; i < n + N_c - 31; i++)
	{
		px1[i + 31] =(px1[i + 3] + px1[i]) % 2;
		px2[i + 31] = (px2[i + 3] + px2[i + 2] + px2[i + 1] + px2[i]) % 2;
	}
	for (i = 0; i < n; i++)
	{
		pScrambInt[i] = 3; // What is this? Any use?
		pScrambInt[i] = (px1[i + N_c] + px2[i + N_c]) % 2;
	}
	/////////////////////END Generate ScrambSeq///////////////////////
}

