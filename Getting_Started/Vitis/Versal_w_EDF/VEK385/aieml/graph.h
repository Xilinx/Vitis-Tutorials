/*
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/// 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689


#include "mysquare.h"
#include <adf.h>

using namespace adf;

struct myGraph : public adf::graph {

public :
adf::kernel sq;


  adf::input_gmio gm0;
  adf::output_plio out0;

  myGraph() { 
  
  gm0 = adf::input_gmio::create("gmio1", 64, 1);
  out0 = adf::output_plio::create("DataOut",adf::plio_32_bits ,"data/output.txt");

    sq = adf::kernel::create( mysquare ) ; 
    adf::source(sq) = "../aieml/kernels/mysquare.cc";

    runtime<ratio>(sq) = 0.6;

    adf::connect<window<128>>(gm0.out[0], sq.in[0]);
    adf::connect<window<128>>(sq.out[0],out0.in[0]);

   // adf::location<kernel>(sq) = adf::tile(1,1);
  }
};

