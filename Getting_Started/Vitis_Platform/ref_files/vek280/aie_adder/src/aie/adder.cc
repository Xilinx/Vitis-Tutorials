/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

#include <adf.h>

void aie_adder(input_stream_int32* in0, input_stream_int32* in1, output_stream_int32* out) {
    v4int32 a = readincr_v4(in0);
    v4int32 b = readincr_v4(in1);
    v4int32 c = operator+(a, b);
    writeincr_v4(out, c);
}
