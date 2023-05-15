/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#pragma once

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_IMAGE_WIDTH     1920
#define MAX_IMAGE_HEIGHT    1080

#define FILTER_V_SIZE		15
#define FILTER_H_SIZE		15

#ifndef MIN
#define MIN(a,b) ((a<b)?a:b)
#endif
#ifndef MAX
#define MAX(a,b) ((a<b)?b:a)
#endif

void Filter2D(
		const char           coeffs[FILTER_V_SIZE][FILTER_H_SIZE],
		float		         factor,
		short                bias,
		unsigned short       width,
		unsigned short       height,
		unsigned short       stride,
		const unsigned char *src,
		unsigned char       *dst);


extern "C" {

  void Filter2DKernel(
		const char          *coeffs,
		float                factor,
		short                bias,
		unsigned short       width,
		unsigned short       height,
		unsigned short       stride,
		const unsigned char *src,
		unsigned char       *dst);

}

typedef unsigned char      		U8;
typedef unsigned short     		U16;
typedef unsigned int       		U32;

typedef signed char        		I8;
typedef signed short       		I16;
typedef signed int         		I32;
