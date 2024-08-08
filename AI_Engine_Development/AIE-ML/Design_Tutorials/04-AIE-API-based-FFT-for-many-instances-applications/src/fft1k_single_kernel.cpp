//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Donato Carlo Giorgio

#include <adf.h>
#include <aie_api/aie.hpp>
#include "fft1k_single_kernel.h"

fft1k_kernel::fft1k_kernel(void)
{
    aie::set_rounding(aie::rounding_mode::positive_inf);
    aie::set_saturation(aie::saturation_mode::saturate);
}

void fft1k_kernel::run( input_buffer<TT_DATA,extents<BUF_SIZE>> & __restrict din,
                        output_buffer<TT_DATA,extents<BUF_SIZE>> & __restrict dout )
{
    TT_DATA* ibuff = din.data();
    TT_DATA* obuff = dout.data();

    for (int i=0; i < REPEAT; i++)
        chess_prepare_for_pipelining
        chess_loop_range(REPEAT,)
        {
            TT_DATA *__restrict in_data  = ibuff;
            TT_DATA *__restrict out_data = obuff;

            aie::fft_dit_r4_stage<256>(in_data, tw0_1, tw0_0, tw0_2, N, SHIFT_TW, SHIFT_DT, INVERSE, out_data);
            aie::fft_dit_r4_stage<64>(out_data, tw1_1, tw1_0, tw1_2, N, SHIFT_TW, SHIFT_DT, INVERSE, in_data);
            aie::fft_dit_r4_stage<16>(in_data, tw2_1, tw2_0, tw2_2, N, SHIFT_TW, SHIFT_DT, INVERSE, out_data);
            aie::fft_dit_r4_stage<4>(out_data, tw3_1, tw3_0, tw3_2, N, SHIFT_TW, SHIFT_DT, INVERSE, in_data);
            aie::fft_dit_r4_stage<1>(in_data, tw4_1, tw4_0, tw4_2, N, SHIFT_TW, SHIFT_DT, INVERSE, out_data);

            ibuff += N;
            obuff += N;
        }

}