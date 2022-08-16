/**********
© Copyright 2020 Xilinx, Inc.
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
#include "stdlib.h"
#include <stdio.h>
#include <adf.h>

static int32 phase_in;

static int16 chess_storage(%chess_alignof(v8cint16)) dds_stored [16] = {32767, 0, 32767, 0, 32767, 0,  32767, 0, 32767, 0, 32767, 0, 32767, 0, 32767, 0};

void init_dds() {
  v8cint16 dds =  *(( v8cint16 *) dds_stored) ;
  phase_in = 0;
  
  cint16 scvalues = sincos(phase_in ); // phase_in + 0
  //sincos() is an intrinsic that returns sin and cos int16 values

  dds = shft_elem(dds,scvalues);

  *(( v8cint16 *) dds_stored) = dds;
}

void sine(const int32 phase_increment,output_window_cint16 * owin) {
  //Initialize vector from memory
  v8cint16 dds =  *(( v8cint16 *) dds_stored) ;

  for (unsigned k = 0; k<128; k++){
    phase_in += (phase_increment << 6);
    cint16 scvalues = sincos(phase_in << 14) ; // phase_in + (7i + j + 1) * phase_increment
    dds = shft_elem(dds,scvalues);

    window_writeincr(owin, *((cint16*)&scvalues));
    *(( v8cint16 *) dds_stored) = dds;
  };
};

