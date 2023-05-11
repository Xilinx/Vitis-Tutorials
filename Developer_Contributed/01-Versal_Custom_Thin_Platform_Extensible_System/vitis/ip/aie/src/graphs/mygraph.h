//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include <adf.h>
#include "../kernels/kernels.hpp"

// skip namespace show explicit adf syntax//using namespace adf;

#define STR_LEN 30
/*
 * Adaptive dataflow graph to move data
 */
template <int IN_SIZE, int IN_MARGIN, int NUM>
class mygraph : public adf::graph {

  private:
    adf::kernel  dmove_i[NUM];

  public:
    adf::input_plio   in[NUM];
    adf::output_plio  out[NUM];
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
        in[i]  = adf::input_plio::create(plio_in_name[i], adf::plio_64_bits, "data/input.txt", 500);
        out[i] = adf::output_plio::create(plio_out_name[i], adf::plio_64_bits, out_filename[i], 500);
      }

      // specify kernels
      dmove_i[0]  = adf::kernel::create_object<my_scalar_dm<cint16, IN_SIZE> >();
      dmove_i[1]  = adf::kernel::create_object<my_vector_dm<cint16, IN_SIZE> >();
      dmove_i[2]  = adf::kernel::create_object<my_mul_dm<cint16, IN_SIZE> >();
      dmove_i[3]  = adf::kernel::create_object<my_stream_dm<cint16> >();
      adf::source(dmove_i[0]) = "kernels/my_scalar_dm.cpp";
      adf::source(dmove_i[1]) = "kernels/my_vector_dm.cpp";
      adf::source(dmove_i[2]) = "kernels/my_mul_dm.cpp";
      adf::source(dmove_i[3]) = "kernels/my_stream_dm.cpp";

      for (int i=0; i<NUM-1; i++){
        adf::runtime<ratio>(dmove_i[i])   = 0.82;
        adf::connect (in[i].out[0], dmove_i[i].in[0]);
        // Connect output
        adf::connect (dmove_i[i].out[0], out[i].in[0]);
        //dimensions(dmove_i[i].in[0]) = { IN_SIZE };
        //dimensions(dmove_i[i].out[0]) = { IN_SIZE };
      }
      adf::runtime<ratio>(dmove_i[3])   = 0.82;
      adf::connect< adf::stream > (in[3].out[0], dmove_i[3].in[0]);
      adf::connect< adf::stream > (dmove_i[3].out[0], out[3].in[0]);
    }
};


