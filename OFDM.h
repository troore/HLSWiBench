
#ifndef __OFDM_H_
#define __OFDM_H_

#include <complex>
//#include <fftw3.h>

#include <stdio.h>
#include <time.h>
#include "lte_phy.h"
#include "fft.h"

//void ofmodulating(LTE_PHY_PARAMS *lte_phy_params, std::complex<float> *pInpData, std::complex<float> *pOutData);
void ofmodulating(float pInpData[N_OFMOD_IN_MAX * 2],
				  float pOutData[N_OFMOD_OUT_MAX * 2],
				  int NumLayer,
				  int NIFFT,
				  int CPLen);
//void ofdemodulating(LTE_PHY_PARAMS *lte_phy_params, std::complex<float> *pInpData, std::complex<float> *pOutData);
void ofdemodulating(float pInpData[N_OFDEMOD_IN_MAX * 2],
					float pOutData[N_OFDEMOD_OUT_MAX * 2],
					int NumRxAntenna,
					int NIFFT,
					int CPLen);

#endif
