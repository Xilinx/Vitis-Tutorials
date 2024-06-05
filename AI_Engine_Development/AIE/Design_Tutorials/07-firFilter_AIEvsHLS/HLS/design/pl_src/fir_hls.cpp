/*
   Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
   SPDX-License-Identifier: MIT
   */


#include "fir_hls.h"

const double fir_params::coeff_vec[N_FIR_TAPS] = FIR_COEF_VALUES;

void complex_split(
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp,
        DataWindow_t DataRealInp, DataWindow_t DataImagInp
        )
{
CMPLX_SPLIT_LOOP:for(int ix = 0; ix < WINDOW_LENGTH; ix += 4) {
#pragma HLS PIPELINE II=1

                     ap_axiu<128, 0, 0, 0> fir_inp = strmInp.read();

                     // To enable Dataflow...
                     Data_t tmp_imag_inp[4];
                     Data_t tmp_real_inp[4];
                     //#pragma HLS ARRAY_RESHAPE variable=tmp_imag_inp cyclic factor=4 dim=1
                     //#pragma HLS ARRAY_RESHAPE variable=tmp_real_inp cyclic factor=4 dim=1

                     tmp_imag_inp[0].range(15, 0) = fir_inp.data.range(15, 0);
                     tmp_real_inp[0].range(15, 0) = fir_inp.data.range(31, 16);

                     tmp_imag_inp[1].range(15, 0) = fir_inp.data.range(47, 32);
                     tmp_real_inp[1].range(15, 0) = fir_inp.data.range(63, 48);

                     tmp_imag_inp[2].range(15, 0) = fir_inp.data.range(79, 64);
                     tmp_real_inp[2].range(15, 0) = fir_inp.data.range(95, 80);

                     tmp_imag_inp[3].range(15, 0) = fir_inp.data.range(111, 96);
                     tmp_real_inp[3].range(15, 0) = fir_inp.data.range(127, 112);

                     DataImagInp[ix] = tmp_imag_inp[0];
                     DataRealInp[ix] = tmp_real_inp[0];

                     DataImagInp[ix + 1] = tmp_imag_inp[1];
                     DataRealInp[ix + 1] = tmp_real_inp[1];

                     DataImagInp[ix + 2] = tmp_imag_inp[2];
                     DataRealInp[ix + 2] = tmp_real_inp[2];

                     DataImagInp[ix + 3] = tmp_imag_inp[3];
                     DataRealInp[ix + 3] = tmp_real_inp[3];
                 }
}

void complex_merge(
        DataWindow_t DataRealOut, DataWindow_t DataImagOut,
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut
        )
{
CMPLX_MERGE_LOOP:for(int ix = 0; ix < WINDOW_LENGTH; ix += 4) {
#pragma HLS PIPELINE II=1

                     ap_axiu<128, 0, 0, 0> fir_out;

                     fir_out.data.range(15,  0) = DataImagOut[ix].range(15, 0);
                     fir_out.data.range(31, 16) = DataRealOut[ix].range(15, 0);

                     fir_out.data.range(47, 32) = DataImagOut[ix + 1].range(15, 0);
                     fir_out.data.range(63, 48) = DataRealOut[ix + 1].range(15, 0);

                     fir_out.data.range(79, 64) = DataImagOut[ix + 2].range(15, 0);
                     fir_out.data.range(95, 80) = DataRealOut[ix + 2].range(15, 0);

                     fir_out.data.range(111,  96) = DataImagOut[ix + 3].range(15, 0);
                     fir_out.data.range(127, 112) = DataRealOut[ix + 3].range(15, 0);

                     strmOut.write(fir_out);
                 }
}

void buffInp(
        DataWindow_t DataRealInp, DataWindow_t DataImagInp,
        DataWindow_t DataRealInp_buff, DataWindow_t DataImagInp_buff
        )
{
BUFF_INP_LOOP:for(int i = 0; i < WINDOW_LENGTH; ++i)
              {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=4

                  DataRealInp_buff[i] = DataRealInp[i];
                  DataImagInp_buff[i] = DataImagInp[i];
              }
}

