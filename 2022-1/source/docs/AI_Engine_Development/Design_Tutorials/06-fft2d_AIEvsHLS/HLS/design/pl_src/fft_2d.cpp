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
   
   #if FFT_2D_DT == 0 // cint16 datatype
      config->setSch(FFT_COLS_SCALING);
   #endif
}

#if FFT_2D_DT == 0 // cint16 datatype
   void fftCol_status(
         statusCol_t* status_in,
         bool* ovflo
         )
   {
      *ovflo = status_in->getOvflo() & 0x1;
   }
#endif

//Workaround to manage DATAFLOW...
void copyCol_inp(cmpxDataOut inp_fft[MAT_ROWS],
                 cmpxDataOut inp[MAT_ROWS]
                )
{
   LOOP_COPY_COL_INP:for(int i = 0; i < MAT_ROWS; ++i)
   {
      #pragma HLS PIPELINE II=1
      #if FFT_2D_DT == 0 // cint16 datatype
         #pragma HLS UNROLL factor=4
      #else // cfloat datatype
         #pragma HLS UNROLL factor=2
      #endif
      
      inp[i]=inp_fft[i];
   }
}

//Workaround to manage DATAFLOW...
void copyCol_out(cmpxDataOut out_fft[MAT_ROWS],
                 cmpxDataOut out[MAT_ROWS]
                )
{
   LOOP_COPY_COL_OUT:for(int i = 0; i < MAT_ROWS; ++i)
   {
      #pragma HLS PIPELINE II=1
      #if FFT_2D_DT == 0 // cint16 datatype
         #pragma HLS UNROLL factor=4
      #else // cfloat datatype
         #pragma HLS UNROLL factor=2
      #endif
      
      out[i]=out_fft[i];
   }
}

#if FFT_2D_DT == 0 // cint16 datatype

   void fftCol(
         bool direction,
         cmpxDataIn   in[MAT_ROWS],
         cmpxDataOut out[MAT_ROWS],
         bool* ovflo)

#else // cfloat datatype

   void fftCol(
         bool direction,
         cmpxDataIn   in[MAT_ROWS],
         cmpxDataOut out[MAT_ROWS])

#endif
{
   #pragma HLS dataflow
   
   configCol_t fft_config;
   statusCol_t fft_status;
   
   cmpxDataOut inp_fft[MAT_ROWS];
   #pragma HLS STREAM variable=inp_fft depth=2
   
   cmpxDataOut out_fft[MAT_ROWS];
   #pragma HLS STREAM variable=out_fft depth=2
   
   fftCol_init(direction, &fft_config);
   
   // FFT IP
   //copyCol_inp(in, inp_fft);
   //hls::fft<configCol>(inp_fft, out_fft, &fft_status, &fft_config);
   //copyCol_out(out_fft, out);
   
   hls::fft<configCol>(in, out, &fft_status, &fft_config);
   
   #if FFT_2D_DT == 0 // cint16 datatype
      fftCol_status(&fft_status, ovflo);
   #endif
}

//2K Point FFT Functions
void fftRow_init (
      bool direction,
      configRow_t *config
      )
{
   config->setDir(direction);
   
   #if FFT_2D_DT == 0 // cint16 datatype
      config->setSch(FFT_COLS_SCALING);
   #endif
}

#if FFT_2D_DT == 0 // cint16 datatype
   void fftRow_status(
         statusRow_t *status_in,
         bool* ovflo
         )
   {
      *ovflo = status_in->getOvflo() & 0x1;
   }
#endif

//Workaround to manage DATAFLOW...
void copyRow_inp(cmpxDataOut inp_fft[MAT_COLS],
                 cmpxDataOut inp[MAT_COLS]
                )
{
   LOOP_COPY_ROW_INP:for(int i = 0; i < MAT_COLS; ++i)
   {
      #pragma HLS PIPELINE II=1
      #if FFT_2D_DT == 0 // cint16 datatype
         #pragma HLS UNROLL factor=4
      
      #else // cfloat datatype
         #pragma HLS UNROLL factor=2
         
      #endif
      
      inp[i]=inp_fft[i];
   }
}

