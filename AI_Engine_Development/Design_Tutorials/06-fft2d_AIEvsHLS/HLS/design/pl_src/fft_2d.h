/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

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

#if FFT_2D_DT == 0 // cint16 datatype
   
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
   
   void fftRow_status(
      statusRow_t* status_in,
      bool* ovflo
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
   
   void fftCol_status(
      statusCol_t* status_in,
      bool* ovflo
      );

#else //cfloat datatype
   
   // Configurable params...
   #define FFT_INPUT_WIDTH  32
   #define FFT_OUTPUT_WIDTH FFT_INPUT_WIDTH
   
   using namespace std;
   using namespace hls;
   
   typedef float data_in_t;
   typedef float data_out_t;
   
   typedef complex<data_in_t>  cmpxDataIn;
   typedef complex<data_out_t> cmpxDataOut;
   
   struct configRow : hls::ip_fft::params_t {
      static const unsigned ordering_opt = hls::ip_fft::natural_order;
      static const unsigned config_width = FFT_ROWS_CONFIG_WIDTH;
      static const unsigned max_nfft = FFT_ROWS_NFFT_MAX;
      static const unsigned stages_block_ram = FFT_ROWS_STAGES_BLK_RAM;
      static const unsigned input_width = FFT_INPUT_WIDTH;
      static const unsigned output_width = FFT_OUTPUT_WIDTH;
      static const unsigned scaling_opt = hls::ip_fft::block_floating_point;
      static const unsigned phase_factor_width = 24;
   };
   
   typedef hls::ip_fft::config_t<configRow> configRow_t;
   typedef hls::ip_fft::status_t<configRow> statusRow_t;
   
   struct configCol : hls::ip_fft::params_t {
      static const unsigned ordering_opt = hls::ip_fft::natural_order;
      static const unsigned config_width = FFT_COLS_CONFIG_WIDTH;
      static const unsigned max_nfft = FFT_COLS_NFFT_MAX;
      static const unsigned stages_block_ram = FFT_COLS_STAGES_BLK_RAM;
      static const unsigned input_width = FFT_INPUT_WIDTH;
      static const unsigned output_width = FFT_OUTPUT_WIDTH;
      static const unsigned scaling_opt = hls::ip_fft::block_floating_point;
      static const unsigned phase_factor_width = 24;
   };
   
   typedef hls::ip_fft::config_t<configCol> configCol_t;
   typedef hls::ip_fft::status_t<configCol> statusCol_t;
   
   typedef union axi_data {
      uint64_t data[2];
      float fl_data[4];
   } AXI_DATA;

#endif

void fftRow_init(
   bool direction,
   configRow_t* config
   );

void copyCol_inp(cmpxDataOut inp_fft[MAT_ROWS],
                 cmpxDataOut inp[MAT_ROWS]
                );

void copyCol_out(cmpxDataOut out_fft[MAT_ROWS],
                 cmpxDataOut out[MAT_ROWS]
                );

#if FFT_2D_DT == 0 // cint16 datatype

   void fftRow(
         bool direction,
         cmpxDataIn   rows_in[MAT_COLS],
         cmpxDataOut rows_out[MAT_COLS],
         bool* ovflo);

#else // cfloat datatype

   void fftRow(
         bool direction,
         cmpxDataIn   rows_in[MAT_COLS],
         cmpxDataOut rows_out[MAT_COLS]);

#endif

void readIn_row(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn rows_in[MAT_COLS]
               );

void writeOut_row(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut rows_out[MAT_COLS]
                 );

void fft_rows(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out
   );

void fftCol_init(
   bool direction,
   configCol_t* config
   );

void copyCol_inp(cmpxDataOut inp_fft[MAT_ROWS],
                 cmpxDataOut inp[MAT_ROWS]
                );

void copyCol_out(cmpxDataOut out_fft[MAT_ROWS],
                 cmpxDataOut out[MAT_ROWS]
                );

#if FFT_2D_DT == 0 // cint16 datatype

   void fftCol(
         bool direction,
         cmpxDataIn   cols_in[MAT_ROWS],
         cmpxDataOut cols_out[MAT_ROWS],
         bool* ovflo);

#else // cfloat datatype

   void fftCol(
         bool direction,
         cmpxDataIn   cols_in[MAT_ROWS],
         cmpxDataOut cols_out[MAT_ROWS]);

#endif

void readIn_col(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn cols_in[MAT_ROWS]
               );

void writeOut_col(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut cols_out[MAT_ROWS]
                 );

void fft_cols(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out
   );

void fft_2d(
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTrows_inp,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTrows_out,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTcols_inp,
   hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTcols_out//,
   //uint32_t iterCnt
   );
