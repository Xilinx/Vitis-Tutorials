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

#include <iostream>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_fir.h>

#include "fir_hls.h"
#include "system_settings_comm.h"

struct fir_params : hls::ip_fir::params_t {
    static const unsigned num_coeffs    = N_FIR_TAPS;
    static const double   coeff_vec[N_FIR_TAPS];
    static const unsigned coeff_width   = 16;
    static const unsigned input_width   = 16;
    static const unsigned output_width  = 16;
    static const unsigned output_rounding_mode = hls::ip_fir::truncate_lsbs;
    static const unsigned input_length  = WINDOW_LENGTH;
    static const unsigned output_length = WINDOW_LENGTH;
    static const unsigned sample_period = 1;  /* Go For Maximum Performance */
    static const unsigned coeff_structure = hls::ip_fir::symmetric;
};

const double fir_params::coeff_vec[N_FIR_TAPS]  = FIR_COEF_VALUES;

static hls::FIR<fir_params> fir_real[N_FIR_FILTERS];
static hls::FIR<fir_params> fir_imag[N_FIR_FILTERS];


void complex_split (hls::stream<ap_axiu<128, 0, 0, 0> >& StreamIn,
                    DataWindow_t DataRealIn,
                    DataWindow_t DataImagIn
                   )
{
   for (int ix = 0 ; ix < WINDOW_LENGTH; ix=ix+4)  {
      #pragma HLS PIPELINE II=1
      
      ap_axiu<128, 0, 0, 0> StreamWord = StreamIn.read();
      ap_uint<128> word = StreamWord.data;
      
      DataRealIn[ix] = word.range(31,16);
      DataImagIn[ix] = word.range(15,0);
      
      DataRealIn[ix+1] = word.range(63,48);
      DataImagIn[ix+1] = word.range(47,32);
      
      DataRealIn[ix+2] = word.range(95,80);
      DataImagIn[ix+2] = word.range(79,64);
      
      DataRealIn[ix+3] = word.range(127,112);
      DataImagIn[ix+3] = word.range(111,96);
      
      //printf("To Filter:  %2d ->  %5d %5di\n", ix, (int) DataRealIn[ix], (int) DataImagIn[ix]);
   }
}


void complex_merge (DataWindow_t DataRealOut,
		    DataWindow_t DataImagOut,
		    hls::stream<ap_axiu<128, 0, 0, 0> >& StreamOut
		)
{
   for (int ix = 0 ; ix < WINDOW_LENGTH; ix=ix+4)  {
      #pragma HLS PIPELINE II=1
         //printf("From Filter:  %2d ->  %5d %5di\n", ix, (int) DataRealOut[ix], (int) DataImagOut[ix]);
         
         ap_uint<128> word;
         
         word.range(31 , 0)   = (((uint32_t) DataRealOut[ix]) << 16) | (DataImagOut[ix] & 0xFFFF);
         word.range(63 , 32)  = (((uint32_t)  DataRealOut[ix+1]) << 16) | (DataImagOut[ix+1] & 0xFFFF);
         word.range(95 , 64)  = (((uint32_t)  DataRealOut[ix+2]) << 16) | (DataImagOut[ix+2] & 0xFFFF);
         word.range(127 , 96) = (((uint32_t)  DataRealOut[ix+3]) << 16) | (DataImagOut[ix+3] & 0xFFFF);
         
         ap_axiu<128, 0, 0, 0> StreamWord;
         
         StreamWord.data = word;
         StreamOut.write(StreamWord);
   }
}


void hls_run (int N,
              DataWindow_t DataRealIn,
              DataWindow_t DataImagIn,
              DataWindow_t DataRealOut,
              DataWindow_t DataImagOut
             )
{
    #pragma HLS dataflow
    fir_real[N].run(DataRealIn, DataRealOut);
    fir_imag[N].run(DataImagIn, DataImagOut);
}


