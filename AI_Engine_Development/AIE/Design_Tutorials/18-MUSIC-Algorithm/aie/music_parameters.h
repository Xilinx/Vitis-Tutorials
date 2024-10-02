//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

static constexpr unsigned ROW        = 128;  // M
static constexpr unsigned COL        =   8;  // N
static constexpr unsigned N          = COL;

static constexpr unsigned ROW_0      = 0;
static constexpr unsigned ROW_1      = 1;
static constexpr unsigned ROW_2      = 2;
static constexpr unsigned ROW_3      = 3;

static constexpr unsigned COL_0      = 0;
static constexpr unsigned COL_1      = 1;
static constexpr unsigned COL_2      = 2;
static constexpr unsigned COL_3      = 3;
static constexpr unsigned COL_4      = 4;
static constexpr unsigned COL_5      = 5;
static constexpr unsigned COL_6      = 6;
static constexpr unsigned COL_7      = 7;
static constexpr unsigned COL_NULL   = 255;

static constexpr unsigned COL_NORM_0 = 0;
static constexpr unsigned COL_NORM_1 = 1;
static constexpr unsigned COL_NORM_2 = 2;
static constexpr unsigned COL_NORM_3 = 3;
static constexpr unsigned COL_NORM_4 = 4;
static constexpr unsigned COL_NORM_5 = 5;
static constexpr unsigned COL_NORM_6 = 6;
static constexpr unsigned COL_NORM_7 = 7;

static constexpr unsigned COL_QR_1   = 1;
static constexpr unsigned COL_QR_2   = 2;
static constexpr unsigned COL_QR_3   = 3;
static constexpr unsigned COL_QR_4   = 4;
static constexpr unsigned COL_QR_5   = 5;
static constexpr unsigned COL_QR_6   = 6;
static constexpr unsigned COL_QR_7   = 7;

static constexpr unsigned SEGMENT_SIZE             = COL;  // 8 * 64bits for cfloat = 512 bits
static constexpr unsigned NUM_SEGMENTS_PER_COLUMN  = ROW / SEGMENT_SIZE;
static constexpr unsigned TOTAL_NUM_SEGMENTS       = NUM_SEGMENTS_PER_COLUMN *  COL;
static constexpr unsigned TOTAL_NUM_SEGMENTS_1     = NUM_SEGMENTS_PER_COLUMN * (COL - 1);
static constexpr unsigned TOTAL_NUM_SEGMENTS_2     = NUM_SEGMENTS_PER_COLUMN * (COL - 2);
static constexpr unsigned TOTAL_NUM_SEGMENTS_3     = NUM_SEGMENTS_PER_COLUMN * (COL - 3);
static constexpr unsigned TOTAL_NUM_SEGMENTS_4     = NUM_SEGMENTS_PER_COLUMN * (COL - 4);
static constexpr unsigned TOTAL_NUM_SEGMENTS_5     = NUM_SEGMENTS_PER_COLUMN * (COL - 5);
static constexpr unsigned TOTAL_NUM_SEGMENTS_6     = NUM_SEGMENTS_PER_COLUMN * (COL - 6);
static constexpr unsigned TOTAL_NUM_SEGMENTS_7     = NUM_SEGMENTS_PER_COLUMN * (COL - 7);

static constexpr unsigned NUM_POINTS               = 256;
static constexpr unsigned DOA_INTERVAL_LEN         =   4;  // MUST be a factor of COL, which is 8 as defined above
static constexpr unsigned NUM_DOA_INTERVALS        = NUM_POINTS / DOA_INTERVAL_LEN;
static constexpr unsigned NUM_DOA_KERNELS          = NUM_DOA_INTERVALS;
static constexpr unsigned NUM_SCANNER_KERNELS      = 2;
static constexpr unsigned ANGLE_FINDER_REGION_LEN  = 8;
static constexpr unsigned NUM_REGIONS              = NUM_POINTS / ANGLE_FINDER_REGION_LEN;  // MUST be a factor of NUM_POINTS
static constexpr unsigned NUM_REGIONS_PER_KERNEL   = 2;
static constexpr unsigned NUM_FINDER_KERNELS       = NUM_REGIONS / NUM_REGIONS_PER_KERNEL;

// system configuration parameters
static constexpr float    NOISE_SUBSPACE_THRESHOLD = 0.05f;     // 5%, threshold used to calculate nSources
static constexpr float    DOA_MIN_THRESHOLD        = 0.25f;     // no rounding error in IEEE-754 format
