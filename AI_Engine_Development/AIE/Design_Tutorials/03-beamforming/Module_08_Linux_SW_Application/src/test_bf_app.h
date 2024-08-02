/*
   Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
   SPDX-License-Identifier: MIT
   */


#ifndef __TEST_BF_APP_H__
#define __TEST_BF_APP_H__

#include <adf.h>
#include "xrt.h"
#include "experimental/xrt_aie.h"
#include "xrt/experimental/xrt_kernel.h"

#include "adf/adf_api/XRTConfig.h"

int test_dlbf(int iterFlag, int dataCheck);
int test_ulbf(int iterFlag, int dataCheck);

void enable_graphs(int niter, int debug);
void test_perf_all(int flag_all, int samples, xrt::device &device_name);
void stop_run();


#endif // __TEST_BF_APP_H_
