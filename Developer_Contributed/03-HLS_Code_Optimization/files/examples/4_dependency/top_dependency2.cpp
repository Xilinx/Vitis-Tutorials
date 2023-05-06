/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include "dependency.h"

#ifdef DB_ORIGINAL // solutions 0 & 1

void top_histogram(unsigned char din[nOfSamples], unsigned short int dout[nOfBins],
		           const int n_Of_Samples, const int n_Of_Bins)
{
	#pragma HLS INTERFACE ap_fifo port=dout
	#pragma HLS INTERFACE ap_fifo port=din

	static unsigned short int hist[nOfBins];
	#pragma HLS BIND_STORAGE variable=hist type=ram_t2p impl=bram
	//#pragma HLS RESOURCE variable=hist core=RAM_T2P_BRAM // NOW OBSOLETE and replaced by BIND_STORAGE
	//#pragma HLS DEPENDENCE variable=hist inter false // this is what Vitis HLS compiler does to get II=1 with this code

	unsigned char val;

	// compute histogram
	L1:for (int i=0; i<n_Of_Samples; i++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=1 max=nOfSamples
	#pragma HLS PIPELINE II=1

		val = din[i];
		hist[val]=hist[val]+1;
	}

	// send the histogram values
	L2:for (int i=0; i<n_Of_Bins; i++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=1 max=nOfBins
	#pragma HLS PIPELINE II=1

		dout[i]=hist[i];
		hist[i]=0; // reset hit[] values for next frame
	}
}


#else // solutions 2 & 3

// This code applies a very simple D-cache mechanism.
// The read and the write to the histogram will always be at different addresses.
// As a result in one execution of the loop body, the read and the  write can be scheduled in the same
// clock without concern over whether the block ram is read after write or write after read.
// This is declared to HLS with the dependence pragma: an 'intra-loop' dependence (intra and not inter!),
// declaring that the read which occurs in the code sequentially after the write (RAW)
// is not a dependence that the tool needs to consider (false).



void top_histogram(unsigned char din[nOfSamples], unsigned short int dout[nOfBins],
		                            const int n_Of_Samples, const int n_Of_Bins) {
	#pragma HLS INTERFACE ap_fifo port=dout
	#pragma HLS INTERFACE ap_fifo port=din

	static unsigned short int hist[nOfBins];
	#pragma HLS BIND_STORAGE variable=hist type=ram_t2p impl=bram
	//#pragma HLS RESOURCE variable=hist core=RAM_T2P_BRAM // NOW OBSOLETE and replaced by BIND_STORAGE
	//#pragma HLS DEPENDENCE variable=hist intra RAW false // this is what Vitis HLS compiler does to get II=1 with this code

	unsigned short int acc = 0; unsigned char old, val;

	// compute histogram
	old = 0;
	L1:for (int i=0; i<n_Of_Samples; i++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=1 max=nOfSamples
	#pragma HLS PIPELINE II=1

		val = din[i];
		//	hist[val]=hist[val]+1;
		if (old==val)
			acc++;
		else
		{
			hist[old]=acc;
			acc=hist[val]+1;
		}
		old = val;
	}
	hist[old]=acc;

	// send the histogram values
	L2:for (int i=0; i<n_Of_Bins; i++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=1 max=nOfBins
	#pragma HLS PIPELINE II=1
		dout[i]=hist[i];
		hist[i]=0; // reset hit[] values for next frame
	}


}
#endif
