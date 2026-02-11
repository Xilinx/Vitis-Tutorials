#include <adf.h>
#include "include.h"


// Nobody would build a classifier this way.  It's just to show the
// sample by sample processing

void __attribute__((noinline)) classifier
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

