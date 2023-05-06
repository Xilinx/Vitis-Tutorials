/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/
#include <stdio.h>
#include <stdlib.h>

#include "dependency.h"

int main(void)
{

	unsigned char             din[nOfSamples];
	unsigned short int       dout[nOfBins], ref[nOfBins];

	int i;
	FILE *f = fopen("data.txt", "w");
	if (f==NULL)
		fprintf(stderr, "error in opening output data file\n");

	// clear output arrays
	for(i=0; i<nOfBins; i++)
	{
		ref[i]=0;
		dout[i]=0;
	}


	// prepare data
	for(i=0; i<nOfSamples; i++)
		din[i] = (rand() % 256);

	// call reference design
	for(i=0; i<nOfSamples; i++)
	{
		unsigned char byte = din[i];
		ref[byte] = ref[byte] + 1;
		fprintf(f, "%4d %4d\n", din[i], ref[byte]);
	}

	// call DUT
	top_histogram(din, dout, nOfSamples, nOfBins);


	// compare results
	int tot_err = 0;
	int diff;
	for(i=0; i<nOfBins; i++)
	{
		diff = ref[i] - dout[i];
		if (diff<0) diff = 0-diff;

		if (diff>0)
		{
			printf("got %5d expected %5d\n", dout[i], ref[i]);
			tot_err++;
		}
	}


	if (tot_err==0)
		printf("\nTEST OK\n");
	else
		printf("\nTEST FAILED: %d wrong values\n", tot_err);

	fclose(f);
	return 0; //tot_err;


}
