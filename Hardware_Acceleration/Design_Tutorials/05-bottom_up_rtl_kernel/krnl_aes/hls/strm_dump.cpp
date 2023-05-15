/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <hls_stream.h>
#include <ap_int.h>
#include <ap_axi_sdata.h>

#define _XF_SYNTHESIS_ 1

// AXI port data width
#define PTR_WIDTH 128


extern "C"
{
    // kernel function. 
    void strm_dump (hls::stream<ap_axiu<PTR_WIDTH, 0, 0, 0>>& data_input,
                    ap_uint<PTR_WIDTH> *data_output,
                    int byte_size)
    {
        for (int i = 0; i < (byte_size * 8 / PTR_WIDTH); i++) {
        // clang-format off
        #pragma HLS PIPELINE II = 1
        // clang-format on
            ap_axiu<PTR_WIDTH, 0, 0, 0> temp = data_input.read();
            ap_uint<PTR_WIDTH> data_in, data_out;
            data_in = temp.data;
            for (int j = 0; j < (PTR_WIDTH / 8); j++) {
                data_out.range(j*8+7, j*8) = data_in.range((PTR_WIDTH/8-1-j)*8+7, (PTR_WIDTH/8-1-j)*8);
            }            
            data_output[i] = data_out;
        }
    }    

}


