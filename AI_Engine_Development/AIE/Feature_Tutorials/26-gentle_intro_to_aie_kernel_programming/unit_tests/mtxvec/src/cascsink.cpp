/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#include "cascsink.hpp"

//#define KDEBUG	// comment out to suppress debug messages

#ifdef KDEBUG
	#include "aie_api/utils.hpp"    // "pretty-print" contents of vector registers
#endif // KDEBUG

// cascade stream sink
template<typename Tacc, typename To, unsigned nelems, unsigned burst_count>
void cascsink(
    input_cascade<Tacc> *icstrm,    // input cascade stream
    output_stream<To> *ostrm        // output stream
) {

    #ifdef KDEBUG
        printf("\n");
        printf("Running cascsink() with %d sample%s...\n", burst_count, (burst_count > 1) ? "s": "");
    #endif // KDEBUG

    for (auto i = 0u; i < burst_count; i++) {
        auto acc = readincr_v<nelems>(icstrm);  // read nelems from input cascade stream
        auto v = acc.template to_vector<To>();  // copy to vector
        writeincr(ostrm, v);                    // write to output stream

        #ifdef KDEBUG 
            printf("  i = %d:\n", i);
            aie::print(v, true, "    v = ");
            printf("\n");
        #endif // KDEBUG

    } // end for (auto i = 0u; i < burst_count; i++)

} // end cascsink()

