/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __XF_DENSE_NONPYR_OPTICAL_FLOW_TYPES__
#define __XF_DENSE_NONPYR_OPTICAL_FLOW_TYPES__

typedef unsigned char pix_t;

template <int BYTES_PER_CYCLE>
struct mywide_t {
    pix_t data[BYTES_PER_CYCLE];
};

typedef struct __yuv { pix_t y, u, v; } yuv_t;

typedef struct __rgb { pix_t r, g, b; } rgb_t;

// kernel returns this type. Packed structs on axi need to be powers-of-2.
typedef struct __rgba {
    pix_t r, g, b;
    pix_t a; // can be unused
} rgba_t;

typedef struct __hsv { pix_t h, s, v; } hsv_t;

#endif
