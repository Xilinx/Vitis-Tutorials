/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/


#ifndef H_DB_DEFINES_H
#define H_DB_DEFINES_H


/* ************************************************************************* */
/* MAX IMAGE SIZE: */
/* ************************************************************************* */


#define MAX_WIDTH	1920
#define MAX_HEIGHT	1080

#define MAX_PATH 1000

#define GRAY_LEVELS 256

#define CLIP(x) (((x)>255) ? 255 : (((x)<0) ? 0 : (x)))
#define ABS(x)   ((x) < (-(x)) ? (-(x)) : (x))

/* ******************************************************************************* */
// I/O Image Settings
#define INPUT_IMAGE_BASE      "./test_data/test_1080p"
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_1080p"
#define REFER_IMAGE_BASE      "./test_data/golden_out_1080p"


/* ************************************************************************* */
/* COMPILERS DIRECTIVES                                                      */
/* ************************************************************************* */

#ifdef DB_BIT_ACCURATE

#include "ap_int.h"

typedef ap_uint<8>          uint8_t;
typedef ap_uint<7>          uint7_t;
typedef ap_uint<1>          uint1_t;
typedef ap_uint<11>        uint11_t;
typedef ap_uint<16>        uint16_t;
typedef ap_uint<19>        uint19_t;
typedef ap_uint<25>        uint25_t;
typedef ap_uint<32>        uint32_t;
typedef ap_int<11>          int11_t;
typedef ap_int<9>            int9_t;

#else

typedef unsigned char       uint8_t;
typedef unsigned char       uint7_t;
typedef unsigned char       uint1_t;
typedef unsigned short     uint11_t;
typedef unsigned short     uint16_t;
typedef unsigned long int  uint19_t;
typedef unsigned long int  uint25_t;
typedef unsigned long int  uint32_t;
typedef   signed short      int11_t;
typedef   signed short       int9_t;

#endif

typedef struct ap_rgb{
    uint8_t  R;
    uint8_t  G;
    uint8_t  B;
  } RGB_t;


  void ref_img_hist_equaliz( uint11_t width, uint11_t height, uint19_t hist[GRAY_LEVELS],
		  RGB_t inp_img[MAX_HEIGHT * MAX_WIDTH], RGB_t out_img[MAX_HEIGHT * MAX_WIDTH]);

  void wrapper_histogram_equaliz( uint11_t width, uint11_t height, uint19_t hist[GRAY_LEVELS],
			uint8_t inp_R[MAX_WIDTH*MAX_HEIGHT],
			uint8_t inp_G[MAX_WIDTH*MAX_HEIGHT],
			uint8_t inp_B[MAX_WIDTH*MAX_HEIGHT],
			uint8_t out_R[MAX_WIDTH*MAX_HEIGHT],
			uint8_t out_G[MAX_WIDTH*MAX_HEIGHT],
			uint8_t out_B[MAX_WIDTH*MAX_HEIGHT]);



#endif /* H_DB_DEFINES_H */
