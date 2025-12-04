//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#pragma once

#include <adf.h>
#include "conv1d_w1_graph.h"
#include "max_pool1d_w2_graph.h"
#include "conv1d_graph_template.h"
#include "max_pool1d_graph_template.h"
#include "dense_w16_graph.h"
#include "dense_w17_graph.h"
#include "dense_w18_graph.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class radioml_top_graph : public graph {
public:
  port<input>                            data_iq;
  port<direction::in>                    weights_w1;
  port<direction::in>                    biases_w1;
  port<direction::in>                    weights_w3;
  port<direction::in>                    biases_w3;
  port<direction::in>                    weights_w5;
  port<direction::in>                    biases_w5;
  port<direction::in>                    weights_w7;
  port<direction::in>                    biases_w7;
  port<direction::in>                    weights_w9;
  port<direction::in>                    biases_w9;
  port<direction::in>                    weights_w11;
  port<direction::in>                    biases_w11;
  port<direction::in>                    weights_w13;
  port<direction::in>                    biases_w13;
  port<direction::in>                    weights_0_w16;
  port<direction::in>                    weights_1_w16;
  port<direction::in>                    biases_w16;
  port<direction::in>                    weights_w17;
  port<direction::in>                    biases_w17;
  port<direction::in>                    weights_w18;
  port<direction::in>                    biases_w18;
  port<output>                           data_o;

  // Graphs:
  conv1d_w1_graph                      layer_w1;
  max_pool1d_w2_graph                  layer_w2;
  conv1d_graph_template<512,64,7>      layer_w3;
  max_pool1d_graph_template<512,64>    layer_w4;
  conv1d_graph_template<256,64,7>      layer_w5;
  max_pool1d_graph_template<256,64>    layer_w6;
  conv1d_graph_template<128,64,7>      layer_w7;
  max_pool1d_graph_template<128,64>    layer_w8;
  conv1d_graph_template<64,64,7>       layer_w9;
  max_pool1d_graph_template<64,64>     layer_w10;
  conv1d_graph_template<32,64,7>       layer_w11;
  max_pool1d_graph_template<32,64>     layer_w12;
  conv1d_graph_template<16,64,7>       layer_w13;
  max_pool1d_graph_template<16,64>     layer_w14;
  dense_w16_graph                      layer_w16;
  dense_w17_graph                      layer_w17;
  dense_w18_graph                      layer_w18;

  radioml_top_graph( void )
  {
    // Connect IOs:
    connect<> ( data_iq,          layer_w1.data_iq );
    connect<> ( weights_w1,       layer_w1.weights );
    connect<> ( biases_w1,        layer_w1.biases );
    connect<> ( layer_w1.data_o,  layer_w2.data_i );
    
    connect<> ( layer_w2.data_o,  layer_w3.data_i );
    connect<> ( weights_w3,       layer_w3.weights );
    connect<> ( biases_w3,        layer_w3.biases );
    connect<> ( layer_w3.data_o,  layer_w4.data_i );
    
    connect<> ( layer_w4.data_o,  layer_w5.data_i );
    connect<> ( weights_w5,       layer_w5.weights );
    connect<> ( biases_w5,        layer_w5.biases );
    connect<> ( layer_w5.data_o,  layer_w6.data_i );
    
    connect<> ( layer_w6.data_o,  layer_w7.data_i );
    connect<> ( weights_w7,       layer_w7.weights );
    connect<> ( biases_w7,        layer_w7.biases );
    connect<> ( layer_w7.data_o,  layer_w8.data_i );
    
    connect<> ( layer_w8.data_o,  layer_w9.data_i );
    connect<> ( weights_w9,       layer_w9.weights );
    connect<> ( biases_w9,        layer_w9.biases );
    connect<> ( layer_w9.data_o,  layer_w10.data_i );
    
    connect<> ( layer_w10.data_o, layer_w11.data_i );
    connect<> ( weights_w11,      layer_w11.weights );
    connect<> ( biases_w11,       layer_w11.biases );
    connect<> ( layer_w11.data_o, layer_w12.data_i );
    
    connect<> ( layer_w12.data_o, layer_w13.data_i );
    connect<> ( weights_w13,      layer_w13.weights );
    connect<> ( biases_w13,       layer_w13.biases );
    connect<> ( layer_w13.data_o, layer_w14.data_i );
    
    connect<> ( layer_w14.data_o, layer_w16.data_i );
    connect<> ( weights_0_w16,    layer_w16.weights_0 );
    connect<> ( weights_1_w16,    layer_w16.weights_1 );
    connect<> ( biases_w16,       layer_w16.biases );
    
    connect<> ( layer_w16.data_o, layer_w17.data_i );
    connect<> ( weights_w17,      layer_w17.weights );
    connect<> ( biases_w17,       layer_w17.biases );
    
    connect<> ( layer_w17.data_o, layer_w18.data_i );
    connect<> ( weights_w18,      layer_w18.weights );
    connect<> ( biases_w18,       layer_w18.biases );
    
    connect<> ( layer_w18.data_o,  data_o );
    
    location<graph>(*this) = area_group({{aie_tile, 6, 0, 10, 3}});
  }
};

