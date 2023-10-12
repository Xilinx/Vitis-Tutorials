// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "adf.h"
#include "system_settings.h"
#include <aie_api/aie.hpp>

using namespace adf;

void k0(input_stream<int16> *sin, output_stream<int32> *sout);
void k0_cascin(input_stream<int16> * sin, output_stream<int32> * sout, input_cascade<acc48>* cin);
void k0_cascout(input_stream<int16> *sin, output_cascade<acc48>* cout);
void k0_cascin_cascout(input_stream<int16> *sin,  input_cascade<acc48>* cin, output_cascade<acc48>* cout);
