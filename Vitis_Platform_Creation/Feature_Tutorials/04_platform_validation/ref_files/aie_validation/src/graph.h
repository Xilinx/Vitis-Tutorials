/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "kernels.h"

using namespace adf;

class gmio_graph : public adf::graph 
{
  private:
	kernel loop0;
  public:
	input_gmio gm_in0;
    output_gmio gm_out0;
    
	port<input> myRtp0;
  
    //static const int is_rtp = 1;  //1- for Enable 0-for Disable    
    gmio_graph() { 
			
		gm_in0=input_gmio::create("gm_in0", 64, 1);
		gm_out0=output_gmio::create("gm_out0",64, 1);
		 
		loop0 = kernel::create(loopback);

		connect< window<128> > net2 (gm_in0.out[0], loop0.in[0]);
		connect< window<128> > net3 (loop0.out[0], gm_out0.in[0]);
		connect<parameter>(myRtp0, async(loop0.in[1] ) );

		location<kernel>(loop0) = tile(9,0);
		location<buffer>(loop0.in[0])  = { address(9, 0, 0x0), address(9, 0, 0x2000) };
		location<buffer>(loop0.out[0]) = { address(9, 0, 0x4000), address(9, 0, 0x6000) };
		
		source(loop0) = "loopback.cc";
		runtime<ratio>(loop0) = 0.9;
    };
};

class plio_graph : public adf::graph 
{
  private:
	kernel loop0;
  public:
    input_plio in0;
    output_plio out0;
   	
	port<input> myRtp0;
  
    //static const int is_rtp = 1;  //1- for Enable 0-for Disable    
    plio_graph() { 
			
		in0  = adf::input_plio::create( "in0",adf::plio_64_bits, "data/input.txt");
		out0 = adf::output_plio::create( "out0", adf::plio_64_bits, "data/output.txt");

		loop0 = kernel::create(loopback);

		connect< window<128> > net0 (in0.out[0], loop0.in[0]);
		connect< window<128> > net1 (loop0.out[0], out0.in[0]);
		connect<parameter>(myRtp0, async(loop0.in[1] ) );	
		
		location<kernel>(loop0) = tile(8,0);
		location<buffer>(loop0.in[0])  = { address(8, 0, 0x0), address(8, 0, 0x2000) };
		location<buffer>(loop0.out[0]) = { address(8, 0, 0x4000), address(8, 0, 0x6000) };
		
		source(loop0) = "loopback.cc";
		runtime<ratio>(loop0) = 0.9;
	};
};


