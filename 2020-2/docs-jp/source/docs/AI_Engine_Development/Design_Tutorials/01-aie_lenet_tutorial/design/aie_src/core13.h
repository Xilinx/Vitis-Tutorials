/* (c) Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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
