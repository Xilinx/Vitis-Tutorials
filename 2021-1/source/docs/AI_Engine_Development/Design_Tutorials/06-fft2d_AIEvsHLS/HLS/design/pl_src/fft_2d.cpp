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

#include "fft_2d.h"

//1K Point FFT Functions
void fftCol_init (
      bool direction,
      configCol_t* config
      )
{
   config->setDir(direction);
   config->setSch(FFT_COLS_SCALING);
}

void fftCol_status(
      statusCol_t* status_in,
      bool* ovflo
      )
{
   *ovflo = status_in->getOvflo() & 0x1;
}

//Workaround to make II=1...
void copyCol(cmpxDataOut out_fft[MAT_ROWS],
             cmpxDataOut out[MAT_ROWS]
            )
{
   LOOP_COPY_COL:for(int i = 0; i < MAT_ROWS; ++i)
   {
      #pragma HLS pipeline
      
      out[i]=out_fft[i];
   }
}

void fftCol(
      bool direction,
      cmpxDataIn   in[MAT_ROWS],
      cmpxDataOut out[MAT_ROWS],
      bool* ovflo)
{
   #pragma HLS dataflow

   configCol_t fft_config;
   statusCol_t fft_status;
   
   cmpxDataOut out_fft[MAT_ROWS];
   #pragma HLS STREAM depth=2 variable=out_fft

   fftCol_init(direction, &fft_config);

   // FFT IP
   hls::fft<configCol>(in, out_fft, &fft_status, &fft_config);
   copyCol(out_fft, out);

   fftCol_status(&fft_status, ovflo);
}

//2K Point FFT Functions
void fftRow_init (
      bool direction,
      configRow_t *config
      )
{
   config->setDir(direction);
   config->setSch(FFT_ROWS_SCALING);
}

void fftRow_status(
      statusRow_t *status_in,
      bool* ovflo
      )
{
   *ovflo = status_in->getOvflo() & 0x1;
}

//Workaround to make II=1...
void copyRow(cmpxDataOut out_fft[MAT_COLS],
             cmpxDataOut out[MAT_COLS]
            )
{
   LOOP_COPY_ROW:for(int i = 0; i < MAT_COLS; ++i)
   {
      #pragma HLS pipeline
      
      out[i]=out_fft[i];
   }
}

void fftRow(
      bool direction,
      cmpxDataIn   in[MAT_COLS],
      cmpxDataOut out[MAT_COLS],
      bool* ovflo)
{
   #pragma HLS dataflow

   configRow_t fft_config;
   statusRow_t fft_status;
   
   cmpxDataOut out_fft[MAT_COLS];
   #pragma HLS STREAM depth=2 variable=out_fft

   fftRow_init(direction, &fft_config);

   // FFT IP
   hls::fft<configRow>(in, out_fft, &fft_status, &fft_config);
   copyRow(out_fft, out);

   fftRow_status(&fft_status, ovflo);
}

void readIn_row(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn in[MAT_COLS]
               )
{
   qdma_axis<128, 0, 0, 0> qdma;
   
   LOOP_FFT_ROW_READ_INP:for(int j = 0; j < MAT_COLS; j += 4) {
      #pragma HLS PIPELINE II=1

      qdma = strm_inp.read();
      qdma.keep_all();

      in[j].real().range(15, 0)     = qdma.data.range( 15,   0);
      in[j].imag().range(15, 0)     = qdma.data.range( 31,  16);

      in[j + 1].real().range(15, 0) = qdma.data.range( 47,  32);
      in[j + 1].imag().range(15, 0) = qdma.data.range( 63,  48);
                       
      in[j + 2].real().range(15, 0) = qdma.data.range( 79,  64);
      in[j + 2].imag().range(15, 0) = qdma.data.range( 95,  80);
                       
      in[j + 3].real().range(15, 0) = qdma.data.range(111,  96);
      in[j + 3].imag().range(15, 0) = qdma.data.range(127, 112);
   }
}

void writeOut_row(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut out[MAT_COLS]
                 )
{
   qdma_axis<128, 0, 0, 0> qdma;

   LOOP_FFT_ROW_WRITE_OUT:for(int j = 0; j < MAT_COLS; j += 4) {
      #pragma HLS PIPELINE II=1

      qdma.data.range( 15,   0) = real(out[j]).range(15, 0);
      qdma.data.range( 31,  16) = imag(out[j]).range(15, 0);

      qdma.data.range( 47,  32) = real(out[j + 1]).range(15, 0);
      qdma.data.range( 63,  48) = imag(out[j + 1]).range(15, 0);

      qdma.data.range( 79,  64) = real(out[j + 2]).range(15, 0);
      qdma.data.range( 95,  80) = imag(out[j + 2]).range(15, 0);

      qdma.data.range(111,  96) = real(out[j + 3]).range(15, 0);
      qdma.data.range(127, 112) = imag(out[j + 3]).range(15, 0);

   	strm_out.write(qdma);
   }
}

