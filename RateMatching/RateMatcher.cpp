
#include "RateMatcher.h"


void TxRateMatching(int piSeq[N_RM_IN_MAX], int pcSeq[N_RM_OUT_MAX], int in_buf_sz, int out_buf_sz)
{
//	int in_buf_sz;
//	int out_buf_sz;
	int n_blocks;
	int rm_blk_sz;
	int rm_data_length;
	int rm_last_blk_len;
	int out_block_offset;
	int n_extra_bits;
	int cur_blk_len;

	int pInMatrix[RATE * (BLOCK_SIZE + 4)];
	int pOutMatrix[RATE * (BLOCK_SIZE + 4)];

	int i, j, r;

//	in_buf_sz = lte_phy_params->rm_in_buf_sz;
//	out_buf_sz = lte_phy_params->rm_out_buf_sz;
	rm_blk_sz = BLOCK_SIZE + 4;
	rm_data_length = (in_buf_sz / RATE);

	n_blocks = (rm_data_length + (rm_blk_sz - 1)) / rm_blk_sz;
	if (rm_data_length % rm_blk_sz)
	{
		rm_last_blk_len = (rm_data_length % rm_blk_sz);
	}
	else
	{
		rm_last_blk_len = rm_blk_sz;
	}
	
	out_block_offset = 0;
	transpositions:for (i = 0; i < n_blocks; i++)
	{
		cur_blk_len = (i != (n_blocks - 1)) ? rm_blk_sz : rm_last_blk_len;

		// Transposition
		transposition1:for (j = 0; j < cur_blk_len; j++)
		{
			for (r = 0; r < RATE; r++)
			{
#pragma HLS PIPELINE
				pInMatrix[r * cur_blk_len + j] = piSeq[out_block_offset + RATE * j + r];
				//	cout << pInMatrix[r][i];
			}
		}
		//cout << endl;
			
		SubblockInterleaving(cur_blk_len, pInMatrix, pOutMatrix);
		
		// Transposition again
		transposition2:for (j = 0; j < cur_blk_len; j++)
		{
			for (r = 0; r < RATE; r++)
			{
#pragma HLS PIPELINE
				pcSeq[out_block_offset + RATE * j + r] = pOutMatrix[r * cur_blk_len + j];
			}
		}

		out_block_offset += RATE * cur_blk_len;
	}

	n_extra_bits = out_buf_sz - in_buf_sz;
	pcSeq_zero_loop:for (i = 0; i < n_extra_bits; i++)
	{
		pcSeq[in_buf_sz + i] = 0;
	}
}

void RxRateMatching(float pLLRin[N_RM_OUT_MAX], float pLLRout[N_RM_IN_MAX], int pHD[N_RM_OUT_MAX], int out_buf_sz)
{
	int n_blocks;
	int rm_blk_sz;
	int rm_data_length;
	int rm_last_blk_len;
	int out_block_offset;
	int cur_blk_len;
//	int out_buf_sz;
	
	float pInMatrix[RATE * (BLOCK_SIZE + 4)];
	float pOutMatrix[RATE * (BLOCK_SIZE + 4)];

	int i, j, r;

//	out_buf_sz = lte_phy_params->rdm_out_buf_sz;
	rm_blk_sz = BLOCK_SIZE + 4;
	rm_data_length = (out_buf_sz / RATE);

	n_blocks = (rm_data_length + (rm_blk_sz - 1)) / rm_blk_sz;
	if (rm_data_length % rm_blk_sz)
	{
		rm_last_blk_len = (rm_data_length % rm_blk_sz);
	}
	else
	{
		rm_last_blk_len = rm_blk_sz;
	}

	out_block_offset = 0;
	DeTransposition_loop:for (i = 0; i < n_blocks; i++)
	{
		cur_blk_len = (i != (n_blocks - 1)) ? rm_blk_sz : rm_last_blk_len;
		
		DeTransposition_inner_loop1:for (j = 0; j < cur_blk_len; j++)
		{
			for (r = 0; r < RATE; r++)
			{
#pragma HLS PIPELINE
				pInMatrix[r * cur_blk_len + j] = pLLRin[out_block_offset + RATE * j + r];
			}
		}  

		SubblockDeInterleaving(cur_blk_len, pInMatrix, pOutMatrix);

		DeTransposition_inner_loop2:for (j = 0; j < cur_blk_len; j++)
		{
			for (r = 0; r < RATE; r++)
			{
#pragma HLS PIPELINE
				pLLRin[out_block_offset + RATE * j + r] = pOutMatrix[r * cur_blk_len + j];
			}
		}

		out_block_offset += RATE * cur_blk_len;
	}

	pHD_pos_neg:for (i = 0; i < out_buf_sz; i++)
	{
#pragma HLS PIPELINE
		if (pLLRin[i] < 0)
		{
			pHD[i] = 0;
		}
		else
		{
			pHD[i] = 1;
		}
	}

	pLLRout_assign:for (i = 0; i < out_buf_sz; i++)
	{
#pragma HLS PIPELINE
		pLLRout[i] = pLLRin[i];
	}
}

