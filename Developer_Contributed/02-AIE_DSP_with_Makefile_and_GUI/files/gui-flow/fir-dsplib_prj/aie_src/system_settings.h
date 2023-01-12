//Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
//SPDX-License-Identifier: MIT

//DB: daniele.bagni@amd.com
//DB: 09 Aug 2022


#pragma once

#include <adf.h>
//#include "fir_sr_sym_graph.hpp"
#include "fir_decimate_asym_graph.hpp"

// Filter parameters
#define DATA_TYPE     cint16
#define COEFF_TYPE    int16

#define FIR_LEN_CHAN    8
#define DECIMATE_FACTOR  2
#define SHIFT_CHAN       15//0 for ramp
#define ROUND_MODE_CHAN   0
#define AIES_CHAN         1


#define INPUT_SIZE  (8192  ) //rows of complex data: Re + jIm
#define OUTPUT_SIZE (8192/2) //rows of complex data: Re + jIm


#define WINDOW_SIZE    1024 //in samples

// Simulation parameters
#define NUM_ITER          (INPUT_SIZE/WINDOW_SIZE)
//DB: a bit redundant but just in case to be sure
#define ITER_CNT          (NUM_ITER)
#define N_ITERATIONS      (NUM_ITER)

#define TP_CASC_LEN 1
#define TP_USE_COEFF_RELOAD 0
#define TP_NUM_OUTPUTS 1
#define TP_DUAL_IP 0
#define TP_API 0

