//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once

// ------------------------------------------------------------
// System Parameters
// ------------------------------------------------------------

static constexpr unsigned NPULSE_USE = 586;
static constexpr unsigned     NPULSE = 586;
static constexpr unsigned      NSAMP = 1024;
static constexpr unsigned     REPEAT = 32; // We have 8 engines, so 8 x 32 = 256
static constexpr unsigned      NIFFT = 2048;
static constexpr unsigned     NFRAME = 16;
static constexpr unsigned    NENGINE = 8;

static constexpr unsigned  LEN_SINGLE_PULSE = NIFFT;
static constexpr unsigned    LEN_PULSE_DATA = NIFFT * NPULSE_USE;
static constexpr unsigned    LEN_RANGE_DATA = NSAMP * REPEAT * 3;
static constexpr unsigned    LEN_COORD_DATA = NPULSE * 3;
static constexpr unsigned LEN_R0_RANGE_DATA = NPULSE;
static constexpr unsigned    LEN_IMAGE_DATA = NSAMP * REPEAT;

static constexpr unsigned NUM_BYTES_SINGLE_PULSE = LEN_SINGLE_PULSE * 8;
static constexpr unsigned NUM_BYTES_PULSE_DATA   = LEN_PULSE_DATA * 8;
static constexpr unsigned NUM_BYTES_RANGE_DATA   = LEN_RANGE_DATA * 4;
static constexpr unsigned NUM_BYTES_IMAGE_DATA   = LEN_IMAGE_DATA * 8;

static_assert(NSAMP*REPEAT*NENGINE==512*512,"Incorrect image parameters");
