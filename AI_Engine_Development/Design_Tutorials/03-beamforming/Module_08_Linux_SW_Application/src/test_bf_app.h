/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#ifndef __TEST_BF_APP_H__
#define __TEST_BF_APP_H__


int test_dlbf(int iterFlag, int dataCheck);
int test_ulbf(int iterFlag, int dataCheck);

void enable_graphs(int niter, int debug);
void test_perf_all(int flag_all, int samples);
void stop_run();


#endif // __TEST_BF_APP_H_
