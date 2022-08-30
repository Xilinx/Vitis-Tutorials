/*
 * Copyright 2019 Xilinx, Inc.
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
 */

#ifndef _RESIZE_RGB_H
#define _RESIZE_RGB_H

#include <ap_int.h>

#define AXI_WIDTH 128
extern "C"
{
    void resize_accel_rgb(ap_uint<AXI_WIDTH> *image_in_y,
                          ap_uint<AXI_WIDTH> *image_in_uv,
                          ap_uint<AXI_WIDTH> *image_out,
                          int width_in,
                          int height_in,
                          int stride_in_x,
                          int width_out,
                          int height_out,
                          int use_mean,
                          int mean);
};
#endif
