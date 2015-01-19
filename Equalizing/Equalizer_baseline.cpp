#include <stdio.h>
#include "Equalizer.h"
#include "dmrs/dmrs.h"

//#include <stdio.h>

static void MatrixProd(int d1, int d2, int d3, float M1[], float M2[], float oM[])
{
	int r, c, i;

	for (r = 0; r < d1; r++)
	{
		for (c = 0; c < d3; c++)
		{
			float tmp[2] = {0.0, 0.0};

			for (i = 0; i < d2; i++)
			{
				//	tmp += M1[r * d2 + i] * M2[i * d3 + c];
				tmp[0] += (M1[2 * (r * d2 + i) + 0] * M2[2 * (i * d3 + c) + 0] - M1[2 * (r * d2 + i) + 1] * M2[2 * (i * d3 + c) + 1]);
				tmp[1] += (M1[2 * (r * d2 + i) + 0] * M2[2 * (i * d3 + c) + 1] + M1[2 * (r * d2 + i) + 1] * M2[2 * (i * d3 + c) + 0]);
			}

			oM[2 * (r * d3 + c) + 0] = tmp[0];
			oM[2 * (r * d3 + c) + 1] = tmp[1];
		}
	}
}


static void MatrixInv(int sz, float pM[], float pInvM[])
{
	float pX[LTE_PHY_N_ANT_MAX * 2 * LTE_PHY_N_ANT_MAX * 2];

	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
			//	pX[r * (2 * sz) + c] = pM[r * sz + c];
			pX[2 * (r * (2 * sz) + c) + 0] = pM[2 * (r * sz + c) + 0];
			pX[2 * (r * (2 * sz) + c) + 1] = pM[2 * (r * sz + c) + 1];
		}
		for (int c = sz; c < 2 * sz; c++)
		{
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

	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
			float A[2], B[2];
			//	pCurRow[c] = pX[r * (2 * sz) + c] / pX[r * (2 * sz) + r];
			A[0] = pX[2 * (r * (2 * sz) + c) + 0];
			A[1] = pX[2 * (r * (2 * sz) + c) + 1];
			B[0] = pX[2 * (r * (2 * sz) + r) + 0];
			B[1] = pX[2 * (r * (2 * sz) + r) + 1];
			pCurRow[2 * c + 0] = (A[0] * B[0] + A[1] * B[1]) / (B[0] * B[0] + B[1] * B[1]);
			pCurRow[2 * c + 1] = (A[1] * B[0] - A[0] * B[1]) / (B[0] * B[0] + B[1] * B[1]);
		}

		for (int c = 0; c < (2 * sz); c++)
		{
			//	pX[r * (2 * sz) + c] = pCurRow[c];
			pX[2 * (r * (2 * sz) + c) + 0] = pCurRow[2 * c + 0];
			pX[2 * (r * (2 * sz) + c) + 1] = pCurRow[2 * c + 1];
		}

		for (int er = r + 1; er < sz; er++)
		{
			float curC[2] = {pX[2 * (er * (2 * sz) + r) + 0], pX[2 * (er * (2 * sz) + r) + 1]};

			for (int c = 0; c < (2 * sz); c++)
			{
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pX[2 * (er * (2 * sz) + c) + 0] -= (curC[0] * pCurRow[2 * c + 0] - curC[1] * pCurRow[2 * c + 1]);
				pX[2 * (er * (2 * sz) + c) + 1] -= (curC[0] * pCurRow[2 * c + 1] + curC[1] * pCurRow[2 * c + 0]);
			}  
		}
	}

	for (int r = sz - 1; r >= 0; r--)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
			//	pCurRow[c] = pX[r * (2 * sz) + c];
			pCurRow[2 * c + 0] = pX[2 * (r * (2 * sz) + c) + 0];
			pCurRow[2 * c + 1] = pX[2 * (r * (2 * sz) + c) + 1];
		}

		for (int er = r - 1; er >= 0; er--)
		{
			float curC[2] = {pX[2 * (er * (2 * sz) + r) + 0], pX[2 * (er * (2 * sz) + r) + 1]};

			for (int c = 0; c < (2 * sz); c++)
			{
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pX[2 * (er * (2 * sz) + c) + 0] -= (curC[0] * pCurRow[2 * c + 0] - curC[1] * pCurRow[2 * c + 1]);
				pX[2 * (er * (2 * sz) + c) + 1] -= (curC[0] * pCurRow[2 * c + 1] + curC[1] * pCurRow[2 * c + 0]);
			}  
		}
	}

	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
			int col = c + sz;

			//	pInvM[r * sz + c] = pX[r * (2 * sz) + col];
			pInvM[2 * (r * sz + c) + 0] = pX[2 * (r * (2 * sz) + col) + 0];
			pInvM[2 * (r * sz + c) + 1] = pX[2 * (r * (2 * sz) + col) + 1];
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
			int IDX = (NumULSymbSF - 2) * nrx + nSymb + 2 * NumRxAntenna;
			//	*(pYData+nrx)=*(*(pInpData+IDX)+m);
			//	pYData[nrx] = pInpData[IDX * MDFTPerUser + m];
			pYData[2 * nrx + 0] = pInpData[2 * (IDX * MDFTPerUser + m) + 0];
			pYData[2 * nrx + 1] = pInpData[2 * (IDX * MDFTPerUser + m) + 1];
		}
		MatrixProd(NumLayer, NumRxAntenna, 1, pHDagger, pYData, pHDY);

		float pXData[LTE_PHY_N_ANT_MAX];
		MatrixProd(NumLayer, NumLayer, 1, pInvHDH, pHDY, pXData);

		/*
		/////////////////////// Get EqW ////////////////////////
#ifdef DEBUG_EQ
std::complex<float> **pW = new std::complex<float>*[NumLayer];
for(int layer=0;layer<NumLayer;layer++){*(pW+layer)=new std::complex<float>[NumRxAntenna];}
		//	MatrixProd<int,std::complex<float> >(NumLayer,NumLayer,1,pInvHDH,pHDagger,pW);
		MatrixProd<int,std::complex<float> >(NumLayer,NumLayer,NumLayer,pInvHDH,pHDagger,pW);
#else
std::complex<float> pW[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
MatrixProd(NumLayer, NumLayer, NumLayer, pInvHDH, pHDagger, pW);
#endif
for (int layer = 0; layer < NumLayer; layer++)
{
for (int nrx = 0; nrx < NumRxAntenna; nrx++)
{
#ifdef DEBUG_EQ
		 *(*(*(pEqW+m)+layer)+nrx)=*(*(pW+layer)+nrx);
#else
pEqW[m * NumLayer * NumRxAntenna + layer * NumRxAntenna + nrx] = pW[layer * NumRxAntenna + nrx];
#endif
}
}   
#ifdef DEBUG_EQ
for(int layer=0;layer<NumLayer;layer++){delete[] *(pW+layer);}
delete[] pW;
#endif
		////////////////////////END Get EqW/////////////////////
		//////////////////////// Get pHdm ////////////////////////
		for(int nrx=0;nrx<NumRxAntenna;nrx++)
		{
		for(int layer=0;layer<NumLayer;layer++)
		{*(*(*(pHdm+m)+nrx)+layer)=*(*(pH+nrx)+layer);}
		}
		/////////////////////// END Get pHdm /////////////////////
		*/

		for (int layer = 0; layer < NumLayer; layer++)
{
	int IDX = (NumULSymbSF - 2) * layer + nSymb;

	//	*(*(pOutData+IDX)+m)=*(pXData+layer);
	//	pOutData[IDX * MDFTPerUser + m] = pXData[layer];
	pOutData[2 * (IDX * MDFTPerUser + m) + 0] = pXData[2 * layer + 0];
	pOutData[2 * (IDX * MDFTPerUser + m) + 1] = pXData[2 * layer + 1];
}
}
}

