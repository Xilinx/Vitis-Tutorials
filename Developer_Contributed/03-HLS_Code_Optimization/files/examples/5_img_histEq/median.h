/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/



#ifndef _H_MEDIAN_H_
#define _H_MEDIAN_H_


//#include "ap_axi_sdata.h"
//#include "ap_interfaces.h"
//#include "ap_bmp.h"

/* ******************************************************************************* */
// CONSTANT PARAMETERS

#define MAX_PATH      1000

#define MAX_HEIGHT    1080
#define MAX_WIDTH     1920

//#define KMED3

#if defined(KMED3)
#define KMED            3
#elif defined(KMED5)
#define KMED            5
#elif defined(KMED7)
#define KMED            7
#else
#error <YOU MUST DEFINE THE SIZE OF THE MEDIAN FILTER KERNEL>
#endif

#define KKMED      (KMED/2)
#define BLOCKSIZE     KMED


/* ******************************************************************************* */
// I/O Image Settings
#define INPUT_IMAGE_BASE      "./test_data/noisy_rgb2y_1080p"

#if (KMED==7)
//#error < KMED IS 7>
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_median7x7_1080p"
#elif (KMED==5)
//#error < KMED IS 5>
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_median5x5_1080p"
#elif (KMED==3)
//#error < KMED IS 3>
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_median3x3_1080p"
#endif

//#define REFER_IMAGE_BASE      "./test_data/golden_out_median_1080p"
//#define REFER_VAR_BASE        "./test_data/golden_out_variance_1080p"
//#define OUTPUT_VAR_BASE       "./test_data/c_out_variance_1080p"
//#define C_REFER_IMAGE_BASE    "./test_data/c_golden_out_median_1080p"

/* ******************************************************************************* */
// SMALL FUNCTIONS IN MACRO
#define ABSDIFF(x,y)	( (x)>(y) ? (x - y) : (y - x) )
#define ABS(x)          ( (x)> 0  ? (x)     : -(x)    )
#define MIN(x,y)        ( (x)>(y) ? (y)     :  (x)    )
#define MAX(x,y)        ( (x)>(y) ? (x)     :  (y)    )




/* ******************************************************************************* */
// DATA TYPES

//#define DB_FIXED_POINT

#ifndef DB_FIXED_POINT
typedef unsigned char      pix_t;
#else
#include <ap_int.h>
typedef ap_uint<11> pix_t;
#endif

//#define FLOAT_VAR
#ifndef FLOAT_VAR
typedef int var_t;
#else
typedef float var_t;
#endif

typedef struct ap_rgb{
    unsigned char B;
    unsigned char  G;
    unsigned char  R;
  } RGB_t;

typedef   signed short int  int16_t;
typedef unsigned short int uint16_t;

//typedef ap_axiu<32,1,1,1> AXI_PIXEL;
//typedef pix_t  AXI_PIXEL;


/* ******************************************************************************* */
// FUNCTION PROTOTYPES

int main_median(void);

pix_t median(pix_t window[KMED*KMED]);

void top_median( pix_t   in_pix[MAX_HEIGHT][MAX_WIDTH],
						  pix_t  out_pix[MAX_HEIGHT][MAX_WIDTH],
						  short height, short width); //, FILE *fp);


void ref_median(pix_t in_pix[MAX_HEIGHT][MAX_WIDTH],
							  pix_t ref_pix[MAX_HEIGHT][MAX_WIDTH],
							  short height, short width); //, FILE *fp);

////extern void sw_naive_median( pix_t   in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t  out_pix[MAX_HEIGHT * MAX_WIDTH], short height, short width); //, FILE *fp);
//extern void hw_naive_median( pix_t   in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t  out_pix[MAX_HEIGHT * MAX_WIDTH], short int height, short int width); //, FILE *fp);
//
////void sw_linebuf_median(pix_t in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t ref_pix[MAX_HEIGHT * MAX_WIDTH], short height, short width); //, FILE *fp);
//extern void hw_linebuf_median(pix_t in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t ref_pix[MAX_HEIGHT * MAX_WIDTH], short int height, short int width); //, FILE *fp);



#endif
