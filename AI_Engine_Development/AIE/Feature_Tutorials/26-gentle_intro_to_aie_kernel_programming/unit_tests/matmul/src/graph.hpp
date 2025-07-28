/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "params.h"
#include "matmul.hpp"

using namespace adf;

// declare the ADF graph (it will be derived from the adf::graph class)
class theGraph : public graph {
private:

	kernel k1;          // matrix multiplier

public:

    input_plio ain, bin;    // A and B inputs
    output_plio pout;       // P output

    theGraph() {
        k1  = kernel::create(matmul<int16, int16, int16, Arows, Acols, Bcols, burst_count>);      // declare the function name and its template parameters
        source(k1)  = "src/matmul.cpp";                                         // declare the location of the source code for the kernel
        runtime<ratio>(k1)  = 1.0;                                              // only this kernel will be placed on this tile

        // note that this system uses the VCK190 evaluation board as a platform
        // this platform has an AIE clock of 1.25GHz
        // the PL portion will have a slower clock of *at most* half of this, or 625MHz
        ain  = input_plio::create(  "Ain", plio_64_bits, "data/Ain.dat", 625);  // declare the input file for the PLIO simulation source to run at 625MHz
        bin  = input_plio::create(  "Bin", plio_64_bits, "data/Bin.dat", 625);  // declare the input file for the PLIO simulation source to run at 625MHz
        pout = output_plio::create("Pout", plio_64_bits, "Pout.dat", 625);      // declare the output file for the PLIO simulation destination to run at 625MHz

        connect(ain.out[0], k1.in[0]);  // connect the A input PLIO port to the 1st kernel input
        connect(bin.out[0], k1.in[1]);  // connect the B input PLIO port to the 2nd kernel input
        connect(k1.out[0], pout.in[0]); // connect the kerenl output to the output PLIO port

        // location constraints on buffers
        // place all buffers on the same tile as the kernel
        location<buffer>(k1.in[0])  = location<kernel>(k1);
        location<buffer>(k1.in[1])  = location<kernel>(k1);
        location<buffer>(k1.out[0]) = location<kernel>(k1);

    } // end theGraph() constructor
    
}; // end class theGraph    
