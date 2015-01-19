#include "Equalizer.h"
#include "lte_phy.h"
#include "dmrs/dmrs.h"
//#include <stdio.h>

static void MatrixProd(int d1, int d2, int d3, float M1[], float M2[], float oM[])
{
	int r, c, i;
	float tmp0[5], tmp1[5];
	tmp0[0] = 0.0; tmp1[0] = 0.0;
	prod_1:for (r = 0; r < d1; r++)
	{
		prod_2:for (c = 0; c < d3; c++)
		{
			prod_3:for (i = 0; i < d2; i++)
			{
#pragma HLS loop_flatten
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				if(i == d2 - 1){
					oM[2 * (r * d3 + c) + 0] = tmp0[i] + (M1[2 * (r * d2 + i) + 0] * M2[2 * (i * d3 + c) + 0] - M1[2 * (r * d2 + i) + 1] * M2[2 * (i * d3 + c) + 1]);
					oM[2 * (r * d3 + c) + 1] = tmp1[i] + (M1[2 * (r * d2 + i) + 0] * M2[2 * (i * d3 + c) + 1] + M1[2 * (r * d2 + i) + 1] * M2[2 * (i * d3 + c) + 0]);
				}
				else{
					tmp0[i + 1] = tmp0[i] + (M1[2 * (r * d2 + i) + 0] * M2[2 * (i * d3 + c) + 0] - M1[2 * (r * d2 + i) + 1] * M2[2 * (i * d3 + c) + 1]);
					tmp1[i + 1] = tmp1[i] + (M1[2 * (r * d2 + i) + 0] * M2[2 * (i * d3 + c) + 1] + M1[2 * (r * d2 + i) + 1] * M2[2 * (i * d3 + c) + 0]);
				}
			}
		}
	}
}


static void MatrixInv(int sz, float pM[], float pInvM[])
{
	float pX[LTE_PHY_N_ANT_MAX * 2 * LTE_PHY_N_ANT_MAX * 2];
	float pX_tmp[LTE_PHY_N_ANT_MAX * 2 * LTE_PHY_N_ANT_MAX * 2];
	float pX_tmp2[LTE_PHY_N_ANT_MAX * 2 * LTE_PHY_N_ANT_MAX * 2];

	inv1:for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			//	pX[r * (2 * sz) + c] = pM[r * sz + c];
			pX[2 * (r * (2 * sz) + c) + 0] = pM[2 * (r * sz + c) + 0];
			pX[2 * (r * (2 * sz) + c) + 1] = pM[2 * (r * sz + c) + 1];
		}
		for (int c = sz; c < 2 * sz; c++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			if (c == (r + sz))
			{
				//	pX[r * (2 * sz) + c] = (std::complex<float>)1.0;
				pX[2 * (r * (2 * sz) + c) + 0] = 1.0;
				pX[2 * (r * (2 * sz) + c) + 1] = 0.0;
			}
			else
			{
				//	pX[r * (2 * sz) + c] = (std::complex<float>)0.0;
				pX[2 * (r * (2 * sz) + c) + 0] = 0.0;
				pX[2 * (r * (2 * sz) + c) + 1] = 0.0;
			}
		}
	}

	float pCurRow[2  * LTE_PHY_N_ANT_MAX * 2];

	inv2:for (int r = 0; r < sz; r++)
	{
		float B[2];
		B[0] = pX[2 * (r * (2 * sz) + r) + 0];
		B[1] = pX[2 * (r * (2 * sz) + r) + 1];

		for (int c = 0; c < (2 * sz); c++)
		{
#pragma HLS PIPELINE
			float A[2];
			//	pCurRow[c] = pX[r * (2 * sz) + c] / pX[r * (2 * sz) + r];
			A[0] = pX[2 * (r * (2 * sz) + c) + 0];
			A[1] = pX[2 * (r * (2 * sz) + c) + 1];
			pCurRow[2 * c + 0] = (A[0] * B[0] + A[1] * B[1]) / (B[0] * B[0] + B[1] * B[1]);
			pCurRow[2 * c + 1] = (A[1] * B[0] - A[0] * B[1]) / (B[0] * B[0] + B[1] * B[1]);
		}

		for (int c = 0; c < (2 * sz); c++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			//	pX[r * (2 * sz) + c] = pCurRow[c];
			pX[2 * (r * (2 * sz) + c) + 0] = pCurRow[2 * c + 0];
			pX[2 * (r * (2 * sz) + c) + 1] = pCurRow[2 * c + 1];
		}

		for (int er = r + 1; er < sz; er++)
		{
			float curC[2] = {pX[2 * (er * (2 * sz) + r) + 0], pX[2 * (er * (2 * sz) + r) + 1]};

			for (int c = 0; c < (2 * sz); c++)
			{
#pragma HLS PIPELINE
#pragma HLS loop_flatten off
#pragma HLS DEPENDENCE array inter false
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pX_tmp[2 * (er * (2 * sz) + c) + 0] = pX[2 * (er * (2 * sz) + c) + 0] - (curC[0] * pCurRow[2 * c + 0] - curC[1] * pCurRow[2 * c + 1]);
				pX_tmp[2 * (er * (2 * sz) + c) + 1] = pX[2 * (er * (2 * sz) + c) + 1] - (curC[0] * pCurRow[2 * c + 1] + curC[1] * pCurRow[2 * c + 0]);
			}  
		}
	}

	inv3:for (int r = sz - 1; r >= 0; r--)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
