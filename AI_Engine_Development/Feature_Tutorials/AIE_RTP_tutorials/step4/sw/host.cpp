/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "host.h"
#include <unistd.h>
#include <complex>
#include "adf/adf_api/XRTConfig.h"
#include "experimental/xrt_kernel.h"

#include "graph.cpp"

#define OUTPUT_SIZE 2048

using namespace adf;

int main(int argc, char* argv[]) {

    size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(int);

    //TARGET_DEVICE macro needs to be passed from gcc command line
    if(argc != 2) {
	std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
	return EXIT_FAILURE;
    }
    char* xclbinFilename = argv[1];
	
    int ret;
    // Open xclbin
    auto dhdl = xrtDeviceOpen(0);//device index=0
    if(!dhdl){
	printf("Device open error\n");
    }
    ret=xrtDeviceLoadXclbinFile(dhdl,xclbinFilename);
    if(ret){
	printf("Xclbin Load fail\n");
    }
    xuid_t uuid;
    xrtDeviceGetXclbinUUID(dhdl, uuid);
    
    // output memory
    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, /*BANK=*/0);
    std::complex<short> *host_out = (std::complex<short>*)xrtBOMap(out_bohdl);

    // s2mm ip
    xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
    xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
    xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
    xrtRunSetArg(s2mm_rhdl, 2, OUTPUT_SIZE);
    xrtRunStart(s2mm_rhdl);
    printf("run s2mm\n");

#if __USE_ADF_API__
    // update graph parameters (RTP) & run
    adf::registerXRT(dhdl, uuid);
    std::cout<<"Register XRT"<<std::endl;
    int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
    int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};
    gr.run(16);//start PL kernel & AIE kernel
    gr.update(gr.noisegen.in[0], 1024);//update PL kernel RTP
    std::cout<<"Update noisegen done"<<std::endl;
    gr.update(gr.fir24.in[1], narrow_filter, 12);//update AIE kernel RTP
    std::cout<<"Update fir24 done"<<std::endl;
    std::cout<<"Graph run done"<<std::endl;
    gr.wait(); // wait for PL kernel & AIE kernel to complete
    std::cout<<"Graph wait done"<<std::endl;
    gr.update(gr.noisegen.in[0], 1024);//update PL kernel RTP
    std::cout<<"Update noisegen done"<<std::endl;
    gr.update(gr.fir24.in[1], wide_filter, 12);//Update AIE kernel RTP
    std::cout<<"Update fir24 done"<<std::endl;
    gr.run(16);//start PL kernel & AIE kernel
    std::cout<<"Graph run done"<<std::endl;
#else
    int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
    int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};
    auto ghdl=xrtGraphOpen(dhdl,uuid,"gr");
    if(!ghdl){
	std::cout << "Graph Open error" << std::endl;
    }else{
	std::cout << "Graph Open ok" <<std::endl;
    }
    int size=1024;
    xrtKernelHandle noisegen_khdl = xrtPLKernelOpen(dhdl, uuid, "random_noise");
    xrtRunHandle noisegen_rhdl = xrtRunOpen(noisegen_khdl);
    xrtRunSetArg(noisegen_rhdl, 1, size);
    xrtRunStart(noisegen_rhdl);
    printf("run noisegen\n");
    ret=xrtGraphUpdateRTP(ghdl,"gr.fir24.in[1]",(char*)narrow_filter,12*sizeof(int));
    if(ret!=0){
	printf("Graph RTP update fail\n");
	return 1;
    }
    ret=xrtGraphRun(ghdl,16);
    if(ret){
	std::cout << "Graph run error" << std::endl;
    }else{
	std::cout << "Graph run ok" << std::endl;
    }
    ret=xrtGraphWait(ghdl,0);
    if(ret){
	std::cout << "Graph wait error" <<std::endl;
    }else{
	std::cout << "Graph wait ok" <<std::endl;
    }
    xrtRunWait(noisegen_rhdl);
    xrtRunSetArg(noisegen_rhdl, 1, size);
    xrtRunStart(noisegen_rhdl);
    printf("run noisegen\n");
    ret=xrtGraphUpdateRTP(ghdl,"gr.fir24.in[1]",(char*)wide_filter,12*sizeof(int));
    if(ret!=0){
	printf("Graph RTP update fail\n");
	return 1;
    }
    ret=xrtGraphRun(ghdl,16);
    if(ret){
	std::cout << "Graph run error" << std::endl;
    }else{
	std::cout << "Graph run ok" << std::endl;
    }
#endif
    // wait for s2mm done
    auto state = xrtRunWait(s2mm_rhdl);
    std::cout << "s2mm completed with status(" << state << ")\n";
	
    xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE , output_size_in_bytes,/*OFFSET=*/ 0);

    std::ofstream out("out.txt",std::ofstream::out);
    std::ifstream golden("data/filtered.txt",std::ifstream::in);
    short g_real=0,g_imag=0;
    int match = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
	golden >> std::dec >> g_real;
	golden >> std::dec >> g_imag;
	if(g_real!=host_out[i].real() || g_imag!=host_out[i].imag()){
		std::cout<<"ERROR: i="<<i<<" gold.real="<<g_real<<" gold.imag="<<g_imag<<" out.real="<<host_out[i].real()<<" out.imag="<<host_out[i].imag()<<std::endl;
		match=1;
	}
	out<<host_out[i].real()<<" "<<host_out[i].imag()<<" "<<std::endl;
    }
    out.close();
    golden.close();

#if __USE_ADF_API__
    gr.end();
#else
    ret=xrtGraphEnd(ghdl,0);
    if(ret){
	std::cout << "Graph end error" << std::endl;
    }
    xrtRunClose(noisegen_rhdl);
    xrtKernelClose(noisegen_khdl);
    xrtGraphClose(ghdl);
#endif
    xrtRunClose(s2mm_rhdl);
    xrtKernelClose(s2mm_khdl);
    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl; 
    return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
}
