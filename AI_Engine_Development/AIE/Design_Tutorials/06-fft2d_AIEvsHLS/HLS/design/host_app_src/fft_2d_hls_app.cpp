/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "experimental/xrt_aie.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"

#define MAT_SIZE (MAT_ROWS * MAT_COLS)

/////////////////////////////////////////////////
// Due to 128bit Data Transfer all dimensions,
// to be given as by 4 for cint16
// since 4 samples of cint16 are passed 
/////////////////////////////////////////////////
#if FFT_2D_DT == 0
   #define MAT_SIZE_128b (MAT_SIZE / 4)
   #define MAT_ROWS_128b (MAT_ROWS / 4)
   #define MAT_COLS_128b (MAT_COLS / 4)
/////////////////////////////////////////////////
// Due to 128bit Data Transfer all dimensions,
// to be given as by 2 for cfloat
// since 2 samples of cfloat are passed 
/////////////////////////////////////////////////

#elif FFT_2D_DT == 1
   #define MAT_SIZE_128b (MAT_SIZE / 2)
   #define MAT_ROWS_128b (MAT_ROWS / 2)
   #define MAT_COLS_128b (MAT_COLS / 2)

#endif

using namespace std;

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
   if (fnm.empty())
      throw std::runtime_error("No xclbin speified");

   // load bit stream
   std::ifstream stream(fnm);
   stream.seekg(0,stream.end);
   size_t size = stream.tellg();
   stream.seekg(0,stream.beg);

   std::vector<char> header(size);
   stream.read(header.data(),size);

   auto top = reinterpret_cast<const axlf*>(header.data());
   if (xrtDeviceLoadXclbin(device, top))
      throw std::runtime_error("Bitstream download failed");

   return header;
}

class datamover
{
   public:
   xrtKernelHandle dma_hls_khdl;
   xrtRunHandle dma_hls_rhdl;
   uint32_t instance_errCnt;

   void init(xrtDeviceHandle dhdl, const axlf *top, char insts, int16_t iterCnt)
   {
      std::string dma_hls_obj_str = "dma_hls:{dma_hls_" + to_string(insts) + "}";
      const char *dma_hls_obj = dma_hls_obj_str.c_str();

      //////////////////////////////////////////
      // Data Mover IP Init
      //////////////////////////////////////////

      // Open kernel handle exclusively to read the ap_return register later for reporting error...
      dma_hls_khdl = xrtPLKernelOpenExclusive(dhdl, top->m_header.uuid, dma_hls_obj);
      dma_hls_rhdl = xrtRunOpen(dma_hls_khdl);

      int rval = xrtRunSetArg(dma_hls_rhdl, 4, MAT_SIZE_128b);
          rval = xrtRunSetArg(dma_hls_rhdl, 5, MAT_ROWS_128b);
          rval = xrtRunSetArg(dma_hls_rhdl, 6, MAT_COLS_128b);
          rval = xrtRunSetArg(dma_hls_rhdl, 7, iterCnt);
      
      printf("Initialised dma_hls...\n");
   }

   void run(void)
   {
      xrtRunStart(dma_hls_rhdl);
      printf("dma_hls is running...\n");
   }

   void waitTo_complete(void)
   {
      auto state = xrtRunWait(dma_hls_rhdl);
      std::cout << "dma_hls Kernel completed with status(" << state << ")\n";
   }

   void golden_check(uint32_t *errCnt, char insts)
   {
      //////////////////////////////////////////
      // Compare results
      //////////////////////////////////////////

      // Reading the error count for the ap_return reg of the hls kernel...
      xrtKernelReadRegister(dma_hls_khdl, 0x10, &instance_errCnt);
      
      //std::cout << "fft_2d_" << insts << std::endl;
      printf("fft_2d_%d ", insts);
      std::cout << (instance_errCnt ? "Failed! " : "Passed! ") << "With error count " << instance_errCnt << ".\n" << std::endl;

      // Adding instance error to the total error count...
      *errCnt += instance_errCnt;
   }

   void close(void)
   {
      xrtRunClose(dma_hls_rhdl);
      printf("Closed dma_hls kernel run handle...\n");

      xrtKernelClose(dma_hls_khdl);
      printf("Closed dma_hls kernel handle...\n");
   }
};

//class fft_2d_class
//{
//   public:
//   xrtKernelHandle fft_2d_khdl;
//   xrtRunHandle fft_2d_rhdl;
//
//   void init(xrtDeviceHandle dhdl, const axlf *top, char insts)
//   {
//      std::string fft_2d_obj_str = "fft_2d:{fft_2d_" + to_string(insts) + "}";
//      const char *fft_2d_obj = fft_2d_obj_str.c_str();
//
//      //////////////////////////////////////////
//      // FFT_2D IP Init
//      //////////////////////////////////////////
//
//      fft_2d_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, fft_2d_obj);
//      fft_2d_rhdl = xrtRunOpen(fft_2d_khdl);
//
//      int rval = xrtRunSetArg(fft_2d_rhdl, 4, ITER_CNT);
//      printf("Initialised fft_2d...\n");
//   }
//
//   void run(void)
//   {
//      //Running fft-2d kernel...
//      xrtRunStart(fft_2d_rhdl);
//      printf("fft_2d is running...\n");
//   }
//
//   void waitTo_complete(void)
//   {
//      auto state = xrtRunWait(fft_2d_rhdl);
//      std::cout << "fft_2d Kernel completed with status(" << state << ")\n";
//   }
//
//   void close(void)
//   {
//      xrtRunClose(fft_2d_rhdl);
//      printf("Closed fft_2d kernel run handle...\n");
//
//      xrtKernelClose(fft_2d_khdl);
//      printf("Closed fft_2d kernel handle...\n");
//   }
//};

