//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __XMC_AI_ENGINE_FIR_H__
#define __XMC_AI_ENGINE_FIR_H__

#include <adf.h>
#include "FIR_Asymmetric_62a73e96.h"

class AI_Engine_FIR_base : public adf::graph {
public:
   FIR_Asymmetric_62a73e96 FIR_Asymmetric_Filter;

public:
   adf::input_port In1;
   adf::output_port AIE_FIR;

   AI_Engine_FIR_base() {
      // create nets to specify connections
      adf::connect net0 (In1, FIR_Asymmetric_Filter.in[0]);
      adf::connect net1 (FIR_Asymmetric_Filter.out[0], AIE_FIR);
   }
};

class AI_Engine_FIR : public adf::graph {
public:
   AI_Engine_FIR_base mygraph;

public:
   adf::input_plio In1;
   adf::output_plio AIE_FIR;

   AI_Engine_FIR() {
      In1 = adf::input_plio::create("In1",
            adf::plio_64_bits,
            "./data/input/In1.txt",
            500);

      AIE_FIR = adf::output_plio::create("AIE_FIR",
            adf::plio_64_bits,
            "AIE_FIR.txt",
            500);

      adf::connect< > (In1.out[0], mygraph.In1);
      adf::connect< > (mygraph.AIE_FIR, AIE_FIR.in[0]);
   }
};

#endif // __XMC_AI_ENGINE_FIR_H__
