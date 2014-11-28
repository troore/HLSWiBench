
#ifndef __MODULATION_H_
#define __MODULATION_H_

#include <complex>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#include "lte_phy.h"

typedef float (*p_a)[2];

float vecmin(float pV[MAX_MOD_TABLE_LEN / 2], int len);
float eudist(float a, float b, float c, float d);
void dec2bits(int i, int n, int *bvec);

void set_mod_params(p_a *pp_table, int *bits_per_samp, int *mod_table_len, int mod_type);
void init_mod_tables();

void Modulating(int pBitsSeq[N_MOD_IN_MAX], std::complex<float> pQAMSeq[N_MOD_OUT_MAX], int in_buf_sz, int mod_type);

void Demodulating(std::complex<float> pDecQAMSeq[N_MOD_OUT_MAX], float pLLR[N_MOD_IN_MAX], int in_buf_sz, int mod_type, float awgnSigma);

void Demodulating(std::complex<float> pDecQAMSeq[N_MOD_OUT_MAX], int pHD[N_MOD_IN_MAX], int in_buf_sz, int mod_type);

#endif
