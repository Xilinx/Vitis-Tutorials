//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include <string>
#include <iostream>
#include <fstream>

#include "sar_top_1engine_cfg.h"
#include "sar_top_1engine_graph.h"

// Instantiate AIE graph:
sar_top_1engine_graph<REPEAT,NSAMP,0> engine0;

// ------------------------------------------------------------
// Read Input File
// ------------------------------------------------------------

template<typename T>
void read_data_file( std::string fname, unsigned NSAMP, T* datavec )
{
  std::fstream fs;
  fs.open(fname, std::ifstream::in);
  if (fs.is_open() == 0) { std::cerr << "Cannot open " << fname << std::endl; exit(1); }
  for (unsigned ii=0; ii < NSAMP; ii++) {
    if constexpr( std::is_same<T,cfloat>::value ) {
      float val_r, val_i;
      fs >> val_r >> val_i;
      datavec[ii] = {val_r,val_i};
    }
    else if constexpr( std::is_same<T,float>::value) {
      T val;
      fs >> val;
      datavec[ii] = val;
    }
  }
  fs.close();
}

// ------------------------------------------------------------
// Initialize Image
// ------------------------------------------------------------

void init_image( cfloat* image_v, unsigned NUM )
{
  for (unsigned rr=0; rr < NUM; rr++) {
    image_v[rr] = { 0, 0 };
  }
}

// ------------------------------------------------------------
// Dump Image
// ------------------------------------------------------------

void dump_image( cfloat* image_v, std::string fname, unsigned NSAMP )
{
  std::fstream fs;
  fs.open(fname,std::ofstream::out);
  fs.precision(9);
  fs.setf(std::ios::scientific,std::ios::floatfield);
  if (fs.is_open() == 0) { std::cerr << "Cannot open " << fname << std::endl; exit(1); }
  for (unsigned ii=0; ii < NSAMP; ii++) {
    fs << image_v[ii].real << " " << image_v[ii].imag << std::endl;
  }
  fs.close();
}

// ------------------------------------------------------------
// Regression
// ------------------------------------------------------------

bool regression(cfloat* image, std::string fname, unsigned NSAMP)
{
  static constexpr float level = 1e-6;
  std::fstream fs;
  fs.open(fname,std::ifstream::in);
  if (fs.is_open() == 0) { std::cerr << "Cannot open " << fname << std::endl; exit(1); }
  bool flag = 0;
  for (unsigned rr=0; rr < NSAMP; rr++) {
    float val_re, val_im;
    fs >> val_re >> val_im;
    bool this_re = ( std::fabs(image[rr].real - val_re) > level );
    bool this_im = ( std::fabs(image[rr].imag - val_im) > level );
    if (this_re | this_im) {
      std::cerr << "image[" << rr <<"] Error:  Golden: (" << val_re << ", " << val_im << ")"
                << "  Actual: (" << image[rr].real << ", " << image[rr].imag << ")" << std::endl;
    }
    flag |= this_re;
    flag |= this_im;
  }
  fs.close();
  return(flag);
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

// This is dummy code. We don't use AIE simulation for this design, use HW Emulation instead
int main(void)
{
  engine0.init();
  engine0.end();

  bool flag = 0;
  if ( flag == 0 )
    std::cout << "--- PASSED ---" << std::endl;
  else
    std::cout << "*** FAILED ***" << std::endl;

  return (flag);
}