#pragma HLS PIPELINE
			//	pCurRow[c] = pX[r * (2 * sz) + c];
			pCurRow[2 * c + 0] = pX_tmp[2 * (r * (2 * sz) + c) + 0];
			pCurRow[2 * c + 1] = pX_tmp[2 * (r * (2 * sz) + c) + 1];
		}

		for (int er = r - 1; er >= 0; er--)
		{
			float curC[2] = {pX_tmp[2 * (er * (2 * sz) + r) + 0], pX_tmp[2 * (er * (2 * sz) + r) + 1]};

			for (int c = 0; c < (2 * sz); c++)
			{
#pragma HLS PIPELINE
#pragma HLS loop_flatten off
#pragma HLS DEPENDENCE array inter false
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pX_tmp2[2 * (er * (2 * sz) + c) + 0] = pX_tmp[2 * (er * (2 * sz) + c) + 0] - (curC[0] * pCurRow[2 * c + 0] - curC[1] * pCurRow[2 * c + 1]);
				pX_tmp2[2 * (er * (2 * sz) + c) + 1] = pX_tmp[2 * (er * (2 * sz) + c) + 1] - (curC[0] * pCurRow[2 * c + 1] + curC[1] * pCurRow[2 * c + 0]);
			}  
		}
	}

	inv4:for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			int col = c + sz;
			//	pInvM[r * sz + c] = pX[r * (2 * sz) + col];
			pInvM[2 * (r * sz + c) + 0] = pX_tmp2[2 * (r * (2 * sz) + col) + 0];
			pInvM[2 * (r * sz + c) + 1] = pX_tmp2[2 * (r * (2 * sz) + col) + 1];
		}

	}
}

