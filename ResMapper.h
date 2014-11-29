
#ifndef __RESMAPPER_H_
#define __RESMAPPER_H_

#include "lte_phy.h"

#include "math.h"

#define PI 3.14159265358979

void geneDMRS(float pDMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2], int N_layer, int N_dft);

void SubCarrierMapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	float pInpData[N_RESMAPPER_IN_MAX * 2],
	float pOutData[N_RESMAPPER_OUT_MAX * 2],
	int dmrs_symb_pos[2],
	int NumLayer,
	int SCLoc,
	int NIFFT,
	int MDFT);

void SubCarrierDemapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	float pInpData[N_RESDEMAPPER_IN_MAX * 2],
	float pOutData[N_RESDEMAPPER_OUT_MAX * 2],
	int dmrs_symb_pos[2],
	int NumRxAntenna,
	int SCLoc,
	int NIFFT,
	int MDFT);

#endif
