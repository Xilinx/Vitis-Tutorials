/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "params.h"
#include "cascsrc.hpp"
#include "sumdiff.hpp"

using namespace adf;

// declare the ADF graph (it will be derived from the adf::graph class)
class theGraph : public graph {
private:

	kernel k1;  // cascade source
    kernel k2;  // sum-difference

public:

    input_port mode;        // input RTP
    input_plio xin, yin;    // vector inputs
    output_plio zout;       // result

    theGraph() {
        
        k1 = kernel::create(cascsrc<int32, acc48, vlen, burst_count>);          // declare the function name and its template parameters
        k2 = kernel::create(sumdiff<acc48, int32, int32, vlen, burst_count>);  
        
        source(k1) = "src/cascsrc.cpp";                                         // declare the location of the kernel source code
        source(k2) = "src/sumdiff.cpp";

        runtime<ratio>(k1) = 1.0;   // only this kernel will be placed on this tile
        runtime<ratio>(k2) = 1.0;

        // note that this system uses the VCK190 evaluation board as a platform
        // this platform has an AIE clock of 1.25GHz
        // the PL portion will have a slower clock of *at most* half of this, or 625MHz
        xin  = input_plio::create("xin", plio_64_bits, "data/x.dat", 625);  // declare the input file for the PLIO simulation source to run at 625MHz
        yin  = input_plio::create("yin", plio_64_bits, "data/y.dat", 625);  // declare the input file for the PLIO simulation source to run at 625MHz
        zout = output_plio::create("zout", plio_64_bits, "z.dat",    625);  // declare the output file for the PLIO simulation destination to run at 625MHz

        connect(xin.out[0], k1.in[0]);              // input plio to cascade source
        connect<parameter>(mode, async(k2.in[0]));  // asynchronous RTP
        connect<cascade>(k1.out[0], k2.in[1]);      // cascade connection between kernels
        connect(yin.out[0], k2.in[2]);
        connect(k2.out[0], zout.in[0]);

        // location constraints
        location<stack>(k1) = location<kernel>(k1);
        location<stack>(k2) = location<kernel>(k2);
        location<buffer>(k2.in[0]) = location<kernel>(k2);
        location<buffer>(k2.out[0]) = location<kernel>(k2);

    } // end theGraph() constructor
    
}; // end class theGraph    