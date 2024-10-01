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
        alignas(32) float (&wbufA)[BUFFSZ];
        alignas(32) float (&wbufB)[BUFFSZ];
        alignas(32) float (&wbufC)[BUFFSZ];
        alignas(32) float (&wbufD)[BUFFSZ];

        // parameters for computing exponential function estimate
        const float log2e =  1.44269502162933349609375f;
        const float p4    = -1.3670309446752071380615234375e-2f;
        const float p3    = -5.1744997501373291015625e-2f;
        const float p2    = -2.416043579578399658203125e-1f;
        const float p1    =  3.070270717144012451171875e-1f;
        const float p0    = -3.492907808322343043982982635498046875e-6f;
        const float exp_S =  8388608.0f;
        const float exp_B =  1065353216.0f;

    public:
        softmax_kernel(float (&wbufA_i)[BUFFSZ], float (&wbufB_i)[BUFFSZ], float (&wbufC_i)[BUFFSZ], float (&wbufD_i)[BUFFSZ]) : 
                               wbufA(wbufA_i), wbufB(wbufB_i), wbufC(wbufC_i), wbufD(wbufD_i) {};
        void softmax(input_stream<int32>* in, output_stream<int32>* out);
	    static void registerKernelClass()
  	    {
		    REGISTER_FUNCTION(softmax_kernel::softmax);
            REGISTER_PARAMETER(wbufA);
            REGISTER_PARAMETER(wbufB);
            REGISTER_PARAMETER(wbufC);
            REGISTER_PARAMETER(wbufD);
	    }
};
