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
#include "host.h"
#include "graph.h"
#include <unistd.h>
#include <complex>
#include "adf/adf_api/XRTConfig.h"
#include "experimental/xrt_kernel.h"

#define OUTPUT_SIZE 512

using namespace adf;

ddsgraph gr;

int main(int argc, char* argv[]) {
    size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(int);

    //TARGET_DEVICE macro needs to be passed from gcc command line
    if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
    }
    char* xclbinFilename = argv[1];
    
    std::vector<cl::Device> devices;
    cl::Device device;
    std::vector<cl::Platform> platforms;
    bool found_device = false;

    //traversing all Platforms To find Xilinx Platform and targeted
    //Device in Xilinx Platform
    cl::Platform::get(&platforms);
    for(size_t i = 0; (i < platforms.size() ) & (found_device == false) ;i++){
        cl::Platform platform = platforms[i];
        std::string platformName = platform.getInfo<CL_PLATFORM_NAME>();
        if ( platformName == "Xilinx"){
            devices.clear();
            platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
	    if (devices.size()){
		    device = devices[0];
		    found_device = true;
		    break;
	    }
        }
    }
    if (found_device == false){
       std::cout << "Error: Unable to find Target Device " 
           << device.getInfo<CL_DEVICE_NAME>() << std::endl;
       return EXIT_FAILURE; 
    }

    // Creating Context and Command Queue for selected device
    cl::Context context(device);
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

    // Load xclbin 
    std::cout << "Loading: '" << xclbinFilename << "'\n";
    std::ifstream bin_file(xclbinFilename, std::ifstream::binary);
    bin_file.seekg (0, bin_file.end);
    unsigned nb = bin_file.tellg();
    bin_file.seekg (0, bin_file.beg);
    char *buf = new char [nb];
    bin_file.read(buf, nb);

    // Creating Program from Binary File
    cl::Program::Binaries bins;
    bins.push_back({buf,nb});
    devices.resize(1);
    cl::Program program(context, devices, bins);
    
    // This call will get the kernel object from program. A kernel is an 
    // OpenCL function that is executed on the FPGA. 
    cl::Kernel krnl_s2mm(program,"s2mm");
    
    // Create XRT device handle for ADF API
    void *dh;
    device.getInfo(CL_DEVICE_HANDLE, &dh);
    auto dhdl = xrtDeviceOpenFromXcl(dh);
    xuid_t uuid;
    xrtDeviceGetXclbinUUID(dhdl, uuid);
    adf::registerXRT(dhdl, uuid);
    
    std::complex<short> *host_out; //host buffer
    // These commands will allocate memory on the Device. The cl::Buffer objects can
    // be used to reference the memory locations on the device. 
    cl::Buffer buffer_out(context, CL_MEM_WRITE_ONLY, output_size_in_bytes);
    host_out=(std::complex<short>*)q.enqueueMapBuffer(buffer_out,true,CL_MAP_READ,0,sizeof(int)*OUTPUT_SIZE,nullptr,nullptr,nullptr);
 
    krnl_s2mm.setArg(0,buffer_out);
    krnl_s2mm.setArg(2,OUTPUT_SIZE);

    //Launch the Kernel
    q.enqueueTask(krnl_s2mm);

    // Initialize, run and update graph parameters (RTP)
    gr.run(2);
    gr.update(gr.trigger,10);
    gr.wait();
    gr.update(gr.trigger,100);
    gr.run(2);
    gr.end();

    
    q.finish();//Wait for s2mm to complete    
    q.enqueueMigrateMemObjects({buffer_out},CL_MIGRATE_MEM_OBJECT_HOST);

    q.finish();
	
    std::ofstream out("out.txt",std::ofstream::out);
    std::ifstream golden("data/golden.txt",std::ifstream::in);
    short g_real=0,g_imag=0;
    int match = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
	golden >> std::dec >> g_real;
	golden >> std::dec >> g_imag;
	if(g_real!=host_out[i].real() || g_imag!=host_out[i].imag()){
		match=1;
	}
	out<<host_out[i].real()<<" "<<host_out[i].imag()<<std::endl;
    }
    out.close();
    golden.close();

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl; 
    return (match ? EXIT_FAILURE :  EXIT_SUCCESS);

}
