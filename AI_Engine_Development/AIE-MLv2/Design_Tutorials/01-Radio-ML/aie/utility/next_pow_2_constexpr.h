//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani
#ifndef NEXT_POW_2_CONSTEXPR_H
#define NEXT_POW_2_CONSTEXPR_H
constexpr int next_pow_2_constexpr(double x) {
    int power = 1;
    while(power < x)
        power*=2;
    return power;
}
#endif
