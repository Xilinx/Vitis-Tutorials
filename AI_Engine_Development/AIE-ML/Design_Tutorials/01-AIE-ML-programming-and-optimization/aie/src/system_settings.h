/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#pragma once

// Multiply 2 matrices   (MxK) x (KxN)
#define A_ROWS sizeM
#define A_COLS sizeK

#define B_ROWS A_COLS
#define B_COLS sizeN

#define C_ROWS A_ROWS
#define C_COLS B_COLS

// // Input/Output Matrix Type
// #define ITYPE int8
// #define OTYPE int16
// #define SHIFT_RESULT 6

// Non Sparse Tiling: 4x16x8

#define ATILES_ROWS subM
#define ATILES_COLS subK
#define BTILES_ROWS ATILES_COLS
#define BTILES_COLS subN
#define CTILES_ROWS ATILES_ROWS
#define CTILES_COLS BTILES_COLS

// Sparse Tiling: 4x16x8

// #define ATILES_ROWS_S subM
// #define ATILES_COLS_S subK
// #define BTILES_ROWS_S ATILES_COLS_S
// #define BTILES_COLS_S subN
// #define CTILES_ROWS_S ATILES_ROWS_S
// #define CTILES_COLS_S BTILES_COLS_S


// Simulation length
#define NITERATIONS NIterations
