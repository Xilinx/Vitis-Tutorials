/**********
© Copyright 2020-2022 Xilinx, Inc.
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
void aie_core1(input_window<int32> *in,output_window<int32> *out);
void aie_core2(input_window<int32> *in,output_window<int32> *out);
void aie_core3(input_window<float> *in,output_window<float> *out);
void aie_core4(input_window<cint16> *in,output_window<cint16> *out);
#endif
