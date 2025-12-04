//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani
#ifndef CEIL_CONSTEXPR_H
#define CEIL_CONSTEXPR_H
constexpr int ceil_constexpr(double x) {
    if (x == static_cast<int>(x)) {
        return x;
    } else if (x > 0) {
        return static_cast<int>(x) + 1;
    } else {
        return static_cast<int>(x);
    }
}
#endif
