/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#include <adf.h>
#include "include.h"
#include "../kernels/kernels.h"


using namespace adf;

template <const int IN_SIZE>
class mygraph : public graph {

  private:
    kernel  passth_i;

  public:
    
    adf::input_plio in;
    adf::output_plio out;
    
    mygraph()
    {
      // specify kernels
      passth_i                   = adf::kernel::create(passth_cint16);
      
      adf::source(passth_i)           = "../kernels/pass_thru.cc";
      runtime<ratio>(passth_i)   = 0.9;
      
#ifdef EXTERNAL_IO 
	in = adf::input_plio::create("DataIn",   adf::plio_64_bits);
	out =adf::output_plio::create("DataOut", adf::plio_64_bits);
#else
	in = adf::input_plio::create("DataIn",   adf::plio_64_bits, "data/input.txt");
	out =adf::output_plio::create("DataOut", adf::plio_64_bits, "data/output.txt");
#endif
	dimensions(passth_i.in[0]) = {IN_SIZE*4};
        dimensions(passth_i.out[0]) = {IN_SIZE*4};
        connect(in.out[0], passth_i.in[0]);
        connect(passth_i.out[0], out.in[0]);

    }
};
