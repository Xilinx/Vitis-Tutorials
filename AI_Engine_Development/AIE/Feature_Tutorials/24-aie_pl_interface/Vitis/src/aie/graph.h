/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#include <adf.h>
#include "k_1_buff_i_1_buff_o.h"

#define NB_KERNELS 1

using namespace adf;

class my_graph : public graph {
private:
  kernel k[NB_KERNELS];
public:
  input_plio in;
  output_plio out;


  my_graph() {
    in  = input_plio::create("PLIO_i_0",plio_64_bits, "data/input.txt");
    out  = output_plio::create("PLIO_o_0",plio_64_bits, "data/output.txt");

    int loc_up = 1, loc_y = 0;
    int loc_x = 0;

    if(NB_KERNELS<312)
        loc_x = 6;
    
    for(int i=0; i<NB_KERNELS; i++){
        k[i] = kernel::create(k_1_buff_i_1_buff_o);
        source(k[i]) = "k_1_buff_i_1_buff_o.cc";

        dimensions(k[i].in[0]) = { 512 };
        dimensions(k[i].out[0]) = { 512 };

        runtime<ratio>(k[i]) = 0.99;


        location<kernel>(k[i]) = tile(loc_x,loc_y);
        location<stack>(k[i]) = location<kernel>(k[i]);

        if(i==0)
            location<buffer>(k[0].in[0]) = location<kernel>(k[0]);


        if(loc_up){
            if(loc_y==7){
                loc_x++;
                loc_up = 0;
            }
            else{
                loc_y++;
            }
        } else{
            if(loc_y==0){
                loc_x++;
                loc_up = 1;
            }
            else{
                loc_y--;
            }
        }

        if(loc_up && loc_y==0){
            location<buffer>(k[i].out[0]) = location<kernel>(k[i]);
        } else {
            location<buffer>(k[i].out[0]) = { address(loc_x, loc_y, 0x0),address(loc_x, loc_y, 0x2000) };
        }

    }


    if(NB_KERNELS==1){
        connect<>(in.out[0], k[0].in[0]);
        connect<>(k[0].out[0], out.in[0]);
    } else {
        connect<>(in.out[0], k[0].in[0]);
        connect<>(k[NB_KERNELS-1].out[0], out.in[0]);

        for(int i=0; i<NB_KERNELS-1; i++){
            connect<>(k[i].out[0], k[i+1].in[0]);
        }

    }
    
  }
};
