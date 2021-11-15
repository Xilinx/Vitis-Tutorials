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
template <int IN_SIZE, int IN_MARGIN, int NUM>
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
       /* non-for-loop */ dmove_i[0]                   = kernel::create(datamover_scalar);
       /* non-for-loop */ source(dmove_i[0])           = "kernels/datamover_scalar.cc";
       /* non-for-loop */ dmove_i[1]                   = kernel::create(datamover_vector_reg);
       /* non-for-loop */ source(dmove_i[1])           = "kernels/datamover_vector_reg.cc";
       /* non-for-loop */ dmove_i[2]                   = kernel::create(datamover_mul_one);
       /* non-for-loop */ source(dmove_i[2])           = "kernels/datamover_mul_one.cc";
       /* non-for-loop */ dmove_i[3]                   = kernel::create(stream_datamover);
       /* non-for-loop */ source(dmove_i[3])           = "kernels/stream_datamover.cc";
      for (int i=0; i<NUM-1; i++){
        // datamover */ dmove_i[i]                   = kernel::create(datamover);
        // datamover */ source(dmove_i[i])           = "kernels/datamover.cc";
        runtime<ratio>(dmove_i[i])   = 0.82;
        connect< stream, window<IN_SIZE, IN_MARGIN> > (in[i], dmove_i[i].in[0]);
        // Connect output
        connect< window<IN_SIZE> > (dmove_i[i].out[0], out[i]);
      }
       runtime<ratio>(dmove_i[3])   = 0.82;
       // skip, not needed in this case // initialization_function(dmove_i[3]) = "stream_datamover_init";
       // use for loop// connect< stream, window<IN_SIZE, IN_MARGIN> > (in[0], dmove_i[0].in[0]);
       // use for loop// connect< window<IN_SIZE> > (dmove_i[0].out[0], out[0]);
       // use for loop// connect< stream, window<2*IN_SIZE, IN_MARGIN> > (in[1], dmove_i[1].in[0]);
       // use for loop// connect< window<2*IN_SIZE> > (dmove_i[1].out[0], out[1]);
       // use for loop// connect< stream, window<3*IN_SIZE, IN_MARGIN> > (in[2], dmove_i[2].in[0]);
       // use for loop// connect< window<3*IN_SIZE> > (dmove_i[2].out[0], out[2]);
       connect< stream > (in[3], dmove_i[3].in[0]);
       connect< stream > (dmove_i[3].out[0], out[3]);
    }
};


