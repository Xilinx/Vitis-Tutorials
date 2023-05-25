/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

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
         cmpxDataIn   cols_in[MAT_ROWS],
         cmpxDataOut cols_out[MAT_ROWS],
         bool* ovflo)

#else // cfloat datatype

   void fftCol(
         bool direction,
         cmpxDataIn   cols_in[MAT_ROWS],
         cmpxDataOut cols_out[MAT_ROWS])

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
   
   hls::fft<configCol>(cols_in, cols_out, &fft_status, &fft_config);
   
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
         cmpxDataIn   rows_in[MAT_COLS],
         cmpxDataOut rows_out[MAT_COLS],
         bool* ovflo)

#else // cfloat datatype

   void fftRow(
         bool direction,
         cmpxDataIn   rows_in[MAT_COLS],
         cmpxDataOut rows_out[MAT_COLS])

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
   
   hls::fft<configRow>(rows_in, rows_out, &fft_status, &fft_config);
   
   #if FFT_2D_DT == 0 // cint16 datatype
      fftRow_status(&fft_status, ovflo);
   #endif
}

void readIn_row(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn rows_in[MAT_COLS]
               )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_ROW_READ_INP:for(int j = 0; j < MAT_COLS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         
         ap_axiu<128, 0, 0, 0> ap = strm_inp.read();
         ap.keep=-1;
         
         cmpxDataIn tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range( 15,   0);
         tmp_in.imag().range(15, 0) = ap.data.range( 31,  16);
         rows_in[j] = tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range( 47,  32);
         tmp_in.imag().range(15, 0) = ap.data.range( 63,  48);
         rows_in[j + 1] = tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range( 79,  64);
         tmp_in.imag().range(15, 0) = ap.data.range( 95,  80);
         rows_in[j + 2] = tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range(111,  96);
         tmp_in.imag().range(15, 0) = ap.data.range(127, 112);
         rows_in[j + 3] = tmp_in;
      }
   
   #else // cfloat datatype
      LOOP_FFT_ROW_READ_INP:for(int j = 0; j < MAT_COLS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=32 max=1024
         
         ap_axiu<128, 0, 0, 0> ap = strm_inp.read();
         ap.keep=-1;
         
         cmpxDataIn tmp_in;
         AXI_DATA rowInp;
         
         rowInp.data[0] = ap.data.range( 63,  0);
         rowInp.data[1] = ap.data.range(127, 64);
         
         tmp_in.real(rowInp.fl_data[0]);
         tmp_in.imag(rowInp.fl_data[1]);
         rows_in[j] = tmp_in;
         
         tmp_in.real(rowInp.fl_data[2]);
         tmp_in.imag(rowInp.fl_data[3]);
         rows_in[j + 1] = tmp_in;
      }
   
   #endif
}

void writeOut_row(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut rows_out[MAT_COLS]
                 )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_ROW_WRITE_OUT:for(int j = 0; j < MAT_COLS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         ap_axiu<128, 0, 0, 0> ap;
         
         cmpxDataOut tmp;
         tmp = rows_out[j];

         ap.data.range( 15,   0) = real(tmp).range(15, 0);
         ap.data.range( 31,  16) = imag(tmp).range(15, 0);

         tmp = rows_out[j+1];

         ap.data.range( 47,  32) = real(tmp).range(15, 0);
         ap.data.range( 63,  48) = imag(tmp).range(15, 0);

         tmp = rows_out[j+2];

         ap.data.range( 79,  64) = real(tmp).range(15, 0);
         ap.data.range( 95,  80) = imag(tmp).range(15, 0);

         tmp = rows_out[j+3];

         ap.data.range(111,  96) = real(tmp).range(15, 0);
         ap.data.range(127, 112) = imag(tmp).range(15, 0);
         
         strm_out.write(ap);
      }
   
   #else // cfloat datatype
      LOOP_FFT_ROW_WRITE_OUT:for(int j = 0; j < MAT_COLS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=32 max=1024
         
         ap_axiu<128, 0, 0, 0> ap;
         
         AXI_DATA rowOut;
         cmpxDataOut tmp;
         tmp = rows_out[j];
         rowOut.fl_data[0] = real(tmp);
         rowOut.fl_data[1] = imag(tmp);
         tmp = rows_out[j+1];
         rowOut.fl_data[2] = real(tmp);
         rowOut.fl_data[3] = imag(tmp);

         ap.data.range( 63,  0) = rowOut.data[0];
         ap.data.range(127, 64) = rowOut.data[1];
         
         strm_out.write(ap);
      }
   #endif
}

