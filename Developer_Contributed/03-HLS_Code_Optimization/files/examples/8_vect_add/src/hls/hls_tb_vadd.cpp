/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "vadd_include.h"


int main(int argc, char* argv[])
{

  uint32_t *ptr_inp_a = (uint32_t *) malloc(DATA_SIZE * sizeof(uint32_t));
  uint32_t *ptr_inp_b = (uint32_t *) malloc(DATA_SIZE * sizeof(uint32_t));
  uint32_t *ptr_res   = (uint32_t *) malloc(DATA_SIZE * sizeof(uint32_t));

  //setting input data
  FILE *fin1=fopen("vector_inputs.txt", "w");

  // Initialization, should only be called once.
  //srand(time(NULL));  //this line makes HLS COSIM TO FAIL: cosim is a two steps process, data originated in the second step would be totally different from first step
  srand(41); // to fix the problem of fist ;line
  for(int i = 0 ; i< DATA_SIZE; i++)
  {
    // to avoid overflows let us use short ints
    unsigned short int r1 = (unsigned short int) rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
    unsigned short int r2 = (unsigned short int) rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
    ptr_inp_a[i] = r1;
    ptr_inp_b[i] = r2;
    fprintf(fin1, "%10d\t%10d\n", r1, r2);
  }
  fclose(fin1);

  //call DUT
  krnl_vadd(ptr_inp_a, ptr_inp_b, ptr_res, DATA_SIZE);

  // Verify the result
  FILE *fin2=fopen("vector_out.txt", "w");
  int match = 0;
  for (int i = 0; i < DATA_SIZE; i++)
  {
    uint32_t host_result = ptr_inp_a[i] + ptr_inp_b[i];
    fprintf(fin2, "%10d\t%10d\n", host_result, ptr_res[i]);
    if (ptr_res[i] != host_result) {
        printf("MISMATCH ERROR at %d: expected %d got %d\n", i, host_result, ptr_res[i]);
        match = 1;
        //break;
    }
  }
  fclose(fin2);
  free(ptr_inp_a);
  free(ptr_inp_b);
  free(ptr_res);

  std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
  return match;

}
