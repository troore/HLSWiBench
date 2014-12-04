
#include "ResMapper.h"

void geneDMRS(float pDMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2],
			  int N_layer,
			  int N_dft)
{
	int i;
	
	int pPrimeTable[6][2];
	int ncs[2] = {3, 11};
	int Nzc;
	int zc_flag;
	int idx;
	int RSu, RSv;
	double qbar, q;
//	std::complex<double> px[1200];
	double px[1200][2];

	pPrimeTable[0][0] = 75;		pPrimeTable[0][1] = 73;
	pPrimeTable[1][0] = 150;	pPrimeTable[1][1] = 149;
	pPrimeTable[2][0] = 300;	pPrimeTable[2][1] = 293;
	pPrimeTable[3][0] = 600;	pPrimeTable[3][1] = 599;
	pPrimeTable[4][0] = 900;	pPrimeTable[4][1] = 887;
	pPrimeTable[5][0] = 1200;	pPrimeTable[5][1] = 1193;

	zc_flag = 1;
	idx = 0;
	while (zc_flag)
	{
		if (pPrimeTable[idx][0] == N_dft)
		{ 
			Nzc = pPrimeTable[idx][1];
			zc_flag = 0;
		}
		else
		{
			idx++;
		}
	}
	
	float tmp1;
	int tmp2;
	
	RSu = 0;
	RSv = 0;
	qbar = (double)Nzc * (RSu + 1.0) / 31.0;
	tmp1 = floor(2.0 * qbar);
	tmp2 = 1.0;
	for (i = 1; i <= (int)tmp1; i++)
		tmp2 = tmp2 * (-1.0);
	q = floor((qbar + 0.5)) + RSv * /*pow(-1.0,(floor((2.0*qbar))))*/ tmp2;


	for (int m = 0;m < Nzc; m++)
	{
		double theta = -PI * q * m * (m + 1.0)/((double)Nzc);
		//	px[m] = std::complex<double>(cos(theta), sin(theta));
		px[m][0] = cos(theta);
		px[m][1] = sin(theta);
	}

	for (int slot = 0; slot < 2; slot++)
	{
		for (int layer = 0; layer < N_layer; layer++)
		{
			int cs = ncs[slot] + 6 * layer;
			double alpha = 2.0 * PI * cs / 12.0;
			
			for (int n = 0; n < N_dft; n++)
			{
				int idx = n % Nzc;
				double a = cos(alpha * n);
				double b = sin(alpha * n);
				//	std::complex<double> c = std::complex<double>((cos(alpha*n)),(sin(alpha*n)));
				//	std::complex<double> c = std::complex<double>(a, b);
				double c[2] = {a, b};
				//	*(*(*(pDMRS+slot)+layer)+n)=(complex<float>)(c * px[idx]);
				//	pDMRS[(slot * N_layer + layer) * N_dft + n] = (std::complex<float>)(c * px[idx]);
				pDMRS[((slot * N_layer + layer) * N_dft + n) * 2 + 0] = c[0] * px[idx][0] - c[1] * px[idx][1];
				pDMRS[((slot * N_layer + layer) * N_dft + n) * 2 + 1] = c[0] * px[idx][1] + c[1] * px[idx][0];
			}
		}
	}

	if (N_layer == 2)
	{
		for (int n = 0; n < N_dft; n++)
		{
			//	(*(*(*(pDMRS+1)+1)+n))*=(-1.0);
			//	pDMRS[(1 * N_layer + 1) * N_dft + n] *= (-1.0);
			pDMRS[2 * ((1 * N_layer + 1) * N_dft + n) + 0] = -1.0 * pDMRS[2 * ((1 * N_layer + 1) * N_dft + n) + 0];
			pDMRS[2 * ((1 * N_layer + 1) * N_dft + n) + 1] = -1.0 * pDMRS[2 * ((1 * N_layer + 1) * N_dft + n) + 1];
		}
	}
	else
	{}
}

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
