/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "common.h"

void Filter2D(
		const char           coeffs[FILTER_V_SIZE][FILTER_H_SIZE],
		float		         factor,
		short                bias,
		unsigned short       width,
		unsigned short       height,
		unsigned short       stride,
		const unsigned char *src,
		unsigned char       *dst)
{
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
        	// Apply 2D filter to the pixel window
			int sum = 0;
			for(int row=0; row<FILTER_V_SIZE; row++)
			{
				for(int col=0; col<FILTER_H_SIZE; col++)
				{
					unsigned char pixel;
					int xoffset = (x+col-(FILTER_H_SIZE/2));
					int yoffset = (y+row-(FILTER_V_SIZE/2));
					// Deal with boundary conditions : clamp pixels to 0 when outside of image 
					if ( (xoffset<0) || (xoffset>=width) || (yoffset<0) || (yoffset>=height) ) {
						pixel = 0;
					} else {
						pixel = src[yoffset*stride+xoffset];
					}
					sum += pixel*coeffs[row][col];
				}
			}
			
        	// Normalize and saturate result
			unsigned char outpix = MIN(MAX((int(factor * sum)+bias), 0), 255);

			// Write output
           	dst[y*stride+x] = outpix;
        }
    }
}



