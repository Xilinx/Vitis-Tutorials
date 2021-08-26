
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

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <chrono>

#include "cmdlineparser.h" 
#include "coefficients.h"
#include "common.h" 

using namespace std;
using namespace sda;
using namespace sda::utils;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

int main(int argc, char** argv)
{
  printf("----------------------------------------------------------------------------\n");
  printf("HLS Testbench for Xilinx 2D Filter Example\n");
        
  // ---------------------------------------------------------------------------------
  // Parse command line
  // ---------------------------------------------------------------------------------
  CmdLineParser parser;
  parser.addSwitch("--width",  "-w", "Image width", "1000");
  parser.addSwitch("--height", "-h", "Image height",  "30");

  //parse all command line options
  parser.parse(argc, argv);
  unsigned width   = parser.value_to_int("width");
  unsigned height  = parser.value_to_int("height");
  unsigned stride  = ceil(width/64.0)*64;
  unsigned nbytes  = stride*height; //(stride*height);

  printf("Image info\n");
  printf("- Width     : %10d\n", width);
  printf("- Height    : %10d\n", height);
  printf("- Stride    : %10d\n", stride);
  printf("- Bytes     : %10d\n", nbytes);

  // Input and output buffers (Y,U, V)
  unsigned char *y_src = (unsigned char *)malloc(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT);
  unsigned char *u_src = (unsigned char *)malloc(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT);
  unsigned char *v_src = (unsigned char *)malloc(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT);
  unsigned char *y_dst = (unsigned char *)malloc(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT);
  unsigned char *u_dst = (unsigned char *)malloc(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT);
  unsigned char *v_dst = (unsigned char *)malloc(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT);

  // Coefficients
  char coeffs[FILTER_V_SIZE][FILTER_H_SIZE];

  for (int i=0; i<nbytes; i++) {
  	y_src[i] = rand();
  	u_src[i] = rand();
  	v_src[i] = rand();
  	y_dst[i] = 0;
  	u_dst[i] = 0;
  	v_dst[i] = 0;
  }

  for(int row=0; row<FILTER_V_SIZE; row++) {
    for(int col=0; col<FILTER_H_SIZE; col++) {
      coeffs[row][col]=1;
    }
  }

  unsigned num_coefs = FILTER_V_SIZE*FILTER_H_SIZE;
  unsigned num_coefs_padded = (((num_coefs-1)/64)+1)*64;
  char *coeffs_padded = (char *)malloc(num_coefs_padded);
  memcpy(coeffs_padded, &coeffs[0][0], num_coefs);

  // filter factor and bias
  float factor = 1.0/(FILTER_V_SIZE*FILTER_H_SIZE);
  short bias   = 0;  

  // ---------------------------------------------------------------------------------
  // Run the FPGA kernels
  // ---------------------------------------------------------------------------------

  printf("Running FPGA accelerator\n");    
 
  Filter2DKernel(coeffs_padded, factor, bias, width, height, stride, y_src, y_dst);
  // Filter2DKernel(coeffs_padded, factor, bias, width, height, stride, u_src, u_dst);
  // Filter2DKernel(coeffs_padded, factor, bias, width, height, stride, v_src, v_dst);


  // ---------------------------------------------------------------------------------
  // Compute reference results and compare
  // ---------------------------------------------------------------------------------

  printf("Comparing results\n");    

  // Create output buffers for reference results
  unsigned char *y_ref = (unsigned char *)malloc(nbytes);
  unsigned char *u_ref = (unsigned char *)malloc(nbytes);
  unsigned char *v_ref = (unsigned char *)malloc(nbytes);

  // Compute reference results
  Filter2D(coeffs, factor, bias, width, height, stride, y_src, y_ref);
  // Filter2D(coeffs, factor, bias, width, height, stride, u_src, u_ref);
  // Filter2D(coeffs, factor, bias, width, height, stride, v_src, v_ref);

  // Compare results
  bool diff = false;
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          if ( y_dst[y*stride+x] != y_ref[y*stride+x] ) diff = true;
          // if ( u_dst[y*stride+x] != u_ref[y*stride+x] ) diff = true;
          // if ( v_dst[y*stride+x] != v_ref[y*stride+x] ) diff = true;
      }
  }

  if(diff) {
      printf("%sTest FAILED: Output has mismatches with reference%s\n", RED, RESET);    
  } else {
      printf("%sTest PASSED: Output matches reference%s\n", GREEN, RESET);    
  }
  printf("----------------------------------------------------------------------------\n"); 

  return (diff?1:0);
}


