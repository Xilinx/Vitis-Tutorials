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

void firfloat(input_circular_buffer<float,adf::extents<NSAMPLES_WINDOW>,adf::margin<NSAMPLES_OVERLAP>> & datain, output_circular_buffer<float,adf::extents<NSAMPLES_WINDOW>> & dataout);
void fircfloat(input_circular_buffer<cfloat,adf::extents<NSAMPLES_WINDOW>,adf::margin<NSAMPLES_OVERLAP>> & datain, output_circular_buffer<cfloat,adf::extents<NSAMPLES_WINDOW>> & dataout);
