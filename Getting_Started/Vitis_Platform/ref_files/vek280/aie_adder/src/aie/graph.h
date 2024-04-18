/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "kernel.h"

using namespace adf;

class simpleGraph : public graph {
   private:
    kernel adder;

   public:
    input_plio pl_in0, pl_in1;
    output_plio pl_out;

    simpleGraph() {
        adder = kernel::create(aie_adder);

        pl_in0 = input_plio::create("DataIn0", adf::plio_32_bits, "data/input0.txt");
        pl_in1 = input_plio::create("DataIn1", adf::plio_32_bits, "data/input1.txt");
        pl_out = output_plio::create("DataOut", adf::plio_32_bits, "data/output.txt");

        connect<stream> net0(pl_in0.out[0], adder.in[0]);
        connect<stream> net1(pl_in1.out[0], adder.in[1]);
        connect<stream> net2(adder.out[0], pl_out.in[0]);

        source(adder) = "adder.cc";

        runtime<ratio>(adder) = 0.1;
    };
};

#endif /**********__GRAPH_H__**********/
