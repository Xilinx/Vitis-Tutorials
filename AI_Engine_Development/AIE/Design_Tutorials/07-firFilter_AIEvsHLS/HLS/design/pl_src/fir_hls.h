/*
   Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
   SPDX-License-Identifier: MIT
   */


#pragma once

//#include <complex>
#include <ap_int.h>
#include <ap_fixed.h>

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_fir.h>

#include "system_settings_comm.h"

#define WINDOW_LENGTH 8192
#define WINDOW_LENGTH_HALF 4096

#define FIR_VALUE_WIDTH   16

#define FIR_SAMPLES WINDOW_LENGTH

#if N_FIR_TAPS > 64
   #define SAMP_PERIOD 4  // Reduced performance by 4 to create a feasible design
#else
   #define SAMP_PERIOD 1  // Go For Maximum Performance
#endif

#if N_FIR_TAPS == 129
   //#if N_FIR_FILTERS == 10
      #define SAMP_PERIOD 2  // Reduced performance by 4 to create a feasible design
   //#elif N_FIR_FILTERS == 1
      //#define SAMP_PERIOD 1  // Reduced performance by 4 to create a feasible design
   //#endif
#endif

#if N_FIR_TAPS == 240
   //#if N_FIR_FILTERS == 10
      #define SAMP_PERIOD 4  // Reduced performance by 4 to create a feasible design
   //#elif N_FIR_FILTERS == 1
      //#define SAMP_PERIOD 2  // Reduced performance by 4 to create a feasible design
   //#endif
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
#if N_FIR_TAPS == 15
    static const unsigned input_length  = WINDOW_LENGTH_HALF;
    static const unsigned output_length = WINDOW_LENGTH_HALF;
    static const unsigned sample_period = SAMP_PERIOD;
//    static const unsigned num_channels = 2;
//    static const unsigned optimization_goal = hls::ip_fir::speed;
//    static const unsigned rate_specification = hls::ip_fir::frequency;
//    static const unsigned sample_frequency = 1200;
#else
    static const unsigned input_length  = WINDOW_LENGTH;
    static const unsigned output_length = WINDOW_LENGTH;
    static const unsigned sample_period = SAMP_PERIOD;
#endif
    static const unsigned coeff_structure = hls::ip_fir::symmetric;
};

#if N_FIR_TAPS == 15
static hls::FIR<fir_params> fir_real[N_FIR_FILTERS*2];
static hls::FIR<fir_params> fir_imag[N_FIR_FILTERS*2];
#else
static hls::FIR<fir_params> fir_real[N_FIR_FILTERS];
static hls::FIR<fir_params> fir_imag[N_FIR_FILTERS];
#endif

void fir_hls(
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp,
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut,
        uint32_t iterCnt
        );
