//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include "config.h"

using namespace adf;

class softmax_kernel
{
    private:
        alignas(32) int16 (&wbufA)[BUFFSZ];
        alignas(32) int16 (&wbufB)[BUFFSZ];

        // parameters for computing exponential function estimate
        const bfloat16 exp_S =  185.0f;
        const float exp_B    =  16256.0f;

    public:
        softmax_kernel(int16 (&wbufA_i)[BUFFSZ], int16 (&wbufB_i)[BUFFSZ]) : wbufA(wbufA_i), wbufB(wbufB_i) {};
        void softmax(input_stream<int16>* in, output_stream<int16>* out);
	    static void registerKernelClass()
  	    {
		    REGISTER_FUNCTION(softmax_kernel::softmax);
            REGISTER_PARAMETER(wbufA);
            REGISTER_PARAMETER(wbufB);
	    }
};
