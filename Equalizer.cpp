
#include "Equalizer.h"

#include <stdio.h>

std::complex<float> pEqW[LTE_PHY_DFT_SIZE_MAX * LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
std::complex<float> pHdm[LTE_PHY_DFT_SIZE_MAX * LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

void FDLSEstimation(std::complex<float> pXt[2 * LTE_PHY_N_ANT_MAX],
					std::complex<float> pXtDagger[LTE_PHY_N_ANT_MAX * 2],
					std::complex<float> pYt[2 * LTE_PHY_N_ANT_MAX],
					std::complex<float> pHTranspose[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX],
					int NumLayer,
					int NumRxAntenna)
{
	std::complex<float> pXDX[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	MatrixProd(NumLayer, 2, NumLayer, pXtDagger, pXt, pXDX);

	std::complex<float> pInvXDX[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	MatrixInv(NumLayer, pXDX, pInvXDX);

	std::complex<float> pXDY[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	MatrixProd(NumLayer, 2, NumRxAntenna, pXtDagger, pYt, pXDY);

	MatrixProd(NumLayer, NumLayer, NumRxAntenna, pInvXDX, pXDY, pHTranspose);
}

void FDLSEqualization(std::complex<float> pInpData[N_EQ_IN_MAX],
					  std::complex<float> pHTranspose[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX],
					  int m,
					  int NumLayer,
					  int NumRxAntenna,
					  int MDFTPerUser,
					  std::complex<float> pOutData[N_EQ_OUT_MAX])
{
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//////////////////// Freq Domain Equalize received Data /////////////////
	std::complex<float> pH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	std::complex<float> pHDagger[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	std::complex<float> pHDH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	std::complex<float> pInvHDH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	std::complex<float> pHDY[LTE_PHY_N_ANT_MAX];

	for (int nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		for (int layer = 0; layer < NumLayer; layer++)
		{
			pH[nrx * NumLayer + layer] = pHTranspose[layer * NumRxAntenna + nrx];
			pHDagger[layer * NumRxAntenna + nrx] = conj(pHTranspose[layer * NumRxAntenna + nrx]);
		}
	}

	MatrixProd(NumLayer, NumRxAntenna, NumLayer, pHDagger, pH, pHDH);
	MatrixInv(NumLayer, pHDH, pInvHDH);

	////////////////// Equalizing Data /////////////////
	for(int nSymb=0;nSymb<NumULSymbSF-2;nSymb++)
	{
		std::complex<float> pYData[LTE_PHY_N_ANT_MAX];

		for(int nrx=0;nrx<NumRxAntenna;nrx++)
		{
			int IDX=(NumULSymbSF-2)*nrx+nSymb+2*NumRxAntenna;
			//	*(pYData+nrx)=*(*(pInpData+IDX)+m);
			pYData[nrx] = pInpData[IDX * MDFTPerUser + m];
		}
		MatrixProd(NumLayer, NumRxAntenna, 1, pHDagger, pYData, pHDY);

		std::complex<float> pXData[LTE_PHY_N_ANT_MAX];
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
			int IDX = (NumULSymbSF-2)*layer+nSymb;
			
			//	*(*(pOutData+IDX)+m)=*(pXData+layer);
			pOutData[IDX * MDFTPerUser + m] = pXData[layer];
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

void LSFreqDomain(std::complex<float> pInpData[N_EQ_IN_MAX], std::complex<float> pOutData[N_EQ_OUT_MAX], int MDFT, int NumLayer, int NumRxAntenna)
{
	std::complex<float> pDMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX];
//	pDMRS = (*VpUser).GetpDMRS();
	geneDMRS(pDMRS, NumLayer, MDFT);
  
	for(int m=0;m<MDFT;m++)
	{
		std::complex<float> pXt[2 * LTE_PHY_N_ANT_MAX];
		std::complex<float> pXtDagger[LTE_PHY_N_ANT_MAX * 2];

		for(int slot=0;slot<2;slot++)
		{
			for(int layer=0;layer<NumLayer;layer++)
			{
				pXt[slot * NumLayer + layer] = pDMRS[(slot * NumLayer + layer) * MDFT + m];
				pXtDagger[layer * 2 + slot] = conj(pDMRS[(slot * NumLayer + layer) * MDFT + m]);
			}
		}

		std::complex<float> pYt[2 * LTE_PHY_N_ANT_MAX];

		for(int slot=0;slot<2;slot++)
		{
			for(int nrx=0;nrx<NumRxAntenna;nrx++)
			{
				pYt[slot * NumRxAntenna + nrx] = pInpData[(nrx * 2 + slot) * MDFT + m];
			}
		}

		std::complex<float> pHTranspose[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

		FDLSEstimation(pXt, pXtDagger, pYt, pHTranspose, NumLayer, NumRxAntenna);

		FDLSEqualization(pInpData, pHTranspose, m, NumLayer, NumRxAntenna, MDFT, pOutData); 
	}
}

void Equalizing(std::complex<float> pInpData[N_EQ_IN_MAX], std::complex<float> pOutData[N_EQ_OUT_MAX], int MDFT, int NumLayer, int NumRxAntenna)
{
//	int MDFT = lte_phy_params->N_dft_sz;
//	int NumLayer = lte_phy_params->N_tx_ant;
//	int NumRxAntenna = lte_phy_params->N_rx_ant;
	
	LSFreqDomain(pInpData, pOutData, MDFT, NumLayer, NumRxAntenna);
}
