// © Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "graph.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>

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

   void init(xrtDeviceHandle dhdl, const axlf *top, char insts)
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
          rval = xrtRunSetArg(dma_hls_rhdl, 7, ITER_CNT);
      
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

class fft2d_graph
{
   public:
   xrtGraphHandle fft_rows_gr, fft_cols_gr;

   int init(xrtDeviceHandle dhdl, const axlf *top, char insts)
   {
      //////////////////////////////////////////
      // 2D-FFT Graph Init for AIE
      //////////////////////////////////////////
      
      std::string fft_rows_obj_str = "fft_rows[" + to_string(insts) + "]";
      std::string fft_cols_obj_str = "fft_cols[" + to_string(insts) + "]";

      const char *fft_rows_obj = fft_rows_obj_str.c_str();
      const char *fft_cols_obj = fft_cols_obj_str.c_str();

      int init_st = 0;

      fft_rows_gr = xrtGraphOpen(dhdl, top->m_header.uuid, fft_rows_obj);
      if (!fft_rows_gr) {
         throw std::runtime_error("Unable to open FFT Rows graph handle");
	      init_st = 1;
      }
      else
      {
         printf("FFT Rows graph Initialised...\n");
      }

      fft_cols_gr = xrtGraphOpen(dhdl, top->m_header.uuid, fft_cols_obj);
      if (!fft_cols_gr) {
         throw std::runtime_error("Unable to open FFT Cols graph handle");
	      init_st = 1;
      }
      else
      {
         printf("FFT Cols graph Initialised...\n");
      }

      return init_st;
   }

   int run(void)
   {
      //////////////////////////////////////////
      // 2D-FFT Graph Run for AIE
      //////////////////////////////////////////
      
      int init_st = 0;

      int ret = xrtGraphReset(fft_rows_gr);
      ret = xrtGraphRun(fft_rows_gr, ITER_CNT * MAT_ROWS);
      if (ret) {
         throw std::runtime_error("Unable to run FFT Rows graph");
         init_st = 1;
      }
      else
      {
         printf("FFT Rows graph running...\n");
      }
       
      ret = xrtGraphReset(fft_cols_gr);
      ///////////////////////////////////////////
      // Due to increased ping-pong buffer size,
      // same no. iterations as rowise fft gr...
      ret = xrtGraphRun(fft_cols_gr, ITER_CNT * MAT_ROWS);
      if (ret) {
         throw std::runtime_error("Unable to run FFT Cols graph");
         init_st = 1;
      }
      else
      {
         printf("FFT Cols graph running...\n");
      }

      return init_st;
   }

   void close(void)
   {
      //////////////////////////////////////////
      // 2D-FFT Graph End for AIE
      //////////////////////////////////////////
       
      xrtGraphClose(fft_rows_gr);
      printf("FFT Rows graph end...\n");

      xrtGraphClose(fft_cols_gr);
      printf("FFT Cols graph end...\n");
   }
};

int main(int argc, char ** argv)
{
   //////////////////////////////////////////
   // Open xclbin
   //////////////////////////////////////////

   if(argc != 2) {
      std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
      return EXIT_FAILURE;
   }

   char* xclbinFilename = argv[1];

   auto dhdl = xrtDeviceOpen(0);
   auto xclbin = load_xclbin(dhdl, xclbinFilename);
   auto top = reinterpret_cast<const axlf*>(xclbin.data());

   //////////////////////////////////////////
   // Data-Mover IP Objects...
   //////////////////////////////////////////

   datamover dmaHls[FFT2D_INSTS];

   //////////////////////////////////////////
   // FFT 2D Graph Objects...
   //////////////////////////////////////////

   fft2d_graph fft2d_gr[FFT2D_INSTS];

   //////////////////////////////////////////
   // Initialising DataMover Units...
   //////////////////////////////////////////

   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      printf("Initialising datamover %d...\n", i);
      dmaHls[i].init(dhdl, top, i);
   }

   //////////////////////////////////////////
   // Initialising 2D-FFT Graphs...
   //////////////////////////////////////////

   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      printf("Initialising 2D-FFT Graph %d...\n", i);
      int ret = fft2d_gr[i].init(dhdl, top, i);
      
      if (ret != 0)
         return ret;
   }

   //////////////////////////////////////////
   // Running 2D-FFT Graphs...
   //////////////////////////////////////////

   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      printf("Running 2D-FFT Graph %d...\n", i);
      int ret = fft2d_gr[i].run();
      
      if (ret != 0)
         return ret;
   }

   //////////////////////////////////////////
   // Running datamovers in each kernel for
   // ITER_CNT times...
   //////////////////////////////////////////

   printf("Running datamovers for %d Iterations...\n", ITER_CNT);

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
   for(int i = 0; i < FFT2D_INSTS; ++i)
   {
      std::cout << "Closing 2D-FFT Graph " << i << "...\n";
      fft2d_gr[i].close();
      printf("2D-FFT Graph %d end.\n",i);
   }

   //Closing Device
   xrtDeviceClose(dhdl);

   //Final Result
   std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl;

   //Exit with result
   return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
}
