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

static const int col[32]={6,13,14,45,18,42,4,30,48,49,9,16,29,39,40,31,2,3,46,0,43,27,41,26,11,17,47,1,19,10,34,7};

class mygraph: public adf::graph
{
private:
  adf::kernel k[32];

public:
  adf::input_gmio gmioIn[32];
  adf::output_gmio gmioOut[32];

  mygraph()
  {
	for(int i=0;i<32;i++){
		gmioIn[i]=adf::input_gmio::create("gmioIn"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
		gmioOut[i]=adf::output_gmio::create("gmioOut"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
		k[i] = adf::kernel::create(vec_incr);
		adf::connect<adf::window<1024>>(gmioIn[i].out[0], k[i].in[0]);	
		adf::connect<adf::window<1032>>(k[i].out[0], gmioOut[i].in[0]);
		adf::source(k[i]) = "vec_incr.cc";
		adf::runtime<adf::ratio>(k[i])= 1;
		adf::location<adf::kernel>(k[i])=adf::tile(col[i],0);
	}
  };
};

#endif
