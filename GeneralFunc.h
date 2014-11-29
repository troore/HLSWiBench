
#ifndef _GENERALFUNC_H_
#define _GENERALFUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <complex>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include "gauss.h"
#include "FIFO.h"

using namespace std;

void ReadInputFromFiles(int *pIn,int Sz,const char *name);

void ReadInputFromFiles(float *pIn, int Sz, const char *name);

void ReadInputFromFiles(complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag);
void ReadInputFromFiles(float *pIn, int Sz, const char *nameReal, const char *nameImag);
void ReadInputFromFiles(float (*pIn)[2], int Sz, const char *nameReal, const char *nameImag);

void GeneRandomInput(complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag);
void GeneRandomInput(float (*pIn)[2], int Sz, const char *nameReal, const char *nameImag);

void GeneRandomInput(int *pIn,int Sz,const char *name);
void GeneRandomInput(float *pIn,int Sz,const char *name);

void WriteOutputToFiles(int *pOut, int Sz, const char *name);

void WriteOutputToFiles(float *pOut, int Sz, const char *name);

void WriteOutputToFiles(complex<float> *pOut, int Sz, const char *nameReal, const char *nameImag);
void WriteOutputToFiles(float *pOut, int Sz, const char *nameReal, const char *nameImag);
void WriteOutputToFiles(float (*pOut)[2], int Sz, const char *nameReal, const char *nameImag);

void ReadOutput(FIFO<int> *pOut,int Sz[]);
void ReadOutput(FIFO<float> *pOut,int Sz[]);
void ReadOutput(FIFO<complex<float> > *pOut,int Sz[]);


//void GenerateLTEChainInput(FIFO<int> *pDataSource, int DataK, int *pTxDS);
void GenerateLTEChainInput(int *pDataSource, int DataK, int *pTxDS, int RANDOMSEED);
void ReadLTEChainOutput(int *pFileSink, int *pRxFS, int DataK);

#endif



