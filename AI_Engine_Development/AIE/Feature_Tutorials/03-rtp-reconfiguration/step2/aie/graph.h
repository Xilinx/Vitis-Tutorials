/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "dds.h"

class ddsgraph: public adf::graph
{
private:
  adf::kernel dds;

public:
  adf::output_plio out;
  adf::port<adf::direction::in> trigger;

  ddsgraph()
  {
    dds = adf::kernel::create(sine);
    out = adf::output_plio::create("Dataout", adf::plio_32_bits, "data/output.txt");
    //connect asynchronous runtime parameter
    adf::connect<adf::parameter>(trigger, adf::async(dds.in[0]));

    adf::connect< >(dds.out[0], out.in[0]);
    adf::source(dds) = "kernels/dds.cc";
    adf::runtime<adf::ratio>(dds)= 0.9;
  };
};

#endif /* __GRAPH_H__ */


