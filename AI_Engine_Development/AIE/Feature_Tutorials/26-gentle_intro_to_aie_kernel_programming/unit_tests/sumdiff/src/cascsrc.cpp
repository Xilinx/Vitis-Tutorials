/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#include "cascsrc.hpp"

//#define KDEBUG	// comment out to suppress debug messages

#ifdef KDEBUG
	#include "aie_api/utils.hpp"    // "pretty-print" contents of vector registers
#endif // KDEBUG

// cascade stream source
template<typename Ti, typename Tacc, unsigned nelems, unsigned burst_count>
void cascsrc(
    input_stream<Ti> *istrm,        // input stream
    output_cascade<Tacc> *ocstrm    // output cascade stream
) {

    #ifdef KDEBUG
        printf("\n");
        printf("Running cascsrc() with %d sample%s...\n", burst_count, (burst_count > 1) ? "s": "");
    #endif // KDEBUG

    for (auto i = 0u; i < burst_count; i++) {
        auto vin = readincr_v<nelems>(istrm); // read nelems from input stream
        auto acc = from_vector<Tacc>(vin);    // copy to accumulator
        writeincr(ocstrm, acc);             // write to output cascade stream

        #ifdef KDEBUG 
            printf("  i = %d:\n", i);
            aie::print(vin, true, "    vin = ");
            printf("\n");
        #endif // KDEBUG

    } // end for (auto i = 0u; i < burst_count; i++)

    
} // end cascsrc()