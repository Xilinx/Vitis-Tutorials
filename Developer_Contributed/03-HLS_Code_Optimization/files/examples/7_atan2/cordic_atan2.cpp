/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include "cordic.h"
/*
 Gain=1.647;
 mode 1: rotation;  Rotates (x0,y0) by z0
 (xn,yn)=(cos(z0),sin(z0)) for (x0,y0)=(1,0)
 x[i+1] = x[i] - y[i] * d[i] * 2^(-1)
 y[i+1] = y[i] + x[i] * d[i] * 2^(-i)
 z[i+1] = z[i]        - d[i] * atan(2^(-i))
 mode 0: vectoring; Rotates (x0,y0) to X-axis
 (xn,zn)=(r,theta)
 -PI/2<z0<PI/2
 */

//void cordic( bool mode, coord_t x0, coord_t y0, phase_t z0, coord_t *xn, coord_t *yn, phase_t *zn)
void cordic_atan2(coord_t y0, coord_t x0, phase_t *zn)

{
	coord_t x, y, xp, yp;
	phase_t z, zp;
	bool xneg, yneg, dneg;
	uint6_t i;

	const phase_t cord_M_PI = 3.14159265358979323846;
	const phase_t cord_M_PI_2 = 1.57079632679489661923;
	// phase_t cord_M_PI_4 = 0.785398163397448309616;

	static const phase_t atan_2Mi[] = { 0.7854, 0.4636, 0.2450, 0.1244, 0.0624,
			0.0312, 0.0156, 0.0078, 0.0039 };

#ifndef BIT_ACCURATE
	static const coord_t lut_pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
#endif

	if (x0 < 0)
		x = -x0;
	else
		x = x0;
	if (y0 < 0)
		y = -y0;
	else
		y = y0;
	z = 0;

	if (y0 == 0) // SPECIAL CASE Y==0
	{
		if (x0 < 0)
			*zn = cord_M_PI;
		else
			*zn = 0;

		return;
	}

	if (x0 == 0) // SPECIAL CASE X==0
	{
		if (y0 < 0)
			*zn = -cord_M_PI_2;
		else
			*zn = cord_M_PI_2;
		return;
	}

	unsigned char quadrant = 0;
	if ((x0 > 0) & (y0 > 0))
		quadrant = 1;
	else if ((x0 < 0) & (y0 > 0))
		quadrant = 2;
	else if ((x0 < 0) & (y0 < 0))
		quadrant = 3;
	else if ((x0 > 0) & (y0 < 0))
		quadrant = 4;

	LOOP1: for (i = 0; i <= ROT; i++)
	{

	#pragma HLS PIPELINE II=1

		//dneg= (mode==1) ? (z<0) : (y>0);
		dneg = (y > 0);
		if (dneg) {
#ifdef BIT_ACCURATE
			xp = x + (y >> i);
			yp = y - (x >> i);
#else
			xp = x + y/lut_pow2[i]; //x+(y>>i);
			yp = y - x/lut_pow2[i];//y-(x>>i);
#endif
			zp = z + atan_2Mi[i];
		} else {
#ifdef BIT_ACCURATE
			xp = x - (y >> i);
			yp = y + (x >> i);
#else
			xp = x-y/lut_pow2[i]; //x - (y>>i);
			yp = y+x/lut_pow2[i];//y + (x>>i);
#endif
			zp = z - atan_2Mi[i];
		}
		x = xp;
		y = yp;
		z = zp;
	}

	if (quadrant == 1) {
		//*xn=x;
		//*yn=y;
		*zn = z;
	} else if (quadrant == 2) {
		//*xn=x;
		//*yn=-y;
		*zn = cord_M_PI - z;
	} else if (quadrant == 3) {
		//*xn=-x;
		//*yn=-y;
		*zn = z - cord_M_PI;
	} else if (quadrant == 4) {
		//*xn=-x;
		//*yn=-y;
		*zn = -z;
	}
}

void top_atan2(coord_t y0, coord_t x0, phase_t *zn)
{
#if defined(DB_DOUBLE_PRECISION)
	*zn = atan2( (double) y0, (double) x0);
#elif	defined(DB_SINGLE_PRECISION)
	*zn = atan2f( (float) y0, (float) x0);
#elif defined(DB_CORDIC)
	cordic_atan2(y0, x0, zn);
#else
#error <UNDEFINED ATAN2 METHOD!>
#endif
}
