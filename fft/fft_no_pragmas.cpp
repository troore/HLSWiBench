
#include <math.h>

#include "fft.h"

static int BitReverse(int src, int size)
{
	int tmp = src;
	int des = 0;
	//for (int i=size-1; i>=0; i--)
	for (int i=1; i<12; i++)
	{
		if(i > size)
			break;
		des = ((tmp & 1) << (size - i)) | des;
		tmp = tmp >> 1;
	}
	return des;
}

static inline int pow2(int n)
{
	return 1<<n;
}

static inline int log2(int n)
{
	int i = 0;

	log_loop:while (n > 1)
	{
		n /= 2;
		i++;
	}
	return i;
}

void fft_recur(int n, float (*a)[2], float (*y)[2], int direction)
{
	if (n > 1)
	{			
		/* otherwise, do nothing and return */
		int k;    
		float z[2], w[2];

		for (k = 0; k < n / 2; k++) 
		{
			y[k][0] = a[2 * k][0];
			y[k][1] = a[2 * k][1];
			y[k + n / 2][0] = a[2 * k + 1][0];
			y[k + n / 2][1] = a[2 * k + 1][1];
		}
		for (k = 0; k < n; k++)
		{
			a[k][0] = y[k][0];
			a[k][1] = y[k][1];
		}

		fft_recur(n / 2, a, y, direction);		/* FFT on even-indexed elements of v[] */
		fft_recur(n / 2, a + (n / 2), y + (n / 2), direction);		/* FFT on odd-indexed elements of v[] */

		for (k = 0; k < n; k++)
		{
			a[k][0] = y[k][0];
			a[k][1] = y[k][1];
		}

		for (k = 0; k < (n / 2); k++) 
		{
			w[0] = cos(2 * PI * k / (float)n);
			w[1] = ((float)direction) * sin(2 * PI * k / (float)n);
			z[0] = w[0] * a[n / 2 + k][0] - w[1] * a[n / 2 + k][1];	/* Re(w * y[n / 2 + k]) */
			z[1] = w[0] * a[n / 2 + k][1] + w[1] * a[n / 2 + k][0];	/* Im(w * y[n / 2 + k]) */
			y[k][0] = a[k][0] + z[0];
			y[k][1] = a[k][1] + z[1];
			y[k + n / 2][0] = a[k][0] - z[0];
			y[k + n / 2][1] = a[k][1] - z[1];
		}
	}
}

void assign(float *target, float *source){
	*target = *source;
}

void fft_iter_same_array(int n, float a[N * 2], float y[N * 2], int direction)
{
	int i, j, s, d, k, m;
	float t;
	float wdr, wdi, wr, wi;
	float wtr, wti;
	float tr, ti;
	float xr, xi;

	s = log2(n);
	bitreverse:for (i = 0; i < n; i++)
	{
		j = BitReverse(i, s);
		//	y[2 * j + 0] = a[2 * i + 0];
		//	y[2 * j + 1] = a[2 * i + 1];
		//y[j] = a[i];
		//y[j + n] = a[i + n];
		assign(&y[j],&a[i]);
		assign(&y[j+n],&a[i+n]);
	}

	fft_iteration:for (j = 1; j <= s; j++)
	{
		d  = pow2(j);
		t = (2 * PI) / d;
		wdr = cos(t);
		wdi = ((float)direction) * sin(t);

		wr = 1; wi = 0;
		fft_iteration2:for (k = 0; k < (d / 2); k++)
		{
			fft_iteration3:for (m = k; m < n; m += d)
			{
				/*
				tr = wr*y[2 * (m + d/2) + 0] - wi*y[2 * (m + d/2) + 1];
				ti = wr*y[2 * (m + d/2) + 1] + wi*y[2 * (m + d/2) + 0];
				*/
				tr = wr * y[(m + d / 2)] - wi * y[(m + d / 2) + n];
				ti = wr * y[(m + d / 2) + n] + wi * y[(m + d/2)];
	//			printf("r=%f, i=%f\n", tr, ti);
				xr = y[m];
				xi = y[m + n];
	//			printf("r=%f, i=%f\n", y[2 * m + 0], y[2 * m + 1]);
				/*
				y[2 * m + 0] = xr + tr;
				y[2 * m + 1] = xi + ti;
				*/
				y[m] = xr + tr; y[m + n] = xi + ti;
				/*
				y[2 * (m+d/2) + 0] = xr - tr;
				y[2 * (m+d/2) + 1] = xi - ti;
				*/
				y[m + d / 2] = xr - tr;
				y[m + d / 2 + n] = xi - ti;  
	//			printf("r=%f, i=%f\n", y[2 * m + 0], y[2 * m + 1]);
	//			printf("r=%f, i=%f\n", y[2 * (m + d/2) + 0], y[2 * (m + d/2) + 1]);
			}
			wtr = wr * wdr - wi * wdi;
			wti = wr * wdi + wi * wdr;
			wr = wtr;
			wi = wti;
		}
	}
}


