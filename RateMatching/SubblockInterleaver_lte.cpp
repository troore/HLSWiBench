
#include "SubblockInterleaver_lte.h"

static int InterColumnPattern[32] = {0,16,8,24,4,20,12,28,
									2,18,10,26,6,22,14,30,
									1,17,9,25,5,21,13,29,3,
									19,11,27,7,23,15,31};
static int Rate = 3, C_sb = 32, DummyValue = 1000000;

void interleaving_first_two(int R_sb, int NumDummy, int SeqLen, int pInpMtr[RATE * (BLOCK_SIZE + 4)], int pInterMatrix[((BLOCK_SIZE + 4 + 31) / 32) * 32], int pOutMtr[RATE * (BLOCK_SIZE + 4)]){
	int OutIdx;
	for (int StrIdx = 0; StrIdx < (Rate - 1); StrIdx++)
		{
			//////////////// write into matrix //////////////////
			from_Input_matrix_to_inter_matrix_loop:for (int r = 0; r < R_sb; r++)
			{
				for (int c = 0; c < C_sb; c++)
				{
	#pragma HLS PIPELINE
					int k = r * C_sb + c;

					if (k < NumDummy)
					{
						pInterMatrix[k] = DummyValue;
					}
					else
					{
						pInterMatrix[k] = pInpMtr[StrIdx * SeqLen + k - NumDummy];
					}
				}
			}

			OutIdx=0;
			from_inter_matrix_to_out_matrix_loop:for (int c = 0; c < C_sb; c++)
			{
	//#pragma HLS LOOP_FLATTEN

				for (int r = 0 ; r < R_sb; r++)
				{
	#pragma HLS PIPELINE
					int col = InterColumnPattern[c];
					//int k = col * R_sb + r; // This may be useless

					int v = pInterMatrix[r * C_sb + col];

					if (v != DummyValue)
					{
						pOutMtr[StrIdx * SeqLen + OutIdx] = v;
						OutIdx++;
					}
				}
			}
		}
}

void interleaving_the_third(int NumDummy, int K_pi, int SeqLen, int R_sb, int pInpMtr[RATE * (BLOCK_SIZE + 4)], int pOutMtr[RATE * (BLOCK_SIZE + 4)]){
	int OutIdx;
	int Pi[((BLOCK_SIZE + 4 + 31) / 32) * 32];  // Why not BLOCK_SIZE + 4 + 31 ?
		int pInterSeq[((BLOCK_SIZE + 4 + 31) / 32) * 32];

		rate3_dummy_assignment_loop:for (int k = 0;k < NumDummy; k++)
		{
			pInterSeq[k] = DummyValue;
		}
		rate3_nondummy_assignment_loop:for (int k = NumDummy; k < K_pi; k++)
		{
	#pragma HLS PIPELINE
			pInterSeq[k] = pInpMtr[(Rate - 1) * SeqLen + (k - NumDummy)];
		}

		rate3_interleave_index_culculate_loop:for (int k = 0; k < K_pi; k++)
		{
	#pragma HLS PIPELINE
			int idxP = k / R_sb;
			int idx = (InterColumnPattern[idxP] + (C_sb * (k % R_sb)) + 1) % K_pi;

			Pi[k] = idx;
		}

		OutIdx=0;
		rate3_from_interseq_to_outmatrix_loop:for (int k = 0; k < K_pi; k++)
		{
	#pragma HLS PIPELINE
			int v = pInterSeq[Pi[k]];

			if (v == DummyValue)
			{}
			else
			{
				pOutMtr[(Rate - 1) * SeqLen + OutIdx] = v;
				OutIdx++;
			}
		}
}

void SubblockInterleaving(int SeqLen, int pInpMtr[RATE * (BLOCK_SIZE + 4)], int pOutMtr[RATE * (BLOCK_SIZE + 4)])
{
	//int D;
	//int K_pi;
	//int Rate = 3;
	//int C_sb = 32;
	//int InterColumnPattern[32];
	//D = SeqLen;
	//Rate = 3;
	//C_sb = 32;
	//R_sb = (SeqLen + (C_sb - 1)) / C_sb;
	//NumDummy = K_pi - SeqLen;
	//DummyValue = 1000000;
	//int pInterMatrix[K_pi];
	int R_sb = (SeqLen + (C_sb - 1)) / C_sb;
	int K_pi = R_sb * C_sb;
	int NumDummy = K_pi - SeqLen;
	int OutIdx;
	int pInterMatrix[((BLOCK_SIZE + 4 + 31) / 32) * 32]; // Why it is not BLOCK_SIZE + 4 + 31 instead of BLOCK_SIZE + 31?

	interleaving_first_two(R_sb, NumDummy, SeqLen, pInpMtr, pInterMatrix, pOutMtr);
	interleaving_the_third(NumDummy, K_pi, SeqLen, R_sb, pInpMtr, pOutMtr);
	/*
	for (int StrIdx = 0; StrIdx < (Rate - 1); StrIdx++)
	{
		//////////////// write into matrix //////////////////
		from_Input_matrix_to_inter_matrix_loop:for (int r = 0; r < R_sb; r++)
		{
			for (int c = 0; c < C_sb; c++)
			{
#pragma HLS PIPELINE
				int k = r * C_sb + c;
				
				if (k < NumDummy)
				{
					pInterMatrix[k] = DummyValue;
				}
				else
				{
					pInterMatrix[k] = pInpMtr[StrIdx * SeqLen + k - NumDummy];
				}
			}
		}
		
		OutIdx=0;
		from_inter_matrix_to_out_matrix_loop:for (int c = 0; c < C_sb; c++)
		{
//#pragma HLS LOOP_FLATTEN

			for (int r = 0 ; r < R_sb; r++)
			{
#pragma HLS PIPELINE
				int col = InterColumnPattern[c];
				//int k = col * R_sb + r; // This may be useless
				
				int v = pInterMatrix[r * C_sb + col];
				
				if (v != DummyValue)
				{
					pOutMtr[StrIdx * SeqLen + OutIdx] = v;
					OutIdx++;
				}  
			}
		}  
	}
	*/
	
//////////////////// Interleaving for i=2 ///////////////////////
	/*
	int Pi[((BLOCK_SIZE + 31) / 32) * 32];  // Why not BLOCK_SIZE + 4 + 31 ?
	int pInterSeq[((BLOCK_SIZE + 31) / 32) * 32];

	rate3_dummy_assignment_loop:for (int k = 0;k < NumDummy; k++)
	{
		pInterSeq[k] = DummyValue;
	}
	rate3_nondummy_assignment_loop:for (int k = NumDummy; k < K_pi; k++)
	{
#pragma HLS PIPELINE
		pInterSeq[k] = pInpMtr[(Rate - 1) * SeqLen + (k - NumDummy)];
	}

	rate3_interleave_index_culculate_loop:for (int k = 0; k < K_pi; k++)
	{
#pragma HLS PIPELINE
		int idxP = k / R_sb;
		int idx = (InterColumnPattern[idxP] + (C_sb * (k % R_sb)) + 1) % K_pi;

		Pi[k] = idx;
	}

	OutIdx=0;
	rate3_from_interseq_to_outmatrix_loop:for (int k = 0; k < K_pi; k++)
	{
#pragma HLS PIPELINE
		int v = pInterSeq[Pi[k]];

		if (v == DummyValue)
		{}
		else
		{
			pOutMtr[(Rate - 1) * SeqLen + OutIdx] = v;
			OutIdx++;
		}
	}
	*/
}


