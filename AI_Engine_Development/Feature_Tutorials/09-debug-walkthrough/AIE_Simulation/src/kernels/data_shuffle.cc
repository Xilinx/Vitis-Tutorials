/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <kernels/kernels.h>

void data_shuffle(
    input_buffer<int32> & __restrict from_prev,
    //input_stream_int32 * __restrict outmax,
    output_buffer<int32> & __restrict out_shift
)
{
	auto InIter = aie::begin_vector<8>(from_prev);
	auto OutIter = aie::begin_vector<8>(out_shift);
	aie::vector<int32,8> prev_in = aie::zeros<int32,8>();                          //8-lane vector initialized to '0'
	aie::vector<int32,8> shuffle_t = aie::zeros<int32,8>();                        //8-lane vector initialized to '0'
	int32 max_scalar;

	for(int i=0;i<NUM_SAMPLES*2;i++)                        
	{
		prev_in = *InIter++;
		//int remainder = i & 1;				                       //Reads 1 stream value for every 2 iterations as the
		//if(remainder == 0)				                           //previous kernel sends 1 output for every 16 values
			//max_scalar = readincr(outmax);
		//aie::vector<int32,8> max_vector = aie::broadcast<int32,8>(max_scalar);//initializes the 8-lane vector with max_scalar
		aie::vector<int32,8> max_vector = aie::zeros<int32,8>();
		shuffle_t = aie::shuffle_up_fill(prev_in,max_vector,4);               //Shuffles the input data - First 4 values in 8-lane vector with max_scalar
		*OutIter++ = shuffle_t;
	}				
}


