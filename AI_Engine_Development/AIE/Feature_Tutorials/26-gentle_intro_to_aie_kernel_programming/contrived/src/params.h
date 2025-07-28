/**************************************/
// Copyright © 2025 Advanced Micro Devices
// SPDX-License-Identifier: MIT
/**************************************/

#pragma once

constexpr unsigned vlen = 8;        // vector length
constexpr unsigned mrows = 4;       // no. of rows in input matrices
constexpr unsigned mcols = 4;       // no. of columns in input matrices
constexpr unsigned burst_count = 8; // number of "samples" to process per function invocation
constexpr unsigned iter_count = 2;  // no. of function invocations

