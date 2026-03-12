/*
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/

#include "graph.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>

#include "golden.h"
#include "mysquare.h"

//#include "experimental/xrt_aie.h"
#include "experimental/xrt_kernel.h"

#include "adf/adf_api/XRTConfig.h"


#define OUTPUT_SIZE 128



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


int main(int argc, char* argv[]) {


    size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(int);
    //////////////////////////////////////////
    // Open xclbin
    //////////////////////////////////////////

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
        return EXIT_FAILURE;
    }

    char* xclbinFilename = argv[1];


    auto dhdl = xrtDeviceOpen(0);
    //auto xclbin = load_xclbin(dhdl, "vck190_lab8.xclbin");
    auto xclbin = load_xclbin(dhdl, xclbinFilename);
    auto top = reinterpret_cast<const axlf*>(xclbin.data());





    //////////////////////////////////////////
    // output memory
    //////////////////////////////////////////

    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl,output_size_in_bytes, 0, 0);
    auto out_bomapped = reinterpret_cast<int*>(xrtBOMap(out_bohdl));
    memset(out_bomapped, 0xABCDEF00, output_size_in_bytes); // no need
    //printf("Output memory virtual addr 0x%llx\n", out_bomapped);



    //////////////////////////////////////////////
    // s2mm ip
    //////////////////////////////////////////

    xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "s2mm");
    //xrtRunHandle s2mm_rhdl = xrtKernelRun(s2mm_khdl, out_bohdl, nullptr, sizeOut);
    xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
    int rval = xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
    rval = xrtRunSetArg(s2mm_rhdl, 2, OUTPUT_SIZE);
    xrtRunStart(s2mm_rhdl);
    printf("run s2mm\n");


    //////////////////////////////////////////
    // graph execution for AIE
    //////////////////////////////////////////	

    adf::registerXRT(dhdl, top->m_header.uuid); //Grapah APIs



    int32* inputArray = (int32*)GMIO::malloc(256*sizeof(int32));




    for (int i=0; i<128; i++)
    {
        inputArray[i] = i+1;
    }


    printf("Address of inputArray  in hex format  : 0x%08x\n",&inputArray[0]);


    gr.init();
    printf("graph int completed \n");


    gr.run(4);
    printf("Run API start with iterartions of 4 \n");
    gr.gm0.gm2aie_nb(&inputArray[0], 128*sizeof(int32));

    gr.gm0.wait();
	
    printf("Wait of  gmio completed \n");


    gr.end();
    printf("graph end\n");







    //////////////////////////////////////////
    // wait for s2mm done
    //////////////////////////////////////////	

    auto state = xrtRunWait(s2mm_rhdl);
    std::cout << "s2mm completed with status(" << state << ")\n";
    xrtRunClose(s2mm_rhdl);
    xrtKernelClose(s2mm_khdl);


    #if defined(__SYNCBO_ENABLE__)
    xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE, output_size_in_bytes, 0);
    #endif
    

    //////////////////////////////////////////
    // compare
    //////////////////////////////////////////	

    //Verify the result
    int match = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        if (out_bomapped[i] != golden[i]) {
            printf("ERROR: Test failed! Error found in sample %d: golden: %d, hardware: %d\n", i, golden[i], out_bomapped[i]);
            match = 1;
            break;
        }

        /*      else
                {
                printf("PASS: Test PASS  found in sample %d: golden: %d, hardware: %d\n", i, golden[i], out_bomapped[i]);

                }  */
    }




    GMIO::free(inputArray); 



    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////	

    std::cout << "Releasing remaining XRT objects...\n";
    std::cout << "GMIO 2020.2 Native XRT Testcase...\n";

    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl; 
    return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
}
