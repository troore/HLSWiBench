
#ifndef __EQUALIZER_H_
#define __EQUALIZER_H_

#include <complex>
#include <iostream>
#include <cmath>

#include "lte_phy.h"
#include "ResMapper.h"
#include "Matrix.h"

void FDLSEstimation(std::complex<float> pXt[], std::complex<float> pXtDagger[], std::complex<float> pYt[], std::complex<float> pHTranspose[], int NumLayer, int NumRxAntenna);

void FDLSEqualization(std::complex<float> pInpData[], std::complex<float> pHTranspose[], int m, int NumLayer, int NumRxAntenna, int MDFTPerUser, std::complex<float> pOutData[]);

void LSFreqDomain(std::complex<float> pInpData[], std::complex<float> pOutData[], int MDFT, int NumLayer, int NumRxAntenna);

void Equalizing(std::complex<float> pInpData[], std::complex<float> pOutData[], int MDFT, int NumLayer, int NumRxAntenna);

#endif
