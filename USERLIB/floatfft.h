#ifndef _FLOATFFT_H
#define _FLOATFFT_H

#include <math.h>
#include "stdio.h"
#include "sys.h"

#define Ndot 64     /* full length of Sinewave[] */

extern float vx[Ndot];
extern float vy[Ndot];
extern float vk[Ndot / 2];

void float_fft(float *input_dot, int m);
void ifloat_fft(int m);

#endif
