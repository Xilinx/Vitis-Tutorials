/*
 Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
 SPDX-License-Identifier: X11
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