void fir_wrap (hls::stream<ap_axiu<128, 0, 0, 0> >& StreamIn,
               hls::stream<ap_axiu<128, 0, 0, 0> >& StreamOut
              )
{
    #pragma HLS dataflow

    DataWindow_t DataRealIn,  DataImagIn;
    DataWindow_t DataRealOut, DataImagOut;
    #pragma HLS stream variable=DataRealIn   depth=16
    #pragma HLS stream variable=DataRealOut  depth=16
    #pragma HLS stream variable=DataImagIn   depth=16
    #pragma HLS stream variable=DataImagOut  depth=16

    complex_split(StreamIn, DataRealIn, DataImagIn);

#if (N_FIR_FILTERS == 1)
    hls_run(0, DataRealIn,  DataImagIn,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 1)
    DataWindow_t DataReal_0,  DataImag_0;
    #pragma HLS stream variable=DataReal_0  depth=16
    #pragma HLS stream variable=DataImag_0  depth=16
    hls_run(0, DataRealIn,  DataImagIn,  DataReal_0, DataImag_0);
#endif

#if (N_FIR_FILTERS == 2)
    hls_run(1, DataReal_0,  DataImag_0,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 2)
    DataWindow_t DataReal_1,  DataImag_1;
    #pragma HLS stream variable=DataReal_1  depth=16
    #pragma HLS stream variable=DataImag_1  depth=16
    hls_run(1, DataReal_0,  DataImag_0,  DataReal_1, DataImag_1);
#endif

#if (N_FIR_FILTERS == 3)
    hls_run(2, DataReal_1,  DataImag_1,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 3)
    DataWindow_t DataReal_2,  DataImag_2;
    #pragma HLS stream variable=DataReal_2  depth=16
    #pragma HLS stream variable=DataImag_2  depth=16
    hls_run(2, DataReal_1,  DataImag_1,  DataReal_2, DataImag_2);
#endif

#if (N_FIR_FILTERS == 4)
    hls_run(3, DataReal_2,  DataImag_2,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 4)
    DataWindow_t DataReal_3,  DataImag_3;
    #pragma HLS stream variable=DataReal_3  depth=16
    #pragma HLS stream variable=DataImag_3  depth=16
    hls_run(3, DataReal_2,  DataImag_2,  DataReal_3, DataImag_3);
#endif

#if (N_FIR_FILTERS == 5)
    hls_run(4, DataReal_3,  DataImag_3,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 5)
    DataWindow_t DataReal_4,  DataImag_4;
    #pragma HLS stream variable=DataReal_4  depth=16
    #pragma HLS stream variable=DataImag_4  depth=16
    hls_run(4, DataReal_3,  DataImag_3,  DataReal_4, DataImag_4);
#endif

#if (N_FIR_FILTERS == 6)
    hls_run(5, DataReal_4,  DataImag_4,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 6)
    DataWindow_t DataReal_5,  DataImag_5;
    #pragma HLS stream variable=DataReal_5  depth=16
    #pragma HLS stream variable=DataImag_5  depth=16
    hls_run(5, DataReal_4,  DataImag_4,  DataReal_5, DataImag_5);
#endif

#if (N_FIR_FILTERS == 7)
    hls_run(6, DataReal_5,  DataImag_5,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 7)
    DataWindow_t DataReal_6,  DataImag_6;
    #pragma HLS stream variable=DataReal_6  depth=16
    #pragma HLS stream variable=DataImag_6  depth=16
    hls_run(6, DataReal_5,  DataImag_5,  DataReal_6, DataImag_6);
#endif

#if (N_FIR_FILTERS == 8)
    hls_run(7, DataReal_6,  DataImag_6,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 8)
    DataWindow_t DataReal_7,  DataImag_7;
    #pragma HLS stream variable=DataReal_7  depth=16
    #pragma HLS stream variable=DataImag_7  depth=16
    hls_run(7, DataReal_6,  DataImag_6,  DataReal_7, DataImag_7);
#endif

#if (N_FIR_FILTERS == 9)
    hls_run(8, DataReal_7,  DataImag_7,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 9)
    DataWindow_t DataReal_8,  DataImag_8;
    #pragma HLS stream variable=DataReal_8  depth=16
    #pragma HLS stream variable=DataImag_8  depth=16
    hls_run(8, DataReal_7,  DataImag_7,  DataReal_8, DataImag_8);
#endif

#if (N_FIR_FILTERS == 10)
    hls_run(9, DataReal_8,  DataImag_8,  DataRealOut, DataImagOut);
#endif

    complex_merge(DataRealOut, DataImagOut, StreamOut);
}


void fir_hls(
      hls::stream<ap_axiu<128, 0, 0, 0> >& StreamIn,
      hls::stream<ap_axiu<128, 0, 0, 0> >& StreamOut//,
      //int iterCnt
   )
{
   #pragma HLS interface axis port=StreamIn
   #pragma HLS interface axis port=StreamOut
   
   //#pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   
   //#pragma HLS INTERFACE s_axilite port=return bundle=control  
   #pragma HLS interface ap_ctrl_none port=return
   
   //#pragma HLS dataflow
   
   //ITER_FIR:for(int i = 0; i < iterCnt; ++i) {
   //   #pragma HLS loop_tripcount min=1 max=8
   //   #pragma HLS dataflow
      
      fir_wrap(StreamIn, StreamOut);
   //}
}

