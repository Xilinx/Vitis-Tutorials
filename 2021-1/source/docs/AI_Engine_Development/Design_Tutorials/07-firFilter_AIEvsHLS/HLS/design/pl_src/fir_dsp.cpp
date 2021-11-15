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

#include "fir_dsp.h"
#include "system_settings.h"

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


void complex_split (hls::stream<ap_axiu<32, 0, 0, 0> >& StreamIn,
                    DataWindow_t DataRealIn,
                    DataWindow_t DataImagIn
                   )
{
    for (int ix = 0 ; ix < WINDOW_LENGTH; ix++)  {
        #pragma HLS PIPELINE II=1
        ap_axiu<32, 0, 0, 0> StreamWord = StreamIn.read();
	uint32_t word = StreamWord.data;
	DataRealIn[ix] = (word >> 16) & 0xFFFF;
	DataImagIn[ix] =  word        & 0xFFFF;
//	printf("To Filter:  %2d ->  %5d %5di\n", ix, (int) DataRealIn[ix], (int) DataImagIn[ix]);
    }
}


void complex_merge (DataWindow_t DataRealOut,
		    DataWindow_t DataImagOut,
		    hls::stream<ap_axiu<32, 0, 0, 0> >& StreamOut
		)
{
    for (int ix = 0 ; ix < WINDOW_LENGTH; ix++)  {
        #pragma HLS PIPELINE II=1
//        printf("From Filter:  %2d ->  %5d %5di\n", ix, (int) DataRealOut[ix], (int) DataImagOut[ix]);
        uint32_t word = (((uint32_t) DataRealOut[ix]) << 16) | (DataImagOut[ix] & 0xFFFF);
        ap_axiu<32, 0, 0, 0> StreamWord;
        StreamWord.data = word;
        StreamOut.write(StreamWord);
    }
}


void dsp_run (int N,
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


void fir_wrap (hls::stream<ap_axiu<32, 0, 0, 0> >& StreamIn,
               hls::stream<ap_axiu<32, 0, 0, 0> >& StreamOut
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
    dsp_run(0, DataRealIn,  DataImagIn,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 1)
    DataWindow_t DataReal_0,  DataImag_0;
    #pragma HLS stream variable=DataReal_0  depth=16
    #pragma HLS stream variable=DataImag_0  depth=16
    dsp_run(0, DataRealIn,  DataImagIn,  DataReal_0, DataImag_0);
#endif

#if (N_FIR_FILTERS == 2)
    dsp_run(1, DataReal_0,  DataImag_0,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 2)
    DataWindow_t DataReal_1,  DataImag_1;
    #pragma HLS stream variable=DataReal_1  depth=16
    #pragma HLS stream variable=DataImag_1  depth=16
    dsp_run(1, DataReal_0,  DataImag_0,  DataReal_1, DataImag_1);
#endif

#if (N_FIR_FILTERS == 3)
    dsp_run(2, DataReal_1,  DataImag_1,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 3)
    DataWindow_t DataReal_2,  DataImag_2;
    #pragma HLS stream variable=DataReal_2  depth=16
    #pragma HLS stream variable=DataImag_2  depth=16
    dsp_run(2, DataReal_1,  DataImag_1,  DataReal_2, DataImag_2);
#endif

#if (N_FIR_FILTERS == 4)
    dsp_run(3, DataReal_2,  DataImag_2,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 4)
    DataWindow_t DataReal_3,  DataImag_3;
    #pragma HLS stream variable=DataReal_3  depth=16
    #pragma HLS stream variable=DataImag_3  depth=16
    dsp_run(3, DataReal_2,  DataImag_2,  DataReal_3, DataImag_3);
#endif

#if (N_FIR_FILTERS == 5)
    dsp_run(4, DataReal_3,  DataImag_3,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 5)
    DataWindow_t DataReal_4,  DataImag_4;
    #pragma HLS stream variable=DataReal_4  depth=16
    #pragma HLS stream variable=DataImag_4  depth=16
    dsp_run(4, DataReal_3,  DataImag_3,  DataReal_4, DataImag_4);
#endif

#if (N_FIR_FILTERS == 6)
    dsp_run(5, DataReal_4,  DataImag_4,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 6)
    DataWindow_t DataReal_5,  DataImag_5;
    #pragma HLS stream variable=DataReal_5  depth=16
    #pragma HLS stream variable=DataImag_5  depth=16
    dsp_run(5, DataReal_4,  DataImag_4,  DataReal_5, DataImag_5);
#endif

#if (N_FIR_FILTERS == 7)
    dsp_run(6, DataReal_5,  DataImag_5,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 7)
    DataWindow_t DataReal_6,  DataImag_6;
    #pragma HLS stream variable=DataReal_6  depth=16
    #pragma HLS stream variable=DataImag_6  depth=16
    dsp_run(6, DataReal_5,  DataImag_5,  DataReal_6, DataImag_6);
#endif

#if (N_FIR_FILTERS == 8)
    dsp_run(7, DataReal_6,  DataImag_6,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 8)
    DataWindow_t DataReal_7,  DataImag_7;
    #pragma HLS stream variable=DataReal_7  depth=16
    #pragma HLS stream variable=DataImag_7  depth=16
    dsp_run(7, DataReal_6,  DataImag_6,  DataReal_7, DataImag_7);
#endif

#if (N_FIR_FILTERS == 9)
    dsp_run(8, DataReal_7,  DataImag_7,  DataRealOut, DataImagOut);
#elif (N_FIR_FILTERS > 9)
    DataWindow_t DataReal_8,  DataImag_8;
    #pragma HLS stream variable=DataReal_8  depth=16
    #pragma HLS stream variable=DataImag_8  depth=16
    dsp_run(8, DataReal_7,  DataImag_7,  DataReal_8, DataImag_8);
#endif

#if (N_FIR_FILTERS == 10)
    dsp_run(9, DataReal_8,  DataImag_8,  DataRealOut, DataImagOut);
#endif

    complex_merge(DataRealOut, DataImagOut, StreamOut);
}


void fir_dsp (hls::stream<ap_axiu<32, 0, 0, 0> >& StreamIn,
              hls::stream<ap_axiu<32, 0, 0, 0> >& StreamOut
             )
{
    #pragma HLS interface ap_ctrl_none port=return

    fir_wrap(StreamIn, StreamOut);
}

