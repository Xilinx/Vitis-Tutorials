/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#ifndef CORE_04_H_
#define CORE_04_H_

#include <stdint.h>
#include<adf.h>

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include <aie_api/utils.hpp>


#ifndef ROW_A_4
#define ROW_A_4 8
#endif

#ifndef COL_A_4
#define COL_A_4 128
#endif

#ifndef COL_B_4
#define COL_B_4 16
#endif 


#ifndef INLINE
#ifndef INLINE_DECL
#define INLINE_DECL
#endif

void core04(
        const int RowA_tile,
        const int ColA_tile,
        const int ColB_tile,
        int8_t* A1_in,
        int8_t* A2_in,
        int8_t* C_out,
        int shift
);

void core04_top(input_window_int32 *inA1, input_window_int32 *inA2, output_window_int32 *out);

#else
#  ifndef INLINE_DECL
#  ifdef __llvm__
#    define INLINE_DECL inline __attribute__((always_inline)) 
#  else
#  define INLINE_DECL inline
#  endif
#  endif
#  undef INLINE
#  include "core04.cc"
#  define INLINE
# endif


#endif 
