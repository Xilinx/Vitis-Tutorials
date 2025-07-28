/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

#include "params.h"
#include "mtxvec.hpp"
#include "cascsink.hpp"

using namespace adf;

// declare the ADF graph (it will be derived from the adf::graph class)
class theGraph : public graph {
private:

	kernel k1;  // matrix-vector multiplier
    kernel k2;  // cascade sink

public:

    input_plio ul_in, ll_in, ur_in, lr_in, vin; // matrix and vector input
    output_plio vout;                         // vector output

    theGraph() {
        
        k1 = kernel::create(mtxvec<int16, acc48, mrows, mcols, burst_count>);  // declare the function name and its template parameters
        k2 = kernel::create(cascsink<acc48, int32, (mcols * 2), burst_count>);  
        
        source(k1) = "src/mtxvec.cpp";                                         // declare the location of the kernel source code
        source(k2) = "src/cascsink.cpp";

        runtime<ratio>(k1) = 1.0;   // only this kernel will be placed on this tile
        runtime<ratio>(k2) = 1.0;

        // note that this system uses the VCK190 evaluation board as a platform
        // this platform has an AIE clock of 1.25GHz
        // the PL portion will have a slower clock of *at most* half of this, or 625MHz
        ul_in = input_plio::create("UL_in", plio_64_bits, "data/ul.dat", 625);  // declare the input file for the PLIO simulation source to run at 625MHz
        ll_in = input_plio::create("LL_in", plio_64_bits, "data/ll.dat", 625);
        ur_in = input_plio::create("UR_in", plio_64_bits, "data/ur.dat", 625);
        lr_in = input_plio::create("LR_in", plio_64_bits, "data/lr.dat", 625);
        vin   = input_plio::create("Vin",   plio_64_bits, "data/vi.dat", 625);
        vout  = output_plio::create("Vout", plio_64_bits, "vout.dat",    625);  // declare the output file for the PLIO simulation destination to run at 625MHz

        connect(ul_in.out[0], k1.in[0]);
        connect(ll_in.out[0], k1.in[1]);
        connect(ur_in.out[0], k1.in[2]);
        connect(lr_in.out[0], k1.in[3]);
        connect(vin.out[0],   k1.in[4]);

        connect(k1.out[0], k2.in[0]);
        connect(k2.out[0], vout.in[0]);

        // location constraints
        location<stack>(k1) = location<kernel>(k1);
        location<stack>(k2) = location<kernel>(k2);

    } // end theGraph() constructor
    
}; // end class theGraph    