void fft_iter_two_arrays(int n, float a_real[N], float a_imag[N], float y_real[N], float y_imag[N], int direction)
{
	int i, j, s, d, k, m;
	float t;
	float wdr, wdi, wr, wi;
	float wtr, wti;
	float tr, ti;
	float xr, xi;

	s = log2(n);
	iter_different_array_bitreverse_loop:for (i = 0; i < n; i++)
	{
		j = BitReverse(i, s);
		assign(&y_real[j], &a_real[i]);
		assign(&y_imag[j], &a_imag[i]);
		/*
		y_real[j] = a_real[i];
		y_imag[j] = a_imag[i];
		*/
	}

	iter_different_array_calculate_loop:for (j = 1; j <= s; j++)
	{
		d  = pow2(j);
		t = (2 * PI) / d;
		wdr = cos(t);
		wdi = ((float)direction) * sin(t);

		wr = 1; wi = 0;
		for (k = 0; k < (d / 2); k++)
		{
			for (m = k; m < n; m += d)
			{
				tr = wr * y_real[(m + d / 2)] - wi * y_imag[(m + d / 2)];
				ti = wr * y_imag[(m + d / 2)] + wi * y_real[(m + d / 2)];
				xr = y_real[m];
				xi = y_imag[m];
				
				y_real[m] = xr + tr;
				y_imag[m] = xi + ti;

				y_real[m + d / 2] = xr - tr;
				y_imag[m + d / 2] = xi - ti;  
			}
			wtr = wr * wdr - wi * wdi;
			wti = wr * wdi + wi * wdr;
			wr = wtr;
			wi = wti;
		}
	}
}

void fft_nrvs_same_array(int n, float a[N * 2], float y[N * 2], int direction)
{
	int p, i, k;
	int lgn;
	float omega_m[2], omega[2];
	float t[2], u[2];
	float ang;

//	lgn = log2(n);
//	for (i = 0; i < n; i++)
//		y[i] = a[i];

	for (p = 1; p <= (n / 2); p <<= 1)
	{
		//	omega_m[0] = cos((2 * PI) / m);
		//	omega_m[1] = ((float)direction) * sin((2 * PI) / m);
		nrvs_fft_calculate_loop:for (i = 0; i < (n >> 1); i++)
		{
			int o_idx = i + (n >> 1);
			
			k = i & (p - 1); // i % p
			ang = ((2 * PI * k) / (2 * p));
			omega[0] = cos(ang);
			omega[1] = ((float)direction) * sin(ang);
			// t = omega * a[i + n / 2];
			t[0] = omega[0] * a[o_idx] - omega[1] * a[o_idx + n];
			t[1] = omega[0] * a[o_idx + n] + omega[1] * a[o_idx];
			// u = a[i];
			u[0] = a[i];
			u[1] = a[i + n];

			//	y[2 * i - k] = u + t;
			y[2 * i - k] = u[0] + t[0];
			y[2 * i - k + n] = u[1] + t[1];
			//	y[2 * i - k + p] = u - t;
			y[2 * i - k + p] = u[0] - t[0];
			y[2 * i - k + p + n] = u[1] - t[1];
		}
		
		nrvs_same_array_assign:for (i = 0; i < n; i++)
		{
			assign(&a[i], &y[i]);
			assign(&a[i + n], &y[i + n]);
			/*
			a[i] = y[i];
			a[i + n] = y[i + n];
			*/
		}
	}
}

void fft_nrvs_two_arrays(int n, float a_real[N], float a_imag[N],
			  float y_real[N], float y_imag[N],
			  int direction)
{
	int p, i, k;
	int lgn;
	float omega_m[2], omega[2];
	float t[2], u[2];
	float ang;

//	lgn = log2(n);
//	for (i = 0; i < n; i++)
//		y[i] = a[i];

	for (p = 1; p <= (n / 2); p <<= 1)
	{
		//	omega_m[0] = cos((2 * PI) / m);
		//	omega_m[1] = ((float)direction) * sin((2 * PI) / m);
		for (i = 0; i < (n >> 1); i++)
		{
			int o_idx = i + (n >> 1);
			
			k = i & (p - 1); // i % p
			ang = ((2 * PI * k) / (2 * p));
			omega[0] = cos(ang);
			omega[1] = ((float)direction) * sin(ang);
			// t = omega * a[i + n / 2];
			t[0] = omega[0] * a_real[o_idx] - omega[1] * a_imag[o_idx];
			t[1] = omega[0] * a_imag[o_idx] + omega[1] * a_real[o_idx];
			// u = a[i];
			u[0] = a_real[i];
			u[1] = a_imag[i];

			//	y[2 * i - k] = u + t;
			y_real[2 * i - k] = u[0] + t[0];
			y_imag[2 * i - k] = u[1] + t[1];
			//	y[2 * i - k + p] = u - t;
			y_real[2 * i - k + p] = u[0] - t[0];
			y_imag[2 * i - k + p] = u[1] - t[1];
		}
		
		for (i = 0; i < n; i++)
		{
			assign(&a_real[i], &y_real[i]);
			assign(&a_imag[i], &y_imag[i]);
			/*
			a_real[i] = y_real[i];
			a_imag[i] = y_imag[i];
			*/
		}
	}
}


