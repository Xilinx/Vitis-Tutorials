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

// HLS Datamover Loops and Graph will run Infinitely...
#if ITER_CNT == -1
   #define MATA_SZ -1 
   #define MATB_SZ -1
   #define MATC_SZ -1

#else
   #if GEMM_SIZE < 1024
      #define MATA_SZ (((GEMM_SIZE * GEMM_SIZE ) / CASC_LN ) / 8) * ITER_CNT
      #define MATB_SZ (((GEMM_SIZE * GEMM_SIZE ) / 32 ) / 8) * ITER_CNT * 8
      #define MATC_SZ (((GEMM_SIZE * GEMM_SIZE ) / SPLIT ) / 8) * ITER_CNT
   
   #else
      #define MATA_SZ (((GEMM_SIZE * GEMM_SIZE * 4) / CASC_LN ) / 8) * ITER_CNT
      #define MATB_SZ (((GEMM_SIZE * GEMM_SIZE ) / 32 ) / 8) * ITER_CNT * 32
      #define MATC_SZ (((GEMM_SIZE * GEMM_SIZE ) / SPLIT ) / 8) * ITER_CNT
   
   #endif
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

      int rval = xrtRunSetArg(dma_hls_rhdl, 44, MATA_SZ);
          rval = xrtRunSetArg(dma_hls_rhdl, 45, MATB_SZ);
          rval = xrtRunSetArg(dma_hls_rhdl, 46, MATC_SZ);
          //rval = xrtRunSetArg(dma_hls_rhdl, 47, ITER_CNT);
      
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
      
      //std::cout << "gemm_" << insts << std::endl;
      printf("gemm_%d ", insts);
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

class gemm_graph
{
   public:
   xrtGraphHandle gemm_aie_gr;

   int init(xrtDeviceHandle dhdl, const axlf *top, char insts)
   {
      //////////////////////////////////////////
      // GEMM Graph Init for AIE
      //////////////////////////////////////////
      
      std::string gemm_aie_obj_str = "g";

      const char *gemm_aie_obj = gemm_aie_obj_str.c_str();

      int init_st = 0;

      gemm_aie_gr = xrtGraphOpen(dhdl, top->m_header.uuid, gemm_aie_obj);
      if (!gemm_aie_gr) {
         throw std::runtime_error("Unable to open GEMM g graph handle");
	      init_st = 1;
      }
      else
      {
         printf("GEMM graph g Initialised...\n");
      }

      return init_st;
   }

   int run(void)
   {
      //////////////////////////////////////////
      // GEMM Graph Run for AIE
      //////////////////////////////////////////
      
      int init_st = 0;

      int ret = xrtGraphReset(gemm_aie_gr);
      ret = xrtGraphRun(gemm_aie_gr, GRAPH_ITER_CNT);
      if (ret) {
         throw std::runtime_error("Unable to run GEMM g graph");
         init_st = 1;
      }
      else
      {
         printf("GEMM graph g running...\n");
      }
     
      return init_st;
   }

   void close(void)
   {
      //////////////////////////////////////////
      // GEMM Graph End for AIE
      //////////////////////////////////////////
       
      xrtGraphClose(gemm_aie_gr);
      printf("GEMM graph g end...\n");

   }
};

int main(int argc, char ** argv)
{
   //////////////////////////////////////////
   // Open xclbin
   //////////////////////////////////////////

   if(argc < 2) {
      std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
      return EXIT_FAILURE;
   }

   else {
      //If argc is 2 it loads xclbin(Normal Flow)
      //If argc is 3 and argv[2] is LOAD_XCLBIN ,it loads xclbin(To get POWER values)
      if(argc==2 || (argc==3 && strcmp(argv[2],"LOAD_XCLBIN")==0)) {

         const char* xclbinFilename = argv[1];
         auto dhdl = xrtDeviceOpen(0);
         auto xclbin = load_xclbin(dhdl, xclbinFilename);
         auto top = reinterpret_cast<const axlf*>(xclbin.data());
      }
      
      //If argc is 2 it runs design for finite iterations (Normal Flow)
      //If argc is 3 and argv[2] is RUN_CODE ,it runs design for infinite iterations(To get POWER values)
      if(argc==2 || (argc==3 && strcmp(argv[2],"RUN_CODE")==0)) {

         const char* xclbinFilename = argv[1];
         auto dhdl = xrtDeviceOpen(0);
         auto xclbin = load_xclbin(dhdl, xclbinFilename);
         auto top = reinterpret_cast<const axlf*>(xclbin.data());         

         //////////////////////////////////////////
         // Data-Mover IP Objects...
         //////////////////////////////////////////
   
         datamover dmaHls[GEMM_INSTS];
   
         //////////////////////////////////////////
         // GEMM Graph Objects...
         //////////////////////////////////////////
   
         gemm_graph gemm_gr[GEMM_INSTS];
   
         //////////////////////////////////////////
         // Initialising DataMover Units...
         //////////////////////////////////////////
   
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            printf("Initialising datamover %d...\n", i);
            dmaHls[i].init(dhdl, top, i);
         }
   
         //////////////////////////////////////////
         // Initialising GEMM Graphs...
         //////////////////////////////////////////
   
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            printf("Initialising GEMM Graph %d...\n", i);
            int ret = gemm_gr[i].init(dhdl, top, i);
            
            if (ret != 0)
               return ret;
         }
   
         //////////////////////////////////////////
         // Running GEMM Graphs...
         //////////////////////////////////////////
   
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            printf("Running GEMM Graph %d...\n", i);
            int ret = gemm_gr[i].run();
            
            if (ret != 0)
               return ret;
         }
   
         //////////////////////////////////////////
         // Running datamovers in each kernel for
         // ITER_CNT times...
         //////////////////////////////////////////
   
         printf("Running datamovers for %d Iterations...\n", ITER_CNT);
   
         //////////////////////////////////////////
         // Running GeMM datamover IPs...
         //////////////////////////////////////////
   
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            printf("Running datamover %d...\n", i);
            dmaHls[i].run();
         }
   
         //////////////////////////////////////////
         // Waiting for Datamover IPs
         // to complete...
         //////////////////////////////////////////
         
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            printf("Waiting for datamover %d to complete...\n", i);
            dmaHls[i].waitTo_complete();
         }
   
         //////////////////////////////////////////
         // Comparing each gemm output with
         // Golden...
         //////////////////////////////////////////
   
         uint32_t errCnt = 0;
         
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            printf("Checking Golden for GEMM Kernel %d...\n", i);
            dmaHls[i].golden_check(&errCnt, i);
         }
   
         /////////////////////////////////////////////////
         // Clean up XRT, close device, pl-kernels
         // and graph handles...
         /////////////////////////////////////////////////
   
         //Closing PL-Handles
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            std::cout << "\nClosing Datamover " << i << " handles...\n";
            dmaHls[i].close();
         }
   
         //Closing Graphs
         for(int i = 0; i < GEMM_INSTS; ++i)
         {
            std::cout << "Closing GEMM Graph " << i << "...\n";
            gemm_gr[i].close();
            printf("GEMM Graph %d end.\n",i);
         }
   
         //Closing Device
         xrtDeviceClose(dhdl);
   
         //Final Result
         std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl;
   
         //Exit with result
         return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
      }
   }
}

