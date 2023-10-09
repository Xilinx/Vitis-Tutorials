/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
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
   
   DataWindow_t DataRealInp_buff, DataImagInp_buff;
   DataWindow_t DataRealOut_buff, DataImagOut_buff;
   #pragma HLS stream variable=DataRealInp_buff depth=16
   #pragma HLS stream variable=DataImagInp_buff depth=16
   #pragma HLS stream variable=DataRealOut_buff depth=16
   #pragma HLS stream variable=DataImagOut_buff depth=16
   
   complex_split(strmInp, DataRealInp, DataImagInp);
   buffInp(DataRealInp, DataImagInp, DataRealInp_buff, DataImagInp_buff);
   
   #if (N_FIR_FILTERS == 1)
      fir_real[0].run(DataRealInp_buff, DataRealOut_buff);
      fir_imag[0].run(DataImagInp_buff, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 1)
      DataWindow_t DataReal_0, DataImag_0;
      #pragma HLS stream variable=DataReal_0 depth=16
      #pragma HLS stream variable=DataImag_0 depth=16
      
      fir_real[0].run(DataRealInp_buff, DataReal_0);
      fir_imag[0].run(DataImagInp_buff, DataImag_0);
   
   #endif
   
   #if (N_FIR_FILTERS == 2)
      fir_real[1].run(DataReal_0, DataRealOut_buff);
      fir_imag[1].run(DataImag_0, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 2)
      DataWindow_t DataReal_1, DataImag_1;
      #pragma HLS stream variable=DataReal_1 depth=16
      #pragma HLS stream variable=DataImag_1 depth=16
      
      fir_real[1].run(DataReal_0, DataReal_1);
      fir_imag[1].run(DataImag_0, DataImag_1);
   
   #endif
   
   #if (N_FIR_FILTERS == 3)
      fir_real[2].run(DataReal_1, DataRealOut_buff);
      fir_imag[2].run(DataImag_1, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 3)
      DataWindow_t DataReal_2, DataImag_2;
      #pragma HLS stream variable=DataReal_2 depth=16
      #pragma HLS stream variable=DataImag_2 depth=16
      
      fir_real[2].run(DataReal_1, DataReal_2);
      fir_imag[2].run(DataImag_1, DataImag_2);
   
   #endif
   
   #if (N_FIR_FILTERS == 4)
      fir_real[3].run(DataReal_2, DataRealOut_buff);
      fir_imag[3].run(DataImag_2, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 4)
      DataWindow_t DataReal_3, DataImag_3;
      #pragma HLS stream variable=DataReal_3 depth=16
      #pragma HLS stream variable=DataImag_3 depth=16
      
      fir_real[3].run(DataReal_2, DataReal_3);
      fir_imag[3].run(DataImag_2, DataImag_3);
   
   #endif
   
   #if (N_FIR_FILTERS == 5)
      fir_real[4].run(DataReal_3, DataRealOut_buff);
      fir_imag[4].run(DataImag_3, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 5)
      DataWindow_t DataReal_4, DataImag_4;
      #pragma HLS stream variable=DataReal_4 depth=16
      #pragma HLS stream variable=DataImag_4 depth=16
      
      fir_real[4].run(DataReal_3, DataReal_4);
      fir_imag[4].run(DataImag_3, DataImag_4);
   
   #endif
   
   #if (N_FIR_FILTERS == 6)
      fir_real[5].run(DataReal_4, DataRealOut_buff);
      fir_imag[5].run(DataImag_4, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 6)
      DataWindow_t DataReal_5, DataImag_5;
      #pragma HLS stream variable=DataReal_5 depth=16
      #pragma HLS stream variable=DataImag_5 depth=16
      
      fir_real[5].run(DataReal_4, DataReal_5);
      fir_imag[5].run(DataImag_4, DataImag_5);
   
   #endif
   
   #if (N_FIR_FILTERS == 7)
      fir_real[6].run(DataReal_5, DataRealOut_buff);
      fir_imag[6].run(DataImag_5, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 7)
      DataWindow_t DataReal_6, DataImag_6;
      #pragma HLS stream variable=DataReal_6 depth=16
      #pragma HLS stream variable=DataImag_6 depth=16
      
      fir_real[6].run(DataReal_5, DataReal_6);
      fir_imag[6].run(DataImag_5, DataImag_6);
   
   #endif
   
   #if (N_FIR_FILTERS == 8)
      fir_real[7].run(DataReal_6, DataRealOut_buff);
      fir_imag[7].run(DataImag_6, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 8)
      DataWindow_t DataReal_7, DataImag_7;
      #pragma HLS stream variable=DataReal_7 depth=16
      #pragma HLS stream variable=DataImag_7 depth=16
      
      fir_real[7].run(DataReal_6, DataReal_7);
      fir_imag[7].run(DataImag_6, DataImag_7);
   
   #endif
   
   #if (N_FIR_FILTERS == 9)
      fir_real[8].run(DataReal_7, DataRealOut_buff);
      fir_imag[8].run(DataImag_7, DataImagOut_buff);
   
   #elif (N_FIR_FILTERS > 9)
      DataWindow_t DataReal_8, DataImag_8;
      #pragma HLS stream variable=DataReal_8 depth=16
      #pragma HLS stream variable=DataImag_8 depth=16
      
      fir_real[8].run(DataReal_7, DataReal_8);
      fir_imag[8].run(DataImag_7, DataImag_8);
   
   #endif
   
   #if (N_FIR_FILTERS == 10)
      fir_real[9].run(DataReal_8, DataRealOut_buff);
      fir_imag[9].run(DataImag_8, DataImagOut_buff);
   
   #endif
   
   buffOut(DataRealOut_buff, DataImagOut_buff, DataRealOut, DataImagOut);
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
   
   #pragma HLS INTERFACE s_axilite port=iterCnt bundle=control
   #pragma HLS INTERFACE s_axilite port=return bundle=control
   //#pragma HLS interface ap_ctrl_none port=return
   
   //#pragma HLS dataflow
   
   FIR_ITER_LOOP:for(int i = iterCnt; i ; --i) {  
      //#pragma HLS dataflow
   
      fir_wrap(strmInp, strmOut);
   }
}
