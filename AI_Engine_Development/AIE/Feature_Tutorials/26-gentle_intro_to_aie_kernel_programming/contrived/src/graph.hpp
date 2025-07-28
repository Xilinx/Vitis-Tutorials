/**************************************/
// Copyright © 2025 Advanced Micro Devices
// SPDX-License-Identifier: MIT
/**************************************/

#pragma once

#include "params.h"
#include "sqmag.hpp"
#include "matmul.hpp"
#include "mtxvec.hpp"
#include "sumdiff.hpp"

using namespace adf;

// declare the ADF graph (it will be derived from the adf::graph class)
class theGraph : public graph {
private:

	kernel k_sqmag;                                     // squared magnitude
    kernel k_matmul1, k_matmul2, k_matmul3, k_matmul4;  // matrix multipliers
    kernel k_mtxvec;                                    // matrix-vector multiplier
    kernel k_sumdiff;                                   // sum-difference

public:

    input_plio xvec;                                // input complex vector
    input_plio Amtx, Bmtx, Cmtx, Dmtx, Emtx, Fmtx;  // input matrices
    input_plio yvec;                                // input vector
    input_port w;                                   // RTP
    output_plio zvec;                               // output vector

    using T1 = cint16;
    using T2 = int16;
    using T3 = int32;
    using Tacc = acc48;

    theGraph() {

        k_sqmag   = kernel::create(sqmag<T1, T2, vlen, burst_count>);  // declare the function name and its template parameters
        k_matmul1 = kernel::create(matmul<T2, T2, T2, mrows, mcols, mcols, burst_count>);
        k_matmul2 = kernel::create(matmul<T2, T2, T2, mrows, mcols, mcols, burst_count>);
        k_matmul3 = kernel::create(matmul<T2, T2, T2, mrows, mcols, mcols, burst_count>);
        k_matmul4 = kernel::create(matmul<T2, T2, T2, mrows, mcols, mcols, burst_count>);
        k_mtxvec  = kernel::create(mtxvec<T2, Tacc, mrows, mcols, burst_count>);
        k_sumdiff = kernel::create(sumdiff<Tacc, T3, T3, vlen, burst_count>);

        source(k_sqmag)   = "src/sqmag.cpp";   // declare the location of the source code for the kernel
        source(k_matmul1) = "src/matmul.cpp";
        source(k_matmul2) = "src/matmul.cpp";
        source(k_matmul3) = "src/matmul.cpp";
        source(k_matmul4) = "src/matmul.cpp";
        source(k_mtxvec)  = "src/mtxvec.cpp";
        source(k_sumdiff) = "src/sumdiff.cpp";

        runtime<ratio>(k_sqmag)   = 1.0;    // only this kernel will be placed on this tile
        runtime<ratio>(k_matmul1) = 1.0;
        runtime<ratio>(k_matmul2) = 1.0;
        runtime<ratio>(k_matmul3) = 1.0;
        runtime<ratio>(k_matmul4) = 1.0;
        runtime<ratio>(k_mtxvec)  = 1.0;
        runtime<ratio>(k_sumdiff) = 1.0;

        // note that this system uses the VCK190 evaluation board as a platform
        // this platform has an AIE clock of 1.25GHz
        // the PL portion will have a slower clock of *at most* half of this, or 625MHz
        xvec = input_plio::create( "xvec", plio_64_bits, "data/x.dat", 625); 
        Amtx = input_plio::create( "Amtx", plio_64_bits, "data/A.dat", 625);
        Bmtx = input_plio::create( "Bmtx", plio_64_bits, "data/B.dat", 625);
        Cmtx = input_plio::create( "Cmtx", plio_64_bits, "data/C.dat", 625);
        Dmtx = input_plio::create( "Dmtx", plio_64_bits, "data/D.dat", 625);
        Emtx = input_plio::create( "Emtx", plio_64_bits, "data/E.dat", 625);
        Fmtx = input_plio::create( "Fmtx", plio_64_bits, "data/F.dat", 625);
        yvec = input_plio::create( "yvec", plio_64_bits, "data/y.dat", 625);
        zvec = output_plio::create("zvec", plio_64_bits, "z.dat",      625);

        // establish connections
        connect(xvec.out[0], k_sqmag.in[0]);
        
        connect(Amtx.out[0], k_matmul1.in[0]);
        connect(Cmtx.out[0], k_matmul1.in[1]);

        connect(Bmtx.out[0], k_matmul2.in[0]);
        connect(Cmtx.out[0], k_matmul2.in[1]);

        connect(Dmtx.out[0], k_matmul3.in[0]);
        connect(Fmtx.out[0], k_matmul3.in[1]);
        
        connect(Emtx.out[0], k_matmul4.in[0]);
        connect(Fmtx.out[0], k_matmul4.in[1]);
        
        connect(k_matmul1.out[0], k_mtxvec.in[0]);
        connect(k_matmul2.out[0], k_mtxvec.in[1]);
        connect(k_matmul3.out[0], k_mtxvec.in[2]);
        connect(k_matmul4.out[0], k_mtxvec.in[3]);
        connect(k_sqmag.out[0],   k_mtxvec.in[4]);
        
        connect<parameter>(w, async(k_sumdiff.in[0]));
        connect<cascade>(k_mtxvec.out[0], k_sumdiff.in[1]);
        connect(yvec.out[0], k_sumdiff.in[2]);

        connect(k_sumdiff.out[0], zvec.in[0]);

        // placement constraints
        location<stack>(k_sqmag)           = location<kernel>(k_sqmag);

        location<stack>(k_matmul1)         = location<kernel>(k_matmul1);
        location<buffer>(k_matmul1.in[0])  = location<kernel>(k_matmul1);
        location<buffer>(k_matmul1.in[1])  = location<kernel>(k_matmul1);

        location<stack>(k_matmul2)         = location<kernel>(k_matmul2);
        location<buffer>(k_matmul2.in[0])  = location<kernel>(k_matmul2);
        location<buffer>(k_matmul2.in[1])  = location<kernel>(k_matmul2);

        location<stack>(k_matmul3)         = location<kernel>(k_matmul3);
        location<buffer>(k_matmul3.in[0])  = location<kernel>(k_matmul3);
        location<buffer>(k_matmul3.in[1])  = location<kernel>(k_matmul3);

        location<stack>(k_matmul4)         = location<kernel>(k_matmul4);
        location<buffer>(k_matmul4.in[0])  = location<kernel>(k_matmul4);
        location<buffer>(k_matmul4.in[1])  = location<kernel>(k_matmul4);

        location<stack>(k_mtxvec)          = location<kernel>(k_mtxvec);

        location<stack>(k_sumdiff)         = location<kernel>(k_sumdiff);
        location<buffer>(k_sumdiff.out[0]) = location<kernel>(k_sumdiff);
        
    } // end theGraph() constructor
    
}; // end class theGraph    