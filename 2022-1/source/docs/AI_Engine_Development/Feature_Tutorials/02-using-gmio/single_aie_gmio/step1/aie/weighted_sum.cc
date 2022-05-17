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
#include "adf.h"

int32_t weights[8]={1,2,3,4,5,6,7,8};

void weighted_sum_with_margin(input_window<int32> * in, output_window<int32> * out) 
{
  window_incr(in, 8);

  for (unsigned i = 0; i < 256; i++) 
  {
    int32 val;
    int32 wsum = 0;

    window_decr(in, 7);

    for (unsigned j = 0; j < 8; j++)
    {
      window_readincr(in, val);
      wsum = wsum + (weights[j] * val);
    }

    window_writeincr(out, wsum);
  }
}
