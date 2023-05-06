// Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef _H_HISTOGRAM_H_
#define _H_HISTOGRAM_H_

#include "my_defines.h"

uint19_t prev_hist[GRAY_LEVELS] = {
#include "./test_data/m_histogram.h"
};

#endif
