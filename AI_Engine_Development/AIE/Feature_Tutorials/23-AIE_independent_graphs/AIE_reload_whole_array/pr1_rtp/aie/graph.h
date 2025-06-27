/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "kernels.h"

class rtpgraph: public adf::graph
{
private:
  adf::kernel k;

public:
  adf::output_plio out;
  adf::input_plio in; 
  adf::port<adf::direction::in> value;

  rtpgraph()
  {
    k = adf::kernel::create(vect_add<256>);
    out = adf::output_plio::create("Dataout0", adf::plio_32_bits, "data/output0.txt");
    in = adf::input_plio::create("Datain0", adf::plio_32_bits, "data/input0.csv");
    //connect runtime parameter
    adf::connect<adf::parameter>(value, adf::async(k.in[1]));

    adf::connect< >(k.out[0], out.in[0]);
    adf::connect< >(in.out[0], k.in[0]);
    adf::source(k) = "vect_add.cc";
    adf::runtime<adf::ratio>(k)= 0.9;
  };
};

#endif /* __GRAPH_H__ */


