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
#include "../../include.h"


// Nobody would build a classifier this way.  It's just to show the
// sample by sample processing


void classifier
(
    input_stream_cint16 * input,
    output_window_int32 * outputw
) {
    const unsigned output_samples =  CLASSIFIER_OUTPUT_SAMPLES ;


    for (unsigned l=0;l<CLASSIFIER_OUTPUT_SAMPLES;l++) {
    #ifdef __chess__

      int32 from_stream = get_ss(0);
      cint16 sample = *(cint16 *)&from_stream;

    #else
      cint16 sample = readincr(input);
    #endif
      if (sample.real >= 0) {
	if (sample.imag > 0)
	  window_writeincr(outputw,0);
        else
          window_writeincr(outputw,1);
      }
      else {
	if (sample.imag > 0)
	  window_writeincr(outputw,2);
        else
          window_writeincr(outputw,3);
      }
    }
}

