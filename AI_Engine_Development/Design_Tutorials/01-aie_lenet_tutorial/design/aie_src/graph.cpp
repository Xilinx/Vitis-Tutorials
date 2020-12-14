/* (c) Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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
#include <adf.h>

class myGraph : public adf::graph { 
private:
  adf::kernel core01;
  adf::kernel core02;
  adf::kernel core03;
  adf::kernel core05;
  adf::kernel core04;
  adf::parameter core01lut;
  adf::parameter core02lut;
  adf::parameter core03lut;
  adf::parameter core05lut;
  adf::parameter core04lut;

public:
   adf::port<output> out[3];
   adf::port<input> in[4];
  myGraph() { 
    core01 = adf::kernel::create(core01_top);
    adf::source(core01) = "core01.cc";
    adf::location<adf::kernel>(core01) = adf::tile(8,0);
    core01lut = adf::parameter::array(B);
    core02 = adf::kernel::create(core02_top);
    adf::source(core02) = "core02.cc";
    adf::location<adf::kernel>(core02) = adf::tile(8,1);
    core02lut = adf::parameter::array(B02);
    core03 = adf::kernel::create(core03_top);
    adf::source(core03) = "core03.cc";
    adf::location<adf::kernel>(core03) = adf::tile(8,2);
    core03lut = adf::parameter::array(B03);
    core05 = adf::kernel::create(core05_top);
    adf::source(core05) = "core05.cc";
    adf::location<adf::kernel>(core05) = adf::tile(8,4);
    core05lut = adf::parameter::array(B05);
    core04 = adf::kernel::create(core04_top);
    adf::source(core04) = "core04.cc";
    adf::location<adf::kernel>(core04) = adf::tile(8,3);
    core04lut = adf::parameter::array(B04);

    /*adf::connect< adf::window<ROW_A * COL_A, 0, 1> > (in[0], core01.in[0]);
    adf::connect<>(core01lut,core01);
    adf::connect< adf::window<ROW_A * COL_B ,0, 1> > (core01.out[0], out[0]);
    adf::connect< adf::window<ROW_A_2 * COL_A_2, 0, 1> > (in[1], core02.in[0]);
    adf::connect<>(core02lut,core02);
    adf::connect< adf::window<ROW_A_2 * COL_B_2 ,0, 1> > (core02.out[0], out[1]);
    adf::connect< adf::window<ROW_A_3 * COL_A_3, 0, 1> > (in[2], core03.in[0]);
    adf::connect<>(core03lut,core03);
    adf::connect< adf::window<ROW_A_3 * COL_B_3 ,0, 1> > (core03.out[0], core04.in[0]);
    adf::connect< adf::window<ROW_A_5 * COL_A_5, 0, 1> > (in[3], core05.in[0]);
    adf::connect<>(core05lut,core05);
    adf::connect< adf::window<ROW_A_5 * COL_B_5 ,0, 1> > (core05.out[0], core04.in[1]);
    adf::connect<>(core04lut,core04);
    adf::connect< adf::window<ROW_A_4 * COL_B_4 ,0, 1> > (core04.out[0], out[2]);*/

    adf::connect< adf::window<ROW_A * COL_A> > (in[0], core01.in[0]);
    adf::connect<>(core01lut,core01);
    adf::connect< adf::window<ROW_A * COL_B> > (core01.out[0], out[0]);
    adf::connect< adf::window<ROW_A_2 * COL_A_2> > (in[1], core02.in[0]);
    adf::connect<>(core02lut,core02);
    adf::connect< adf::window<ROW_A_2 * COL_B_2> > (core02.out[0], out[1]);
    adf::connect< adf::window<ROW_A_3 * COL_A_3> > (in[2], core03.in[0]);
    adf::connect<>(core03lut,core03);
    adf::connect< adf::window<ROW_A_3 * COL_B_3> > (core03.out[0], core04.in[0]);
    adf::connect< adf::window<ROW_A_5 * COL_A_5> > (in[3], core05.in[0]);
    adf::connect<>(core05lut,core05);
    adf::connect< adf::window<ROW_A_5 * COL_B_5> > (core05.out[0], core04.in[1]);
    adf::connect<>(core04lut,core04);
    adf::connect< adf::window<ROW_A_4 * COL_B_4> > (core04.out[0], out[2]);

    single_buffer(in[0]);
    single_buffer(in[1]);
    single_buffer(in[2]);
    single_buffer(in[3]);

    single_buffer(out[0]);
    single_buffer(out[1]);
    single_buffer(out[2]);

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
    adf::runtime<ratio>(core01) = 0.6;
    adf::runtime<ratio>(core02) = 0.6;
    adf::runtime<ratio>(core03) = 0.6;
    adf::runtime<ratio>(core05) = 0.6;
    adf::runtime<ratio>(core04) = 0.6;
  }
};

adf::PLIO *attr_i1 = new adf::PLIO("prod_in1", adf::plio_128_bits, "data/0_1/matA_in_128plio.txt");
adf::PLIO *attr_i2 = new adf::PLIO("prod_in3", adf::plio_128_bits, "data/0_2/matA_in_128plio.txt");
adf::PLIO *attr_i3 = new adf::PLIO("prod_in5", adf::plio_128_bits, "data/0_3/matA_in_128plio.txt");
adf::PLIO *attr_i4 = new adf::PLIO("prod_in7", adf::plio_128_bits, "data/0_5/matA_in_128plio.txt");

adf::PLIO *attr_o1 = new adf::PLIO("prod_out1", adf::plio_128_bits, "output01.txt");
adf::PLIO *attr_o2 = new adf::PLIO("prod_out2", adf::plio_128_bits, "output02.txt");
adf::PLIO *attr_o3 = new adf::PLIO("prod_out3", adf::plio_128_bits, "output04.txt");

adf::simulation::platform<4,3> platform(attr_i1,attr_i2,attr_i3,attr_i4,attr_o1,attr_o2,attr_o3);
  myGraph g;
  adf::connect<> net010(platform.src[0], g.in[0]);
  adf::connect<> net01_0(g.out[0], platform.sink[0]);
  adf::connect<> net020(platform.src[1], g.in[1]);
  adf::connect<> net02_0(g.out[1], platform.sink[1]);
  adf::connect<> net030(platform.src[2], g.in[2]);
  adf::connect<> net050(platform.src[3], g.in[3]);
  adf::connect<> net04_0(g.out[2], platform.sink[2]);

#ifdef __AIESIM__
   int main(int argc, char ** argv)
   { 
      g.init();
      g.run();
      g.end();
     
      return 0;
   }
#endif

