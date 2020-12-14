/* (c) Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>

#include "input.h"
#include "golden.h"

#include "graph.cpp"

#include "xrt/experimental/xrt_aie.h"
#include "xrt/experimental/xrt_kernel.h"
#include "xrt/experimental/xrt_bo.h"

#include "adf/adf_api/XRTConfig.h"

//Control Offsets for DMA HLS
#define MEM_RD_OFFSET	  0x10
#define MEM_WR_OFFSET	  0x1C
#define SIZE_RD_OFFSET	  0x28
#define SIZE_WR_OFFSET	  0x30
#define CTRL_OFFSET		  0x00

//Data
#define NO_IMAGES		     100
#define OUT_IMAGE_LENGTH  32
#define WORD_LENGTH		2

#define INPUT_SIZE		  (0x80 * NO_IMAGES)
#define OUTPUT_SIZE		  (0x10 * NO_IMAGES)
//
//#define INPUT_SIZE  12800
//#define OUTPUT_SIZE  1600

// load_xclbin function is used to read in xclbin file
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


int main(int argc, char ** argv)
{
   size_t input_size_in_bytes = INPUT_SIZE * sizeof(uint32_t) * WORD_LENGTH;
   size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(uint32_t) * WORD_LENGTH;

   //////////////////////////////////////////
   // Open xclbin
   //////////////////////////////////////////

   if(argc != 2) {
      std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
      return EXIT_FAILURE;
   }

   char* xclbinFilename = argv[1];
   //open a device and obtain its handle
   auto dhdl = xrtDeviceOpen(0);

   //Download FPGA image (xclbin) to the device
   auto xclbin = load_xclbin(dhdl, xclbinFilename);

   //point to top of xclbin data
   auto top = reinterpret_cast<const axlf*>(xclbin.data());


   //Allocate BOs (buffer objects) of requested size with appropriate flags
   //Memory map BOs into user's address space (DDR Memory)
   xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
   auto in_bomapped = reinterpret_cast<uint32_t*>(xrtBOMap(in_bohdl));

   //Set the input mapped region needs to have the same data as the input_pl. 
   memcpy(in_bomapped, input_pl, input_size_in_bytes);
   printf("Input memory virtual addr 0x%llx\n", in_bomapped);


   xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
   auto out_bomapped = reinterpret_cast<uint32_t*>(xrtBOMap(out_bohdl));
   printf("Output memory virtual addr 0x%llx\n", out_bomapped);
   

   //open PL kernels and obtain handles
   xrtKernelHandle dmahls_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma_hls:{dma_hls}");


   //Create a kernel handle to start DMA HLS pl kernel
   //Set the DMA HLS arguments (input buffer handle, output
   //buffer handle, input size and output size) 
   // 0,1,4,5 are port number on the IP.
   xrtRunHandle dmahls_rhdl = xrtRunOpen(dmahls_khdl); 
   int rval = xrtRunSetArg(dmahls_rhdl, 0, in_bohdl);
   rval     = xrtRunSetArg(dmahls_rhdl, 1, out_bohdl);
   rval     = xrtRunSetArg(dmahls_rhdl, 4, INPUT_SIZE);
   rval     = xrtRunSetArg(dmahls_rhdl, 5, OUTPUT_SIZE);

   //Start the DMA HLS kernel
   //Moving data from DDR to AI Engine  
   xrtRunStart(dmahls_rhdl);
   printf("Run Datamover\n");

   //////////////////////////////////////////
   // graph execution for AIE
   //////////////////////////////////////////	

   adf::registerXRT(dhdl, top->m_header.uuid);

   auto graphHandle = xrtGraphOpen(dhdl, top->m_header.uuid, "g");
   if (!graphHandle) {
      throw std::runtime_error("Unable to open graph handle");
	   return 1;
   }

   printf("graph run\n");    
   int ret = xrtGraphReset(graphHandle);
   ret = xrtGraphRun(graphHandle,100);
   if (ret) {
      throw std::runtime_error("Unable to run graph");
	   
      return 1;
   }

   printf("graph running..\n");

   //Wait for DMA HLS execution to finish
   printf("Waiting for dma hls to complete...\n");
   auto state = xrtRunWait(dmahls_rhdl);
   std::cout << "Datamover completed with status(" << state << ")\n";

   //Close the run handles obtained by in xrtRunOpen() 
   xrtRunClose(dmahls_rhdl);

   //Close opened kernel handles obtained from xrtPLKernelOpen()
   xrtKernelClose(dmahls_khdl);
   
   printf("Closed dma hls kernel...\n");

   //Compare data in out_bomapped to golden data in golden.h
   int errCnt = 0;
   int errFlag = 0;
   int out_count = 0;
   int image_count = 0;

   printf("Comparing output with golden\n");
   for(int i = 0; i < OUTPUT_SIZE * WORD_LENGTH; i++) {
       if( out_bomapped[i] == int32golden[i] ) {
           errFlag = errFlag || 0;
           out_count++;
       } else {
           errFlag = errFlag || 1;
           printf("Error found in sample 0x%X != to the golden 0x%X\n", out_bomapped[i], int32golden[i] );
           errCnt++;
       }
       if ( (out_count == OUT_IMAGE_LENGTH) ) {
           if (!errFlag) {
               printf("Pass for image %d \n", image_count);
                   image_count++;
                   out_count = 0;
                   errFlag = 0;
           } else {
               printf("Fail for image %d \n", image_count);
               image_count++;
               out_count = 0;
               errFlag = 0;
           }
       }
   }


   //Release allocated resources
   std::cout << "Releasing remaining XRT objects...\n";
   xrtBOFree(in_bohdl);
   xrtBOFree(out_bohdl);

   xrtGraphClose(graphHandle);
   printf("graph end\n");

   xrtDeviceClose(dhdl);

   std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl;

   return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
}
