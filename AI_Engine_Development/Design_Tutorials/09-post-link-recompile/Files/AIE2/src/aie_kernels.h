/*
 * (c) Copyright 2021 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <adf.h>


  void weighted_sum(input_window_int32 * in, output_window_int32 * out);
  void weighted_sum_with_margin(input_window_int32 * in, output_window_int32 * out);
  void vectorized_weighted_sum_with_margin(input_window_int32 * in, output_window_int32 * out);
  void vectorized_weighted_cpx_sum_with_margin(input_window_cint16 * restrict in, output_window_cint32 * restrict out);
  void weighted_cpx_sum_with_margin(input_window_cint16 * restrict in, output_window_cint32 * restrict out);

  void average_div(input_window_int32 * in, output_window_int32 * out);
  void vectorized_average_div(input_window_int32 * in, output_window_int32 * out);
  void vectorized_cpx_average_div(input_window_cint32 * restrict in, output_window_cint16 * restrict out);

  void classifier(input_stream_cint16 * input,output_window_int32 * outputw);




#endif /* __KERNELS_H__ */
