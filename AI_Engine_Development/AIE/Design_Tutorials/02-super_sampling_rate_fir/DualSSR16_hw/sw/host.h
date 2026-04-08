/*
Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#pragma once


// AIE parameters
#define DATA_NBYTES 4
#define FULL_FRAME_LENGTH 512
#define FRAME_LENGTH 256

// PL parameters
#define PLIO_NDATA 4
#define PLIO_NBYTES (PLIO_NDATA*DATA_NBYTES)

#define PL_MEMORY_SIZE 512
