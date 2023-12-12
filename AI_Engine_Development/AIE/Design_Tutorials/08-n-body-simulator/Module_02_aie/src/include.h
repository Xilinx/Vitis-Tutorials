/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef INCLUDE_H
#define INCLUDE_H
#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;
#define ENABLE_KERNEL_DEBUG_PRINTS	0

#define NUM_ENGINES_PER_COL			4

#define NUM_I				        32
#define NUM_I_VECTORS				NUM_I/8
#define WINDOW_SIZE_I				NUM_I*7	// x, y, z, m, vx, vy, v

#define NUM_PARTICLES				12800
#define NUM_J					32
#define NUM_J_VECTORS				NUM_J/8
#define LOOP_COUNT_J				NUM_PARTICLES/NUM_J 
#define WINDOW_SIZE_J				NUM_J*4	// x, y, z, m		



//#define ABS_BUFFER_CONSTRAINTS
//#define KERNEL_BYPASS
 
#endif // INCLUDE_H
