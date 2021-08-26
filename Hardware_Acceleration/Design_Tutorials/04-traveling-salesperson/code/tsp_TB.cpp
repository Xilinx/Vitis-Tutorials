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

#include <iostream>
#include <ap_fixed.h>
#include <iomanip>
#include <vector>

#include "tsp.h"

// main
// 

int main()
{

  // Stream from the Vitis HLS built-in library hls_stream.h
  hls::stream< uint16_t > inputDistances("distances");
  unsigned int outputDistance;

  const Coord cities[] = {{40.7127837, -74.0059413 },
                          {34.0522342, -118.2436849},
                          {41.8781136, -87.6297982 },
                          {29.7604267, -95.3698028 },
                          {39.9525839, -75.1652215 },
                          {33.4483771, -112.0740373},
                          {29.4241219, -98.4936282 },
                          {32.715738,  -117.1610838},
                          {32.7766642, -96.7969879 },
                          {37.3382082, -121.8863286},
                          {30.267153,  -97.7430608 },
                          {39.768403,  -86.158068  },
                          {30.3321838, -81.655651  },
                          {37.7749295, -122.4194155},
                          {39.9611755, -82.9987942 }
                         };

  // Avoids lengthy simulations...
  // and makes sure the "expected" array isn't accessed out-of-bound
  //
  assert(N<12);
  unsigned int expected[11] = {0, 65535, 66318, 75186, 75662, 75709, 76001, 76967, 79715, 80994, 81559};

  float distances[N*N];

  float maxDist = 0;

  for( short int i = 0; i < N; ++i ) {
    for( short int j = 0; j < N; ++j ) {
      // Euclidian distance calculation
      float dist = std::sqrt(
                             std::pow(cities[i].x-cities[j].x, 2)
                           + std::pow(cities[i].y-cities[j].y, 2)
                             );
      //std::cout << "Current distance: " << dist << "\n";
      // Array "distances" holds all the measures
      distances[i*N+j] = dist;
      // Report greatest distance seen
      maxDist = std::max(maxDist, dist);
      std::cout << "Current computed distance: " << dist << "\n";
    }
  }

  // Normalizing distances to improve precision
  uint16_t maxUint = std::numeric_limits<uint16_t>::max();
  for ( int i = 0; i < N*N; ++i ) {
    //std::cout << "Distance (fp32): " << distances[i] << " Normalized (uint16): " << (uint16_t)((distances[i]/maxDist) * maxUint) << "\n";
    inputDistances.write((distances[i]/maxDist) * maxUint);
  }

  // Call the hardware function, the dut (device under test).
  // inputDistances is the stream of distances and
  // outputDistance is the result.
  tsp(inputDistances, outputDistance);

  // Print result
  std::cout << "Shortest trip (raw): " << outputDistance << "\n";
  std::cout << "Shortest trip: " << (float)(outputDistance * maxDist) / (float)(maxUint) << "\n";

  // Check against expected answer only for N from 1 to 11...

  if (outputDistance == expected[N-1]) {
    std::cout << "Result matches...\n";
    return 0;
    }
  else {
    std::cout << "Result " << outputDistance << " does not match expected result !!! -> " << expected[N-1] << "\n";
    return 1;
  }
}