void fft_rows(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out
     )
{
   cmpxDataIn   in[MAT_COLS];
   cmpxDataOut out[MAT_COLS];
   #pragma HLS ARRAY_RESHAPE variable=in cyclic factor=4 dim=1
   #pragma HLS ARRAY_RESHAPE variable=out cyclic factor=4 dim=1
   
   bool directionStub = 1;
   bool ovfloStub;

   LOOP_FFT_ROWS:for(int i = 0; i < MAT_ROWS; ++i) {

      #pragma HLS DATAFLOW

      readIn_row(strm_inp, in);
      
      fftRow(directionStub, in, out, &ovfloStub);

      writeOut_row(strm_out, out);
   }
}

void readIn_col(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn in[MAT_ROWS]
               )
{
   qdma_axis<128, 0, 0, 0> qdma;

   LOOP_FFT_COL_READ_INP:for(int j = 0; j < MAT_ROWS; j += 4) {
      #pragma HLS PIPELINE II=1

      qdma = strm_inp.read();
      qdma.keep_all();

      in[j].real().range(15, 0)     = qdma.data.range( 15,   0);
      in[j].imag().range(15, 0)     = qdma.data.range( 31,  16);

      in[j + 1].real().range(15, 0) = qdma.data.range( 47,  32);
      in[j + 1].imag().range(15, 0) = qdma.data.range( 63,  48);
                       
      in[j + 2].real().range(15, 0) = qdma.data.range( 79,  64);
      in[j + 2].imag().range(15, 0) = qdma.data.range( 95,  80);
                       
      in[j + 3].real().range(15, 0) = qdma.data.range(111,  96);
      in[j + 3].imag().range(15, 0) = qdma.data.range(127, 112);
   }
}

void writeOut_col(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut out[MAT_ROWS]
                 )
{
   qdma_axis<128, 0, 0, 0> qdma;

   LOOP_FFT_COL_WRITE_OUT:for(int j = 0; j < MAT_ROWS; j += 4) {
      #pragma HLS PIPELINE II=1

      qdma.data.range( 15,   0) = real(out[j]).range(15, 0);
      qdma.data.range( 31,  16) = imag(out[j]).range(15, 0);

      qdma.data.range( 47,  32) = real(out[j + 1]).range(15, 0);
      qdma.data.range( 63,  48) = imag(out[j + 1]).range(15, 0);

      qdma.data.range( 79,  64) = real(out[j + 2]).range(15, 0);
      qdma.data.range( 95,  80) = imag(out[j + 2]).range(15, 0);

      qdma.data.range(111,  96) = real(out[j + 3]).range(15, 0);
      qdma.data.range(127, 112) = imag(out[j + 3]).range(15, 0);

   	strm_out.write(qdma);
   }
}

void fft_cols(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out
     )
{
   cmpxDataIn   in[MAT_ROWS];
   cmpxDataOut out[MAT_ROWS];
   #pragma HLS ARRAY_RESHAPE variable=in cyclic factor=4 dim=1
   #pragma HLS ARRAY_RESHAPE variable=out cyclic factor=4 dim=1
   
   bool directionStub = 1;
   bool ovfloStub;

   LOOP_FFT_COLS:for(int i = 0; i < MAT_COLS; ++i) {

      #pragma HLS DATAFLOW

      readIn_col(strm_inp, in);
      
      fftCol(directionStub, in, out, &ovfloStub);

      writeOut_col(strm_out, out);
   }
}

void fft_2d(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTrows_inp,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTrows_out,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTcols_inp,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strmFFTcols_out,
      uint32_t iterCnt
     )
{
   #pragma HLS interface axis port=strmFFTrows_inp
   #pragma HLS interface axis port=strmFFTrows_out
   #pragma HLS interface axis port=strmFFTcols_inp
   #pragma HLS interface axis port=strmFFTcols_out

   #pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control

   #pragma HLS DATAFLOW

   ITER_LOOP_FFT_ROWS:for(int i = 0; i < iterCnt; ++i) {
      fft_rows(strmFFTrows_inp, strmFFTrows_out);
   }

   ITER_LOOP_FFT_COLS:for(int i = 0; i < iterCnt; ++i) {
      fft_cols(strmFFTcols_inp, strmFFTcols_out);
   }
}
