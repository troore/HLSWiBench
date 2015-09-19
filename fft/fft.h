
#ifndef __FFT_H_
#define __FFT_H_

#define N 512
#define PI	3.14159265358979323846264338327950288

void fft_recur(int n, float (*a)[2], float (*y)[2], int direction);
void fft_iter(int n, float *a, float *y, int direction);
void fft_iter_two_arrays(int n, float *a_real, float *a_imag, float *y_real, float *y_imag, int direction);
void fft_nrvs_same_array(int n, float *a, float *y, int direction);
void fft_nrvs_same_array_cyclic(int n, float *a, float *y, int direction);
void fft_nrvs_two_arrays(int n, float *a_real, float *a_imag, float *y_real, float *y_imag, int direction);

#endif
