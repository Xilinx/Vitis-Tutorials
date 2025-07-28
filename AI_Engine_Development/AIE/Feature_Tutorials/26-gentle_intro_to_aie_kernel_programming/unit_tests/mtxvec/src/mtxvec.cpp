/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#include "mtxvec.hpp"

//#define KDEBUG	// comment out to suppress debug messages

#ifdef KDEBUG
	#include "aie_api/utils.hpp"    // "pretty-print" contents of vector registers
#endif // KDEBUG

// concatenates 4 sub-matrices into a larger matrix
// then multiplies the resulting matrix with an input vector
template<typename Ti, typename Tacc, unsigned mrows, unsigned mcols, unsigned burst_count>
void mtxvec(
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &UL_in,  // input upper left matrix
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &LL_in,  // input lower left matrix
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &UR_in,  // input upper right matrix
    adf::input_buffer<Ti, adf::extents<burst_count * mrows * mcols>> &LR_in,  // input lower right matrix
    input_stream<Ti> *istrm,        // input vector
    output_cascade<Tacc> *ocstrm    // output accumulator cascade
) {

    // initialize iterators for input submatrices
    auto ul_ptr = aie::begin_vector<mrows * mcols>(UL_in);
    auto ll_ptr = aie::begin_vector<mrows * mcols>(LL_in);
    auto ur_ptr = aie::begin_vector<mrows * mcols>(UR_in);
    auto lr_ptr = aie::begin_vector<mrows * mcols>(LR_in);

    using MMUL = aie::mmul<(mrows * 2), (mcols * 2), 1, Ti, Ti, Tacc>;    // alias for matrix-vector multiplication class
    MMUL mvmul; // instantiate matrix-vector multiplication class

    #ifdef KDEBUG
        printf("\n");
        printf("Running mtxvec() with %d sample%s...\n", burst_count, (burst_count > 1) ? "s": "");
    #endif // KDEBUG

    for (auto i = 0u; i < burst_count; i++) {

        #ifdef KDEBUG
            auto p_ul = ul_ptr;
            auto p_ur = ur_ptr;
            auto p_ll = ll_ptr;
            auto p_lr = lr_ptr;
        #endif // KDEBUG
        
        // concatenate sub-matrices
        auto [t1, t2] = aie::interleave_zip(*ul_ptr++, *ur_ptr++, mcols);
        auto top = aie::concat(t1, t2);
        auto [b1, b2] = aie::interleave_zip(*ll_ptr++, *lr_ptr++, mcols);
        auto bottom = aie::concat(b1, b2);
        auto m = aie::concat(top, bottom);

        auto v = readincr_v<mcols * 2>(istrm);  // form vector from stream
        mvmul.mul(m, v);                        // perform matrix-vector multiplication
        auto acc = mvmul.to_accum();            // put results in accumulator register
        writeincr(ocstrm, acc);                 // write accumulator to cascade stream

        #ifdef KDEBUG 
            printf("  i = %d:\n", i);
            aie::print_matrix(*p_ul++, mcols, "    a = ");
            printf("\n");
            aie::print_matrix(*p_ll++, mcols, "    b = ");
            printf("\n");
            aie::print_matrix(*p_ur++, mcols, "    d = ");
            printf("\n");
            aie::print_matrix(*p_lr++, mcols, "    e = ");
            printf("\n");
            aie::print_matrix(m, 2 * mcols, "    m = ");
            printf("\n");
            aie::print(v, true, "    v = ");
            printf("\n");
            auto p = mvmul.template to_vector<int32>();
            aie::print(p, true, "    p = ");
            printf("\n");
        #endif // KDEBUG
        
    } // end for (auto i = 0u; i < burst_count; i++)

} // end mtxvec()