//Workaround to manage DATAFLOW...
void copyRow_out(cmpxDataOut out_fft[MAT_COLS],
                 cmpxDataOut out[MAT_COLS]
                )
{
   LOOP_COPY_ROW_OUT:for(int i = 0; i < MAT_COLS; ++i)
   {
      #pragma HLS PIPELINE II=1
      #if FFT_2D_DT == 0 // cint16 datatype
         #pragma HLS UNROLL factor=4
      
      #else // cfloat datatype
         #pragma HLS UNROLL factor=2
         
      #endif
      
      out[i]=out_fft[i];
   }
}

#if FFT_2D_DT == 0 // cint16 datatype

   void fftRow(
         bool direction,
         cmpxDataIn   in[MAT_COLS],
         cmpxDataOut out[MAT_COLS],
         bool* ovflo)

#else // cfloat datatype

   void fftRow(
         bool direction,
         cmpxDataIn   in[MAT_COLS],
         cmpxDataOut out[MAT_COLS])

#endif
{
   #pragma HLS dataflow
   
   configRow_t fft_config;
   statusRow_t fft_status;
   
   cmpxDataOut inp_fft[MAT_COLS];
   #pragma HLS STREAM variable=inp_fft depth=2
   
   cmpxDataOut out_fft[MAT_COLS];
   #pragma HLS STREAM variable=out_fft depth=2
   
   fftRow_init(direction, &fft_config);
   
   // FFT IP
   //copyRow_inp(in, inp_fft);
   //hls::fft<configRow>(inp_fft, out_fft, &fft_status, &fft_config);
   //copyRow_out(out_fft, out);
   
   hls::fft<configRow>(in, out, &fft_status, &fft_config);
   
   #if FFT_2D_DT == 0 // cint16 datatype
      fftRow_status(&fft_status, ovflo);
   #endif
}

void readIn_row(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn in[MAT_COLS]
               )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_ROW_READ_INP:for(int j = 0; j < MAT_COLS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         
         qdma_axis<128, 0, 0, 0> qdma = strm_inp.read();
         qdma.keep_all();
         
         cmpxDataIn tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range( 15,   0);
         tmp_in.imag().range(15, 0) = qdma.data.range( 31,  16);
         in[j] = tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range( 47,  32);
         tmp_in.imag().range(15, 0) = qdma.data.range( 63,  48);
         in[j + 1] = tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range( 79,  64);
         tmp_in.imag().range(15, 0) = qdma.data.range( 95,  80);
         in[j + 2] = tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range(111,  96);
         tmp_in.imag().range(15, 0) = qdma.data.range(127, 112);
         in[j + 3] = tmp_in;
      }
   
   #else // cfloat datatype
      LOOP_FFT_ROW_READ_INP:for(int j = 0; j < MAT_COLS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=32 max=1024
         
         qdma_axis<128, 0, 0, 0> qdma = strm_inp.read();
         qdma.keep_all();
         
         cmpxDataIn tmp_in;
         AXI_DATA rowInp;
         
         rowInp.data[0] = qdma.data.range( 63,  0);
         rowInp.data[1] = qdma.data.range(127, 64);
         
         tmp_in.real(rowInp.fl_data[0]);
         tmp_in.imag(rowInp.fl_data[1]);
         in[j] = tmp_in;
         
         tmp_in.real(rowInp.fl_data[2]);
         tmp_in.imag(rowInp.fl_data[3]);
         in[j + 1] = tmp_in;
      }
   
   #endif
}

