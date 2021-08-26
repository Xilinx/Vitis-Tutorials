/*
 * Copyright 2020 Xilinx, Inc.
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

#include "diamond.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{

  data_t test[N];
  data_t outcome[N];

  int retval = 0;
  ofstream FILE;

  // Init test vector
  for (int i = 0; i < N; i++)
  {
    test[i] = (data_t)i;
  }
  // Save the results to a file
    FILE.open ("result.dat");

  // Executing the DUT thrice
  for (int iter = 0; iter < 3; iter++)
  {
    // Execute DUT
    diamond(test, outcome);

    // Display the results
    for (int i = 0; i < N; i++)
    {
      cout << "Series " << iter;
      cout << " Outcome: " << (int)outcome[i] << endl;
      FILE << (int)outcome[i] << endl;
    }
    FILE.close();
  }

  // Compare the results file with the golden results
  retval = system("diff --brief -w result.dat result.golden.dat");
  if (retval != 0)
  {
    cout << "Test failed  !!!" << endl;
    retval = 1;
  }
  else
  {
    cout << "Test passed !" << endl;
  }

  // Return 0 if the test passed
  return retval;
}
