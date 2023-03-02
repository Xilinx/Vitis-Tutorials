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

#define STR_LEN 30
/*
 * Adaptive dataflow graph to move data
 */
template <int IN_SIZE, int IN_MARGIN, int NUM>
class mygraph : public graph {

  private:
    kernel  dmove_i[NUM];

  public:
    input_plio   in[NUM];
    output_plio  out[NUM];
    char plio_in_name[NUM][STR_LEN];
    char plio_out_name[NUM][STR_LEN];
    char out_filename[NUM][STR_LEN];


    mygraph()
    {
      // specify PLIO ports
      for(int i=0; i<NUM; i++) {
        sprintf(plio_in_name[i],"DataIn%d",i);
        sprintf(plio_out_name[i],"DataOut%d",i);
        sprintf(out_filename[i],"data/output%d.txt",i);
        in[i]  = input_plio::create(plio_in_name[i], plio_64_bits, "data/input.txt", 500);
        out[i] = output_plio::create(plio_out_name[i], plio_64_bits, out_filename[i], 500);
      }

      // specify kernels
      dmove_i[0]                   = kernel::create(datamover_scalar);
      source(dmove_i[0])           = "kernels/datamover_scalar.cc";
      dmove_i[1]                   = kernel::create(datamover_vector_reg);
      source(dmove_i[1])           = "kernels/datamover_vector_reg.cc";
      dmove_i[2]                   = kernel::create(datamover_mul_one);
      source(dmove_i[2])           = "kernels/datamover_mul_one.cc";
      dmove_i[3]                   = kernel::create(stream_datamover);
      source(dmove_i[3])           = "kernels/stream_datamover.cc";
      for (int i=0; i<NUM-1; i++){
        runtime<ratio>(dmove_i[i])   = 0.82;
        connect< stream, window<IN_SIZE, IN_MARGIN> > (in[i].out[0], dmove_i[i].in[0]);
        // Connect output
        connect< window<IN_SIZE> > (dmove_i[i].out[0], out[i].in[0]);
      }
       runtime<ratio>(dmove_i[3])   = 0.82;
       connect< stream > (in[3].out[0], dmove_i[3].in[0]);
       connect< stream > (dmove_i[3].out[0], out[3].in[0]);
    }
};