void buffOut(
        DataWindow_t DataRealOut_buff, DataWindow_t DataImagOut_buff,
        DataWindow_t DataRealOut, DataWindow_t DataImagOut
        )
{
BUFF_OUT_LOOP:for(int i = 0; i < WINDOW_LENGTH; ++i)
              {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=4

                  DataRealOut[i] = DataRealOut_buff[i];
                  DataImagOut[i] = DataImagOut_buff[i];
              }
}
void buffInp_15tap(
        DataWindow_t DataRealInp, DataWindow_t DataImagInp,
        DataWindow_t DataRealInp_buff1, DataWindow_t DataImagInp_buff1,
        DataWindow_t DataRealInp_buff2, DataWindow_t DataImagInp_buff2
        )
{
BUFF_INP_LOOP:for(int i = 0; i < WINDOW_LENGTH_HALF; ++i)
              {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=4

                  DataRealInp_buff1[i] = DataRealInp[i];
                  DataImagInp_buff1[i] = DataImagInp[i];
                  DataRealInp_buff2[i] = DataRealInp[(WINDOW_LENGTH_HALF) + i];
                  DataImagInp_buff2[i] = DataImagInp[(WINDOW_LENGTH_HALF) + i];
              }
}
void buffOut_15tap(
        DataWindow_t DataRealOut_buff1, DataWindow_t DataImagOut_buff1,
        DataWindow_t DataRealOut_buff2, DataWindow_t DataImagOut_buff2,
        DataWindow_t DataRealOut, DataWindow_t DataImagOut
        )
{
BUFF_OUT_LOOP:for(int i = 0; i < WINDOW_LENGTH_HALF; ++i)
              {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=4

                  DataRealOut[i] = DataRealOut_buff1[i];
                  DataImagOut[i] = DataImagOut_buff1[i];
                  DataRealOut[(WINDOW_LENGTH_HALF) + i] = DataRealOut_buff2[i];
                  DataImagOut[(WINDOW_LENGTH_HALF) + i] = DataImagOut_buff2[i];
              }
}
void fir_wrap(
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp,
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut
        )
{
#pragma HLS dataflow

    DataWindow_t DataRealInp, DataImagInp;
    DataWindow_t DataRealOut, DataImagOut;
#pragma HLS stream variable=DataRealInp depth=16
#pragma HLS stream variable=DataRealOut depth=16
#pragma HLS stream variable=DataImagInp depth=16
#pragma HLS stream variable=DataImagOut depth=16
#pragma HLS ARRAY_RESHAPE variable=DataRealInp cyclic factor=4 dim=1
#pragma HLS ARRAY_RESHAPE variable=DataImagInp cyclic factor=4 dim=1
#pragma HLS ARRAY_RESHAPE variable=DataRealOut cyclic factor=4 dim=1
#pragma HLS ARRAY_RESHAPE variable=DataImagOut cyclic factor=4 dim=1

#if (N_FIR_TAPS == 15)   
    DataWindow_t DataRealInp_buff1, DataImagInp_buff1;
    DataWindow_t DataRealInp_buff2, DataImagInp_buff2;
    DataWindow_t DataRealOut_buff1, DataImagOut_buff1;
    DataWindow_t DataRealOut_buff2, DataImagOut_buff2;
#pragma HLS stream variable=DataRealInp_buff1 depth=16
#pragma HLS stream variable=DataImagInp_buff1 depth=16
#pragma HLS stream variable=DataRealInp_buff2 depth=16
#pragma HLS stream variable=DataImagInp_buff2 depth=16
#pragma HLS stream variable=DataRealOut_buff1 depth=16
#pragma HLS stream variable=DataImagOut_buff1 depth=16
#pragma HLS stream variable=DataRealOut_buff2 depth=16
#pragma HLS stream variable=DataImagOut_buff2 depth=16
#else
    DataWindow_t DataRealOut_buff, DataImagOut_buff;
    DataWindow_t DataRealInp_buff, DataImagInp_buff;
#pragma HLS stream variable=DataRealInp_buff depth=16
#pragma HLS stream variable=DataImagInp_buff depth=16
#pragma HLS stream variable=DataRealOut_buff depth=16
#pragma HLS stream variable=DataImagOut_buff depth=16
#endif

    complex_split(strmInp, DataRealInp, DataImagInp);
#if (N_FIR_TAPS == 15)   
    buffInp_15tap(DataRealInp, DataImagInp, DataRealInp_buff1, DataImagInp_buff1, DataRealInp_buff2, DataImagInp_buff2);
#else
    buffInp(DataRealInp, DataImagInp, DataRealInp_buff, DataImagInp_buff);
#endif

#if (N_FIR_FILTERS == 1)
#if (N_FIR_TAPS == 15)
    fir_real[0].run(DataRealInp_buff1, DataRealOut_buff1);
    fir_imag[0].run(DataImagInp_buff1, DataImagOut_buff1);
    fir_real[1].run(DataRealInp_buff2, DataRealOut_buff2);
    fir_imag[1].run(DataImagInp_buff2, DataImagOut_buff2);
#else
    fir_real[0].run(DataRealInp_buff, DataRealOut_buff);
    fir_imag[0].run(DataImagInp_buff, DataImagOut_buff);
#endif

#elif (N_FIR_FILTERS > 1)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_a0, DataImag_a0;
    DataWindow_t DataReal_a1, DataImag_a1;
#pragma HLS stream variable=DataReal_a0 depth=16
#pragma HLS stream variable=DataImag_a0 depth=16
#pragma HLS stream variable=DataReal_a1 depth=16
#pragma HLS stream variable=DataImag_a1 depth=16

    fir_real[0].run(DataRealInp_buff1, DataReal_a0);
    fir_imag[0].run(DataImagInp_buff1, DataImag_a0);
    fir_real[1].run(DataRealInp_buff2, DataReal_a1);
    fir_imag[1].run(DataImagInp_buff2, DataImag_a1);
#else
    DataWindow_t DataReal_0, DataImag_0;
#pragma HLS stream variable=DataReal_0 depth=16
#pragma HLS stream variable=DataImag_0 depth=16

    fir_real[0].run(DataRealInp_buff, DataReal_0);
    fir_imag[0].run(DataImagInp_buff, DataImag_0);
#endif

#endif

#if (N_FIR_FILTERS == 2)
    fir_real[1].run(DataReal_0, DataRealOut_buff);
    fir_imag[1].run(DataImag_0, DataImagOut_buff);

#elif (N_FIR_FILTERS > 2)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_b0, DataImag_b0;
    DataWindow_t DataReal_b1, DataImag_b1;
#pragma HLS stream variable=DataReal_b0 depth=16
#pragma HLS stream variable=DataImag_b0 depth=16
#pragma HLS stream variable=DataReal_b1 depth=16
#pragma HLS stream variable=DataImag_b1 depth=16

    fir_real[2].run(DataReal_a0, DataReal_b0);
    fir_imag[2].run(DataImag_a0, DataImag_b0);
    fir_real[3].run(DataReal_a1, DataReal_b1);
    fir_imag[3].run(DataImag_a1, DataImag_b1);
#else
    DataWindow_t DataReal_1, DataImag_1;
#pragma HLS stream variable=DataReal_1 depth=16
#pragma HLS stream variable=DataImag_1 depth=16

    fir_real[1].run(DataReal_0, DataReal_1);
    fir_imag[1].run(DataImag_0, DataImag_1);
#endif

#endif

#if (N_FIR_FILTERS == 3)
    fir_real[2].run(DataReal_1, DataRealOut_buff);
    fir_imag[2].run(DataImag_1, DataImagOut_buff);

#elif (N_FIR_FILTERS > 3)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_c0, DataImag_c0;
    DataWindow_t DataReal_c1, DataImag_c1;
#pragma HLS stream variable=DataReal_c0 depth=16
#pragma HLS stream variable=DataImag_c0 depth=16
#pragma HLS stream variable=DataReal_c1 depth=16
#pragma HLS stream variable=DataImag_c1 depth=16

    fir_real[4].run(DataReal_b0, DataReal_c0);
    fir_imag[4].run(DataImag_b0, DataImag_c0);
    fir_real[5].run(DataReal_b1, DataReal_c1);
    fir_imag[5].run(DataImag_b1, DataImag_c1);
#else
    DataWindow_t DataReal_2, DataImag_2;
#pragma HLS stream variable=DataReal_2 depth=16
#pragma HLS stream variable=DataImag_2 depth=16

    fir_real[2].run(DataReal_1, DataReal_2);
    fir_imag[2].run(DataImag_1, DataImag_2);
#endif

#endif

#if (N_FIR_FILTERS == 4)
    fir_real[3].run(DataReal_2, DataRealOut_buff);
    fir_imag[3].run(DataImag_2, DataImagOut_buff);

#elif (N_FIR_FILTERS > 4)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_d0, DataImag_d0;
    DataWindow_t DataReal_d1, DataImag_d1;
#pragma HLS stream variable=DataReal_d0 depth=16
#pragma HLS stream variable=DataImag_d0 depth=16
#pragma HLS stream variable=DataReal_d1 depth=16
#pragma HLS stream variable=DataImag_d1 depth=16

    fir_real[6].run(DataReal_c0, DataReal_d0);
    fir_imag[6].run(DataImag_c0, DataImag_d0);
    fir_real[7].run(DataReal_c1, DataReal_d1);
    fir_imag[7].run(DataImag_c1, DataImag_d1);
#else
    DataWindow_t DataReal_3, DataImag_3;
#pragma HLS stream variable=DataReal_3 depth=16
#pragma HLS stream variable=DataImag_3 depth=16

    fir_real[3].run(DataReal_2, DataReal_3);
    fir_imag[3].run(DataImag_2, DataImag_3);
#endif

#endif

#if (N_FIR_FILTERS == 5)
    fir_real[4].run(DataReal_3, DataRealOut_buff);
    fir_imag[4].run(DataImag_3, DataImagOut_buff);

#elif (N_FIR_FILTERS > 5)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_e0, DataImag_e0;
    DataWindow_t DataReal_e1, DataImag_e1;
#pragma HLS stream variable=DataReal_e0 depth=16
#pragma HLS stream variable=DataImag_e0 depth=16
#pragma HLS stream variable=DataReal_e1 depth=16
#pragma HLS stream variable=DataImag_e1 depth=16

    fir_real[8].run(DataReal_d0, DataReal_e0);
    fir_imag[8].run(DataImag_d0, DataImag_e0);
    fir_real[9].run(DataReal_d1, DataReal_e1);
    fir_imag[9].run(DataImag_d1, DataImag_e1);
#else
    DataWindow_t DataReal_4, DataImag_4;
#pragma HLS stream variable=DataReal_4 depth=16
#pragma HLS stream variable=DataImag_4 depth=16

    fir_real[4].run(DataReal_3, DataReal_4);
    fir_imag[4].run(DataImag_3, DataImag_4);
#endif   

#endif

#if (N_FIR_FILTERS == 6)
    fir_real[5].run(DataReal_4, DataRealOut_buff);
    fir_imag[5].run(DataImag_4, DataImagOut_buff);

#elif (N_FIR_FILTERS > 6)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_f0, DataImag_f0;
    DataWindow_t DataReal_f1, DataImag_f1;
#pragma HLS stream variable=DataReal_f0 depth=16
#pragma HLS stream variable=DataImag_f0 depth=16
#pragma HLS stream variable=DataReal_f1 depth=16
#pragma HLS stream variable=DataImag_f1 depth=16

    fir_real[10].run(DataReal_e0, DataReal_f0);
    fir_imag[10].run(DataImag_e0, DataImag_f0);
    fir_real[11].run(DataReal_e1, DataReal_f1);
    fir_imag[11].run(DataImag_e1, DataImag_f1);
#else
    DataWindow_t DataReal_5, DataImag_5;
#pragma HLS stream variable=DataReal_5 depth=16
#pragma HLS stream variable=DataImag_5 depth=16

    fir_real[5].run(DataReal_4, DataReal_5);
    fir_imag[5].run(DataImag_4, DataImag_5);
#endif 
#endif

#if (N_FIR_FILTERS == 7)
    fir_real[6].run(DataReal_5, DataRealOut_buff);
    fir_imag[6].run(DataImag_5, DataImagOut_buff);

#elif (N_FIR_FILTERS > 7)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_g0, DataImag_g0;
    DataWindow_t DataReal_g1, DataImag_g1;
#pragma HLS stream variable=DataReal_g0 depth=16
#pragma HLS stream variable=DataImag_g0 depth=16
#pragma HLS stream variable=DataReal_g1 depth=16
#pragma HLS stream variable=DataImag_g1 depth=16

    fir_real[12].run(DataReal_f0, DataReal_g0);
    fir_imag[12].run(DataImag_f0, DataImag_g0);
    fir_real[13].run(DataReal_f1, DataReal_g1);
    fir_imag[13].run(DataImag_f1, DataImag_g1);
#else
    DataWindow_t DataReal_6, DataImag_6;
#pragma HLS stream variable=DataReal_6 depth=16
#pragma HLS stream variable=DataImag_6 depth=16

    fir_real[6].run(DataReal_5, DataReal_6);
    fir_imag[6].run(DataImag_5, DataImag_6);
#endif

#endif

#if (N_FIR_FILTERS == 8)
    fir_real[7].run(DataReal_6, DataRealOut_buff);
    fir_imag[7].run(DataImag_6, DataImagOut_buff);

#elif (N_FIR_FILTERS > 8)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_h0, DataImag_h0;
    DataWindow_t DataReal_h1, DataImag_h1;
#pragma HLS stream variable=DataReal_h0 depth=16
#pragma HLS stream variable=DataImag_h0 depth=16
#pragma HLS stream variable=DataReal_h1 depth=16
#pragma HLS stream variable=DataImag_h1 depth=16

    fir_real[14].run(DataReal_g0, DataReal_h0);
    fir_imag[14].run(DataImag_g0, DataImag_h0);
    fir_real[15].run(DataReal_g1, DataReal_h1);
    fir_imag[15].run(DataImag_g1, DataImag_h1);
#else
    DataWindow_t DataReal_7, DataImag_7;
#pragma HLS stream variable=DataReal_7 depth=16
#pragma HLS stream variable=DataImag_7 depth=16

    fir_real[7].run(DataReal_6, DataReal_7);
    fir_imag[7].run(DataImag_6, DataImag_7);
#endif

#endif

#if (N_FIR_FILTERS == 9)
    fir_real[8].run(DataReal_7, DataRealOut_buff);
    fir_imag[8].run(DataImag_7, DataImagOut_buff);

#elif (N_FIR_FILTERS > 9)
#if (N_FIR_TAPS == 15)
    DataWindow_t DataReal_i0, DataImag_i0;
    DataWindow_t DataReal_i1, DataImag_i1;
#pragma HLS stream variable=DataReal_i0 depth=16
#pragma HLS stream variable=DataImag_i0 depth=16
#pragma HLS stream variable=DataReal_i1 depth=16
#pragma HLS stream variable=DataImag_i1 depth=16

    fir_real[16].run(DataReal_h0, DataReal_i0);
    fir_imag[16].run(DataImag_h0, DataImag_i0);
    fir_real[17].run(DataReal_h1, DataReal_i1);
    fir_imag[17].run(DataImag_h1, DataImag_i1);
#else
    DataWindow_t DataReal_8, DataImag_8;
#pragma HLS stream variable=DataReal_8 depth=16
#pragma HLS stream variable=DataImag_8 depth=16

    fir_real[8].run(DataReal_7, DataReal_8);
    fir_imag[8].run(DataImag_7, DataImag_8);
#endif 
#endif

#if (N_FIR_FILTERS == 10)
#if (N_FIR_TAPS == 15)
    fir_real[18].run(DataReal_i0, DataRealOut_buff1);
    fir_imag[18].run(DataImag_i0, DataImagOut_buff1);
    fir_real[19].run(DataReal_i1, DataRealOut_buff2);
    fir_imag[19].run(DataImag_i1, DataImagOut_buff2);
#else
    fir_real[9].run(DataReal_8, DataRealOut_buff);
    fir_imag[9].run(DataImag_8, DataImagOut_buff);
#endif

#endif

#if (N_FIR_TAPS == 15)
    buffOut_15tap(DataRealOut_buff1, DataImagOut_buff1,DataRealOut_buff2, DataImagOut_buff2, DataRealOut, DataImagOut);
#else
    buffOut(DataRealOut_buff, DataImagOut_buff, DataRealOut, DataImagOut);
#endif
    complex_merge(DataRealOut, DataImagOut, strmOut);
}

void fir_hls(
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmInp,
        hls::stream<ap_axiu<128, 0, 0, 0>> &strmOut,
        uint32_t iterCnt
        )
{
#pragma HLS interface axis port=strmInp
#pragma HLS interface axis port=strmOut

#if ((N_FIR_TAPS == 15 || N_FIR_TAPS == 129) && N_FIR_FILTERS == 10)
   #pragma HLS LATENCY min=100
#endif

#pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
    //#pragma HLS interface ap_ctrl_none port=return

    //#pragma HLS dataflow

FIR_ITER_LOOP:for(int i = iterCnt; i ; --i) {  
                  //#pragma HLS dataflow

                  fir_wrap(strmInp, strmOut);
              }
}
