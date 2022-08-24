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
#include "dds.h"

class ddsgraph: public adf::graph
{
private:
  adf::kernel dds;

public:
  adf::port<adf::direction::out> out;
  adf::port<adf::direction::in> trigger;

  ddsgraph()
  {
    dds = adf::kernel::create(sine);
    //connect asynchronous runtime parameter
    adf::connect<adf::parameter>(trigger, adf::async(dds.in[0]));

    adf::connect< adf::window<512> >(dds.out[0], out);
    adf::source(dds) = "kernels/dds.cc";
    adf::runtime<adf::ratio>(dds)= 0.9;
  };
};

#endif /* __GRAPH_H__ */


