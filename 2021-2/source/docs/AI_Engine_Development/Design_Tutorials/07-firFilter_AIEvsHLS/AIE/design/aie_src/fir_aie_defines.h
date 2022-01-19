//------------------------------------------------------------------------------
// © Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

#pragma once

#include <adf.h>
#include "fir_sr_sym_graph.hpp"

#define T_DATA           cint16
#define T_COEF            int16

// Filter Parameters
#define FIR_DOWNSHIFT        14
#define FIR_ROUND_MODE        0
#define N_AIES_PER_FIR        1

// Simulation parameters
#define N_ITERATIONS          8

