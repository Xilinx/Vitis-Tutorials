/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#ifndef __PK_H__
#define __PK_H__

#include <adf.h>
#include <kernels/kernels.h>

using namespace adf;

class converter : public adf::graph {
private:
   adf::kernel p_d,u_s,d_s;

public:
	output_plio out0,out1;
	input_plio in;   

   converter() {
      
	// Mapping inputs to PLIO
	in = input_plio::create("inx", plio_64_bits, "./data/inx.txt");
	
	// Mapping outputs to PLIO
	out0 = output_plio::create("upscale_out", plio_64_bits, "out_upscale.txt");
	out1 = output_plio::create("data_shuffle", plio_64_bits, "out_data_shuffle.txt");

	// create kernel 
      	p_d = adf::kernel::create(peak_detect);
      	u_s = adf::kernel::create(upscale);
     	d_s = adf::kernel::create(data_shuffle);
      
     	adf::source(p_d) = "peak_detect.cc";
      	adf::source(u_s) = "upscale.cc";
      	adf::source(d_s) = "data_shuffle.cc";
      	
      	//Specifying run-time ratio
        adf::runtime<ratio>(p_d) = 0.2;
      	adf::runtime<ratio>(u_s) = 0.2;
      	adf::runtime<ratio>(d_s) = 0.1;

	//Connecting PLIO ports with the kernel ports
        connect(in.out[0], p_d.in[0]);  
      	connect(p_d.out[1], u_s.in[0]);
      	connect(u_s.out[0],out0.in[0]);
      	connect(p_d.out[0],d_s.in[0]);
      	connect(d_s.out[0],out1.in[0]);

	//Specifying the size of IO buffers
	dimensions(p_d.in[0])  = {BUFFER_SIZE};
	dimensions(p_d.out[1]) = {BUFFER_SIZE};
	dimensions(u_s.in[0])  = {BUFFER_SIZE};
	dimensions(u_s.out[0]) = {BUFFER_SIZE};
	dimensions(p_d.out[0]) = {BUFFER_SIZE};
	dimensions(d_s.in[0])  = {BUFFER_SIZE};
	dimensions(d_s.out[0]) = {BUFFER_SIZE};
   }
};
#endif
