/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#ifndef CORE_03_H_
#define CORE_03_H_

#include <stdint.h>
#include <adf.h>

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include <aie_api/utils.hpp>


#ifndef ROW_A_3 
#define ROW_A_3 8
#endif

#ifndef COL_A_3
#define COL_A_3 264
#endif

#ifndef COL_B_3
#define COL_B_3 64
#endif 

#ifndef COL_C_3
#define COL_C_3 128
#endif 

using namespace adf;

#ifndef INLINE
#ifndef INLINE_DECL
#define INLINE_DECL
#endif

void core03(
        const int RowA_tile,
        const int ColA_tile,
        const int ColB_tile,
        const int tileX_A,
        const int tileY_A,
        const int tileX_C,
        const int tileY_C,
        int8_t* A_in,
        int8_t* C_out,
        int shift
);

void core03_top(input_buffer<int> & __restrict inA, output_buffer<int> & __restrict out);

#else
#  ifndef INLINE_DECL
#  ifdef __llvm__
#    define INLINE_DECL inline __attribute__((always_inline)) 
#  else
#  define INLINE_DECL inline
#  endif
#  endif
#  undef INLINE 
#  include "core03.cc"
#  define INLINE
# endif


#endif 