/*
/////////////////////////Frequency Domain MMSE Equalization///////////////////////////////////
////////////////////////////////////////////////////////////
void FDMMSEEqualization(std::complex<float> *pInpData, std::complex<float>** pHTranspose, int m, int NumLayer, int NumRxAntenna, int MDFTPerUser, float No, std::complex<float> *pOutData)
{
int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//////////////////// Freq Domain Equalize received Data /////////////////
std::complex<float>** pH=new std::complex<float>*[NumRxAntenna];
for(int nrx=0;nrx<NumRxAntenna;nrx++){*(pH+nrx)=new std::complex<float>[NumLayer];}
std::complex<float>** pHDagger=new std::complex<float>*[NumLayer];
for(int layer=0;layer<NumLayer;layer++){*(pHDagger+layer)=new std::complex<float>[NumRxAntenna];}
std::complex<float>** pHHD=new std::complex<float>*[NumRxAntenna];
for(int nrx=0;nrx<NumRxAntenna;nrx++){*(pHHD+nrx)=new std::complex<float>[NumRxAntenna];}
std::complex<float>** pInvHHDN=new std::complex<float>*[NumRxAntenna];
for(int nrx=0;nrx<NumRxAntenna;nrx++){*(pInvHHDN+nrx)=new std::complex<float>[NumRxAntenna];}

std::complex<float>** pEo=new std::complex<float>*[NumLayer];
for(int layer=0;layer<NumLayer;layer++){*(pEo+layer)=new std::complex<float>[NumRxAntenna];}

for(int nrx=0;nrx<NumRxAntenna;nrx++)
{
for(int layer=0;layer<NumLayer;layer++)
{
 *(*(pH+nrx)+layer)=*(*(pHTranspose+layer)+nrx);
 *(*(pHDagger+layer)+nrx)=conj((*(*(pHTranspose+layer)+nrx)));
 }
 }

 MatrixProd<int,std::complex<float> >(NumRxAntenna,NumLayer,NumRxAntenna,pH,pHDagger,pHHD);

 for(int nrx=0;nrx<NumRxAntenna;nrx++)
 {(*(*(pHHD+nrx)+nrx))+=No;}   

 MatrixInv<int,std::complex<float> >(NumRxAntenna,pHHD,pInvHHDN);
 MatrixProd<int,std::complex<float> >(NumLayer,NumRxAntenna,NumRxAntenna,pHDagger,pInvHHDN,pEo);
/////////////////////// Get EqW ////////////////////////
for(int layer=0;layer<NumLayer;layer++)
{
for(int nrx=0;nrx<NumRxAntenna;nrx++)
{*(*(*(pEqW+m)+layer)+nrx)=*(*(pEo+layer)+nrx);}
}   
////////////////////////END Get EqW/////////////////////
//////////////////////// Get pHdm ////////////////////////
for(int nrx=0;nrx<NumRxAntenna;nrx++)
{
for(int layer=0;layer<NumLayer;layer++)
{*(*(*(pHdm+m)+nrx)+layer)=*(*(pH+nrx)+layer);}
}
/////////////////////// END Get pHdm /////////////////////

////////////////// Equalizing Data /////////////////
for(int nSymb=0;nSymb<NumULSymbSF-2;nSymb++)
{
std::complex<float>* pYData=new std::complex<float>[NumRxAntenna];
for(int nrx=0;nrx<NumRxAntenna;nrx++)
{
int IDX=(NumULSymbSF-2)*nrx+nSymb+2*NumRxAntenna;
//	*(pYData+nrx)=*(*(pInpData+IDX)+m);
 *(pYData + nrx) = pInpData[IDX * MDFTPerUser + m];
 }

 std::complex<float>* pXData=new std::complex<float>[NumLayer];
 MatrixProd<int,std::complex<float> >(NumLayer,NumRxAntenna,1,pEo,pYData,pXData);


 for(int layer=0;layer<NumLayer;layer++)
 {
 int IDX = (NumULSymbSF-2)*layer+nSymb;
//	*(*(pOutData+IDX)+m)=*(pXData+layer);
pOutData[IDX * MDFTPerUser + m] = *(pXData + layer);
}

delete[] pYData;
delete[] pXData;
}
//////////////// END Equalizing Data ///////////////  

for(int nrx=0;nrx<NumRxAntenna;nrx++){delete[] *(pH+nrx); delete[] *(pHHD+nrx); delete[] *(pInvHHDN+nrx);}
delete[] pH; delete[] pHHD; delete[] pInvHHDN;
for(int layer=0;layer<NumLayer;layer++)
{delete[] *(pHDagger+layer);delete[] *(pEo+layer);}
delete[] pHDagger;delete[] pEo;
//////////////////// END Freq Domain Equalize received Data//////////////////

}
*/

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
	//printf("%d, %d, %d\n", MDFT, NumLayer, NumRxAntenna);
	LSFreqDomain(pInpData, pOutData, MDFT, NumLayer, NumRxAntenna);
	//LSFreqDomain(pInpData, pOutData, 1200, 2, 2);
}
