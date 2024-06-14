/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#pragma once


// AIE parameters
// Number of bytes per sample (data are cint16)
#define DATA_NBYTES 4
// Number of samples in a frame (defined in the Makefile)
// FRAME_LENGTH

// PL parameters
// PLIO_Width is 128 bits --> it contains 4 samples --> 16 bytes
#define PLIO_NBYTES (PLIOW/8)
#define PLIO_NDATA (PLIO_NBYTES/DATA_NBYTES)

// Array size defined in mm2s and s2mm
#define PL_MEMORY_SIZE 512
