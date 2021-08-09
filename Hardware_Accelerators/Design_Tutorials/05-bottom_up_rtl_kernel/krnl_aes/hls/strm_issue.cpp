/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
    void strm_issue (hls::stream<ap_axiu<PTR_WIDTH, 0, 0, 0>>& data_output,
                    ap_uint<PTR_WIDTH> *data_input,
                    int byte_size)
    {
        for (int i = 0; i < (byte_size * 8 / PTR_WIDTH); i++) {
        // clang-format off
        #pragma HLS PIPELINE II = 1
        // clang-format on
            ap_uint<PTR_WIDTH> data_in, data_out;
            ap_axiu<PTR_WIDTH, 0, 0, 0> temp; 
            data_in = data_input[i];
            for (int j = 0; j < (PTR_WIDTH / 8); j++) {
                data_out.range(j*8+7, j*8) = data_in.range((PTR_WIDTH/8-1-j)*8+7, (PTR_WIDTH/8-1-j)*8);
            }
            temp.data = data_out;
            data_output.write(temp);
        }
    }    

}


