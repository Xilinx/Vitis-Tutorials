#pragma once

#define FILTER2D_KERNEL_H_SIZE 	15
#define FILTER2D_KERNEL_V_SIZE 	15

void Filter2D(
        const    short coeffs[FILTER2D_KERNEL_V_SIZE][FILTER2D_KERNEL_H_SIZE],
		unsigned char *srcImg,
		unsigned int   width,
		unsigned int   height,
		unsigned int   stride,
		unsigned char *dstImg );

