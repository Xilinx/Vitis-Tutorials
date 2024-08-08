//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Donato Carlo Giorgio

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "fft1k_single_twiddles.h"

using namespace adf;

#define REPS   2        // Used for batching the FFT
#define POINTS 1024     // Number of points

class fft1k_kernel {
    public:
    // AIE API FFT stage template and function parameters
        typedef cint16 TT_DATA;
        typedef cint16 TT_TWID;
        static constexpr unsigned N = POINTS;
        static constexpr unsigned SHIFT_TW = 15;
        static constexpr unsigned SHIFT_DT = 15;
        static constexpr bool     INVERSE  = false;
        static constexpr unsigned REPEAT   = REPS;       // Used for batching the FFT
        static constexpr unsigned BUF_SIZE = N * REPEAT;
    
    private:
    // Twiddle factor allocation
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw0_0[1]	= TWID0_0;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw0_1[1]	= TWID0_1;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw0_2[1]	= TWID0_2;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw1_0[4]	= TWID1_0;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw1_1[4]	= TWID1_1;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw1_2[4]	= TWID1_2;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw2_0[16]	= TWID2_0;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw2_1[16]	= TWID2_1;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw2_2[16]	= TWID2_2;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw3_0[64]	= TWID3_0;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw3_1[64]	= TWID3_1;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw3_2[64]	= TWID3_2;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw4_0[256]	= TWID4_0;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw4_1[256]	= TWID4_1;
        alignas(aie::vector_decl_align) static constexpr TT_TWID	tw4_2[256]	= TWID4_2;

    // Temporary buffers
    //    alignas(aie::vector_decl_align) TT_DATA tbuff[N];         // Unused if odd number of stages
    
    public:
        // Constructor
        fft1k_kernel(void);

        // Run
        void run(input_buffer<TT_DATA,extents<BUF_SIZE> >& __restrict din,
                 output_buffer<TT_DATA,extents<BUF_SIZE> >& __restrict dout );

        static void registerKernelClass(void)
        {
            REGISTER_FUNCTION(fft1k_kernel::run);
        }
};