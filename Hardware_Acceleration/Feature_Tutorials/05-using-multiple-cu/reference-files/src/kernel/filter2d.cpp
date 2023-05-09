/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "filter2d.h"
#include "window_2d.h"


static void Filter2D(
		const short   *srcCoeffs, 
		STREAM_PIXELS& srcImg,
		U16            width,
		U16            height,
		STREAM_PIXELS& dstImg)
{
    I16 loopHeight = height+(FILTER_KERNEL_V_SIZE/2);
    I16 loopWidth  = width+(FILTER_KERNEL_H_SIZE/2);

    // Filtering 2D window
    Window2D<MAX_WIDTH, FILTER_KERNEL_V_SIZE, FILTER_KERNEL_H_SIZE, U8> pixelWindow(width, height);

    // Filtering coefficients
    short coeffs[15][15];
    #pragma HLS ARRAY_PARTITION variable=coeffs complete dim=0

    // Copy the coefficients from global memory to local memory
    memcpy(&coeffs[0][0], &srcCoeffs[0], FILTER_KERNEL_V_SIZE*FILTER_KERNEL_V_SIZE*sizeof(short));

    for(int y=0; y<loopHeight; ++y)
    {
        for(int x=0; x<loopWidth; ++x)
        {
            #pragma HLS PIPELINE II=1

            // Determine whether to get a new pixel and update the 2D window
            bool is_valid = pixelWindow.next(srcImg, x, y);

            //Apply 2D filter
            int sum = 0;
            for(int row=0; row<FILTER_KERNEL_V_SIZE; row++) {
				for(int col=0; col<FILTER_KERNEL_H_SIZE; col++) {
					sum += pixelWindow(row,col)*coeffs[row][col];
				}
			}

			//Clamp the normalized mean value to s7.4 bits
			U8 outpix = sum/(FILTER_KERNEL_V_SIZE*FILTER_KERNEL_H_SIZE);

            // Take care of run-in effect, write output only when the window is valid
			// i.e. if kernel is VxH need at least V/2 rows and H/2 pixels before generating output
            if ( is_valid )
            {
            	dstImg << outpix;
            }
        }
    }
}


extern "C" {

void Filter2DKernel(
        const short* coeffs,
		const ap_uint<AXIMM_DATA_WIDTH>* src,
		unsigned int width,
		unsigned int height,
		unsigned int stride,
		ap_uint<AXIMM_DATA_WIDTH>* dst)
  {
    #pragma HLS INTERFACE m_axi     port=src    offset=slave bundle=gmem0  
    #pragma HLS INTERFACE m_axi     port=coeffs offset=slave bundle=gmem1
    #pragma HLS INTERFACE m_axi     port=dst    offset=slave bundle=gmem1
  
#ifndef __SYNTHESIS__
	assert(width <= 1920);
	assert(height<= 1080);
#endif
            
	// Stream of pixels from kernel input to filter, and from filter to output
	hls::stream<U8> src_pixels;
	hls::stream<U8> dst_pixels;
	#pragma HLS stream variable=src_pixels depth=64
	#pragma HLS stream variable=dst_pixels depth=64


	#pragma HLS DATAFLOW

	// Read image data from global memory over AXI4 MM, and stream pixels out
	AXIBursts2PixelStream((AXIMM)src, width, height, stride, src_pixels);

	// Process incoming stream of pixels, and stream pixels out
	Filter2D(coeffs, src_pixels, width, height, dst_pixels);

	// Write incoming stream of pixels and write them to global memory over AXI4 MM
	PixelStream2AXIBursts(dst_pixels, width, height, stride, (AXIMM)dst);

  }

}
