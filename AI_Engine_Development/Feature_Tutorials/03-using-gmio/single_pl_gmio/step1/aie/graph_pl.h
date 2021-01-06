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
#ifndef __GRAPH_PL_H__
#define __GRAPH_PL_H__

#include <adf.h>
#include "kernel.h"

class mygraph_pl: public adf::graph
{
private:
  adf::kernel k;
  adf::kernel k_mm2s;
  adf::kernel k_s2mm;

public:
  adf::port<adf::direction::inout> inoutMem;
  adf::port<adf::direction::in> inMem;

  mygraph_pl()
  {
		k = adf::kernel::create(vec_incr);
		adf::source(k) = "vec_incr.cc";
		adf::runtime<adf::ratio>(k)= 1;
	
		k_mm2s= adf::kernel::create(mm2s);
		adf::source(k_mm2s) = "hls/mm2s.cpp";
		adf::fabric<adf::pl>(k_mm2s);
		k_s2mm = adf::kernel::create(s2mm);
		adf::source(k_s2mm) = "hls/s2mm.cpp";
		adf::fabric<adf::pl>(k_s2mm);
		
		adf::connect<adf::gmem>(inMem,k_mm2s.in[0]);
		adf::connect<adf::stream,adf::window<1024>>(k_mm2s.out[0],k.in[0]);
		adf::connect<adf::window<1024>,adf::stream>(k.out[0],k_s2mm.in[0]);
		adf::connect<adf::gmem>(k_s2mm.inout[0],inoutMem);
  };
};

#endif
