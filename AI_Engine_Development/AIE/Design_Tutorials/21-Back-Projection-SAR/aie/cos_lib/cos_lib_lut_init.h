//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

#include <cmath>

template <typename T, unsigned LUT_SIZE, unsigned TP_COARSE_BITS, unsigned TP_DOMAIN_MODE>
std::array<T,LUT_SIZE> initialize_cos_LUT(void)
{
  std::array<T,LUT_SIZE> LUT;
  switch (TP_DOMAIN_MODE) {
  case 0:                       // 0 <= arg < 1
    {
      T pi = 3.141592653589793238462643383279502884197f;
      T deltaX = pow(0.5f,float(TP_COARSE_BITS));
      for (unsigned ii=0; ii < (1<<TP_COARSE_BITS); ii++) {
        T xlow = deltaX * float(ii);
        T xhgh = xlow + deltaX;
        T ylow = std::cos(2.0f*pi*xlow);
        T yhgh = std::cos(2.0f*pi*xhgh);
        T slope = (yhgh - ylow)/deltaX;
        T offset = ylow - slope*xlow;
        LUT[2*ii+0] = slope;
        LUT[2*ii+1] = offset;
      }
      break;
    }
  case 1:                       // 1 <= arg < 2
    break;
  case 2:                       // 1 <= arg < 4
    break;
  }
  return(LUT);
}
