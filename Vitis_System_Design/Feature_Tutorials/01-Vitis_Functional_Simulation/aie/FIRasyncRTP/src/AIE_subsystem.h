//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __XMC_AIE_SUBSYSTEM_H__
#define __XMC_AIE_SUBSYSTEM_H__

#include <adf.h>
#include "FIR_Symmetric_d2ee5c2f.h"

class AIE_subsystem_base : public adf::graph {
public:
   FIR_Symmetric_d2ee5c2f FIR_Symmetric;

public:
   adf::input_port In1, In2;
   adf::output_port Out1;

   AIE_subsystem_base() {
      // create nets to specify connections
      adf::connect net0 (In1, FIR_Symmetric.in[0]);
      adf::connect net1 (In2, FIR_Symmetric.coeff[0]);
      adf::connect net2 (FIR_Symmetric.out[0], Out1);
   }
};

class AIE_subsystem : public adf::graph {
public:
   AIE_subsystem_base mygraph;

public:
   adf::input_plio In1;
   adf::input_port In2;
   adf::output_plio Out1;

   AIE_subsystem() {
      In1 = adf::input_plio::create("In1",
            adf::plio_32_bits,
            "./data/input/In1.txt");

      Out1 = adf::output_plio::create("Out1",
            adf::plio_32_bits,
            "Out1.txt");

      adf::connect< > (In1.out[0], mygraph.In1);
      adf::connect< > (In2, mygraph.In2);
      adf::connect< > (mygraph.Out1, Out1.in[0]);
   }
};

#endif // __XMC_AIE_SUBSYSTEM_H__