void fft_rows(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out
     )
{
   LOOP_FFT_ROWS:for(int i = 0; i < MAT_ROWS; ++i) {
      #pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=32 max=1024
      
      #if FFT_2D_DT == 0 // cint16 datatype
         cmpxDataIn rows_in[MAT_COLS];
         #pragma HLS STREAM variable=rows_in depth=1024
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=4 dim=1
         
         cmpxDataOut rows_out[MAT_COLS];
         #pragma HLS STREAM variable=rows_out depth=1024
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=4 dim=1
      
      #else // cfloat datatype
         cmpxDataIn rows_in[MAT_COLS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=rows_in depth=512
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=2 dim=1
         
         cmpxDataOut rows_out[MAT_COLS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=rows_out depth=512
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=2 dim=1
      
      #endif
      
      bool directionStub = 1;
      
      #if FFT_2D_DT == 0 // cint16 datatype
         bool ovfloStub;
      #endif
      
      readIn_row(strm_inp, rows_in);
      
      #if FFT_2D_DT == 0 // cint16 datatype
         fftRow(directionStub, rows_in, rows_out, &ovfloStub);
      
      #else // cfloat datatype
         fftRow(directionStub, rows_in, rows_out);
      
      #endif
      
      writeOut_row(strm_out, rows_out);
   }
}

void readIn_col(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
                cmpxDataIn cols_in[MAT_ROWS]
               )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_COL_READ_INP:for(int j = 0; j < MAT_ROWS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=8 max=256
         
         ap_axiu<128, 0, 0, 0> ap = strm_inp.read();
         ap.keep=-1;
         
         cmpxDataIn tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range( 15,   0);
         tmp_in.imag().range(15, 0) = ap.data.range( 31,  16);
         cols_in[j] = tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range( 47,  32);
         tmp_in.imag().range(15, 0) = ap.data.range( 63,  48);
         cols_in[j + 1] = tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range( 79,  64);
         tmp_in.imag().range(15, 0) = ap.data.range( 95,  80);
         cols_in[j + 2] = tmp_in;
         
         tmp_in.real().range(15, 0) = ap.data.range(111,  96);
         tmp_in.imag().range(15, 0) = ap.data.range(127, 112);
         cols_in[j + 3] = tmp_in;
      }
   
   #else // cfloat datatype
      LOOP_FFT_COL_READ_INP:for(int j = 0; j < MAT_ROWS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         
         ap_axiu<128, 0, 0, 0> ap = strm_inp.read();
         ap.keep=-1;
         
         cmpxDataIn tmp_in;
         AXI_DATA colInp;
         
         colInp.data[0] = ap.data.range( 63,  0);
         colInp.data[1] = ap.data.range(127, 64);
         
         tmp_in.real(colInp.fl_data[0]);
         tmp_in.imag(colInp.fl_data[1]);
         cols_in[j] = tmp_in;
         
         tmp_in.real(colInp.fl_data[2]);
         tmp_in.imag(colInp.fl_data[3]);
         cols_in[j + 1] = tmp_in;
      }
   
   #endif
}

void writeOut_col(hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out,
                  cmpxDataOut cols_out[MAT_ROWS]
                 )
{
   #if FFT_2D_DT == 0 // cint16 datatype
      LOOP_FFT_COL_WRITE_OUT:for(int j = 0; j < MAT_ROWS; j += 4) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=16 max=512
         
         ap_axiu<128, 0, 0, 0> ap;
         cmpxDataOut tmp;
         tmp = cols_out[j];

         ap.data.range( 15,   0) = real(tmp).range(15, 0);
         ap.data.range( 31,  16) = imag(tmp).range(15, 0);
         tmp = cols_out[j+1];

         ap.data.range( 47,  32) = real(tmp).range(15, 0);
         ap.data.range( 63,  48) = imag(tmp).range(15, 0);
         tmp = cols_out[j+2];

         ap.data.range( 79,  64) = real(tmp).range(15, 0);
         ap.data.range( 95,  80) = imag(tmp).range(15, 0);
         tmp = cols_out[j+3];

         ap.data.range(111,  96) = real(tmp).range(15, 0);
         ap.data.range(127, 112) = imag(tmp).range(15, 0);
         
         strm_out.write(ap);
      }
   
   #else // cfloat datatype
      LOOP_FFT_COL_WRITE_OUT:for(int j = 0; j < MAT_ROWS; j += 2) {
         #pragma HLS PIPELINE II=1
         #pragma HLS loop_tripcount min=32 max=1024
         
         ap_axiu<128, 0, 0, 0> ap;
         
         AXI_DATA colOut;
         cmpxDataOut tmp;
         tmp = cols_out[j];
         colOut.fl_data[0] = real(tmp);
         colOut.fl_data[1] = imag(tmp);
         tmp = cols_out[j+1];
         colOut.fl_data[2] = real(tmp);
         colOut.fl_data[3] = imag(tmp);

         ap.data.range( 63,  0) = colOut.data[0];
         ap.data.range(127, 64) = colOut.data[1];
         
         strm_out.write(ap);
      }
   #endif
}

