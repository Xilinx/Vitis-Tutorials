//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include "music_parameters.h"

template<unsigned IN_V_FLAG, unsigned OUT_V_START, unsigned P0, unsigned Q0, unsigned P1, unsigned Q1, unsigned P2, unsigned Q2>
class SVD {
  public:
    SVD(void) { };
    void run(adf::input_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict in, adf::output_buffer<cfloat, adf::extents<adf::inherited_extent>> & __restrict out);
    static void registerKernelClass(void) { REGISTER_FUNCTION(SVD::run); };

  private:
    cfloat dot_product(aie::vector<cfloat,N> A, aie::vector<cfloat,N> B);
    void   compute_rotation(const unsigned p, const unsigned q);
    void   compute_VW(const unsigned p, const unsigned q);

    cfloat calc_ei_2t(const float x, const float y);
    cfloat calc_ei_t(const float sin_2t, const float cos_2t);
    void   calc_rot(const cfloat eit, const float c, const float s);

    alignas(32) aie::vector<cfloat,N> W[COL];
    alignas(32) aie::vector<cfloat,N> V[COL];
    alignas(32) aie::vector<float, N> S;
    alignas(32) aie::vector<cfloat,4> Rot;
};
