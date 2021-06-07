/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

const int N = 13;

struct Coord
{
  float x;
  float y;
};
void tsp(hls::stream<uint16_t>& streamDistances, unsigned int& shortestDistance);

#endif