void FDLSEstimation(float pXt[2 * LTE_PHY_N_ANT_MAX * 2],
				    float pXtDagger[LTE_PHY_N_ANT_MAX * 2 * 2],
					float pYt[2 * LTE_PHY_N_ANT_MAX * 2],
					float pHTranspose[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2],
					int NumLayer,
					int NumRxAntenna)
{
	float pXDX[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	MatrixProd(NumLayer, 2, NumLayer, pXtDagger, pXt, pXDX);

	float pInvXDX[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	MatrixInv(NumLayer, pXDX, pInvXDX);

	float pXDY[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	MatrixProd(NumLayer, 2, NumRxAntenna, pXtDagger, pYt, pXDY);

	MatrixProd(NumLayer, NumLayer, NumRxAntenna, pInvXDX, pXDY, pHTranspose);
}

void FDLSEqualization(float pInpData[N_EQ_IN_MAX * 2],
					  float pHTranspose[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2],
					  int m,
					  int NumLayer,
					  int NumRxAntenna,
					  int MDFTPerUser,
					  float pOutData[N_EQ_OUT_MAX * 2])
{
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//////////////////// Freq Domain Equalize received Data /////////////////
	float pH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];
	float pHDagger[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];
	float pHDH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];
	float pInvHDH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	float pHDY[LTE_PHY_N_ANT_MAX * 2];

	for (int nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		for (int layer = 0; layer < NumLayer; layer++)
		{
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
			//	pH[nrx * NumLayer + layer] = pHTranspose[layer * NumRxAntenna + nrx];
			pH[2 * (nrx * NumLayer + layer) + 0] = pHTranspose[2 * (layer * NumRxAntenna + nrx) + 0];
			pH[2 * (nrx * NumLayer + layer) + 1] = pHTranspose[2 * (layer * NumRxAntenna + nrx) + 1];
			//	pHDagger[layer * NumRxAntenna + nrx] = conj(pHTranspose[layer * NumRxAntenna + nrx]);
			pHDagger[2 * (layer * NumRxAntenna + nrx) + 0] = pHTranspose[2 * (layer * NumRxAntenna + nrx) + 0];
			pHDagger[2 * (layer * NumRxAntenna + nrx) + 1] = (-1.0) * pHTranspose[2 * (layer * NumRxAntenna + nrx) + 1];
		}
	}

	MatrixProd(NumLayer, NumRxAntenna, NumLayer, pHDagger, pH, pHDH);
	MatrixInv(NumLayer, pHDH, pInvHDH);

	////////////////// Equalizing Data /////////////////
	for (int nSymb = 0; nSymb < NumULSymbSF - 2; nSymb++)
	{
		float pYData[LTE_PHY_N_ANT_MAX * 2];
		for (int nrx = 0; nrx < NumRxAntenna; nrx++)
		{
#pragma HLS PIPELINE
			int IDX = (NumULSymbSF - 2) * nrx + nSymb + 2 * NumRxAntenna;
			//	*(pYData+nrx)=*(*(pInpData+IDX)+m);
			//	pYData[nrx] = pInpData[IDX * MDFTPerUser + m];
			pYData[2 * nrx + 0] = pInpData[2 * (IDX * MDFTPerUser + m) + 0];
			pYData[2 * nrx + 1] = pInpData[2 * (IDX * MDFTPerUser + m) + 1];
		}
		MatrixProd(NumLayer, NumRxAntenna, 1, pHDagger, pYData, pHDY);

		float pXData[LTE_PHY_N_ANT_MAX];
		MatrixProd(NumLayer, NumLayer, 1, pInvHDH, pHDY, pXData);

		for (int layer = 0; layer < NumLayer; layer++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			int IDX = (NumULSymbSF - 2) * layer + nSymb;
			
			//	*(*(pOutData+IDX)+m)=*(pXData+layer);
			//	pOutData[IDX * MDFTPerUser + m] = pXData[layer];
			pOutData[2 * (IDX * MDFTPerUser + m) + 0] = pXData[2 * layer + 0];
			pOutData[2 * (IDX * MDFTPerUser + m) + 1] = pXData[2 * layer + 1];
		}
	}
}

void LSFreqDomain(float pInpData[N_EQ_IN_MAX * 2], float pOutData[N_EQ_OUT_MAX * 2], int MDFT, int NumLayer, int NumRxAntenna)
{
	float pDMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2];
	//	pDMRS = (*VpUser).GetpDMRS();
	geneDMRS(pDMRS, NumLayer, MDFT);
 
	for (int m = 0; m < MDFT; m++)
	{
		float pXt[2 * LTE_PHY_N_ANT_MAX * 2];
		float pXtDagger[LTE_PHY_N_ANT_MAX * 2 * 2];

		for (int slot = 0; slot < 2; slot++)
		{
			for (int layer = 0; layer < NumLayer; layer++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				//	pXt[slot * NumLayer + layer] = pDMRS[(slot * NumLayer + layer) * MDFT + m];
				pXt[2 * (slot * NumLayer + layer) + 0] = pDMRS[2 * ((slot * NumLayer + layer) * MDFT + m) + 0];
				pXt[2 * (slot * NumLayer + layer) + 1] = pDMRS[2 * ((slot * NumLayer + layer) * MDFT + m) + 1];
				//	pXtDagger[layer * 2 + slot] = conj(pDMRS[(slot * NumLayer + layer) * MDFT + m]);
				pXtDagger[2 * (layer * 2 + slot) + 0] = pDMRS[2 * ((slot * NumLayer + layer) * MDFT + m) + 0];
				pXtDagger[2 * (layer * 2 + slot) + 1] = (-1.0) * pDMRS[2 * ((slot * NumLayer + layer) * MDFT + m) + 1];
			}
		}

		float pYt[2 * LTE_PHY_N_ANT_MAX * 2];

		for (int slot = 0; slot < 2; slot++)
		{
			for (int nrx = 0; nrx < NumRxAntenna; nrx++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				//	pYt[slot * NumRxAntenna + nrx] = pInpData[(nrx * 2 + slot) * MDFT + m];
				pYt[2 * (slot * NumRxAntenna + nrx) + 0] = pInpData[2 * ((nrx * 2 + slot) * MDFT + m) + 0];
				pYt[2 * (slot * NumRxAntenna + nrx) + 1] = pInpData[2 * ((nrx * 2 + slot) * MDFT + m) + 1];
			}
		}

		float pHTranspose[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

		FDLSEstimation(pXt, pXtDagger, pYt, pHTranspose, NumLayer, NumRxAntenna);

		FDLSEqualization(pInpData, pHTranspose, m, NumLayer, NumRxAntenna, MDFT, pOutData); 
	}
}

void Equalizing(float pInpData[N_EQ_IN_MAX * 2], float pOutData[N_EQ_OUT_MAX * 2], int MDFT, int NumLayer, int NumRxAntenna)
{
	//	int MDFT = lte_phy_params->N_dft_sz;
	//	int NumLayer = lte_phy_params->N_tx_ant;
	//	int NumRxAntenna = lte_phy_params->N_rx_ant;
	//LSFreqDomain(pInpData, pOutData, MDFT, NumLayer, NumRxAntenna);
	LSFreqDomain(pInpData, pOutData, 1200, 2, 2);
}
