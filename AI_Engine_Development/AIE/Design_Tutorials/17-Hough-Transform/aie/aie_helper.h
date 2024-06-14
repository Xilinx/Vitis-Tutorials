//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

// ------------------------------------------------------------
// static constexpr "ceil" function
// ------------------------------------------------------------

constexpr int aie_ceil( float f )
{
  const int val = static_cast<int>(f);
  return( (f > val) ? val + 1 : val );
}

