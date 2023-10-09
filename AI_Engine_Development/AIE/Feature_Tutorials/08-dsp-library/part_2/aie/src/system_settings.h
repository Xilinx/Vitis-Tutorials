/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include "fir_sr_sym_graph.hpp"
#include "fir_interpolate_hb_graph.hpp"

// Filter parameters
#define DATA_TYPE    cint16
#define COEFF_TYPE    int16

#define FIR_LEN_CHAN    151
#define SHIFT_CHAN       15
#define ROUND_MODE_CHAN   0
#define AIES_CHAN         1

#define FIR_LEN_HB       43
#define SHIFT_HB         15
#define ROUND_MODE_HB     0

#define WINDOW_SIZE    1024

// Simulation parameters
#define NUM_ITER          8