void SubblockDeInterleaving(int SeqLen, float pInpMtr[RATE * (BLOCK_SIZE + 4)], float pOutMtr[RATE * (BLOCK_SIZE + 4)])
{
	int D;
	int K_pi;
	int Rate;
	int R_sb, C_sb;
	int NumDummy;
	float DummyValue;
	
	int InIdx;
	int OutIdx;

	D = SeqLen;
	Rate = 3;
	C_sb = 32;
	
	R_sb = (D + (C_sb - 1)) / C_sb;
	
	K_pi = R_sb * C_sb;
	NumDummy = K_pi - D;
	DummyValue = (float)1000000;
	
//////////////////// DeInterleaving for i=0,1 ///////////////////////
	float pInterMatrix[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0.0};
#pragma HLS DEPENDENCE variable=pInterMatrix inter false

	for (int StrIdx = 0; StrIdx < (Rate - 1); StrIdx++)
	{
		InIdx=0;
		inter_matrix_assign:for (int r = 0;r < R_sb; r++)
		{
			for (int c = 0; c < C_sb; c++)
			{
#pragma HLS PIPELINE
				int k = r * C_sb + c;
				
				if (k < NumDummy)
				{
					pInterMatrix[r * C_sb + c] = DummyValue;
				}
				else
				{
					pInterMatrix[r * C_sb + c] = (float)0;
				}
			}
		}

		from_input_to_inter:for(int c = 0; c < C_sb; c++)
		{

			for (int r = 0; r < R_sb; r++)
			{
#pragma HLS PIPELINE
				int col = InterColumnPattern[c];
				int k = col * R_sb + r;
				//float v = pInterMatrix[r * C_sb + col];
				
				if (pInterMatrix[r * C_sb + col] != DummyValue)
				{
					pInterMatrix[r * C_sb + col] = pInpMtr[StrIdx * D + InIdx];
					InIdx++;
				}  
			}
		}  

		OutIdx=0;
		from_inter_to_output:for (int r = 0; r < R_sb; r++)
		{
			for(int c = 0; c < C_sb; c++)
			{
#pragma HLS PIPELINE
				float v = pInterMatrix[r * C_sb + c];

				if (v == DummyValue)
				{}
				else
				{
					pOutMtr[StrIdx * D + OutIdx] = pInterMatrix[r * C_sb + c];
					OutIdx++;
				}
			}
		}
	}

//////////////////// DeInterleaving for i=2 ///////////////////////
	int Pi[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0};
	float pInterSeq[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0.0};
#pragma HLS DEPENDENCE variable=pInterSeq inter false
	
	rate3_dummyvalue_assign:for (int k = 0; k < NumDummy; k++)
		pInterSeq[k] = DummyValue;
//////////////// Pi & DePi//////////////////
	calculate_index:for(int k=0;k<K_pi;k++)
	{
#pragma HLS PIPELINE
		int idxP = k / R_sb;
		int idx = (InterColumnPattern[idxP] + (C_sb * (k % R_sb)) + 1) % K_pi;
		
		Pi[k]=idx;
	}
/////////////// DeInterleaving ////////////////////
	InIdx=0;
	rate3_from_input_to_inter:for(int k=0;k<K_pi;k++)
	{
#pragma HLS PIPELINE
		//float v = pInterSeq[Pi[k]];
		if (pInterSeq[Pi[k]] != DummyValue)
		{
			pInterSeq[Pi[k]] = pInpMtr[(Rate - 1) * D + InIdx];
			InIdx++;
		}
	}
	OutIdx=0;
	from_inter_to_out:for (int k = NumDummy; k < K_pi; k++)
	{
#pragma HLS PIPELINE
		pOutMtr[(Rate - 1) * D + OutIdx] = pInterSeq[k];
		OutIdx++;
	}
}

