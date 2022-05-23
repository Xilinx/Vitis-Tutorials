//------------------------------------------------------------------------------
// © Copyright 2022 Xilinx, Inc.
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

//#include <complex>
#include <ap_int.h>
#include <ap_fixed.h>

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_fir.h>

#include "system_settings_comm.h"

#define WINDOW_LENGTH 8192

#define FIR_VALUE_WIDTH   16

#define FIR_SAMPLES WINDOW_LENGTH

#if N_FIR_TAPS > 64
   #define SAMP_PERIOD 4  // Reduced performance by 4 to create a feasible design

#else
   #define SAMP_PERIOD 1  // Go For Maximum Performance

#endif

typedef ap_fixed<FIR_VALUE_WIDTH, FIR_VALUE_WIDTH> Data_t;
typedef Data_t DataWindow_t[WINDOW_LENGTH];

using namespace std;
using namespace hls;

struct fir_params : hls::ip_fir::params_t {
   static const unsigned num_coeffs    = N_FIR_TAPS;
   static const double   coeff_vec[N_FIR_TAPS];
   static const unsigned coeff_width   = 16;
   static const unsigned input_width   = 16;
   static const unsigned output_width  = 16;
   static const unsigned output_rounding_mode = hls::ip_fir::truncate_lsbs;
   static const unsigned input_length  = WINDOW_LENGTH;
   static const unsigned output_length = WINDOW_LENGTH;
   static const unsigned sample_period = SAMP_PERIOD;
   static const unsigned coeff_structure = hls::ip_fir::symmetric;
};

static hls::FIR<fir_params> fir_real[N_FIR_FILTERS];
static hls::FIR<fir_params> fir_imag[N_FIR_FILTERS];

void fir_hls(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut,
   uint32_t iterCnt
   );
