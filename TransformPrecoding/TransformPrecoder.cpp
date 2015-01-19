#include "TransformPrecoder.h"
#include "dft/dft.h"
#include <cstring>

void TransformPrecoding(float pInpSeq[N_TRANS_ENCODER_IN_MAX], float pDataMatrix[N_TRANS_ENCODER_OUT_MAX],int NumLayer, int MDFT)
{
	//	int NumLayer = lte_phy_params->N_tx_ant;
	//	int MDFT = lte_phy_params->N_dft_sz;
#pragma HLS ARRAY_PARTITION variable=pDataMatrix cyclic factor=2 dim=1
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	TransformPrecoding_loop1:for(int nlayer = 0; nlayer < NumLayer; nlayer++)
	{
		TransformPrecoding_loop2:for(int nsym = 0; nsym < NumULSymbSF - 2; nsym++)
		{
#pragma HLS LOOP_FLATTEN
			int idx = nlayer * (MDFT * (NumULSymbSF - 2)) + nsym * MDFT;
			float norm = (float)sqrt((float)MDFT);
			
			dft(MDFT, pInpSeq + idx * 2, pDataMatrix + idx * 2, 1);
				
			TransformPrecoding_loop3:for(int m = 0;m < MDFT; m++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				pDataMatrix[2 * (idx + m) + 0] = pDataMatrix[2 * (idx + m) + 0] / norm;
				pDataMatrix[2 * (idx + m) + 1] = pDataMatrix[2 * (idx + m) + 1] / norm;
			}
		}
	}
}

void TransformDecoding(float pDataMatrix[N_TRANS_DECODER_IN_MAX], float pDecSeq[N_TRANS_DECODER_OUT_MAX], int NumLayer, int MDFT)
{
//	int NumLayer = lte_phy_params->N_rx_ant;
//	int MDFT = lte_phy_params->N_dft_sz;
#pragma HLS ARRAY_PARTITION variable=pDecSeq cyclic factor=2 dim=1
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	for(int nlayer=0;nlayer<NumLayer;nlayer++)
	{
		for(int nsym=0;nsym<NumULSymbSF-2;nsym++)
		{
#pragma HLS LOOP_FLATTEN
			int idx = nlayer*(MDFT*(NumULSymbSF-2))+nsym*MDFT;
			float norm = (float)sqrt((float)MDFT);
			dft(MDFT, pDataMatrix + idx * 2, pDecSeq + idx * 2, -1);
			for(int m = 0; m < MDFT; m++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				pDecSeq[2 * (idx + m) + 0] = pDecSeq[2 * (idx + m) + 0] / norm;
				pDecSeq[2 * (idx + m) + 1] = pDecSeq[2 * (idx + m) + 1] / norm;
			}                              
		}
	}
}
