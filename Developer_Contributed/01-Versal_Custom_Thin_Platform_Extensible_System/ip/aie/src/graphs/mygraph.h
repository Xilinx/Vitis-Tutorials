// /**********
// © Copyright 2020 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **********/

#include <adf.h>
#include "../kernels/kernels.h"

using namespace adf;

/*
 * Adaptive dataflow graph to move data
 */
template <const int IN_SIZE, const int IN_MARGIN, const int NUM>
class mygraph : public graph {

  private:
    kernel  dmove_i[NUM];

  public:
    port<input>   in[NUM];
    port<output>  out[NUM];


    mygraph()
    {
      // specify kernels
      // Debug prints//printf("Template Win size = %d\n",IN_SIZE);
      // Debug prints//printf("Template Win margin = %d\n",IN_MARGIN);
      // datamover */ dmove_i[0]                   = kernel::create(datamover);
      // datamover */ source(dmove_i[0])           = "kernels/datamover.cc";
      // datascale */ dmove_i[1]                   = kernel::create(datascale);
      // datascale */ source(dmove_i[1])           = "kernels/datascale.cc";
      for (int i=0; i<NUM; i++){
        /* datamover */ dmove_i[i]                   = kernel::create(datamover);
        /* datamover */ source(dmove_i[i])           = "kernels/datamover.cc";
        // datascale */ dmove_i[i]                   = kernel::create(datascale);
        // datascale */ source(dmove_i[i])           = "kernels/datascale.cc";
        runtime<ratio>(dmove_i[i])   = 0.82;
        //location<kernel>(dmove_i[i]) = tile(20, 0);
        connect< stream, window<IN_SIZE, IN_MARGIN> > (in[i], dmove_i[i].in[0]);
        // Connect output
        connect< window<IN_SIZE> > (dmove_i[i].out[0], out[i]);
      }
    }
};


