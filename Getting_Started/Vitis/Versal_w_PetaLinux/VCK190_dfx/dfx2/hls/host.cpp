

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include "input.h"
#include "golden.h"
#include <cstring>

#include "experimental/xrt_kernel.h"

#include "adf/adf_api/XRTConfig.h"

#define INPUT_SIZE  128
#define OUTPUT_SIZE 256

#define NO_OF_ITERATIONS  4 

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

	//////////////////////////////////////////
	// Open xclbin
	//////////////////////////////////////////
	
    if(argc <2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}

    char* xclbinFilename = argv[1];

	
	
    xuid_t uuid;
    auto dhdl = xrtDeviceOpen(0);
    auto xclbin = load_xclbin(dhdl, xclbinFilename);//xrtDeviceLoadXclbinFile(dhdl, xclbinFilename);
    xrtDeviceGetXclbinUUID(dhdl, uuid);




//This dictates number of iterations to run through.
    long itr = NO_OF_ITERATIONS;
   
//calculate input/output data size in number of samples.
    int sizeIn = INPUT_SIZE * itr;
    int sizeOut = OUTPUT_SIZE * itr;

    size_t input_size_in_bytes = sizeIn * sizeof(int);
    size_t output_size_in_bytes = sizeOut * sizeof(int);	

    //Manage/map input/output file
   
	//////////////////////////////////////////
	// input memory
	// No cache no sync seems not working. Should ask SSW team to investigate.
	//
    ////////////////////////////////////////	
    
    xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
    auto in_bomapped = reinterpret_cast<short int*>(xrtBOMap(in_bohdl));
    memcpy(in_bomapped, cint16input, input_size_in_bytes);
    printf("Input memory virtual addr 0x%llx\n", in_bomapped);



    
    	
	//////////////////////////////////////////
	// output memory
	//////////////////////////////////////////
	
	xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
    auto out_bomapped = reinterpret_cast<int*>(xrtBOMap(out_bohdl));
	printf("Output memory virtual addr 0x%llx\n", out_bomapped);
	
	
	//////////////////////////////////////////
	// mm2s ip
	//////////////////////////////////////////
	
	xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
	xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
    int rval = xrtRunSetArg(mm2s_rhdl, 0, in_bohdl);
    rval = xrtRunSetArg(mm2s_rhdl, 2, sizeIn);
    xrtRunStart(mm2s_rhdl);
	printf("run mm2s\n");
	
	//////////////////////////////////////////
	// s2mm ip
	//////////////////////////////////////////
	
	xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
	xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
    rval = xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
    rval = xrtRunSetArg(s2mm_rhdl, 2, sizeOut);
    xrtRunStart(s2mm_rhdl);
	printf("run s2mm\n");
	
    //////////////////////////////////////////
    // polar clip ip
    //////////////////////////////////////////

    xrtKernelHandle polar_clip_khdl = xrtPLKernelOpen(dhdl, uuid, "polar_clip");
    //xrtRunHandle s2mm_rhdl = xrtKernelRun(s2mm_khdl, out_bohdl, nullptr, sizeOut);
    xrtRunHandle polar_clip_rhdl = xrtRunOpen(polar_clip_khdl);
    rval = xrtRunSetArg(polar_clip_rhdl, 2, sizeOut);
    xrtRunStart(polar_clip_rhdl);
    printf("run polar_clip\n");
	
	
	

    printf("xrtGraphOpen\n"); 
    auto ghdl = xrtGraphOpen(dhdl, uuid, "clipgraph"); 
    printf("xrtGraphRun\n"); 
    xrtGraphRun(ghdl, itr);
     
    //////////////////////////////////////////
	// wait for mm2s done
	//////////////////////////////////////////	
    auto state = xrtRunWait(mm2s_rhdl);
    std::cout << "mm2s completed with status(" << state << ")\n";
   
    //////////////////////////////////////////
    // wait for polar clip done
    //////////////////////////////////////////	
    state = xrtRunWait(polar_clip_rhdl);
    std::cout << "polar_clip completed with status(" << state << ")\n";
    
    //////////////////////////////////////////
    // wait for s2mm done
	//////////////////////////////////////////	
	
	state = xrtRunWait(s2mm_rhdl);
    std::cout << "s2mm completed with status(" << state << ")\n";

   
    xrtGraphEnd(ghdl,0);
    printf("xrtGraphEnd..\n"); 
    xrtGraphClose(ghdl);
	

    xrtRunClose(s2mm_rhdl);
    xrtKernelClose(s2mm_khdl);

    xrtRunClose(mm2s_rhdl);
    xrtKernelClose(mm2s_khdl);
  
    xrtRunClose(polar_clip_rhdl);
    xrtKernelClose(polar_clip_khdl);

	
	//////////////////////////////////////////
	// compare results
	//////////////////////////////////////////	
	
    int errCnt = 0;
    for (int i = 0; i < sizeOut; i++) {
        if (out_bomapped[i] != golden[i]) {
            printf("ERROR: Test failed! Error found in sample %d: golden: %d, hardware: %d\n", i, golden[i], out_bomapped[i]);
            errCnt++;
        }
    }

	
    //////////////////////////////////////////
	// clean up XRT
	//////////////////////////////////////////	
    
    std::cout << "Releasing remaining XRT objects...\n";
    xrtBOFree(in_bohdl);
    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);
	
    std::cout << "TEST " << (errCnt ? "FAILED" : "PASSED") << std::endl; 
    return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
}