int main(int argc, char ** argv)
{
   //////////////////////////////////////////
   // Open xclbin
   //////////////////////////////////////////
   if((argc < 2) || (argc > 3)) {
      std::cout << "Usage: " << argv[0] <<" <xclbin>" << " iteration count(optional)" << std::endl;
      return EXIT_FAILURE;
   }
   
   const char* xclbinFilename = argv[1];
   int16_t iterCnt = 0;
   
   if(argc == 3) {
      std::string iter = argv[2];
      iterCnt = stoi(iter);
   }
   else {
      iterCnt = ITER_CNT;
   }
   printf("Iteration : %d...\n", iterCnt);
   auto dhdl = xrtDeviceOpen(0);
   auto xclbin = load_xclbin(dhdl, xclbinFilename);
   auto top = reinterpret_cast<const axlf*>(xclbin.data());
            
    //////////////////////////////////////////
    // Data-Mover IP Objects...
    //////////////////////////////////////////

    datamover dmaHls[FFT2D_INSTS];

    ////////////////////////////////////////////
    //// fft_2d IP Objects....
    ////////////////////////////////////////////
    //
    //fft_2d_class fft_2d_kr[FFT2D_INSTS];

    ////////////////////////////////////////////
    //// Initialising fft_2d Kernels...
    ////////////////////////////////////////////
    //
    //for(int i = 0; i < FFT2D_INSTS; ++i)
    //{
    //   printf("Initialising fft_2d %d...\n", i);
    //   fft_2d_kr[i].init(dhdl, top, i);
    //}

    //////////////////////////////////////////
    // Initialising DataMover Units...
    //////////////////////////////////////////

    for(int i = 0; i < FFT2D_INSTS; ++i)
    {
       printf("Initialising datamover %d...\n", i);
       dmaHls[i].init(dhdl, top, i, iterCnt);
    }

    ////////////////////////////////////////////
    //// Running fft_2d Kernels...
    ////////////////////////////////////////////

    //for(int i = 0; i < FFT2D_INSTS; ++i)
    //{
    //   printf("Running fft_2d %d...\n", i);
    //   fft_2d_kr[i].run();
    //}

    //////////////////////////////////////////
    // Running datamovers in each kernel for
    //////////////////////////////////////////

    printf("Running datamovers for %d Iterations...\n", iterCnt);

    //////////////////////////////////////////
    // Running for Datamover IPs...
    //////////////////////////////////////////

    for(int i = 0; i < FFT2D_INSTS; ++i)
    {
       printf("Running datamover %d...\n", i);
       dmaHls[i].run();
    }

    ////////////////////////////////////////////
    //// Wait for fft_2d Kernels to complete...
    ////////////////////////////////////////////	
    //
    //for(int i = 0; i < FFT2D_INSTS; ++i)
    //{
    //   printf("Waiting for fft_2d %d to complete...\n", i);
    //   fft_2d_kr[i].waitTo_complete();
    //}

    //////////////////////////////////////////
    // Waiting for Datamover IPs
    // to complete...
    //////////////////////////////////////////
    
    for(int i = 0; i < FFT2D_INSTS; ++i)
    {
       printf("Waiting for datamover %d to complete...\n", i);
       dmaHls[i].waitTo_complete();
    }

    //////////////////////////////////////////
    // Comparing each fft_2d output with
    // Golden...
    //////////////////////////////////////////

    uint32_t errCnt = 0;
    
    for(int i = 0; i < FFT2D_INSTS; ++i)
    {
       printf("Checking Golden for FFT_2D Kernel %d...\n", i);
       dmaHls[i].golden_check(&errCnt, i);
    }

    /////////////////////////////////////////////////
    // Clean up XRT close device and
    // kernel handles...
    /////////////////////////////////////////////////

    ////Closing PL-Handles
    //for(int i = 0; i < FFT2D_INSTS; ++i)
    //{
    //   std::cout << "\nClosing FFT_2D " << i << " handles...\n";
    //   fft_2d_kr[i].close();
    //}

    //Closing PL-Handles
    for(int i = 0; i < FFT2D_INSTS; ++i)
    {
       std::cout << "\nClosing Datamover " << i << " handles...\n";
       dmaHls[i].close();
    }

    //Closing Device
    std::cout << "Closing Device...\n";
    xrtDeviceClose(dhdl);

    std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl;

    return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
}
