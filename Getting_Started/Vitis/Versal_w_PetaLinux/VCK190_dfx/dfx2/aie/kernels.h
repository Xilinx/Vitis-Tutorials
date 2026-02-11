#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <adf/window/types.h>
#include <adf/stream/types.h>

  void classifier(input_stream_cint16 * input, 
		output_window_int32 * output) ;

  void fir_27t_sym_hb_2i(input_window_cint16 * inputcb,
			 output_window_cint16 * outputcb) ;

			
#endif /* __KERNELS_H__ */

