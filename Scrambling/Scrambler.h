
#ifndef __SCRAMBLER_H_
#define __SCRAMBLER_H_

#include <stdio.h>
#include <stdlib.h>

#include "lte_phy.h"

void GenScrambInt(int pScrambInt[], int n);
void Scrambling(int pInpSeq[N_SCRAMB_IN_MAX], int pOutSeq[N_SCRAMB_OUT_MAX], int n);

void Descrambling(float pInpSeq[N_SCRAMB_IN_MAX], float pOutBuf[N_SCRAMB_OUT_MAX], int n);

#endif
