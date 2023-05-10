/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <adf.h>
#include <aie_api/aie.hpp>
#include "../../include.h"

using namespace adf;

void classifier(input_stream_cint16 * input, 
                output_buffer<int32> & __restrict output
){
    
    const unsigned output_samples =  CLASSIFIER_OUTPUT_SAMPLES ;
    auto OutIter = aie::begin(output);
    for (unsigned l=0;l<output_samples;l++) {
      cint16 sample = readincr(input);

    if (sample.real >= 0) {
      if (sample.imag > 0)
        //window_writeincr(outputw,0);
        *OutIter++ = 0;
        else
          //window_writeincr(outputw,1);
          *OutIter++ = 1;
      }
      else {
        
      if (sample.imag > 0)
          //window_writeincr(outputw,2);
          *OutIter++ = 2;
        else
          //window_writeincr(outputw,3);
	  *OutIter++ = 3;
      }
    }
}

