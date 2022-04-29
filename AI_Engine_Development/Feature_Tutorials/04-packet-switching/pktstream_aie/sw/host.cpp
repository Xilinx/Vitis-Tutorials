/**********
© Copyright 2020-2022 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <complex>
#include "adf/adf_api/XRTConfig.h"
#include "experimental/xrt_kernel.h"

#include "graph.cpp"

using namespace adf;
using namespace std;

int main(int argc, char* argv[]) {
	int packet_num=2;
	int total_packet_num=2*4;
	int mem_size=packet_num*32;

	if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
    	}
    	char* xclbinFilename = argv[1];
	
	int ret=0;
	int match=0;
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
	xrtBufferHandle out_bo1 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	xrtBufferHandle out_bo2 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	xrtBufferHandle out_bo3 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	xrtBufferHandle out_bo4 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	int *host_out1 = (int*)xrtBOMap(out_bo1);
	int *host_out2 = (int*)xrtBOMap(out_bo2);
	int *host_out3 = (int*)xrtBOMap(out_bo3);
	int *host_out4 = (int*)xrtBOMap(out_bo4);
	
	// input memory
	xrtBufferHandle in_bo1 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	xrtBufferHandle in_bo2 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	xrtBufferHandle in_bo3 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	xrtBufferHandle in_bo4 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
	int *host_in1 = (int*)xrtBOMap(in_bo1);
	int *host_in2 = (int*)xrtBOMap(in_bo2);
	int *host_in3 = (int*)xrtBOMap(in_bo3);
	int *host_in4 = (int*)xrtBOMap(in_bo4);

	std::cout<<" memory allocation complete"<<std::endl;
	// initialize input memory
	for(int i=0;i<mem_size/sizeof(int);i++){
		*(host_in1+i)=i;
		*(host_in2+i)=2*i;
		*(host_in3+i)=3*i;
		*(host_in4+i)=4*i;
	}
	
	// start output kernels
	xrtKernelHandle s2mm_k1 = xrtPLKernelOpen(dhdl, uuid, "s2mm:{s2mm_1}");
	xrtRunHandle s2mm_r1 = xrtRunOpen(s2mm_k1);
	xrtRunSetArg(s2mm_r1, 0, out_bo1);
	xrtRunSetArg(s2mm_r1, 2, mem_size/sizeof(int));
	xrtRunStart(s2mm_r1);
	xrtKernelHandle s2mm_k2 = xrtPLKernelOpen(dhdl, uuid, "s2mm:{s2mm_2}");
	xrtRunHandle s2mm_r2 = xrtRunOpen(s2mm_k2);
	xrtRunSetArg(s2mm_r2, 0, out_bo2);
	xrtRunSetArg(s2mm_r2, 2, mem_size/sizeof(int));
	xrtRunStart(s2mm_r2);
	xrtKernelHandle s2mm_k3 = xrtPLKernelOpen(dhdl, uuid, "s2mm:{s2mm_3}");
	xrtRunHandle s2mm_r3 = xrtRunOpen(s2mm_k3);
	xrtRunSetArg(s2mm_r3, 0, out_bo3);
	xrtRunSetArg(s2mm_r3, 2, mem_size/sizeof(int));
	xrtRunStart(s2mm_r3);
	xrtKernelHandle s2mm_k4 = xrtPLKernelOpen(dhdl, uuid, "s2mm:{s2mm_4}");
	xrtRunHandle s2mm_r4 = xrtRunOpen(s2mm_k4);
	xrtRunSetArg(s2mm_r4, 0, out_bo4);
	xrtRunSetArg(s2mm_r4, 2, mem_size/sizeof(int));
	xrtRunStart(s2mm_r4);
	xrtKernelHandle hls_packet_receiver_k = xrtPLKernelOpen(dhdl, uuid, "hls_packet_receiver");
	xrtRunHandle hls_packet_receiver_r = xrtRunOpen(hls_packet_receiver_k);
	xrtRunSetArg(hls_packet_receiver_r, 5, total_packet_num);
	xrtRunStart(hls_packet_receiver_r);
	std::cout<<" output kernel complete"<<std::endl;

	// start input kernels
	xrtKernelHandle mm2s_k1 = xrtPLKernelOpen(dhdl, uuid, "mm2s:{mm2s_1}");
	xrtRunHandle mm2s_r1 = xrtRunOpen(mm2s_k1);
	xrtRunSetArg(mm2s_r1, 0, in_bo1);
	xrtRunSetArg(mm2s_r1, 2, mem_size/sizeof(int));
	xrtRunStart(mm2s_r1);
	xrtKernelHandle mm2s_k2 = xrtPLKernelOpen(dhdl, uuid, "mm2s:{mm2s_2}");
	xrtRunHandle mm2s_r2 = xrtRunOpen(mm2s_k2);
	xrtRunSetArg(mm2s_r2, 0, in_bo2);
	xrtRunSetArg(mm2s_r2, 2, mem_size/sizeof(int));
	xrtRunStart(mm2s_r2);
	xrtKernelHandle mm2s_k3 = xrtPLKernelOpen(dhdl, uuid, "mm2s:{mm2s_3}");
	xrtRunHandle mm2s_r3 = xrtRunOpen(mm2s_k3);
	xrtRunSetArg(mm2s_r3, 0, in_bo3);
	xrtRunSetArg(mm2s_r3, 2, mem_size/sizeof(int));
	xrtRunStart(mm2s_r3);
	xrtKernelHandle mm2s_k4 = xrtPLKernelOpen(dhdl, uuid, "mm2s:{mm2s_4}");
	xrtRunHandle mm2s_r4 = xrtRunOpen(mm2s_k4);
	xrtRunSetArg(mm2s_r4, 0, in_bo4);
	xrtRunSetArg(mm2s_r4, 2, mem_size/sizeof(int));
	xrtRunStart(mm2s_r4);
	xrtKernelHandle hls_packet_sender_k = xrtPLKernelOpen(dhdl, uuid, "hls_packet_sender");
	xrtRunHandle hls_packet_sender_r = xrtRunOpen(hls_packet_sender_k);
	xrtRunSetArg(hls_packet_sender_r, 5, packet_num);
	xrtRunStart(hls_packet_sender_r);
	std::cout<<" input kernel complete"<<std::endl;

	// start graph
	adf::registerXRT(dhdl, uuid);
	gr.run(2);
	std::cout<<" graph run complete"<<std::endl;

	// wait for s2mm to complete
	xrtRunWait(s2mm_r1);
	xrtRunWait(s2mm_r2);
	xrtRunWait(s2mm_r3);
	xrtRunWait(s2mm_r4);
	std::cout<<" s2mm wait complete"<<std::endl;

	// post-processing data;
	for(int i=0;i<mem_size/sizeof(int);i++){	
		if(*(host_out1+i)!=*(host_in1+i)+1){
			match=1;
			std::cout<<"host_out1["<<i<<"]="<<host_out1[i]<<std::endl;
		}
		if(*(host_out2+i)!=*(host_in2+i)+2){
			match=1;
			std::cout<<"host_out2["<<i<<"]="<<host_out2[i]<<std::endl;
		}
		if(*(host_out3+i)!=*(host_in3+i)+3){
			match=1;
			std::cout<<"host_out3["<<i<<"]="<<host_out3[i]<<std::endl;
		}
		if(*(host_out4+i)!=*(host_in4+i)+4){
			match=1;
			std::cout<<"host_out4["<<i<<"]="<<host_out4[i]<<std::endl;
		}
	}

	// release memory
	xrtRunClose(s2mm_r1);
	xrtRunClose(s2mm_r2);
	xrtRunClose(s2mm_r3);
	xrtRunClose(s2mm_r4);
	xrtRunClose(hls_packet_receiver_r);
	xrtKernelClose(s2mm_k1);
	xrtKernelClose(s2mm_k2);
	xrtKernelClose(s2mm_k3);
	xrtKernelClose(s2mm_k4);
	xrtKernelClose(hls_packet_receiver_k);
	xrtRunClose(mm2s_r1);
	xrtRunClose(mm2s_r2);
	xrtRunClose(mm2s_r3);
	xrtRunClose(mm2s_r4);
	xrtRunClose(hls_packet_sender_r);
	xrtKernelClose(mm2s_k1);
	xrtKernelClose(mm2s_k2);
	xrtKernelClose(mm2s_k3);
	xrtKernelClose(mm2s_k4);
	xrtKernelClose(hls_packet_sender_k);
	xrtBOFree(out_bo1);
	xrtBOFree(out_bo2);
	xrtBOFree(out_bo3);
	xrtBOFree(out_bo4);
	xrtBOFree(in_bo1);
	xrtBOFree(in_bo2);
	xrtBOFree(in_bo3);
	xrtBOFree(in_bo4);
	gr.end();
	xrtDeviceClose(dhdl);
	
	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl; 
	return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
}
