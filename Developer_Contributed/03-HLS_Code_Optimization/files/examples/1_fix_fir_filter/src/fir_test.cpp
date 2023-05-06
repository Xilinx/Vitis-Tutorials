/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

Author: Daniele Bagni, Xilinx Inc
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fir.h"

#ifndef HLS_FIXED_POINT
const float      factor = 16384.0; // 2^14
const int   log2_factor = 14;

unsigned int ref_fir_filter (signed int x,   signed int c[N])
{
  static signed int ref_shift_reg[N];
  signed long long int acc = 0;
  signed int mult;
  signed int y;
  for (int i=N-1;i>=0;i--)
  {
     if (i==0)
     {
       ref_shift_reg[0]=x;
     }
     else
     {
       ref_shift_reg[i] = ref_shift_reg[i-1];
     }
     mult = ref_shift_reg[i]*c[i];
     acc  = acc + ((unsigned long long int) mult);
  }
  y = acc;
  return y;
}
#endif //#ifdef HLS_FIXED_POINT

int main (void)
{

  inp_data_t signal[SAMPLES];
  out_data_t output[SAMPLES], golden_ref[SAMPLES];
  coef_t taps[N];

  FILE         *fp1, *fp2, *fp3;
  int i, ret_value;
  float val1, val2;
  int val3;

#ifndef HLS_FIXED_POINT
  float diff, tot_diff, max_diff;
  tot_diff = 1e-9f;
  max_diff = 1e-9f;
#else
  signed long int diff;
  signed long long tot_diff=0;
  signed long max_diff=0;
#endif

  // LOAD FILTER COEFFICIENTS
  fp1=fopen("./data/fir_coeff.dat","r");
  fscanf(fp1, "%*[^\n]\n"); //skip first  line
  fscanf(fp1, "%*[^\n]\n"); //skip second line
  for (i=0;i<N;i++)
  {
	  fscanf(fp1,"%f\n", &val1);
#ifdef HLS_FIXED_POINT
	  taps[i] = (coef_t) val1;
	  fprintf(stdout,"taps[%4d]=%10.5f\n", i, taps[i].to_double());
#else
	  val1 = round(factor*val1);
	  taps[i] = (coef_t) val1;
	  fprintf(stdout,"taps[%4d]=%10d\n", i, taps[i]);
#endif
  }
  fclose(fp1);

  // LOAD INPUT DATA  AND GOLDEN_REF RESULTS
  fp2=fopen("./data/input.dat","r");
  fp3=fopen("./data/ref_res.dat","r");
  fscanf(fp2, "%*[^\n]\n"); //skip first  line
  fscanf(fp2, "%*[^\n]\n"); //skip second line
  fscanf(fp3, "%*[^\n]\n"); //skip first  line
  fscanf(fp3, "%*[^\n]\n"); //skip second line
  for (i=0;i<SAMPLES;i++)
  {
    fscanf(fp2,"%f\n", &val1);
    fscanf(fp3,"%f\n", &val2);
#ifdef HLS_FIXED_POINT
    signal[i]     = (inp_data_t) val1;
    golden_ref[i] = (out_data_t) val2;
    //if (i < 32) fprintf(stdout,"signal[%4d]=%10.5f \t golden_ref[%4d]=%10.5f\n", i, signal[i].to_double(), i, golden_ref[i].to_double() );
#else
    val1 = round(factor*val1);
    val2 = round(factor*val2);
    signal[i]     = (inp_data_t) (val1);
    golden_ref[i] = (out_data_t) val2;
    //golden_ref[i] = ref_fir_filter(val2, taps);
    //if (i < 32) fprintf(stdout,"signal[%4d]=%10d \t golden_ref[%4d]=%10d\n", i, signal[i], i, golden_ref[i] );
#endif
  }
  fclose(fp2);
  fclose(fp3);

// CALL DESIGN UNDER TEST
  for (i=0;i<SAMPLES;i++)
  {
	  output[i] = fir_filter(signal[i], taps);
  }

  // WRITE OUTPUT RESULTS
  fp1=fopen("./data/out_res.dat","w");
  for (i=0;i<SAMPLES;i++)
  {
#ifdef HLS_FIXED_POINT
	   fprintf(fp1,"%10.5f\n", output[i].to_double());
#else
	   unsigned int tmp = output[i] >> log2_factor;
	   output[i] = tmp;
	   fprintf(fp1,"%10.5f\n", (float) output[i]);
#endif
  }
  fclose(fp1);

  // CHECK RESULTS
  for (i=0;i<SAMPLES;i++)
  {
#ifdef HLS_FIXED_POINT
	   diff = output[i].to_double() - golden_ref[i].to_double();
	   if (i<64) fprintf(stdout,"output[%4d]=%10.5f \t golden_ref[%4d]=%10.5f\n", i, output[i].to_double(), i, golden_ref[i].to_double() );
	   diff = fabs(diff);
	   if (diff > max_diff) max_diff=diff;
	   tot_diff += diff;
#else
	   diff = output[i] - golden_ref[i];
	   if (i<64) fprintf(stdout,"output[%4d]=%10d \t golden_ref[%4d]=%10d\n", i, output[i], i, golden_ref[i] );
	   if (diff<0) diff = 0 - diff;
	   if (diff > max_diff) max_diff=diff;
	   tot_diff += diff;
	   //fprintf(stdout,"diff=%10d max_diff=%10d\n", (int) diff, (int) max_diff);
#endif
  }
  fprintf(stdout, "TOTAL ERROR =%f MAX DIFF=%f\n", (float) tot_diff, (float) max_diff);

  if (tot_diff < THRESHOLD)
  {
	  fprintf(stdout, "\nTEST PASSED!\n");
	  ret_value =0;
  }
  else
  {
	  fprintf(stdout, "\nTEST FAILED!\n");
	  ret_value =1;
  }


  return ret_value;


}
