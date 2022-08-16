/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
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

#pragma once

#include <adf.h>
#include "fir_sr_sym_graph.hpp"

// Filter parameters
#define DATA_TYPE    cint16
#define COEFF_TYPE    int16

#define FIR_LEN_CHAN    151
#define SHIFT_CHAN       15
#define ROUND_MODE_CHAN   0
#define AIES_CHAN         1

#define WINDOW_SIZE    1024

// Simulation parameters
#define NUM_ITER          8
