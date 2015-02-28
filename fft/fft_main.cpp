#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fft.h"


#ifdef DEBUG
float v[N][2], v1[N][2], vout[N][2], v1out[N][2];
#else
/*
float v[2 * N], v1[2 * N], vout[2 * N], v1out[2 * N];
*/
float v_real[N], v1_real[N], vout_real[N], v1out_real[N];
float v_imag[N], v1_imag[N], vout_imag[N], v1out_imag[N];
#endif

/* Print a vector of complexes as ordered pairs. */

static void print_vector(
	const char *title,
	float (*x)[2],
	int n)
{
	int i;
	printf("%s (dim=%d):", title, n);

	for (i = 0; i < n; i++ )
		printf(" %f,%f ", x[i][0], x[i][1]);
	
	putchar('\n');
}

static void print_vector(
		const char *title,
		float *x,
		int n)
{
	int i;
	printf("%s (dim=%d):", title, n);

	for (i = 0; i < n; i++ )
		printf(" %f,%f ", x[i], x[i + n]);
	
	putchar('\n');
}

static void print_vector(
	const char *title,
	float *x_real, float *x_imag,
	int n)
{
	int i;
	printf("%s (dim=%d):", title, n);

	for (i = 0; i < n; i++ )
		printf(" %f,%f ", x_real[i], x_imag[i]);
	
	putchar('\n');
}

int main(int argc, char *argv[])
{
	int n;
	int k;

	if (argc < 2)
	{
		printf("Usage: ./dft n\n");
		exit(1);
	}


	n = atoi(argv[1]);
	if (n > N)
	{
		printf("Number of DFT points cannot exceed %d.\n", N);
		exit(1);
	}
	// Fill v[] with a function of known FFT:

	// Fill v[] with a function of known FFT:
	for (k = 0; k < n; k++)
	{
#ifdef DEBUG
		v[k][0] = 0.125 * cos((2 * PI * k ) / (float)n);
		v[k][1] = 0.125 * sin((2 * PI * k) / (float)n);
#else
//			v[k] = 0.125 * cos((2 * PI * k) / (float)N);
//			v[k + N] = 0.125 * sin((2 * PI * k ) / (float)N);
		v_real[k] = 0.125 * cos((2 * PI * k) / (float)n);
		v_imag[k] = 0.125 * sin((2 * PI * k ) / (float)n);
#endif
	//	v1[k][0] =  0.3*cos(2*PI*k/(float)N);
	//	v1[k][1] = -0.3*sin(2*PI*k/(float)N);

	//	fscanf(fptr_real, "%f", &(v[k][0]));
	//	fscanf(fptr_imag, "%f", &(v[k][1]));
	}


#ifdef DEBUG
//	print_vector("Orig", v, n);
#else
//	print_vector("Orig", v_real, v_imag, n);
#endif

#ifdef DEBUG
	fft_recur(n, v, vout, -1);
#else
	fft_nrvs_two_arrays(n, v_real, v_imag, vout_real, vout_imag, -1);
#endif

#ifdef DEBUG
	for (k = 0; k < n; k++)
	{
		vout[k][0] /= n;
		vout[k][1] /= n;
	}
#else
	for (k = 0; k < n; k++)
	{
		//	vout[k] /= N;
		//	vout[k + N] /= N;
		vout_real[k] /= n;
		vout_imag[k] /= n;
	}
#endif

/*
#ifdef DEBUG
	print_vector("iFFT", vout, n);
#else
	print_vector("iFFT", vout_real, vout_imag, n);
#endif

#ifdef DEBUG
	fft_recur(n, vout, v, 1);
#else
	fft_nrvs_two_arrays(n, vout_real, vout_imag, v_real, v_imag, 1);
#endif

#ifdef DEBUG
	print_vector("FFT", v, n);
#else
	print_vector("FFT", v_real, v_imag, n);
#endif
*/
	return 0;
}
