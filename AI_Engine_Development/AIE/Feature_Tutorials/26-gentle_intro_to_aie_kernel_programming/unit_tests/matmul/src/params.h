/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#pragma once

constexpr unsigned Arows = 4;       // no. of rows in A
constexpr unsigned Acols = 4;       // no. of columns in A
constexpr unsigned Bcols = 4;       // no. of columns in B

constexpr unsigned burst_count = 8; // number of "samples" to process per function invocation
constexpr unsigned iter_count = 2;  // no. of function invocations

