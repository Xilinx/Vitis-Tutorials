/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#pragma once

#include "adf.h"
#include "fir_sr_sym_graph.hpp"

#define T_DATA           cint16
#define T_COEF            int16

// Filter Parameters
#define FIR_DOWNSHIFT        14
#define FIR_ROUND_MODE        0
//#define N_AIES_PER_FIR        1

// Simulation parameters
#define N_ITERATIONS          8

