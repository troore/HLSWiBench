
#include "dmrs.h"
#include "lte_phy.h"

void SubCarrierMapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	float pInpData[N_RESMAPPER_IN_MAX * 2],
					   float pOutData[N_RESMAPPER_OUT_MAX * 2],
	int dmrs_symb_pos[2],
	int NumLayer,
	int SCLoc,
	int NIFFT,
	int MDFT)
{
//	int NumLayer = lte_phy_params->N_tx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//	int SCLoc = lte_phy_params->sc_loc;
//	int NIFFT = lte_phy_params->N_fft_sz;
//	int MDFT = lte_phy_params->N_dft_sz;

	float DMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2];

	geneDMRS(DMRS, NumLayer, MDFT);

	for (int nlayer = 0; nlayer < NumLayer; nlayer++)
	{
		for (int nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int SymIdx = nlayer * NumULSymbSF + nsym;

			for (int n = 0; n < NIFFT; n++)
			{
				//	pOutData[SymIdx * NIFFT + n] = std::complex<float>(1.0, 0.0);
				pOutData[2 * (SymIdx * NIFFT + n) + 0] = 1.0;
				pOutData[2 * (SymIdx * NIFFT + n) + 1] = 0.0;
			}
		}
	}

	for (int nlayer = 0; nlayer < NumLayer; nlayer++)
	{
		int DMRSslot = 0;
			
		for (int nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int SymIdx = nlayer * NumULSymbSF + nsym;
				
			if (nsym == /*lte_phy_params->*/dmrs_symb_pos[DMRSslot])
			{
				for (int n = SCLoc; n < SCLoc + MDFT; n++)
				{
					//	pOutData[SymIdx * NIFFT + n] = *(*(*(VpDMRS + DMRSslot) + nlayer) + n - SCLoc);
					//	pOutData[SymIdx * NIFFT + n] = DMRS[(DMRSslot * NumLayer + nlayer) * MDFT + (n - SCLoc)];
					pOutData[2 * (SymIdx * NIFFT + n) + 0] = DMRS[2 * ((DMRSslot * NumLayer + nlayer) * MDFT + (n - SCLoc)) + 0];
					pOutData[2 * (SymIdx * NIFFT + n) + 1] = DMRS[2 * ((DMRSslot * NumLayer + nlayer) * MDFT + (n - SCLoc)) + 1];
				}
				DMRSslot++;
			}
			else
			{

				for (int n = SCLoc; n < SCLoc + MDFT; n++)
				{
					//	*(*(pOutData+SymIdx)+n)=*(*(pInpData+nlayer*(NumULSymbSF-2)+nsym-DMRSslot)+n-SCLoc);
					//	pOutData[SymIdx * NIFFT + n] = pInpData[nlayer * (NumULSymbSF - 2) * MDFT + (nsym - DMRSslot) * MDFT + n - SCLoc];
					pOutData[2 * (SymIdx * NIFFT + n) + 0] = pInpData[2 * (nlayer * (NumULSymbSF - 2) * MDFT + (nsym - DMRSslot) * MDFT + n - SCLoc) + 0];
					pOutData[2 * (SymIdx * NIFFT + n) + 1] = pInpData[2 * (nlayer * (NumULSymbSF - 2) * MDFT + (nsym - DMRSslot) * MDFT + n - SCLoc) + 1];
				}
			}
		}
	}
}

void SubCarrierDemapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	float pInpData[N_RESDEMAPPER_IN_MAX * 2],
	float pOutData[N_RESDEMAPPER_OUT_MAX * 2],
	int dmrs_symb_pos[2],
	int NumRxAntenna,
	int SCLoc,
	int NIFFT,
	int MDFT)
{
//	int SCLoc = lte_phy_params->sc_loc;
//	int NumRxAntenna = lte_phy_params->N_rx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//	int NIFFT = lte_phy_params->N_fft_sz;
//	int MDFT = lte_phy_params->N_dft_sz;

	//////////// Get DMRS /////////////
	for (int nrs = 0; nrs < 2; nrs++)
	{
		int DMRSPos = /*lte_phy_params->*/dmrs_symb_pos[nrs];
			
		for (int nrx = 0; nrx < NumRxAntenna; nrx++)
		{
			int SymIdxIn = nrx * NumULSymbSF + DMRSPos;
			int SymIdxOut= nrx * 2 + nrs;

			for (int n = 0; n < MDFT; n++)
			{
				//	pOutData[SymIdxOut * MDFT + n] = pInpData[SymIdxIn * NIFFT + (n + SCLoc)];
				pOutData[2 * (SymIdxOut * MDFT + n) + 0] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 0];
				pOutData[2 * (SymIdxOut * MDFT + n) + 1] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 1];
			}
		}
	}
		
	//////////// Get Data /////////////
	for (int nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		int SymOutIdx = 0;
		for(int nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int SymIdxIn = NumULSymbSF * nrx + nsym;
			if(((nsym == /*lte_phy_params->*/dmrs_symb_pos[0]) || (nsym == /*lte_phy_params->*/dmrs_symb_pos[1])))
			{}
			else
			{
				int SymOutT = (NumULSymbSF - 2) * nrx + SymOutIdx+NumRxAntenna * 2;

				for (int n = 0; n < MDFT; n++)
				{
					//	pOutData[SymOutT * MDFT + n] = pInpData[SymIdxIn * NIFFT + (n + SCLoc)];
					pOutData[2 * (SymOutT * MDFT + n) + 0] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 0];
					pOutData[2 * (SymOutT * MDFT + n) + 1] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 1];
				}
				SymOutIdx++;
			}
		}
	}
}
