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

using namespace sda;
using namespace sda::utils;

// -------------------------------------------------------------------------------------------
// This version of the host application uses random input images instead of actual images
// and is primeraly meant to be used for performance measurement
// -------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
  printf("----------------------------------------------------------------------------\n");

  // ---------------------------------------------------------------------------------
  // Parse command line
  // ---------------------------------------------------------------------------------

  CmdLineParser parser;
  parser.addSwitch("--nruns",  "-n", "Number of times to image is processed", "1");
  parser.addSwitch("--width",  "-w", "Image width",  "1920");
  parser.addSwitch("--height", "-h", "Image height", "1080");
  parser.addSwitch("--filter", "-f", "Filter type (0-6)", "0");

  //parse all command line options
  parser.parse(argc, argv);
  unsigned width       = parser.value_to_int("width");
  unsigned height      = parser.value_to_int("height");
  int      numRuns     = parser.value_to_int("nruns");
  unsigned filterType  = parser.value_to_int("filter");

  if ((width>MAX_IMAGE_WIDTH) || (height>MAX_IMAGE_HEIGHT)) {
    printf("ERROR: Maximum image size is %dx%d\n", MAX_IMAGE_WIDTH, MAX_IMAGE_HEIGHT);
    return -1;
  }
  if (filterType>6) {
    printf("ERROR: Supported filter type values are [0:6]\n");
    return -1;
  }

  printf("Number of runs    : %d\n", numRuns);
  printf("Image width       : %d\n", width);
  printf("Image height      : %d\n", height);
  printf("Filter type       : %d\n", filterType);
  printf("\n"); 
    
  // ---------------------------------------------------------------------------------
  // Generate input image
  // ---------------------------------------------------------------------------------
  
  // Read Input image
  unsigned stride    = ceil(width/64.0)*64;
  unsigned nbytes    = (stride*height);

  // Input and output buffers (Y,U, V)
  unsigned char *y_src = (unsigned char *)malloc(nbytes);
  unsigned char *u_src = (unsigned char *)malloc(nbytes);
  unsigned char *v_src = (unsigned char *)malloc(nbytes);
  unsigned char *y_dst = (unsigned char *)malloc(nbytes);
  unsigned char *u_dst = (unsigned char *)malloc(nbytes);
  unsigned char *v_dst = (unsigned char *)malloc(nbytes);

  // Generate random image data
  printf("Generating a random %dx%d input image\n", width, height);
  for (int i=0; i<nbytes; i++) {
  	y_src[i] = rand();
  	u_src[i] = rand();
  	v_src[i] = rand();
  	y_dst[i] = 0;
  	u_dst[i] = 0;
  	v_dst[i] = 0;
  }

  // Retrieve filter factor and bias
  float factor = filterFactors[filterType];
  short bias   = filterBiases[filterType];  

  // ---------------------------------------------------------------------------------
  // Run SW version
  // ---------------------------------------------------------------------------------

  printf("Running Software version on %d images\n", numRuns);    

auto cpu_begin = std::chrono::high_resolution_clock::now();

  #pragma omp parallel for num_threads(3)
  for(int n=0; n<numRuns; n++) 
  {
    // Compute reference results
    Filter2D(filterCoeffs[filterType], factor, bias, width, height, stride, y_src, y_dst);
    Filter2D(filterCoeffs[filterType], factor, bias, width, height, stride, u_src, u_dst);
    Filter2D(filterCoeffs[filterType], factor, bias, width, height, stride, v_src, v_dst);
  }

auto cpu_end = std::chrono::high_resolution_clock::now();

  // Report performance 
  std::chrono::duration<double> cpu_duration  = cpu_end - cpu_begin;
  float cpu_throughput  = (double) numRuns*3*nbytes / cpu_duration.count() / (1024.0*1024.0);
  printf("\n");
  printf("CPU  Time         : %10.4f s\n",    cpu_duration.count());
  printf("CPU  Throughput   : %10.4f MB/s\n", cpu_throughput);      
  printf("----------------------------------------------------------------------------\n"); 

  return 0;
}


