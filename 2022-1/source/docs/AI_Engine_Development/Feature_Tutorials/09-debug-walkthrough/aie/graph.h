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

#include <adf.h>
#include "subsys.h" 

using namespace adf;

template<int COL0>
class TEST_BF: public graph{
  
public:
    input_plio  dldin[4];
    input_plio  dlcin[32];
    output_plio dlout[8];

    input_plio  uldin[8];
    input_plio  ulcin[32];
    output_plio ulout[4];

    // beamforming, 64 antenna, 32 layers
    DL64A32L<COL0+0, 0>  dlbf;
    UL64A32L<COL0+8, 0>  ulbf;

    TEST_BF(){

        // DLBF
        for(unsigned k=0;k<4; k++) {
            dldin[k]=input_plio::create("dlbfid"+std::to_string(k), plio_64_bits, "data/dlbf_din"+std::to_string(k)+".txt");
            connect<>(dldin[k].out[0], dlbf.din[k]);
        }
        for(unsigned k=0;k<32;k++) {
            dlcin[k]=input_plio::create("dlbfic"+std::to_string(k/4)+std::to_string(k%4), plio_64_bits, "data/dlbf_cin"+std::to_string(k/4)+std::to_string(k%4)+".txt");
            connect<>(dlcin[k].out[0], dlbf.cin[k]);
        }
        for(unsigned k=0;k<8; k++) {
            dlout[k]=output_plio::create("dlbfo"+std::to_string(k), plio_64_bits, "data/dlbf_out"+std::to_string(k)+".txt");
            connect<>(dlbf.out[k], dlout[k].in[0]);
        }

        // ULBF
        for(unsigned k=0;k<8; k++) {
            uldin[k]=input_plio::create("ulbfid"+std::to_string(k), plio_64_bits, "data/ulbf_din"+std::to_string(k)+".txt");
            connect<>(uldin[k].out[0], ulbf.din[k]);
        }
        for(unsigned k=0;k<32;k++) {
            ulcin[k]=input_plio::create("ulbfic"+std::to_string(k/8)+std::to_string(k%8), plio_64_bits, "data/ulbf_cin"+std::to_string(k/8)+std::to_string(k%8)+".txt");
            connect<>(ulcin[k].out[0], ulbf.cin[k]);
        }
        for(unsigned k=0;k<4; k++) {
            ulout[k]=output_plio::create("ulbfo"+std::to_string(k), plio_64_bits, "data/ulbf_out"+std::to_string(k)+".txt");
            connect<>(ulbf.out[k], ulout[k].in[0]);	
        }
    };

}; // end of class

