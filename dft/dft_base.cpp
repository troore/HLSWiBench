#include <math.h>
#include "dft.h"
/*
   Discrete Fourier Transform
 */
//void dft(int n, float a[N], float y[N], int direction)
void dft(int n, float a[], float y[], int direction)
{
	int k, j;
	float ang;
	float p[2];
	float pre_ang = (float)direction * TWO_PI / (float)n;
	dft_loop1:for (k = 0; k < /*n*/75; k++)
	{
		p[0] = 0.0;
		p[1] = 0.0;
		ang = pre_ang * ((float)k);
		dft_loop2:for (j = 0; j < /*n*/75; j++)
		{
			p[0] += (a[2 * j] * cosf(j * ang) - a[2 * j + 1] * sinf(j * ang));
			p[1] += (a[2 * j] * sinf(j * ang) - a[2 * j + 1] * cosf(j * ang));
		}
		y[2 * k + 0] = p[0];
		y[2 * k + 1] = p[1];
	}
}

