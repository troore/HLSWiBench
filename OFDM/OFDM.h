
#ifndef __OFDM_H_
#define __OFDM_H_

#include "lte_phy.h"

void ofmodulating(LTE_PHY_PARAMS *lte_phy_params, float pInpData[2 * LTE_PHY_FFT_SIZE_MAX], float pOutData[2 * LTE_PHY_FFT_SIZE_MAX]);
void ofmodulating_two_arrays(LTE_PHY_PARAMS *lte_phy_params, float pInpDataReal[LTE_PHY_FFT_SIZE_MAX], float pInpDataImag[LTE_PHY_FFT_SIZE_MAX],
				  float pOutDataReal[LTE_PHY_FFT_SIZE_MAX], float pOutDataImag[LTE_PHY_FFT_SIZE_MAX]);
void ofdemodulating(LTE_PHY_PARAMS *lte_phy_params, float pInpData[2 * LTE_PHY_FFT_SIZE_MAX], float pOutData[2 * LTE_PHY_FFT_SIZE_MAX]);
void ofdemodulating(LTE_PHY_PARAMS *lte_phy_params, float pInpDataReal[LTE_PHY_FFT_SIZE_MAX], float pInpDataImag[LTE_PHY_FFT_SIZE_MAX],
					float pOutDataReal[LTE_PHY_FFT_SIZE_MAX], float pOutDataImag[LTE_PHY_FFT_SIZE_MAX]);

#endif
