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

