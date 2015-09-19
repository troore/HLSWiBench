
#include <stdlib.h>
#include <stdio.h>
#include "fft/fft.h"
#include "lte_phy.h"

void ofmodulating(float pInpData[2 * N_OFMOD_IN_MAX], float pOutData[2 * N_OFMOD_IN_MAX],
				  int NumLayer, int NIFFT, int CPLen)
{
//	int NumLayer = lte_phy_params->N_tx_ant;
//	int NIFFT = lte_phy_params->N_fft_sz;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//	int CPLen = lte_phy_params->N_samps_cp_l_0;

//	int in_buf_sz = lte_phy_params->ofmod_in_buf_sz;
//	int out_buf_sz = lte_phy_params->ofmod_out_buf_sz;

	int nlayer, nsym, i;

	float p_samp_in_buf[2 * LTE_PHY_FFT_SIZE_MAX];
	float p_samp_out_buf[2 * LTE_PHY_FFT_SIZE_MAX];
	
	for (nlayer = 0; nlayer < NumLayer; nlayer++)
	{
		for (nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int symb_idx = nlayer * NumULSymbSF + nsym;
			float norm = (float)sqrt((float)NIFFT)/* (float)NIFFT*/;

			for (i = 0; i < NIFFT; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				p_samp_in_buf[2 * i + 0] = pInpData[2 * (symb_idx * NIFFT + i) + 0];
				p_samp_in_buf[2 * i + 1] = pInpData[2 * (symb_idx * NIFFT + i) + 1];
			}
			
		//	fft_nrvs(NIFFT, p_samp_in_buf, p_samp_out_buf, -1);
			fft_nrvs_same_array_cyclic(NIFFT, p_samp_in_buf, p_samp_out_buf, -1);
			
			for (i = 0; i < NIFFT; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				pOutData[2 * (symb_idx * (NIFFT + CPLen) + CPLen + i) + 0] = p_samp_out_buf[2 * i + 0] / norm;
				pOutData[2 * (symb_idx * (NIFFT + CPLen) + CPLen + i) + 1] = p_samp_out_buf[2 * i + 1] / norm;
			}
		
			for (i = 0; i < CPLen; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				pOutData[2 * (symb_idx * (NIFFT + CPLen) + i) + 0] = pOutData[2 * (symb_idx * (NIFFT + CPLen) + NIFFT + i) + 0];
				pOutData[2 * (symb_idx * (NIFFT + CPLen) + i) + 1] = pOutData[2 * (symb_idx * (NIFFT + CPLen) + NIFFT + i) + 1];
			}
		}
	}

}


void ofdemodulating(float pInpData[2 * N_OFDEMOD_IN_MAX], float pOutData[2 * N_OFDEMOD_OUT_MAX],
					int NumRxAntenna, int NIFFT, int CPLen)
{
//	int NumRxAntenna = lte_phy_params->N_rx_ant;
//	int NIFFT = lte_phy_params->N_fft_sz;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//	int CPLen = lte_phy_params->N_samps_cp_l_0;

//	int in_buf_sz = lte_phy_params->ofdemod_in_buf_sz;
//	int out_buf_sz = lte_phy_params->ofdemod_out_buf_sz;

	int nrx, nsym, i;

	float p_samp_in_buf[2 * LTE_PHY_FFT_SIZE_MAX];
	float p_samp_out_buf[2 * LTE_PHY_FFT_SIZE_MAX];
	
	for (nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		for (nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int symb_idx = nrx * NumULSymbSF + nsym;
			float norm = (float)sqrt((float)NIFFT) /*(float)1*/;

			for (i = 0; i < NIFFT; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				p_samp_in_buf[2 * i + 0] = pInpData[2 * (symb_idx * (CPLen + NIFFT) + CPLen + i) + 0];
				p_samp_in_buf[2 * i + 1] = pInpData[2 * (symb_idx * (CPLen + NIFFT) + CPLen + i) + 1];
			}

		//	fft_iter(NIFFT, p_samp_in_buf, p_samp_out_buf, 1);
			fft_nrvs_same_array_cyclic(NIFFT, p_samp_in_buf, p_samp_out_buf, 1);

			for(i = 0; i < NIFFT; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				pOutData[2 * (symb_idx * NIFFT + i) + 0] = p_samp_out_buf[2 * i + 0] / norm;
				pOutData[2 * (symb_idx * NIFFT + i) + 1] = p_samp_out_buf[2 * i + 1] / norm;
			}
		}
	}
}

