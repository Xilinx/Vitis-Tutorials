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

// FFTrows_graph FFT point size...
#define FFT_ROW_TP_POINT_SIZE MAT_COLS
// FFTcols_graph FFT point size...
#define FFT_COL_TP_POINT_SIZE MAT_ROWS

// 1 (FFT) or 0 (IFFT)...
#define FFT_2D_TP_FFT_NIFFT 1
// 0 Bit Shift before output, will have to change based on input...
#define FFT_2D_TP_SHIFT 0
// FFT divided over 1 FFT Kernel...
#define FFT_2D_TP_CASC_LEN 1
// Dynamic FFT Point Size is disabled...
#define FFT_2D_TP_DYN_PT_SIZE 0

// TP_WINDOW_VSIZE for FFTrows_graph...
#define FFT_ROW_TP_WINDOW_VSIZE MAT_COLS

// TP_WINDOW_VSIZE for FFTcols_graph
// Increasing the "TP__WINDOW _VSIZE" so that the ping-pong overhead is less
// Assigning it as MAT_COLS instead of MAT_ROWS...
#define FFT_COL_TP_WINDOW_VSIZE MAT_COLS

////////////////////////////////////////////////////////
// FFT_2D Datatype related Macros
// Datatype can be:
// cint16 (Default) or cfloat...
// 0=cint16(Default)
// 1=cfloat
#if FFT_2D_DT == 0

   // Input data type...
   #define FFT_2D_TT_DATA cint16
   // Twiddle Factor data type...
   #define FFT_2D_TT_TWIDDLE cint16
   
   // FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 4)
   // FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 4)
   
#elif FFT_2D_DT == 1

   // Input data type...
   #define FFT_2D_TT_DATA cfloat
   // Twiddle Factor data type...
   #define FFT_2D_TT_TWIDDLE cfloat
   
   // FFTrows_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_ROW_WINDOW_BUFF_SIZE (FFT_ROW_TP_WINDOW_VSIZE * 8)
   // FFTcols_graph I/O WINDOW BUFF SIZE IN BYTES...
   #define FFT_COL_WINDOW_BUFF_SIZE (FFT_COL_TP_WINDOW_VSIZE * 8)
   
#endif

#include "adf.h"
#include "fft_ifft_dit_1ch_graph.hpp"

using namespace adf;
namespace dsplib = xf::dsp::aie;

class FFTrows_graph: public graph
{
   public:
   	port<input>   in;
   	port<output> out;
      
   	// Constructor - with Rowise FFT graph class initialization...
   	FFTrows_graph()
      {
         dsplib::fft::dit_1ch::fft_ifft_dit_1ch_graph<FFT_2D_TT_DATA, FFT_2D_TT_TWIDDLE, FFT_ROW_TP_POINT_SIZE,
         FFT_2D_TP_FFT_NIFFT, FFT_2D_TP_SHIFT, FFT_2D_TP_CASC_LEN, FFT_2D_TP_DYN_PT_SIZE, FFT_ROW_TP_WINDOW_VSIZE> FFTrow_gr;
         
         //runtime<ratio>(*FFTrow_gr.getKernels()) = 0.6;
         
         connect< window<FFT_ROW_WINDOW_BUFF_SIZE> > (in,   FFTrow_gr.in[0]);
         connect< window<FFT_ROW_WINDOW_BUFF_SIZE> > (FFTrow_gr.out[0], out);
   	}
};

class FFTcols_graph: public graph
{
   public:
   	port<input>   in;
   	port<output> out;
   
   	// Constructor - with Colwise FFT graph class initialization...
   	FFTcols_graph()
      {
         dsplib::fft::dit_1ch::fft_ifft_dit_1ch_graph<FFT_2D_TT_DATA, FFT_2D_TT_TWIDDLE, FFT_COL_TP_POINT_SIZE,
         FFT_2D_TP_FFT_NIFFT, FFT_2D_TP_SHIFT, FFT_2D_TP_CASC_LEN, FFT_2D_TP_DYN_PT_SIZE, FFT_COL_TP_WINDOW_VSIZE> FFTcol_gr;
         
         //runtime<ratio>(*FFTcol_gr.getKernels()) = 0.6;
         
         connect< window<FFT_COL_WINDOW_BUFF_SIZE> > (in,   FFTcol_gr.in[0]);
         connect< window<FFT_COL_WINDOW_BUFF_SIZE> > (FFTcol_gr.out[0], out);
   	}
};