void fft_cols(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strm_inp,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strm_out
     )
{
   LOOP_FFT_COLS:for(int i = 0; i < MAT_COLS; ++i) {
      #pragma HLS DATAFLOW
      #pragma HLS loop_tripcount min=64 max=2048
      
      #if FFT_2D_DT == 0 // cint16 datatype
         cmpxDataIn cols_in[MAT_ROWS];
         //#pragma HLS STREAM variable=in depth=1024
         #pragma HLS STREAM variable=cols_in depth=32
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=4 dim=1
         
         cmpxDataOut cols_out[MAT_ROWS];
         #pragma HLS STREAM variable=cols_out depth=32
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=4 dim=1
      
      #else // cfloat datatype
         cmpxDataIn cols_in[MAT_ROWS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=cols_in depth=16
         //#pragma HLS ARRAY_RESHAPE variable=in cyclic factor=2 dim=1
         
         cmpxDataOut cols_out[MAT_ROWS] __attribute__((no_ctor));
         #pragma HLS STREAM variable=cols_out depth=16
         //#pragma HLS ARRAY_RESHAPE variable=out cyclic factor=2 dim=1
      
      #endif
      
      bool directionStub = 1;
      
      #if FFT_2D_DT == 0 // cint16 datatype
         bool ovfloStub;
      #endif
      
      readIn_col(strm_inp, cols_in);
      
      #if FFT_2D_DT == 0 // cint16 datatype
         fftCol(directionStub, cols_in, cols_out, &ovfloStub);
      
      #else // cfloat datatype
         fftCol(directionStub, cols_in, cols_out);
      
      #endif
      
      writeOut_col(strm_out, cols_out);
   }
}

void fft_2d(
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTrows_inp,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTrows_out,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTcols_inp,
      hls::stream<ap_axiu<128, 0, 0, 0>> &strmFFTcols_out//,
      //uint32_t iterCnt
     )
{
   #pragma HLS interface axis port=strmFFTrows_inp
   #pragma HLS interface axis port=strmFFTrows_out
   #pragma HLS interface axis port=strmFFTcols_inp
   #pragma HLS interface axis port=strmFFTcols_out
   
   //#pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   //#pragma HLS INTERFACE s_axilite port=return bundle=control
   #pragma HLS interface ap_ctrl_none port=return
   
   #pragma HLS DATAFLOW
   
   //ITER_LOOP_FFT_ROWS:for(int i = iterCnt; i ; --i) {
   //   #pragma HLS loop_tripcount min=1 max=8
   //   //#pragma HLS DATAFLOW
   //   
   //   fft_rows(strmFFTrows_inp, strmFFTrows_out);
   //}
   //
   //ITER_LOOP_FFT_COLS:for(int j = iterCnt; j ; --j) {
   //   #pragma HLS loop_tripcount min=1 max=8
   //   //#pragma HLS DATAFLOW
   //   
   //   fft_cols(strmFFTcols_inp, strmFFTcols_out);
   //}
   
   fft_rows(strmFFTrows_inp, strmFFTrows_out);
   fft_cols(strmFFTcols_inp, strmFFTcols_out);
}
