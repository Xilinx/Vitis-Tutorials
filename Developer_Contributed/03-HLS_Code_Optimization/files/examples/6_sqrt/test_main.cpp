/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cordic_defines.h"

#define AMP 32768


int main(void)
{

	int ret_val = 0;
	float rel_err;
	unsigned long int i, j;
	int max_err, tot_err;

	dinp_t x, y;

	dinp_t *real, *imag;
	dout_t  *magn, *ref;

	int d;

	FILE *fil1 = fopen("cordic_isqrt.txt", "wt");
	if(!fil1)
		printf("ERROR: could not open output file1\n");

	// MEMORY ALLOCATION
    real  = (dinp_t *)malloc(NC * sizeof(dinp_t));
    imag  = (dinp_t *)malloc(NC * sizeof(dinp_t));
    magn  = (dout_t *)malloc(NC * sizeof(dout_t));
    ref   = (dout_t *)malloc(NC * sizeof(dout_t));

	if (!ref || !magn || !real || !imag) {
		if (ref )   free(ref);
		if (magn)   free(magn);
		if (real)   free(real);
		if (imag)   free(imag);
	}
	// PREPARE INPUT STIMULA
	printf("\npreparing input stimula\n");

	for (i=0; i<NC; i++)
	{
		x = floor( (((float)rand())/RAND_MAX) * AMP); // real part
		y = floor( (((float)rand())/RAND_MAX) * AMP); // imaginary part
		if (x > AMP) x = AMP; // clipping
		if (y > AMP) y = AMP; // clipping
		real[i] = x;
		imag[i] = y;
		ref[i]  = process_magnitude_ref(real[i],imag[i]); // reference magnitude result
	}


	// CALL DESIGN UNDER TEST
	printf("running Design Under Test\n");
	top_process_magnitude(real, imag, magn);

	// CHECK RESULTS
	printf("check results\n");
	tot_err = 0;
	max_err = 0;
	for (j=0; j<NC; j++)
	{
		d = (magn[j] - ref[j]);
		d = ABS(d);
		tot_err += d;
		if (d >= max_err)
		{
			max_err = d;
			i = j; // store the address of max error
		}
		fprintf(fil1, "%10d %10d %10d %10d %10d %10d\n", j, (int) real[j], (int) imag[j], (int) magn[j], (int) ref[j], d);
	}

	// SIMULATION RESULT
	if (max_err > THRESHOLD)
	{
		ret_val = 1;
		printf("Test failed !\n\n");
	}
	else
	{
		ret_val = 0;
		printf("Test successful!\n\n");
	}


	fclose(fil1);
		rel_err = ((float) tot_err)/NC; // relative error
		printf("RELATIVE ERROR = %6.4f\n",rel_err);
		if (max_err > 0)
			printf("MAX ERROR = %d in sqrt(%6d^2 + %6d^2): EXPECTED=%6d, EFFECTIVE=%6d\n", max_err, (int) real[i], (int) imag[i], (int) ref[i], (int) magn[i]);

		free(ref);
		free(magn);
		free(real);
		free(imag);



	return ret_val;

}
