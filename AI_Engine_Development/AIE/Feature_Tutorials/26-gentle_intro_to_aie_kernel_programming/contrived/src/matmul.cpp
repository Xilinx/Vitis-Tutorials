/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#include "matmul.hpp"

//#define KDEBUG	// comment out to suppress debug messages

#ifdef KDEBUG
	#include "aie_api/utils.hpp"    // "pretty-print" contents of vector registers
#endif // KDEBUG

// Amtx: Arows x Acols
// Bmtx: Acols x Bcols
// Pmtx: Arows x Bcols
// multiply 2 matrix *tiles*
template <typename Ta, typename Tb, typename Tp, unsigned Arows, unsigned Acols, unsigned Bcols, unsigned burst_count>
void matmul(
    adf::input_buffer< Ta, adf::extents<burst_count * Arows * Acols>> &Amtx,  // input "A" matrix
    adf::input_buffer< Tb, adf::extents<burst_count * Acols * Bcols>> &Bmtx,  // input "B" matrix
    adf::output_buffer<Tp, adf::extents<burst_count * Arows * Bcols>> &Pmtx   // output "P" product matrix
) {

    constexpr unsigned Aelems = Arows * Acols;  // no. of elements in "A" matrix
    constexpr unsigned Belems = Acols * Bcols;  // no. of elements in "B" matrix
    constexpr unsigned Pelems = Arows * Bcols;  // no. of elements in "P" matrix
    using MMUL = aie::mmul<Arows, Acols, Bcols, Ta, Tb>;    // alias for matrix multiplication class

    auto aptr = aie::begin_vector<Aelems>(Amtx);    // iterator for "A" matrix
    auto bptr = aie::begin_vector<Belems>(Bmtx);    // iterator for "B" matrix
    auto pptr = aie::begin_vector<Pelems>(Pmtx);    // iterator for "P" matrix

    #ifdef KDEBUG
        printf("\n");
        printf("Running matmul() with %d samples...\n", burst_count);
    #endif // KDEBUG

    for (auto i = 0u; i < burst_count; i++) {

        auto a = *aptr++;   // load "Aelems" from buffer to vector register "a"
        auto b = *bptr++;   // load "Belems" from buffer to vector register "b"
        MMUL mtxmul;        // instantiate matrix multiplication class
        mtxmul.mul(a, b);   // perform matrix multiplication
        auto p = mtxmul.template to_vector<Tp>();   // store product matrix in accumulator to vector register "p"
        *pptr++ = p;        // send product matrix to output

		#ifdef KDEBUG
			printf("  i = %d:\n", i);
			aie::print_matrix(a, Acols, "    a = ");
            printf("\n");
			aie::print_matrix(b, Bcols, "    b = ");
            printf("\n");
            aie::print_matrix(p, Bcols, "    p = ");
			printf("\n");
		#endif // KDEBUG

    } // end for (auto i = 0u; i < burst_count; i++)

} // end matmul()


