/**********
© Copyright 2020 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "kernel.h"

class mygraph: public adf::graph
{
private:
  adf::kernel k_m;

public:
  adf::output_gmio gmioOut;
  adf::input_gmio gmioIn;

  mygraph()
  {
	k_m = adf::kernel::create(weighted_sum_with_margin);
	gmioOut = adf::output_gmio::create("gmioOut",64,1000);
	gmioIn = adf::input_gmio::create("gmioIn",64,1000);

	adf::connect<adf::window<1024,32>>(gmioIn.out[0], k_m.in[0]);
	adf::connect<adf::window<1024>>(k_m.out[0], gmioOut.in[0]);
	adf::source(k_m) = "weighted_sum.cc";
	adf::runtime<adf::ratio>(k_m)= 0.9;
  };
};

#endif
