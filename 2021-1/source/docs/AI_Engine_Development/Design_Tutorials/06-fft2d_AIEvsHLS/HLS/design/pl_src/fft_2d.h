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

#pragma once

#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <hls_fft.h>
#include <stdbool.h>
#include <complex>
#include <stdint.h>
#include <ap_axi_sdata.h>
#include "fft_config.h"

// Configurable params...
#define FFT_INPUT_WIDTH  16
#define FFT_OUTPUT_WIDTH FFT_INPUT_WIDTH

using namespace std;
using namespace hls;

typedef ap_fixed<FFT_INPUT_WIDTH,  1> data_in_t;
typedef ap_fixed<FFT_OUTPUT_WIDTH, 1> data_out_t;

typedef complex<data_in_t>  cmpxDataIn;
typedef complex<data_out_t> cmpxDataOut;

struct configRow : hls::ip_fft::params_t {
   static const unsigned ordering_opt = hls::ip_fft::natural_order;
   static const unsigned config_width = FFT_ROWS_CONFIG_WIDTH;
   static const unsigned max_nfft = FFT_ROWS_NFFT_MAX;
   static const unsigned stages_block_ram = FFT_ROWS_STAGES_BLK_RAM;
   static const unsigned input_width = FFT_INPUT_WIDTH;
   static const unsigned output_width = FFT_OUTPUT_WIDTH;
};

typedef hls::ip_fft::config_t<configRow> configRow_t;
typedef hls::ip_fft::status_t<configRow> statusRow_t;

void fftRow_init(
   bool direction,
   configRow_t* config
   );

void fftRow_status(
   statusRow_t* status_in,
   bool* ovflo
   );

void copyRow(cmpxDataOut out_fft[MAT_COLS],
             cmpxDataOut out[MAT_COLS]
            );

void fftRow(
   bool direction,
   cmpxDataIn in[MAT_COLS],
   cmpxDataIn out[MAT_COLS],
	bool* ovflo
   );

void readIn_row(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn in[MAT_COLS]
               );

void writeOut_row(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut out[MAT_COLS]
                 );

void fft_rows(
   hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out
   );

struct configCol : hls::ip_fft::params_t {
   static const unsigned ordering_opt = hls::ip_fft::natural_order;
   static const unsigned config_width = FFT_COLS_CONFIG_WIDTH;
   static const unsigned max_nfft = FFT_COLS_NFFT_MAX;
   static const unsigned stages_block_ram = FFT_COLS_STAGES_BLK_RAM;
   static const unsigned input_width = FFT_INPUT_WIDTH;
   static const unsigned output_width = FFT_OUTPUT_WIDTH;
};

typedef hls::ip_fft::config_t<configCol> configCol_t;
typedef hls::ip_fft::status_t<configCol> statusCol_t;

void fftCol_init(
   bool direction,
   configCol_t* config
   );

void fftCol_status(
   statusCol_t* status_in,
   bool* ovflo
   );

void copyCol(cmpxDataOut out_fft[MAT_ROWS],
             cmpxDataOut out[MAT_ROWS]
            );

void fftCol(
   bool direction,
   cmpxDataIn in[MAT_ROWS],
   cmpxDataIn out[MAT_ROWS],
	bool* ovflo
   );

void readIn_col(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn in[MAT_ROWS]
               );

void writeOut_col(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut out[MAT_ROWS]
                 );

void fft_cols(
   hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out
   );

void fft_2d(
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTrows_inp,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTrows_out,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTcols_inp,
   hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTcols_out,
   uint32_t iterCnt
   );
