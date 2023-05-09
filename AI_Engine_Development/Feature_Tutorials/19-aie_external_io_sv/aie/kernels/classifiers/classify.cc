/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <adf.h>
#include "../../include.h"
#include <aie_api/aie.hpp>

// Nobody would build a classifier this way.  It's just to show the
// sample by sample processing
using namespace adf;

void classifier
(
    input_stream_cint16 * input,
    output_buffer<int32> & __restrict outputw
) {
    const unsigned output_samples =  CLASSIFIER_OUTPUT_SAMPLES ;
    auto OutIter = aie::begin(outputw);

    for (unsigned l=0;l<CLASSIFIER_OUTPUT_SAMPLES;l++) {
    #ifdef __chess__

      int32 from_stream = get_ss(0);
      cint16 sample = *(cint16 *)&from_stream;

    #else
      cint16 sample = readincr(input);
    #endif
      if (sample.real >= 0) {
	if (sample.imag > 0)
	   *OutIter++ = 0;
        else
	   *OutIter++ = 1;
      }
      else {
	if (sample.imag > 0)
	   *OutIter++ = 2;
        else
	   *OutIter++ = 3;
      }
    }
}

