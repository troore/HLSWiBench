
#include "SubblockInterleaver_lte.h"

static int InterColumnPattern[32] = {0,16,8,24,4,20,12,28,
									2,18,10,26,6,22,14,30,
									1,17,9,25,5,21,13,29,3,
									19,11,27,7,23,15,31};

void SubblockInterleaving(int SeqLen, int pInpMtr[RATE * (BLOCK_SIZE + 4)], int pOutMtr[RATE * (BLOCK_SIZE + 4)])
{
	int D;
	int K_pi;
	int Rate;
	int R_sb, C_sb;
	int NumDummy;
	int DummyValue;
//	int InterColumnPattern[32];
		
	int OutIdx;

	D = SeqLen;
	Rate = 3;
	C_sb = 32;
	
	R_sb = (D + (C_sb - 1)) / C_sb;
	
	K_pi = R_sb * C_sb;
	NumDummy = K_pi - D;
	DummyValue = 1000000;

	int pInterMatrix[((BLOCK_SIZE + 31) / 32) * 32];

	for (int StrIdx = 0; StrIdx < (Rate - 1); StrIdx++)
	{
		//////////////// write into matrix //////////////////
		for (int r = 0; r < R_sb; r++)
		{
			for (int c = 0; c < C_sb; c++)
			{
				int k = r * C_sb + c;
				
				if (k < NumDummy)
				{
					pInterMatrix[r * C_sb + c] = DummyValue;
				}
				else
				{
					pInterMatrix[r * C_sb + c] = pInpMtr[StrIdx * D + k - NumDummy];
				}
			}
		}
		
		OutIdx=0;
		for (int c = 0; c < C_sb; c++)
		{
			int col = InterColumnPattern[c];
			for (int r = 0; r < R_sb; r++)
			{
				int k = col * R_sb + r;
				
				int v = pInterMatrix[r * C_sb + col];
				
				if (v == DummyValue)
				{}
				else
				{
					pOutMtr[StrIdx * D + OutIdx] = v;
					OutIdx++;
				}  
			}
		}  
	}
	
//////////////////// Interleaving for i=2 ///////////////////////
	int Pi[((BLOCK_SIZE + 31) / 32) * 32];
	int pInterSeq[((BLOCK_SIZE + 31) / 32) * 32];

	for (int k = 0;k < NumDummy; k++)
	{
		pInterSeq[k] = DummyValue;
	}
	for (int k = NumDummy; k < K_pi; k++)
	{
		pInterSeq[k] = pInpMtr[(Rate - 1) * D + (k - NumDummy)];
	}

	for (int k = 0; k < K_pi; k++)
	{
		int idxP = k / R_sb;
		int idx = (InterColumnPattern[idxP] + (C_sb * (k % R_sb)) + 1) % K_pi;

		Pi[k] = idx;
	}

	OutIdx=0;
	for (int k = 0; k < K_pi; k++)
	{
		int v = pInterSeq[Pi[k]];

		if (v == DummyValue)
		{}
		else
		{
			pOutMtr[(Rate - 1) * D + OutIdx] = v;
			OutIdx++;
		}
	}
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
	float pInterMatrix[((BLOCK_SIZE + 31) / 32) * 32];

	for (int StrIdx = 0; StrIdx < (Rate - 1); StrIdx++)
	{
		InIdx=0;
		for (int r = 0;r < R_sb; r++)
		{
			for (int c = 0; c < C_sb; c++)
			{
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

		for (int c = 0; c < C_sb; c++)
		{
			int col = InterColumnPattern[c];
			for (int r = 0; r < R_sb; r++)
			{
				int k = col * R_sb + r;
				float v = pInterMatrix[r * C_sb + col];
				
				if (v == DummyValue)
				{}
				else
				{
					pInterMatrix[r * C_sb + col] = pInpMtr[StrIdx * D + InIdx];
					InIdx++;
				}  
			}
		}  

		OutIdx=0;
		for (int r = 0; r < R_sb; r++)
		{
			for(int c = 0; c < C_sb; c++)
			{
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
	int Pi[((BLOCK_SIZE + 31) / 32) * 32];
	float pInterSeq[((BLOCK_SIZE + 31) / 32) * 32];
	
	for (int k = 0; k < NumDummy; k++)
		pInterSeq[k] = DummyValue;
//////////////// Pi & DePi//////////////////
	for(int k=0;k<K_pi;k++)
	{
		int idxP = k / R_sb;
		int idx = (InterColumnPattern[idxP] + (C_sb * (k % R_sb)) + 1) % K_pi;
		
		Pi[k]=idx;
	}
/////////////// DeInterleaving ////////////////////
	InIdx=0;
	for(int k=0;k<K_pi;k++)
	{
		float v = pInterSeq[Pi[k]];
		if (v == DummyValue)
		{}
		else
		{
			pInterSeq[Pi[k]] = pInpMtr[(Rate - 1) * D + InIdx];
			InIdx++;
		}
	}
	OutIdx=0;
	for (int k = NumDummy; k < K_pi; k++)
	{
		pOutMtr[(Rate - 1) * D + OutIdx] = pInterSeq[k];
		OutIdx++;
	}
}