void writeOut_row(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut out[MAT_COLS]
                 )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_ROW_WRITE_OUT:for(int j = 0; j < MAT_COLS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         qdma_axis<128, 0, 0, 0> qdma;
         
         cmpxDataOut tmp;
         tmp = out[j];

         qdma.data.range( 15,   0) = real(tmp).range(15, 0);
         qdma.data.range( 31,  16) = imag(tmp).range(15, 0);

         tmp = out[j+1];

         qdma.data.range( 47,  32) = real(tmp).range(15, 0);
         qdma.data.range( 63,  48) = imag(tmp).range(15, 0);

         tmp = out[j+2];

         qdma.data.range( 79,  64) = real(tmp).range(15, 0);
         qdma.data.range( 95,  80) = imag(tmp).range(15, 0);

         tmp = out[j+3];

         qdma.data.range(111,  96) = real(tmp).range(15, 0);
         qdma.data.range(127, 112) = imag(tmp).range(15, 0);
         
         strm_out.write(qdma);
      }
   
   #else // cfloat datatype
      LOOP_FFT_ROW_WRITE_OUT:for(int j = 0; j < MAT_COLS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=32 max=1024
         
         qdma_axis<128, 0, 0, 0> qdma;
         
         AXI_DATA rowOut;
         cmpxDataOut tmp;
         tmp = out[j];
         rowOut.fl_data[0] = real(tmp);
         rowOut.fl_data[1] = imag(tmp);
         tmp = out[j+1];
         rowOut.fl_data[2] = real(tmp);
         rowOut.fl_data[3] = imag(tmp);

         qdma.data.range( 63,  0) = rowOut.data[0];
         qdma.data.range(127, 64) = rowOut.data[1];
         
         strm_out.write(qdma);
      }
   #endif
}

void fft_rows(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out
     )
{
   LOOP_FFT_ROWS:for(int i = 0; i < MAT_ROWS; ++i) {
      #pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=32 max=1024
      
      #if FFT_2D_DT == 0 // cint16 datatype
         cmpxDataIn in[MAT_COLS];
         #pragma HLS STREAM variable=in depth=1024
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=4 dim=1
         
         cmpxDataOut out[MAT_COLS];
         #pragma HLS STREAM variable=out depth=1024
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=4 dim=1
      
      #else // cfloat datatype
         cmpxDataIn in[MAT_COLS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=in depth=512
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=2 dim=1
         
         cmpxDataOut out[MAT_COLS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=out depth=512
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=2 dim=1
      
      #endif
      
      bool directionStub = 1;
      
      #if FFT_2D_DT == 0 // cint16 datatype
         bool ovfloStub;
      #endif
      
      readIn_row(strm_inp, in);
      
      #if FFT_2D_DT == 0 // cint16 datatype
         fftRow(directionStub, in, out, &ovfloStub);
      
      #else // cfloat datatype
         fftRow(directionStub, in, out);
      
      #endif
      
      writeOut_row(strm_out, out);
   }
}

void readIn_col(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn in[MAT_ROWS]
               )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_COL_READ_INP:for(int j = 0; j < MAT_ROWS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=8 max=256
         
         qdma_axis<128, 0, 0, 0> qdma = strm_inp.read();
         qdma.keep_all();
         
         cmpxDataIn tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range( 15,   0);
         tmp_in.imag().range(15, 0) = qdma.data.range( 31,  16);
         in[j] = tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range( 47,  32);
         tmp_in.imag().range(15, 0) = qdma.data.range( 63,  48);
         in[j + 1] = tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range( 79,  64);
         tmp_in.imag().range(15, 0) = qdma.data.range( 95,  80);
         in[j + 2] = tmp_in;
         
         tmp_in.real().range(15, 0) = qdma.data.range(111,  96);
         tmp_in.imag().range(15, 0) = qdma.data.range(127, 112);
         in[j + 3] = tmp_in;
      }
   
   #else // cfloat datatype
      LOOP_FFT_COL_READ_INP:for(int j = 0; j < MAT_ROWS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         
         qdma_axis<128, 0, 0, 0> qdma = strm_inp.read();
         qdma.keep_all();
         
         cmpxDataIn tmp_in;
         AXI_DATA colInp;
         
         colInp.data[0] = qdma.data.range( 63,  0);
         colInp.data[1] = qdma.data.range(127, 64);
         
         tmp_in.real(colInp.fl_data[0]);
         tmp_in.imag(colInp.fl_data[1]);
         in[j] = tmp_in;
         
         tmp_in.real(colInp.fl_data[2]);
         tmp_in.imag(colInp.fl_data[3]);
         in[j + 1] = tmp_in;
      }
   
   #endif
}

void writeOut_col(hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut out[MAT_ROWS]
                 )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_COL_WRITE_OUT:for(int j = 0; j < MAT_ROWS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         
         qdma_axis<128, 0, 0, 0> qdma;
         cmpxDataOut tmp;
         tmp = out[j];

         qdma.data.range( 15,   0) = real(tmp).range(15, 0);
         qdma.data.range( 31,  16) = imag(tmp).range(15, 0);
         tmp = out[j+1];

         qdma.data.range( 47,  32) = real(tmp).range(15, 0);
         qdma.data.range( 63,  48) = imag(tmp).range(15, 0);
         tmp = out[j+2];

         qdma.data.range( 79,  64) = real(tmp).range(15, 0);
         qdma.data.range( 95,  80) = imag(tmp).range(15, 0);
         tmp = out[j+3];

         qdma.data.range(111,  96) = real(tmp).range(15, 0);
         qdma.data.range(127, 112) = imag(tmp).range(15, 0);
         
         strm_out.write(qdma);
      }
   
   #else // cfloat datatype
      LOOP_FFT_COL_WRITE_OUT:for(int j = 0; j < MAT_ROWS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=32 max=1024
         
         qdma_axis<128, 0, 0, 0> qdma;
         
         AXI_DATA colOut;
         cmpxDataOut tmp;
         tmp = out[j];
         colOut.fl_data[0] = real(tmp);
         colOut.fl_data[1] = imag(tmp);
         tmp = out[j+1];
         colOut.fl_data[2] = real(tmp);
         colOut.fl_data[3] = imag(tmp);

         qdma.data.range( 63,  0) = colOut.data[0];
         qdma.data.range(127, 64) = colOut.data[1];
         
         strm_out.write(qdma);
      }
   #endif
}

