// Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_graph.h>
#include <xrt/xrt_bo.h>

#include "mmult.h"

#define  TYPE_DATA signed int

static const char*    STR_ERROR   = "ERROR:   ";
static const char*    STR_FAILED  = "FAILED:  ";
static const char*    STR_PASSED  = "PASSED:  ";
static const char*    STR_INFO    = "INFO:    ";
static const char*    STR_USAGE   = "USAGE:   ";

/*
static const char*    STR_CHRONO  = "CHRONO:  ";
static const int      CHRONO_CAL_ITERATIONS = 2000;

void chrono_spend(
  int         chrono_diff,
  const char* str
) {
  if(chrono_diff >= 10000) {
    std::cout << STR_CHRONO << (int) (chrono_diff / 1000)    << "ms -> " << str << std::endl;
  } else {
    std::cout << STR_CHRONO << (int) (chrono_diff)           << "us -> " << str << std::endl;
  }
}
*/

int main(int argc, char* argv[]) {

  // Check command line
  if(argc < 2) {
    std::cout << STR_USAGE << argv[0] <<" <xclbin> " << std::endl;
    return EXIT_FAILURE;
  }

  int wSizeIn  = 2*SIZE;
  int bSizeIn  = wSizeIn  * sizeof(TYPE_DATA);
  int wSizeOut =   SIZE;
  int bSizeOut = wSizeOut * sizeof(TYPE_DATA);

//  std::cout << STR_INFO << "ARM sizeof(char)          = " << sizeof(char)          << std::endl;
//  std::cout << STR_INFO << "ARM sizeof(short int)     = " << sizeof(short int)     << std::endl;
//  std::cout << STR_INFO << "ARM sizeof(int)           = " << sizeof(int)           << std::endl;
//  std::cout << STR_INFO << "ARM sizeof(long int)      = " << sizeof(long int)      << std::endl;
//  std::cout << STR_INFO << "ARM sizeof(long long int) = " << sizeof(long long int) << std::endl;
//  std::cout << STR_INFO << "ARM sizeof(float)         = " << sizeof(float)         << std::endl;
//  std::cout << STR_INFO << "ARM sizeof(double)        = " << sizeof(double)        << std::endl;
/*
  INFO:    ARM sizeof(char)          = 1
  INFO:    ARM sizeof(short int)     = 2
  INFO:    ARM sizeof(int)           = 4
  INFO:    ARM sizeof(long int)      = 8
  INFO:    ARM sizeof(long long int) = 8
  INFO:    ARM sizeof(float)         = 4
  INFO:    ARM sizeof(double)        = 8
*/
  std::cout << STR_INFO << "wSizeIn    = " << wSizeIn    << std::endl;
  std::cout << STR_INFO << "wSizeOut   = " << wSizeOut   << std::endl;
  std::cout << STR_INFO << "bSizeIn    = " << bSizeIn    << std::endl;
  std::cout << STR_INFO << "bSizeOut   = " << bSizeOut   << std::endl;

  /*
  // Chrono Timer Calibration
  auto chrono_t0 = std::chrono::high_resolution_clock::now();
  auto chrono_t1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::micro> chrono_diff(0);
  std::chrono::duration<double, std::micro> chrono_average_calibration(0);
  for(int cnt = 0; cnt < CHRONO_CAL_ITERATIONS; cnt++) {
      chrono_t0   = std::chrono::high_resolution_clock::now();
      chrono_t1   = std::chrono::high_resolution_clock::now();
      chrono_diff = chrono_t1 - chrono_t0;
      chrono_average_calibration += chrono_diff;
  }
  chrono_average_calibration /= CHRONO_CAL_ITERATIONS;
  std::cout << STR_CHRONO << "Average Calibration   = " << chrono_average_calibration.count() << std::endl;
   */

  // Loading the xclbin
  char* xclbinFilename = argv[1];
  unsigned int dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << STR_PASSED << "auto my_device = xrt::device(" << dev_index << ")" << std::endl;
  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << STR_PASSED << "auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  // Input mm2s kernel
  std::cout << STR_INFO << "Opening the Input mm2s Kernel..." << std::endl;
  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_1}");
  std::cout << STR_PASSED << "auto in_0 = xrt::kernel(my_device, xclbin_uuid, \"mm2s:{mm2s_1}\")" << std::endl;
  auto in_0_bo = xrt::bo(my_device, bSizeIn, XCL_BO_FLAGS_NONE, in_0.group_id(0));
  std::cout << STR_PASSED << "auto in_0_bo = xrt::bo(my_device, bSizeIn, XCL_BO_FLAGS_NONE, in_0.group_id(0) (=" << in_0.group_id(0) << "))" << std::endl;
  auto in_0_bo_mapped = in_0_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto in_0_bo_mapped = in_0_bo.map<TYPE_DATA*>()" << std::endl;

  // Populating Input Buffer with random data
  std::cout << STR_INFO << "Setting Input Data " << std::endl;
  TYPE_DATA *matOp1 = (TYPE_DATA *) malloc(bSizeIn);
  TYPE_DATA *matOp2; matOp2 = matOp1 + wSizeIn/2;
  TYPE_DATA *matRef = (TYPE_DATA *) malloc(bSizeOut);
  union //PS: this conversion does not work if you used unsigned
  {
	TYPE_DATA 	i_val;
	T 			f_val;
  } converter;
  FILE *fin1=fopen("matrix_inputs.txt", "w");
  srand(time(NULL));   // Initialization, should only be called once.
  for(int r = 0; r<DIM; r++)
  {
	for(int c = 0; c<DIM; c++)
	{
      unsigned short int r1 = (unsigned short int) rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
      unsigned short int r2 = (unsigned short int) rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
      r1 = (r+c);
      r2 = (r*c);
      converter.f_val = (T) r1;
      matOp1[r*DIM+c] = converter.i_val;
      converter.f_val = (T) r2;
      matOp2[r*DIM+c] = converter.i_val;
      fprintf(fin1, "%10d\t%10d\n", r1, r2);
	}
  }
  fclose(fin1);
  for(int i = 0; i<wSizeIn/2; i++)
  {
	  TYPE_DATA t = matOp1[i];
	  in_0_bo_mapped[i] = t;
  }
  for(int i = wSizeIn/2; i<wSizeIn; i++)
  {
	  TYPE_DATA t = matOp2[i-wSizeIn/2];
	  in_0_bo_mapped[i] = t;
  }
  std::cout << STR_PASSED << "in_0_bo_mapped[i] = t " << std::endl;

  //compute golden results and write to file
  FILE *fin2=fopen("matrix_out_i_f.txt", "w");
  std::cout << STR_INFO << "Compute Golden Result " << std::endl;
  for (int r = 0; r < DIM; ++r)
  {
  	for (int c = 0; c < DIM; ++c)
  	{
  		T sum = 0;
  		for (int i = 0; i < DIM; ++i)
  		{
  			converter.i_val = matOp1[r*DIM+i];
  			T inp1 = converter.f_val;
  			converter.i_val = matOp2[i*DIM+c];
  			T inp2 = converter.f_val;
  			sum += inp1*inp2; //a[r][i] * b[i][c];
  		}
  		converter.f_val = sum;
  		matRef[r*DIM+c] = converter.i_val;
      fprintf(fin2, "%d %f\n", converter.i_val, sum);
  	}
  }
  fclose(fin2);

  /*
  //re-compute golden results to measure CPU time without file I/O
  chrono_t0 = std::chrono::high_resolution_clock::now();
  for (int r = 0; r < DIM; ++r)
  {
  	for (int c = 0; c < DIM; ++c)
  	{
  		T sum = 0;
  		for (int i = 0; i < DIM; ++i)
  		{
  			converter.i_val = matOp1[r*DIM+i];
  			T inp1 = converter.f_val;
  			converter.i_val = matOp2[i*DIM+c];
  			T inp2 = converter.f_val;
  			sum += inp1*inp2; //a[r][i] * b[i][c];
  		}
  		converter.f_val = sum;
  		matRef[r*DIM+c] = converter.i_val;
  	}
  }
  chrono_t1 = std::chrono::high_resolution_clock::now();
  chrono_diff = chrono_t1 - chrono_t0 - chrono_average_calibration;
  chrono_spend(chrono_diff.count(), "Matrix Multiplication in SW on ARM CPU: ");
  */

  // Sync Input Buffers
  std::cout << STR_INFO << "Sync Input Buffer..." << std::endl;
  in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  std::cout << STR_PASSED << "in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)" << std::endl;

  // Output s2mm Kernel
  std::cout << STR_INFO << "Opening Output s2mm Kernel..." << std::endl;
  auto out_0 = xrt::kernel(my_device, xclbin_uuid, "s2mm:{s2mm_1}");
  std::cout << STR_PASSED << "auto out_0 = xrt::kernel(my_device, xclbin_uuid, \"s2mm:{s2mm_1}\")" << std::endl;
  auto out_0_bo = xrt::bo(my_device, bSizeOut, XCL_BO_FLAGS_NONE, out_0.group_id(0));
  std::cout << STR_PASSED << "auto out_0_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, out.group_id(0) (=" << out_0.group_id(0) << "))" << std::endl;
  auto out_0_bo_mapped = out_0_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto out_0_bo_mapped = out_0_bo.map<TYPE_DATA*>()" << std::endl;
  memset(out_0_bo_mapped, 0x0, bSizeOut);

  // Opening the matrix multiplier kernel
  std::cout << STR_INFO << "Opening HLS Accel kernel " << std::endl;
  auto my_krnl  = xrt::kernel(my_device, xclbin_uuid, "HLS_accel");
  std::cout << STR_PASSED << "auto my_krnl  = xrt::kernel(my_device, xclbin_uuid, \"HLS_accel\")" << std::endl;

  /*
  chrono_t0 = std::chrono::high_resolution_clock::now();
  */
  // Start Input mm2s Kernel
  std::cout << STR_INFO << "Starting Input mm2s Kernel..." << std::endl;
  auto in_0_run = in_0(in_0_bo, nullptr, wSizeIn);
  std::cout << STR_PASSED << "auto in_0_run = in_0(in_0_bo, nullptr, " << wSizeIn << ")" << std::endl;
  // Starting the Output s2mm Kernels
  std::cout << STR_INFO << "Starting Output s2mm Kernel..." << std::endl;
  auto out_0_run = out_0(out_0_bo, nullptr, wSizeOut);
  std::cout << STR_PASSED << "auto out_0_run = out(out_0_bo, nullptr, " << wSizeOut << ")" << std::endl;
  // Starting the matrix multiplier kernel
  std::cout << STR_INFO << "starting HLS Accel kernel " << std::endl;
  auto my_krnl_run = my_krnl(nullptr, nullptr);
  std::cout << STR_PASSED << "my_krnl_run" << std::endl;

  // Waiting for all kernels to end
  std::cout << std::endl << STR_INFO << "Waiting for kernels to end..." << std::endl << std::endl;
  in_0_run.wait();
  std::cout << STR_PASSED << "in_0_run.wait()" << std::endl;
  my_krnl_run.wait();
  std::cout << STR_PASSED << "my_krnl_run.wait()" << std::endl;
  out_0_run.wait();
  std::cout << STR_PASSED << "out_0_run.wait()" << std::endl;

  // Sync Output Buffers
  std::cout << STR_INFO << "Sync Output Buffer..." << std::endl;
  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  std::cout << STR_PASSED << "out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)" << std::endl;

  /*
  chrono_t1 = std::chrono::high_resolution_clock::now();
  chrono_diff = chrono_t1 - chrono_t0 - chrono_average_calibration;
  chrono_spend(chrono_diff.count(), "Run+Wait Kernels & Sync Output Buffer");
   */

  // Verifying the results
  std::cout << std::endl << STR_INFO << "Verifying output data vs. golden ones " << std::endl << std::endl;
  FILE *fin3=fopen("matrix_out_bufs.txt", "w");
  int error_cnt = 0;
  TYPE_DATA *golden; golden = matRef;
  for(int i = 0; i < wSizeOut; i++)
  {
    if(out_0_bo_mapped[i] != golden[i])
    {
	    printf("Error found: @ %d, %d != %d\n", i, out_0_bo_mapped[i], golden[i]);
	    error_cnt++;
    }
    fprintf(fin3, "%d %d\n", out_0_bo_mapped[i], golden[i]);
  }
  fclose(fin3);

  //free memory
  free(matRef);
  free(matOp1);

  //close program
  std::cout << std::endl << ( (error_cnt>0) ? STR_FAILED : STR_PASSED) << argv[0] << std::endl << std::endl;
  return ( (error_cnt>0) ? EXIT_FAILURE : EXIT_SUCCESS );
}
