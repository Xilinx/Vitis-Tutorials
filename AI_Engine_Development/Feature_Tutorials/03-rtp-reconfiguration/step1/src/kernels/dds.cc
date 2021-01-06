/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

