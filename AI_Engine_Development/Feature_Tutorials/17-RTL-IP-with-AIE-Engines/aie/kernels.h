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

#ifndef __KERNELS_H__
#define __KERNELS_H__

//#include <adf/window/types.h>
//#include <adf/stream/types.h>
#include <adf.h>
#include <aie_api/aie.hpp>
using namespace adf;
 void classifier(input_stream_cint16 * input, 
                  output_buffer<int32> & __restrict outputw);

  void fir_27t_sym_hb_2i(       
	input_buffer<cint16,adf::extents<adf::inherited_extent>,adf::margin<INTERPOLATOR27_COEFFICIENTS>>  & __restrict cb_input,
	output_buffer<cint16> & __restrict cb_output);
			
#endif /**********__KERNELS_H__**********/

// 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
