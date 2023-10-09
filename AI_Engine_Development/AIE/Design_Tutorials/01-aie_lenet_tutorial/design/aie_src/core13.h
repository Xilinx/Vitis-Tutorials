/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#ifndef CORE_13_H_
#define CORE_13_H_

#include <stdint.h>
#include <adf.h>

#ifndef OUT_13
#define OUT_13 2112
#endif

#ifndef INLINE
#ifndef INLINE_DECL
#define INLINE_DECL
#endif

void core13(
        const int A,
        const int B,
        const int C,
        const int D,
        const int E,
        const int F,
        const int G,
        const int H,
        int8_t* A_in,
        int8_t* C_out1,
        int8_t* C_out2,
        int shift
);

void core13_top(input_window_int32 *in, output_window_int32 *out1, output_window_int32 *out2);

#else
#  ifndef INLINE_DECL
#  ifdef __llvm__
#    define INLINE_DECL inline __attribute__((always_inline)) 
#  else
#  define INLINE_DECL inline
#  endif
#  endif
#  undef INLINE 
#  include "core13.cc"
#  define INLINE
# endif


#endif 
