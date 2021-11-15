/**********
© Copyright 2021 Xilinx, Inc.
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
#ifndef FIR24_SYM_H
#define FIR24_SYM_H

#include <adf.h>

  void fir24_sym(input_window_cint16 * iwin, output_window_cint16 * owin,  const int32(&coeffs)[12], int32(&coeffs_readback)[12]);

#endif


