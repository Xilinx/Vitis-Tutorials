/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef CORE_01_H_
#define CORE_01_H_

#include <stdint.h>
#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include <aie_api/utils.hpp>


#ifndef ROW_A
#define ROW_A 576
#endif

#ifndef COL_A
#define COL_A 32
#endif

#ifndef COL_B
#define COL_B 8
#endif 



#ifndef INLINE
#ifndef INLINE_DECL
#define INLINE_DECL
#endif

#define __AIE_API_TYPES__HPP__

void core01(
        const int RowA_tile,
        const int ColA_tile,
        const int ColB_tile,
        int8_t* A_in,
        int8_t* C_out,
	int shift
);

void core01_top(input_window_int32 *in1, output_window_int32 *out);

#else
#  ifndef INLINE_DECL
#  ifdef __llvm__
#    define INLINE_DECL inline __attribute__((always_inline)) 
#  else
#  define INLINE_DECL inline
#  endif
#  endif
#  undef INLINE 
#  include "core01.cc"
#  define INLINE
# endif


#endif 
