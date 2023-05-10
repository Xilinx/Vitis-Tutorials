/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "system_settings.h"

using namespace adf;
using namespace aie;


void matmult_float(
    input_buffer<float,adf::extents<NSAMPLES_WINDOW_F_A>> &  __restrict matA,
    input_buffer<float,adf::extents<NSAMPLES_WINDOW_F_B>> &  __restrict matB,
    output_buffer<float,adf::extents<NSAMPLES_WINDOW_F_C>> &  __restrict matC);

    void matmult_cfloat(
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_A>> &  __restrict matA,
        input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_B>> &  __restrict matB,
        output_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_C>> &  __restrict matC);

        void matmult_cfloat_conf(
            input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_A>> &  __restrict matA,
            input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_B>> &  __restrict matB,
            output_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_C>> &  __restrict matC);

            void matmult_cfloat_conj(
                input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_A>> &  __restrict matA,
                input_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_B>> &  __restrict matB,
                output_buffer<cfloat,adf::extents<NSAMPLES_WINDOW_CF_C>> &  __restrict matC);
