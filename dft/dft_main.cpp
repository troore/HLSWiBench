
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dft.h"

float v[N * 2], v1[N * 2], vout[N * 2], v1out[N * 2];

/* Print a vector of complexes as ordered pairs. */
	static void
print_vector(
		const char *title,
		float *x,
		int n)
{
	int i;
	printf("%s (dim=%d):", title, n);
	for(i=0; i<n; i++ ) printf(" %f,%f ", x[2 * i + 0], x[2 * i + 1]);
	putchar('\n');
}


int main(int argc, char *argv[])
{
	int k;
	// Fill v[] with a function of known FFT:
	for (k = 0; k < N; k++)
	{
		v[2 * k + 0] = 0.125*cos(2*PI*k/(float)N);
		v[2 * k + 1] = 0.125*sin(2*PI*k/(float)N);
		// v1[k][0] = 0.3*cos(2*PI*k/(float)N);
		// v1[k][1] = -0.3*sin(2*PI*k/(float)N);
		// fscanf(fptr_real, "%f", &(v[k][0]));
		// fscanf(fptr_imag, "%f", &(v[k][1]));
	}
	// v[0]=1;v[1]=0;
	// v[2]=2;v[3]=0;
	// v[4]=4;v[5]=0;
	// v[6]=3;v[7]=0;
	print_vector("Orig", v, N);
	dft(N, v, vout, -1);
	for (k = 0; k < N; k++)
	{
		vout[2 * k + 0] /= N;
		vout[2 * k + 1] /= N;
	}
	print_vector("iFFT", vout, N);
	dft(N, vout, v, 1);
	print_vector(" FFT", v, N);
	return 0;
}
