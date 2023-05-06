/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/


#ifndef CORDIC_H_
#define CORDIC_H_

// LEGENDA: 4 possible implementations:
// 1)
// #define DB_DOUBLE_PRECISION //DB: solution1_double
//
// 2)
// #define DB_SINGLE_PRECISION //DB: solution1_float
//
// 3)
// #define DB_CORDIC             //DB: solution1_cordic
//
// 4)
// #define DB_CORDIC             //DB: solution2_cordic_bitaccurate
// #define BIT_ACCURATE



#if defined(DB_DOUBLE_PRECISION)

typedef double coord_t;
typedef double phase_t;
typedef unsigned char    uint6_t;

#elif	defined(DB_SINGLE_PRECISION)

typedef float coord_t;
typedef float phase_t;
typedef unsigned char    uint6_t;

#elif defined(DB_CORDIC)

#include <ap_int.h>
#include <ap_fixed.h>

typedef ap_fixed<18,9> coord_t;
typedef ap_fixed<16,4> phase_t;
typedef ap_uint<6> uint6_t;

#else

#error <UNDEFINED ATAN2 METHOD!>

#endif

#include <math.h>


#define ROT   8
#define SCALE 10


void top_atan2 (coord_t y0, coord_t x0, phase_t *zn);

#endif
