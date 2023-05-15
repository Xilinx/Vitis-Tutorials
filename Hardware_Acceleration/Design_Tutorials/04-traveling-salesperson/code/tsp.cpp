/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "tsp.h"

constexpr long int factorial(const int N)
{
  long int ret = 1;
  for(int i = 0 ; i < N; ++i)
    ret = ret * (i+1);
  return ret;
}


//
//  The function getDistance takes two arrays as inputs:
//  - An array of length N for the permutation indices of the cities
//  - An array of length N*N containing the distances between each pair of cities
//

template<typename T>
unsigned int getDistance(const T perm[N], const uint16_t distances[N][N])
{
  unsigned int ret = 0;
  for(int i = 0; i < N-1; ++i)
    ret += distances[perm[i]][perm[i+1]];
  return ret;

}


auto compute(const unsigned long int i_, const uint16_t distances[N][N])
{
  #pragma HLS INLINE

  unsigned long int i = i_;
  int perm[N] = {0};

  // The permutation generator is composed of two parts:
  // 1. Represent the loop index into a factorial base
  // 2. Generates the permutation vector 'perm'
  //
  // It uses this algorithm: https://stackoverflow.com/a/7919887/11316188
  // More info here: https://en.wikipedia.org/wiki/Factorial_number_system
  for (int k = 0; k < N; ++k) {
    perm[k] = i / factorial(N - 1 - k);
    i = i % factorial(N - 1 - k);
  }

  for (char k = N - 1; k > 0; --k)
    for (char j = k - 1; j >= 0; --j)
      perm[k] += (perm[j] <= perm[k]);

  // This is perm[] content for N=4 across all 3!=6 iterations:
  // 0 | 1 | 2 | 3
  // 0 | 1 | 3 | 2
  // 0 | 2 | 1 | 3
  // 0 | 2 | 3 | 1
  // 0 | 3 | 1 | 2
  // 0 | 3 | 2 | 1

  std::cout << "getDistance: "<< getDistance(perm,distances) << std::endl;

  return getDistance(perm,distances);

}


void tsp(hls::stream<uint16_t>& streamDistances, unsigned int& shortestDistance)
{
    // Specifies our input is AXI-Stream via the INTERFACE pragma
    #pragma HLS INTERFACE axis port=streamDistances

    uint16_t distances[N][N];
    #pragma HLS BIND_STORAGE variable=distances type=ram_1wnr

    // Load the 'distances' array
    loop_distances: for (int i = 0; i < N*N; ++i)
    {
        uint16_t val;
        streamDistances >> val;
        distances[i/N][i%N] = val;
    }

    // Main loop with (N-1)! iterations as one city is fixed
    constexpr long int factorialN = factorial(N-1);
    unsigned int candidate = std::numeric_limits<unsigned int>::max();
    loop_compute: for( unsigned long int i_ = 0; i_ < factorialN; i_ += 1 )
    {
      #pragma HLS pipeline II=1
      candidate = std::min(candidate, compute(i_, distances));
    }

    // Last candidate from loop above holds the shortest distance
    shortestDistance = candidate;

}
