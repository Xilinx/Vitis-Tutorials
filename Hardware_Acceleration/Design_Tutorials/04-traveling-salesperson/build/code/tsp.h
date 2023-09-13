/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef TRAVELLING_SALESMAN
#define TRAVELLING_SALESMAN
#include <algorithm>
#include <iostream>
#include <array>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <limits>
#include <hls_stream.h>

const int N = 5;

struct Coord
{
  float x;
  float y;
};
void tsp(hls::stream<uint16_t>& streamDistances, unsigned int& shortestDistance);

#endif
