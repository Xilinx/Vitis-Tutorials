/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "adf/adf_api/XRTConfig.h"

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

#elif FFT_2D_DT ==  1
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

class fft2d_hostapp_graph
{
   public:
   xrtGraphHandle fft2d_graph_gr;

   int init(xrtDeviceHandle dhdl, const axlf *top, char insts)
   {
      //////////////////////////////////////////
      // 2D-FFT Graph Init for AIE
      //////////////////////////////////////////
      
      std::string fft2d_graph_obj_str = "fft2d_graph";

      const char *fft2d_graph_obj = fft2d_graph_obj_str.c_str();

      int init_st = 0;

      fft2d_graph_gr = xrtGraphOpen(dhdl, top->m_header.uuid, fft2d_graph_obj);
      if (!fft2d_graph_gr) {
         throw std::runtime_error("Unable to open FFT graph handle");
	      init_st = 1;
      }
      else
      {
         printf("FFT graph Initialised...\n");
      }

      return init_st;
   }

   int run(int16_t graph_iter_cnt)
   {
      //////////////////////////////////////////
      // 2D-FFT Graph Run for AIE
      //////////////////////////////////////////
      
      int init_st = 0;

      int ret = xrtGraphReset(fft2d_graph_gr);
      ret = xrtGraphRun(fft2d_graph_gr, graph_iter_cnt);
      if (ret) {
         throw std::runtime_error("Unable to run FFT  graph");
         init_st = 1;
      }
      else
      {
         printf("FFT graph running...\n");
      }
       
      return init_st;
   }

   void close(void)
   {
      //////////////////////////////////////////
      // 2D-FFT Graph End for AIE
      //////////////////////////////////////////
       
      xrtGraphClose(fft2d_graph_gr);
      printf("FFT graph end...\n");

   }
};

int main(int argc, char ** argv)
{
   //////////////////////////////////////////
   // Open xclbin
   //////////////////////////////////////////
   
   if((argc < 2) || (argc > 3)) {
      std::cout << "Usage: " << argv[0] <<" <xclbin>" << " iteration count(optional)" << std::endl;
      return EXIT_FAILURE;
   }
   
   char* xclbinFilename = argv[1];
   int16_t iterCnt;
   int16_t graph_itercnt = 0;

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
   
   if (iterCnt == -1)
      graph_itercnt = iterCnt;
   else
      graph_itercnt = (iterCnt * MAT_ROWS);
   printf("AIE_GRAPH_ITER_CNT........%d\n",graph_itercnt);

   //////////////////////////////////////////
   // Data-Mover IP Objects...
   //////////////////////////////////////////
   
   datamover dmaHls[FFT2D_INSTS];
   
   //////////////////////////////////////////
   // FFT 2D Graph Objects...
   //////////////////////////////////////////
   
   fft2d_hostapp_graph fft2d_gr;
   
   //////////////////////////////////////////
   // Initialising DataMover Units...
   //////////////////////////////////////////
   
   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      printf("Initialising datamover %d...\n", i);
      dmaHls[i].init(dhdl, top, i, iterCnt);
   }
   
   //////////////////////////////////////////
   // Initialising 2D-FFT Graphs...
   //////////////////////////////////////////
   
   printf("Initialising 2D-FFT Graph %d...\n", 0);
   int ret = fft2d_gr.init(dhdl, top, 0);
   
   if (ret != 0)
      return ret;
   
   //////////////////////////////////////////
   // Running 2D-FFT Graphs...
   //////////////////////////////////////////
   
   printf("Running 2D-FFT Graph %d...\n", 0);
   ret = fft2d_gr.run(graph_itercnt);
   
   if (ret != 0)
      return ret;
   
   //////////////////////////////////////////
   // Running datamovers in each kernel for
   // ITER_CNT times...
   //////////////////////////////////////////
   
   printf("Running datamovers for %d Iterations...\n", iterCnt);
   
   //////////////////////////////////////////
   // Running for 2kpt s2mm/mm2s IPs...
   //////////////////////////////////////////
   
   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      printf("Running datamover %d...\n", i);
      dmaHls[i].run();
   }
   
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
   // Clean up XRT, close device, pl-kernels
   // and graph handles...
   /////////////////////////////////////////////////
   
   //Closing PL-Handles
   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      std::cout << "\nClosing Datamover " << i << " handles...\n";
      dmaHls[i].close();
   }
   
   //Closing Graphs
   std::cout << "Closing 2D-FFT Graph " << 0 << "...\n";
   fft2d_gr.close();
   printf("2D-FFT Graph %d end.\n",0);
   
   
   //Closing Device
   xrtDeviceClose(dhdl);
   
   //Final Result
   std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl;
   
   //Exit with result
   return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
}
