/**********
© Copyright 2021 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include "kernels.h"
#include "../include.h"

void passth(input_window_int32* wIn,
               output_window_int32* wOut) {
   printf("AIE:Inside the kernel\r\n");

   for (unsigned i=0; i<WIN_SAMPS; i++) {
      int32 data = window_readincr(wIn);
      printf("iteration = %d : %d\r\n",i, data);
      window_writeincr(wOut, data);
   }
}


void passth_cint16(input_window_cint16* wIn,
               output_window_cint16* wOut) {
   printf("AIE passth_cint16:Inside the kernel\r\n");

   for (unsigned i=0; i<WIN_SAMPS; i++) {
      cint16 data = window_readincr(wIn);
      int16* tmp_print_ptr = (int16*)(&data);
      printf("cint iteration = %d : %d %d\r\n",i, tmp_print_ptr[0],tmp_print_ptr[1]);
      //printf("cint iteration = %d : %d %d\r\n",i, data);
      window_writeincr(wOut, data);
   }
}

void passth_cfloat(input_window_cfloat* wIn,
               output_window_cfloat* wOut) {
   
   printf("AIE passth_cfloat:Inside the kernel\r\n");

   for (unsigned i=0; i<WIN_SAMPS; i++) {
      cfloat data = window_readincr(wIn);
      float* tmp_print_ptr = (float*)(&data);
      printf("cfloat iteration = %d : %f, %f\r\n",i, tmp_print_ptr[0],tmp_print_ptr[1]);
      window_writeincr(wOut, data);
   }
}


