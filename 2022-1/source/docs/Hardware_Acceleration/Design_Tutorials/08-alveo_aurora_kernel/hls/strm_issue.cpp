/*
 * Copyright 2022 Xilinx, Inc.
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
#define PTR_WIDTH 256
#define PTR_BYTE_WIDTH 32


extern "C"
{
    // kernel function. 
    void strm_issue (hls::stream<ap_axiu<PTR_WIDTH, 0, 0, 0>>& data_output,
                    ap_uint<PTR_WIDTH> *data_input,
                    unsigned int byte_size)
    {
        for (int i = 0; i < (byte_size / PTR_BYTE_WIDTH); i++) {
        // clang-format off
        #pragma HLS PIPELINE II = 1
        // clang-format on
            ap_axiu<PTR_WIDTH, 0, 0, 0> temp; 
            temp.data = data_input[i];
            data_output.write(temp);
        }
    }    

}


