//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include "m16_ssr8_cyclic_shift.h"

using TT_DUT = m16_ssr8_cyclic_shift;

void m16_ssr8_cyclic_shift_wrapper( TT_DUT::TT_STREAM sig_i[TT_DUT::SSR_I],
                                    TT_DUT::TT_STREAM sig_o[TT_DUT::SSR_O] )
{
  static TT_DUT dut;
  dut.run( sig_i, sig_o );
}
