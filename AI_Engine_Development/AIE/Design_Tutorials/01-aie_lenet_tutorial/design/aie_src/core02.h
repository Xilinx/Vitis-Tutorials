/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef CORE_02_H_
#define CORE_02_H_

#include <stdint.h>
#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include <aie_api/utils.hpp>



#ifndef ROW_A_2 
#define ROW_A_2 64
#endif

#ifndef COL_A_2
#define COL_A_2 152
#endif

#ifndef COL_B_2
#define COL_B_2 16
#endif 



#ifndef INLINE
#ifndef INLINE_DECL
#define INLINE_DECL
#endif

void core02_top(input_window_int32 *inA, output_window_int32 *out);

void core02(
        const int RowA_tile,
        const int ColA_tile,
        const int ColB_tile,
        int8_t* A_in,
        int8_t* C_out,
        int shift
);


#else
#  ifndef INLINE_DECL
#  ifdef __llvm__
#    define INLINE_DECL inline __attribute__((always_inline)) 
#  else
#  define INLINE_DECL inline
#  endif
#  endif
#  undef INLINE 
#  include "core02.cc"
#  define INLINE
# endif

#endif 
