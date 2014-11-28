
#ifndef __RESMAPPER_H_
#define __RESMAPPER_H_

#include <complex>

#include "lte_phy.h"

#define PI 3.14159265358979

void geneDMRS(std::complex<float> pDMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX], int N_layer, int N_dft);

void SubCarrierMapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	std::complex<float> pInpData[N_RESMAPPER_IN_MAX],
	std::complex<float> pOutData[N_RESMAPPER_OUT_MAX],
	int dmrs_symb_pos[2],
	int NumLayer,
	int SCLoc,
	int NIFFT,
	int MDFT);

void SubCarrierDemapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	std::complex<float> pInpData[N_RESDEMAPPER_IN_MAX],
	std::complex<float> pOutData[N_RESDEMAPPER_OUT_MAX],
	int dmrs_symb_pos[2],
	int NumRxAntenna,
	int SCLoc,
	int NIFFT,
	int MDFT);

#endif
