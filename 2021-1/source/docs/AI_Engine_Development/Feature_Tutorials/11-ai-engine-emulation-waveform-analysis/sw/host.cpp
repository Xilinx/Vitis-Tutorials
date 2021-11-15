/**********
© Copyright 2021 Xilinx, Inc.
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

#define OUTPUT_SIZE 2048

using namespace adf;

int run(int argc, char* argv[]){

	size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(int);

	//TARGET_DEVICE macro needs to be passed from gcc command line
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}
	char* xclbinFilename = argv[1];
	
	int ret;
	// Open xclbin
	auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);
	auto dhdl = xrtDeviceOpenFromXcl(device);

	// s2mm & random_noise kernel handle
	auto s2mm = xrt::kernel(device, uuid, "s2mm");
	auto random_noise = xrt::kernel(device, uuid, "random_noise");

	// output memory
	auto out_bo = xrt::bo(device, output_size_in_bytes,s2mm.group_id(0));
	auto host_out=out_bo.map<std::complex<short>*>();

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, OUTPUT_SIZE);//1st run for s2mm has started
	auto random_noise_run = random_noise(nullptr, OUTPUT_SIZE);

	int coeffs_readback[12]={0,0,0,0,0,0,0,0,0,0,0,0};

	// update graph parameters (RTP) & run
	adf::registerXRT(dhdl, uuid.get());
	std::cout<<"Register XRT"<<std::endl;
	int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
	int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};
	ret=gr.update(gr.fir24.in[1], narrow_filter, 12);//update AIE kernel RTP
	if(ret){
		std::cout<<"Update fir24 error"<<std::endl;
		return 1;
	}else{
		std::cout<<"Update fir24 done"<<std::endl;
	}
	gr.run(16);//start PL kernel & AIE kernel
	ret=gr.read(gr.coefficients_readback,coeffs_readback,12);//Async read
	if(ret){
		std::cout<<"Coeffs read back error"<<std::endl;
		return 1;
	}
	std::cout<<"Coefficients read back are:";
	for(int i=0;i<12;i++){
		std::cout<<coeffs_readback[i]<<",\t";
	}
	std::cout<<std::endl;

	gr.wait(); // wait for PL kernel & AIE kernel to complete
	std::cout<<"Graph wait done"<<std::endl;
	ret=gr.read(gr.coefficients_readback,coeffs_readback,12);//read after gr.wait, gr.update has been taken effective
	if(ret){
		std::cout<<"Coeffs read back error"<<std::endl;
		return 1;
	}
	std::cout<<"Coefficients read back are:";
	for(int i=0;i<12;i++){
		std::cout<<coeffs_readback[i]<<",\t";
	}
	std::cout<<std::endl;
	ret=gr.update(gr.fir24.in[1], wide_filter, 12);//Update AIE kernel RTP
	if(ret){
		std::cout<<"Update fir24 error"<<std::endl;
		return 1;
	}else{
		std::cout<<"Update fir24 done"<<std::endl;
	}
	gr.run(16);//start PL kernel & AIE kernel
	ret=gr.read(gr.coefficients_readback,coeffs_readback,12);//Async read
	if(ret){
		std::cout<<"Coeffs read back error"<<std::endl;
		return 1;
	}
	std::cout<<"Coefficients read back are:";
	for(int i=0;i<12;i++){
		std::cout<<coeffs_readback[i]<<",\t";
	}
	std::cout<<std::endl;
	// wait for s2mm done
	auto state = s2mm_run.wait();
	std::cout << "s2mm completed with status(" << state << ")\n";
	
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

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

	gr.end();
	xrtDeviceClose(dhdl);

	return match;
}

int main(int argc, char* argv[])
{
	try {
		auto match = run(argc, argv);
		std::cout << "TEST " << (match ? "FAILED" : "PASSED") << "\n";
		return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
	}	
		catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << "\n";
		std::cout << "FAILED TEST\n";
		return 1;
	}
}
