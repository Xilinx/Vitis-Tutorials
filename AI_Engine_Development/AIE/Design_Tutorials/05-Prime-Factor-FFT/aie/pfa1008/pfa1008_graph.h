//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <adf.h>
#include <vector>

#include "dft7_graph.h"
#include "dft9_graph.h"
#include "dft16_graph.h"

using namespace adf;

class pfa1008_graph : public graph {
public:

  static constexpr int DFT7_X  = 18;
  static constexpr int DFT7_Y  =  0;
  static constexpr int DFT9_X  = 20;
  static constexpr int DFT9_Y  =  0;
  static constexpr int DFT16_X = 22;
  static constexpr int DFT16_Y =  0;

  // Declare vector of PLIO ports:
  std::array< input_plio,2> dft7_i;
  std::array<output_plio,2> dft7_o;
  std::array< input_plio,2> dft9_i;
  std::array<output_plio,2> dft9_o;
  std::array< input_plio,2> dft16_i;
  std::array<output_plio,2> dft16_o;

  // Declare all graphs:
  dft7_graph  dft7;
  dft9_graph  dft9;
  dft16_graph dft16;

  // Graph constructor:
  pfa1008_graph( void )
  {
    for (int ii=0; ii < 2; ii++) {
      std::string pname_dft7_i = "PLIO_dft7_i_" + std::to_string(ii);
      std::string pname_dft7_o = "PLIO_dft7_o_" + std::to_string(ii);
      std::string pname_dft9_i = "PLIO_dft9_i_" + std::to_string(ii);
      std::string pname_dft9_o = "PLIO_dft9_o_" + std::to_string(ii);
      std::string pname_dft16_i = "PLIO_dft16_i_" + std::to_string(ii);
      std::string pname_dft16_o = "PLIO_dft16_o_" + std::to_string(ii);
#ifdef AIE_SIM_ONLY
      std::string fname_dft7_i = "data/dft7_i_" + std::to_string(ii) + ".txt";
      std::string fname_dft7_o = "data/dft7_o_" + std::to_string(ii) + ".txt";
      std::string fname_dft9_i = "data/dft9_i_" + std::to_string(ii) + ".txt";
      std::string fname_dft9_o = "data/dft9_o_" + std::to_string(ii) + ".txt";
      std::string fname_dft16_i = "data/dft16_i_" + std::to_string(ii) + ".txt";
      std::string fname_dft16_o = "data/dft16_o_" + std::to_string(ii) + ".txt";
      dft7_i[ii] =  input_plio::create(pname_dft7_i,plio_64_bits,fname_dft7_i);
      dft7_o[ii] = output_plio::create(pname_dft7_o,plio_64_bits,fname_dft7_o);
      dft9_i[ii] =  input_plio::create(pname_dft9_i,plio_64_bits,fname_dft9_i);
      dft9_o[ii] = output_plio::create(pname_dft9_o,plio_64_bits,fname_dft9_o);
      dft16_i[ii] =  input_plio::create(pname_dft16_i,plio_64_bits,fname_dft16_i);
      dft16_o[ii] = output_plio::create(pname_dft16_o,plio_64_bits,fname_dft16_o);
#else
      dft7_i[ii] =  input_plio::create(pname_dft7_i,plio_64_bits);
      dft7_o[ii] = output_plio::create(pname_dft7_o,plio_64_bits);
      dft9_i[ii] =  input_plio::create(pname_dft9_i,plio_64_bits);
      dft9_o[ii] = output_plio::create(pname_dft9_o,plio_64_bits);
      dft16_i[ii] =  input_plio::create(pname_dft16_i,plio_64_bits);
      dft16_o[ii] = output_plio::create(pname_dft16_o,plio_64_bits);
#endif
    // Connect inputs and outputs:
      connect<stream,stream>( dft7_i[ii].out[0],  dft7.sig_i[ii] );
      connect<stream,stream>( dft7.sig_o[ii],     dft7_o[ii].in[0] );

      connect<stream,stream>( dft9_i[ii].out[0],  dft9.sig_i[ii] );
      connect<stream,stream>( dft9.sig_o[ii],     dft9_o[ii].in[0] );

      connect<stream,stream>( dft16_i[ii].out[0], dft16.sig_i[ii] );
      connect<stream,stream>( dft16.sig_o[ii],    dft16_o[ii].in[0] );
    } // ii

    // ==================== DFT7 Placement ====================
    // This places everything within a 2x2 square with (DFT7_X,DFT7_Y) in bottom left corner
    // --> The 'stack' for 'tile2' is placed outside the square to the left
    location<kernel>(dft7.k_tile2) = tile(DFT7_X+0,DFT7_Y+0);
    location<kernel>(dft7.k_tile0) = tile(DFT7_X+1,DFT7_Y+0);
    location<kernel>(dft7.k_tile1) = tile(DFT7_X+0,DFT7_Y+1);

    location<stack>(dft7.k_tile0) = bank(DFT7_X+1,DFT7_Y+1,0);
    location<stack>(dft7.k_tile1) = bank(DFT7_X+1,DFT7_Y+1,3);
    location<stack>(dft7.k_tile2) = bank(DFT7_X-1,DFT7_Y+0,0); // Outside 2x2 box

    location<parameter>(dft7.k_tile0.param[0]) = bank(DFT7_X+1,DFT7_Y+1,1);
    location<parameter>(dft7.k_tile1.param[0]) = bank(DFT7_X+1,DFT7_Y+1,2);

    location<buffer>(dft7.k_tile0.in[0]) = { bank(DFT7_X+1,DFT7_Y+0,0), bank(DFT7_X+1,DFT7_Y+0,1) };
    location<buffer>(dft7.k_tile0.in[1]) = { bank(DFT7_X+1,DFT7_Y+0,2), bank(DFT7_X+1,DFT7_Y+0,3) };
    location<buffer>(dft7.k_tile1.in[0]) = { bank(DFT7_X+0,DFT7_Y+1,0), bank(DFT7_X+0,DFT7_Y+1,1) };
    location<buffer>(dft7.k_tile1.in[1]) = { bank(DFT7_X+0,DFT7_Y+1,2), bank(DFT7_X+0,DFT7_Y+1,3) };
    location<buffer>(dft7.k_tile2.in[0]) = { bank(DFT7_X+0,DFT7_Y+0,0), bank(DFT7_X+0,DFT7_Y+0,1) };
    location<buffer>(dft7.k_tile2.in[1]) = { bank(DFT7_X+0,DFT7_Y+0,2), bank(DFT7_X+0,DFT7_Y+0,3) };

    // ==================== DFT9 Placement ====================
    // This places everything within a 2x3 square with (DFT9_X,DFT9_Y) in bottom left corner
    // --> The 'stack' for 'tile1' is placed outside the square to the left
    location<kernel>(dft9.k_tile0) = tile(DFT9_X+1,DFT9_Y+1);
    location<kernel>(dft9.k_tile1) = tile(DFT9_X+0,DFT9_Y+2);
    location<kernel>(dft9.k_tile2) = tile(DFT9_X+1,DFT9_Y+0);
    location<kernel>(dft9.k_tile3) = tile(DFT9_X+0,DFT9_Y+1);

    location<stack>(dft9.k_tile0) = bank(DFT9_X+1,DFT9_Y+0,2);
    location<stack>(dft9.k_tile1) = bank(DFT9_X-1,DFT9_Y+2,3); // Outside 2x3 box
    location<stack>(dft9.k_tile2) = bank(DFT9_X+1,DFT9_Y+0,0);
    location<stack>(dft9.k_tile3) = bank(DFT9_X+0,DFT9_Y+1,2);

    location<parameter>(dft9.k_tile0.param[0]) = bank(DFT9_X+1,DFT9_Y+0,3);
    location<parameter>(dft9.k_tile1.param[0]) = bank(DFT9_X+0,DFT9_Y+1,3);
    location<parameter>(dft9.k_tile2.param[0]) = bank(DFT9_X+1,DFT9_Y+0,1);

    location<buffer>(dft9.k_tile0.in[0]) = { bank(DFT9_X+1,DFT9_Y+2,0),bank(DFT9_X+1,DFT9_Y+2,1) };
    location<buffer>(dft9.k_tile0.in[1]) = { bank(DFT9_X+1,DFT9_Y+2,2),bank(DFT9_X+1,DFT9_Y+2,3) };
    location<buffer>(dft9.k_tile1.in[0]) = { bank(DFT9_X+0,DFT9_Y+2,0),bank(DFT9_X+0,DFT9_Y+2,1) };
    location<buffer>(dft9.k_tile1.in[1]) = { bank(DFT9_X+0,DFT9_Y+2,2),bank(DFT9_X+0,DFT9_Y+2,3) };
    location<buffer>(dft9.k_tile2.in[0]) = { bank(DFT9_X+0,DFT9_Y+0,0),bank(DFT9_X+0,DFT9_Y+0,1) };
    location<buffer>(dft9.k_tile2.in[1]) = { bank(DFT9_X+0,DFT9_Y+0,2),bank(DFT9_X+0,DFT9_Y+0,3) };
    location<buffer>(dft9.k_tile3.in[0]) = { bank(DFT9_X+1,DFT9_Y+1,0),bank(DFT9_X+1,DFT9_Y+1,1) };
    location<buffer>(dft9.k_tile3.in[1]) = { bank(DFT9_X+0,DFT9_Y+1,0),bank(DFT9_X+0,DFT9_Y+1,1) };
    location<buffer>(dft9.k_tile3.in[2]) = { bank(DFT9_X+1,DFT9_Y+1,2),bank(DFT9_X+1,DFT9_Y+1,3) };

    // ==================== DFT16 Placement ====================
    location<kernel>(dft16.k_tile0) = tile(DFT16_X+1,DFT16_Y+0);
    location<kernel>(dft16.k_tile1) = tile(DFT16_X+2,DFT16_Y+1);
    location<kernel>(dft16.k_tile2) = tile(DFT16_X+1,DFT16_Y+2);
    location<kernel>(dft16.k_tile3) = tile(DFT16_X+0,DFT16_Y+1);
    location<kernel>(dft16.k_tile4) = tile(DFT16_X+1,DFT16_Y+1);

    location<stack>(dft16.k_tile0) = bank(DFT16_X+1,DFT16_Y+0,0);
    location<stack>(dft16.k_tile1) = bank(DFT16_X+2,DFT16_Y+0,0);
    location<stack>(dft16.k_tile2) = bank(DFT16_X+1,DFT16_Y+2,3);
    location<stack>(dft16.k_tile3) = bank(DFT16_X+0,DFT16_Y+1,0);
    location<stack>(dft16.k_tile4) = bank(DFT16_X+1,DFT16_Y+2,0);

    location<parameter>(dft16.k_tile0.param[0]) = bank(DFT16_X+1,DFT16_Y+0,1);
    location<parameter>(dft16.k_tile0.param[1]) = bank(DFT16_X+1,DFT16_Y+0,2);
    location<parameter>(dft16.k_tile1.param[0]) = bank(DFT16_X+2,DFT16_Y+1,2);
    location<parameter>(dft16.k_tile1.param[1]) = bank(DFT16_X+2,DFT16_Y+1,3);
    location<parameter>(dft16.k_tile2.param[0]) = bank(DFT16_X+0,DFT16_Y+2,2);
    location<parameter>(dft16.k_tile2.param[1]) = bank(DFT16_X+0,DFT16_Y+2,3);
    location<parameter>(dft16.k_tile3.param[0]) = bank(DFT16_X+0,DFT16_Y+1,1);
    location<parameter>(dft16.k_tile3.param[1]) = bank(DFT16_X+0,DFT16_Y+1,2);

    location<buffer>(dft16.k_tile4.in[0]) = { bank(DFT16_X+1,DFT16_Y+1,1), bank(DFT16_X+1,DFT16_Y+1,3) };
    location<buffer>(dft16.k_tile4.in[1]) = { bank(DFT16_X+2,DFT16_Y+1,0), bank(DFT16_X+2,DFT16_Y+1,1) };
    location<buffer>(dft16.k_tile4.in[2]) = { bank(DFT16_X+1,DFT16_Y+2,1), bank(DFT16_X+1,DFT16_Y+2,2) };
    location<buffer>(dft16.k_tile4.in[3]) = { bank(DFT16_X+1,DFT16_Y+1,0), bank(DFT16_X+1,DFT16_Y+1,2) };

  }
};

