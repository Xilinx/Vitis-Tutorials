/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __KERNELS_H__
#define __KERNELS_H__

//AIE API versions of kernels
void bf8x8_fst_api(input_window<cint16> * restrict c_input,
                   input_window<cint16> * restrict x_input,
                   output_stream_cacc48 * restrict cascade_out);

void bf8x8_mid_api(input_window<cint16> * restrict c_input,
                   input_window<cint16> * restrict x_input,
                   input_stream_cacc48 * cascade_in,
                   output_stream_cacc48 * cascade_out);

void bf8x8_lst_api(input_window<cint16> * restrict c_input,
                   input_window<cint16> * restrict x_input,
                   input_stream_cacc48 * cascade_in,
                   output_window<cint16> * restrict c_output);

#endif // __KERNELS_H__

