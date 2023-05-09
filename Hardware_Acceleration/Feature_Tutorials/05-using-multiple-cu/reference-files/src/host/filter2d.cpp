/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "filter2d.h"
#include "window2d.h"

void Filter2D(
        const    short coeffs[FILTER2D_KERNEL_V_SIZE][FILTER2D_KERNEL_H_SIZE],
		unsigned char *srcImg,
		unsigned int   width,
		unsigned int   height,
		unsigned int   stride,
		unsigned char *dstImg )
{

	Window2D<FILTER2D_KERNEL_H_SIZE, FILTER2D_KERNEL_V_SIZE, unsigned char> pixelWindow(width, height, stride);

    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            // Determine whether to get a new pixel and update the 2D window
			bool is_valid = pixelWindow.next(srcImg, x, y);

			//Apply 2D filter
			int sum = 0;
			for(int row=0; row<FILTER2D_KERNEL_V_SIZE; row++)
			{
				for(int col=0; col<FILTER2D_KERNEL_H_SIZE; col++)
				{
					sum += pixelWindow(row,col)*coeffs[row][col];
				}
			}
			
			//Clamp the normalized mean value to s7.4 bits
			unsigned char outpix = sum/(FILTER2D_KERNEL_V_SIZE*FILTER2D_KERNEL_H_SIZE);

            // Take care of run-in effect, write output only when the window is valid
			// i.e. if kernel is VxH need at least V/2 rows and H/2 pixels before generating output
            if ( is_valid )
            {
            	dstImg[y*stride+x] = outpix;
            }
        }
    }
}



