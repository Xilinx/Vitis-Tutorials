/**********
© Copyright 2022 Xilinx, Inc.

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



#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define POLAR_CLIP_INPUT_SAMPLES 1024
#define PLIOWIDTH 32
typedef qdma_axis<PLIOWIDTH, 0, 0, 0> pliopkt;

extern "C" {
inline void process_polar_clip(int &in_sample, int &out_sample);
void polar_clip(hls::stream<pliopkt>& sin,hls::stream<pliopkt>& sout);
}
