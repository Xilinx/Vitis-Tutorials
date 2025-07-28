/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#include "sumdiff.hpp"

//#define KDEBUG	// comment out to suppress debug messages

#ifdef KDEBUG
	#include "aie_api/utils.hpp"    // "pretty-print" contents of vector registers
#endif // KDEBUG

// vector addition or subtraction depending on input RTP
template<typename Tacc, typename Ti, typename To, unsigned nelems, unsigned burst_count>
void sumdiff(
    const int8 mode,                                                // runtime parameter: 0, add; otherwise subtract
    input_cascade<Tacc> *icstrm,                                    // input vector via accumulator cascade
    input_stream<Ti> *istrm,                                        // input vector via plio stream
    adf::output_buffer<To, adf::extents<burst_count * nelems>> &out // output sum or difference
) {

    // initialize iterator
    auto optr = aie::begin_vector<nelems>(out);

   #ifdef KDEBUG
        printf("\n");
        printf("Running sumdiff() with %d sample%s and mode = %d...\n", burst_count, (burst_count > 1) ? "s": "", mode);
    #endif // KDEBUG

    // avoid if-else whenever possible in innermost loops as they will be executed on the scalar processor
    if (mode == 0) {    // perform addition

        #ifdef KDEBUG
            printf("mode = %d\n", mode);
        #endif // KDEBUG

        for (auto i = 0u; i < burst_count; i++) {
            auto acci = readincr_v<nelems>(icstrm);         // get input from previous accumulator
            auto v2 = readincr_v<nelems>(istrm);            // get input from plio
            auto acc = aie::add(acci, v2);                  // perform addition
            auto result = acc.template to_vector<To>();     // copy accumulator to vector
            *optr++ = result;                               // write to output buffer

           #ifdef KDEBUG 
                printf("  i = %d:\n", i);
                auto v1 = acci.template to_vector<Ti>();
                aie::print(v1, true, "    v1 = ");
                printf("\n");
                aie::print(v2, true, "    v2 = ");
                printf("\n");
                aie::print(result, true, "    result = ");
                printf("\n");
            #endif // KDEBUG  

        } // end for (auto i = 0u; i < burst_count; i++) 
        
    } else {            // perform subtraction

        #ifdef KDEBUG
            printf("mode = %d\n", mode);
        #endif // KDEBUG

        for (auto i = 0u; i < burst_count; i++) {
            auto acci = readincr_v<nelems>(icstrm);     // get input from previous accumulator
            auto v2 = readincr_v<nelems>(istrm);        // get input from plio
            auto acc = aie::sub(acci, v2);              // perform subtraction
            auto result = acc.template to_vector<To>(); // copy accumulator to vector
            *optr++ = result;                           // write to output buffer
    
           #ifdef KDEBUG 
                printf("  i = %d:\n", i);
                auto v1 = acci.template to_vector<Ti>();
                aie::print(v1, true, "    v1 = ");
                printf("\n");
                aie::print(v2, true, "    v2 = ");
                printf("\n");
                aie::print(result, true, "    result = ");
                printf("\n");
            #endif // KDEBUG  
    
        } // end for (auto i = 0u; i < burst_count; i++) 
    } // end if-else (mode == 0)

} // end sumdiff()

