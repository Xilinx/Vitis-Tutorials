/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <hls_stream.h>
#include <ap_int.h>
#include <ap_axi_sdata.h>

#define _XF_SYNTHESIS_ 1

// AXI port data width
#define OUTPUT_PTR_WIDTH 64
#define INPUT_PTR_WIDTH 64


extern "C"
{
    // kernel function. 
    void strm_dump (hls::stream<ap_axiu<INPUT_PTR_WIDTH, 0, 0, 0>>& data_input,
                    ap_uint<OUTPUT_PTR_WIDTH> *data_output,
                    int byte_size)
    {
        for (int i = 0; i < (byte_size / 8); i++) {
        // clang-format off
        #pragma HLS PIPELINE II = 1
        // clang-format on
            ap_axiu<INPUT_PTR_WIDTH, 0, 0, 0> temp = data_input.read();
            data_output[i] = temp.data;
        }
    }    

}


