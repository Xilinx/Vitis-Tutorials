/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#pragma once

#include "adf.h"
using namespace adf;

#include "core01.h"
#include "core02.h"
#include "core03.h"
#include "core05.h"
#include "core04.h"
#include "core01lut.h"
#include "core02lut.h"
#include "core03lut.h"
#include "core05lut.h"
#include "core04lut.h"

class myGraph : public graph { 
    private:
       kernel core01;
       kernel core02;
       kernel core03;
       kernel core05;
       kernel core04;
       parameter core01lut;
       parameter core02lut;
       parameter core03lut;
       parameter core05lut;
       parameter core04lut;

   public:
      
      input_plio in[4];
      output_plio out[3];
      

      myGraph() { 
         
         in[0] = input_plio::create("prod_in1", plio_64_bits, "0_1/matA_in_64plio.txt");
         in[1] = input_plio::create("prod_in3", plio_64_bits, "0_2/matA_in_64plio.txt");
         in[2] = input_plio::create("prod_in5", plio_64_bits, "0_3/matA_in_64plio.txt");
         in[3] = input_plio::create("prod_in7", plio_64_bits, "0_5/matA_in_64plio.txt");
         out[0] = output_plio::create("prod_out1", plio_64_bits, "output01.txt");
         out[1] = output_plio::create("prod_out2", plio_64_bits, "output02.txt");
         out[2] = output_plio::create("prod_out3", plio_64_bits, "output04.txt");
         
         core01 = kernel::create(core01_top);
         core02 = kernel::create(core02_top);
         core03 = kernel::create(core03_top);
         core05 = kernel::create(core05_top);
         core04 = kernel::create(core04_top);

         source(core01) = "core01.cc";
         source(core02) = "core02.cc";
	 source(core03) = "core03.cc";
	 source(core05) = "core05.cc";
	 source(core04) = "core04.cc";

         location<kernel>(core01) = tile(8,0);
	 location<kernel>(core02) = tile(8,1);
	 location<kernel>(core03) = tile(8,2);
	 location<kernel>(core05) = tile(8,4);
	 location<kernel>(core04) = tile(8,3);

         core01lut = parameter::array(B);      
         core02lut = parameter::array(B02);
         core03lut = parameter::array(B03);      
         core05lut = parameter::array(B05);
         core04lut = parameter::array(B04);


         connect< window<ROW_A * COL_A> > (in[0].out[0], core01.in[0]);
         connect<>(core01lut,core01);
         connect< window<ROW_A * COL_B> > (core01.out[0], out[0].in[0]);
         connect< window<ROW_A_2 * COL_A_2> > (in[1].out[0], core02.in[0]);
         connect<>(core02lut,core02);
         connect< window<ROW_A_2 * COL_B_2> > (core02.out[0], out[1].in[0]);
         connect< window<ROW_A_3 * COL_A_3> > (in[2].out[0], core03.in[0]);
         connect<>(core03lut,core03);
         connect< window<ROW_A_3 * COL_B_3> > (core03.out[0], core04.in[0]);
         connect< window<ROW_A_5 * COL_A_5> > (in[3].out[0], core05.in[0]);
         connect<>(core05lut,core05);
         connect< window<ROW_A_5 * COL_B_5> > (core05.out[0], core04.in[1]);
         connect<>(core04lut,core04);
         connect< window<ROW_A_4 * COL_B_4> > (core04.out[0], out[2].in[0]);

         //single_buffer(in0.out[0]);
         //single_buffer(in1.out[0]);
         //single_buffer(in2.out[0]);
         //single_buffer(in3.out[0]);

         //single_buffer(out0.in[0]);
         //single_buffer(out1.in[0]);
         //single_buffer(out2.in[0]);

         single_buffer(core01.in[0]);
         single_buffer(core01.out[0]);

         single_buffer(core02.in[0]);
         single_buffer(core02.out[0]);

         single_buffer(core03.in[0]);
         single_buffer(core03.out[0]);

         single_buffer(core04.in[0]);
         single_buffer(core04.out[0]);

         single_buffer(core05.in[0]);
         single_buffer(core05.out[0]);
         //------
         runtime<ratio>(core01) = 0.6;
         runtime<ratio>(core02) = 0.6;
         runtime<ratio>(core03) = 0.6;
         runtime<ratio>(core05) = 0.6;
         runtime<ratio>(core04) = 0.6;
  }
};


