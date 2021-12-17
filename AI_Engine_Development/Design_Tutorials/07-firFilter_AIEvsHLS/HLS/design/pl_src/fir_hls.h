//------------------------------------------------------------------------------
// © Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

#pragma once

#include <complex>
#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_fir.h>

#define WINDOW_LENGTH   8192
#define N_WINDOWS           1

#define FIR_VALUE_WIDTH   16
#define FIR_PORT_WIDTH    (2*FIR_VALUE_WIDTH)


typedef ap_fixed<FIR_VALUE_WIDTH, FIR_VALUE_WIDTH>   Data_t;
typedef Data_t    DataWindow_t[WINDOW_LENGTH];
typedef std::complex<Data_t>    Complex_t;


void fir_hls(hls::stream<ap_axiu<128, 0, 0, 0>>& StreamIn,
	     hls::stream<ap_axiu<128, 0, 0, 0>>& StreamOut
    	);
