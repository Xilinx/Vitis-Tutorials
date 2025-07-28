/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "params.h"
#include "sqmag.hpp"

using namespace adf;

// declare the ADF graph (it will be derived from the adf::graph class)
class theGraph : public graph {
private:

	kernel k1;          // magnitude squared

public:

    input_plio ivec;    // input vector
    output_plio ovec;   // output vector

    theGraph() {
        k1  = kernel::create(sqmag<cint16, int16, vlen, burst_count>);  // declare the function name and its template parameters
        source(k1)  = "src/sqmag.cpp";                                  // declare the location of the source code for the kernel
        runtime<ratio>(k1)  = 1.0;                                      // only this kernel will be placed on this tile

        // note that this system uses the VCK190 evaluation board as a platform
        // this platform has an AIE clock of 1.25GHz
        // the PL portion will have a slower clock of *at most* half of this, or 625MHz
        ivec = input_plio::create( "ivec", plio_64_bits, "data/ivec.dat", 625); // declare the input file for the PLIO simulation source to run at 625MHz
        ovec = output_plio::create("ovec", plio_64_bits, "ovec.dat", 625);      // declare the output file for the PLIO simulation destination to run at 625MHz
        connect(ivec.out[0], k1.in[0]);                                         // connect the input PLIO port to the kernel input
        connect(k1.out[0], ovec.in[0]);                                         // connect the kerenl output to the output PLIO port

    } // end theGraph() constructor
    
}; // end class theGraph    