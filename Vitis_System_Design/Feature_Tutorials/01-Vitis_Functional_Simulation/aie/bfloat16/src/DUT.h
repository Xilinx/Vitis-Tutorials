//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __XMC_DUT_H__
#define __XMC_DUT_H__

#include <adf.h>
#include "kernels.h"

class DUT_base : public adf::graph {
public:
   adf::kernel myFunc_1df_0;
   adf::kernel myFunc_1df1;

public:
   adf::input_port In1;
   adf::output_port Out1;

   DUT_base() {
      // create kernel myFunc_1df_0
      myFunc_1df_0 = adf::kernel::create(myFunc_1df);
      adf::source(myFunc_1df_0) = "kernels.cpp";

      // create kernel myFunc_1df1
      myFunc_1df1 = adf::kernel::create(myFunc_1df);
      adf::source(myFunc_1df1) = "kernels.cpp";

      // create kernel constraints myFunc_1df_0
      adf::runtime<ratio>(myFunc_1df_0) = 0.9;

      // create kernel constraints myFunc_1df1
      adf::runtime<ratio>(myFunc_1df1) = 0.9;

      // create nets to specify connections
      adf::connect net0 (In1, myFunc_1df_0.in[0]);
      adf::dimensions(myFunc_1df_0.in[0]) = {64};
      adf::connect net1 (myFunc_1df_0.out[0], myFunc_1df1.in[0]);
      adf::dimensions(myFunc_1df_0.out[0]) = {64};
      adf::dimensions(myFunc_1df1.in[0]) = {64};
      adf::connect net2 (myFunc_1df1.out[0], Out1);
      adf::dimensions(myFunc_1df1.out[0]) = {64};
   }
};

class DUT : public adf::graph {
public:
   DUT_base mygraph;

public:
   adf::input_plio In1;
   adf::output_plio Out1;

   DUT() {
      In1 = adf::input_plio::create("In1",
            adf::plio_32_bits,
            "./data/input/In1.txt");

      Out1 = adf::output_plio::create("Out1",
            adf::plio_32_bits,
            "Out1.txt");

      adf::connect< > (In1.out[0], mygraph.In1);
      adf::connect< > (mygraph.Out1, Out1.in[0]);
   }
};

#endif // __XMC_DUT_H__
