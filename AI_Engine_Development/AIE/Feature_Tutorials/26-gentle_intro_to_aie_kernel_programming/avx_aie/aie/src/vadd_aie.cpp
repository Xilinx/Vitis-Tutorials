/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>    // for aie::print()

#include "vadd_aie.h"

void vadd() {

    constexpr unsigned vsize = 8;           // number of vector elements
    using v8f = aie::vector<float, vsize>;  // floating-point vector register with "vsize" elements

    alignas(aie::vector_decl_align) const float x[] = { 1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0 };
    alignas(aie::vector_decl_align) const float y[] = { 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0 };
    alignas(aie::vector_decl_align) float z[vsize]; // z = x + y

    v8f vx = aie::load_v<vsize>(x); // transfer data from memory to vector registers
    v8f vy = aie::load_v<vsize>(y);

    v8f vz = aie::add(vx, vy);  // move sum from accumulator to vector register
    aie::store_v(z, vz);        // transfer data from vector register to memory

    printf("\n");
    aie::print(vz, true, "vz = "); // show contents of vector register
    printf("\n");

} // end vadd()