void fft_cols(
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_inp,
      hls::stream<qdma_axis<128, 0, 0, 0>> &strm_out
     )
{
   LOOP_FFT_COLS:for(int i = 0; i < MAT_COLS; ++i) {
      #pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=64 max=2048
      
      #if FFT_2D_DT == 0 // cint16 datatype
         cmpxDataIn in[MAT_ROWS];
         #pragma HLS STREAM variable=in depth=1024
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=4 dim=1
         
         cmpxDataOut out[MAT_ROWS];
         #pragma HLS STREAM variable=out depth=1024
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=4 dim=1
      
      #else // cfloat datatype
         cmpxDataIn in[MAT_ROWS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=in depth=512
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=2 dim=1
         
         cmpxDataOut out[MAT_ROWS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=out depth=512
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=2 dim=1
      
      #endif
      
      bool directionStub = 1;
      
      #if FFT_2D_DT == 0 // cint16 datatype
         bool ovfloStub;
      #endif
      
      readIn_col(strm_inp, in);
      
      #if FFT_2D_DT == 0 // cint16 datatype
         fftCol(directionStub, in, out, &ovfloStub);
      
      #else // cfloat datatype
         fftCol(directionStub, in, out);
      
      #endif
      
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
      #pragma HLS loop_tripcount min=1 max=8
      //#pragma HLS DATAFLOW
      
      fft_rows(strmFFTrows_inp, strmFFTrows_out);
   }
   
   ITER_LOOP_FFT_COLS:for(int i = 0; i < iterCnt; ++i) {
      #pragma HLS loop_tripcount min=1 max=8
      //#pragma HLS DATAFLOW
      
      fft_cols(strmFFTcols_inp, strmFFTcols_out);
   }
}
