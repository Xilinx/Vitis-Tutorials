/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

Author: Daniele Bagni, Xilinx Inc
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fir.h"


int main (void)
{

  inp_data_t signal[SAMPLES];
  out_data_t output[SAMPLES], reference[SAMPLES];
  coef_t taps[N];

  FILE         *fp1, *fp2, *fp3;
  int i, ret_value;
  float val1, val2;
  float diff, tot_diff;
  int val3;

  tot_diff = 0;

  // LOAD FILTER COEFFICIENTS
  fp1=fopen("./data/fir_coeff.dat","r");
  fscanf(fp1, "%*[^\n]\n"); //skip first  line
  fscanf(fp1, "%*[^\n]\n"); //skip second line
  for (i=0;i<N;i++)
  {
	  fscanf(fp1,"%f\n", &val1);
	  taps[i] = (coef_t) val1;
#ifdef HLS_FIXED_POINT
	  fprintf(stdout,"taps[%4d]=%10.5f\n", i, taps[i].to_double());
#else
	  fprintf(stdout,"taps[%4d]=%10.5f\n", i, taps[i]);
#endif
  }
  fclose(fp1);

  // LOAD INPUT DATA  AND REFERENCE RESULTS
  fp2=fopen("./data/input.dat","r");
  fp3=fopen("./data/ref_res.dat","r");
  fscanf(fp2, "%*[^\n]\n"); //skip first  line
  fscanf(fp2, "%*[^\n]\n"); //skip second line
  fscanf(fp3, "%*[^\n]\n"); //skip first  line
  fscanf(fp3, "%*[^\n]\n"); //skip second line
  for (i=0;i<SAMPLES;i++)
  {
    fscanf(fp2,"%f\n", &val1);
    signal[i]    = (inp_data_t) val1;
    fscanf(fp3,"%f\n", &val2);
    reference[i] = (out_data_t) val2;
#ifdef HLS_FIXED_POINT
	if (i < 32) fprintf(stdout,"signal[%4d]=%10.5f \t reference[%4d]=%10.5f\n", i, signal[i].to_double(), i, reference[i].to_double() );
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
	  fprintf(fp1,"%10.5f\n", output[i]);
#endif
  }
  fclose(fp1);

  // CHECK RESULTS
  for (i=0;i<SAMPLES;i++)
  {
#ifdef HLS_FIXED_POINT
	   diff = output[i].to_double() - reference[i].to_double();
	   if (i<64) fprintf(stdout,"output[%4d]=%10.5f \t reference[%4d]=%10.5f\n", i, output[i].to_double(), i, reference[i].to_double() );
#else
    	   diff = output[i] - reference[i];
	   if (i<64) fprintf(stdout,"output[%4d]=%10.5f \t reference[%4d]=%10.5f\n", i, output[i], i, reference[i]);
#endif
	   diff = fabs(diff);
	   tot_diff += diff;
  }
  fprintf(stdout, "TOTAL ERROR =%f\n",tot_diff);

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
