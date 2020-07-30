/*******************************************************************************
Vendor: Xilinx
Associated Filename: vadd.cpp
Purpose: VITIS vector addition

*******************************************************************************
Copyright (C) 2019 XILINX, Inc.

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law:
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising under
or in connection with these materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage (including loss of data,
profits, goodwill, or any type of loss or damage suffered as a result of any
action brought by a third party) even if such damage or loss was reasonably
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES.

*******************************************************************************/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "dct_top.h"
#include "dct.h"


static const std::string error_message =
    "Error: Result mismatch:\n"
    "i = %d CPU result = %d Device result = %d\n";

int main(int argc, char* argv[]) {

    //int arr_in[N], arr_out[N];
    int retval = 0, i;
    short tmp;
    FILE *fp;

    std::cout << "Current directory: " << system("pwd") << " \n";

    //TARGET_DEVICE macro needs to be passed from gcc command line
    if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}

    char* xclbinFilename = argv[1];
    
    // Compute the size of array in bytes
    //size_t size_in_bytes = DATA_SIZE * sizeof(int);
    size_t size_in_bytes = N * sizeof(short);
    std::cout << " size_in_bytes = '" << size_in_bytes << "'\n";
    std::cout << " sizeof(int) = '" << sizeof(int) << "'\n";

    
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
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE);

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
    cl::Kernel krnl_dct(program,"dct");

    // These commands will allocate memory on the Device. The cl::Buffer objects can
    // be used to reference the memory locations on the device. 
    cl::Buffer buffer_in(context, CL_MEM_READ_ONLY, size_in_bytes);
    cl::Buffer buffer_out(context, CL_MEM_WRITE_ONLY, size_in_bytes);

    //We then need to map our OpenCL buffers to get the pointers
    short *arr_in = (short *) q.enqueueMapBuffer (buffer_in , CL_TRUE , CL_MAP_WRITE , 0, size_in_bytes);
    short *arr_out = (short *) q.enqueueMapBuffer (buffer_out , CL_TRUE , CL_MAP_READ , 0, size_in_bytes);

    // Read input file into arr_in[], close file
    fp=fopen("/home/randyh/hls_lab_host_code/in.dat","r");
    if (fp == NULL) {
    	printf("Could not find in.dat. \n");
        retval = 1;
    } else {
    std::cout << "Reading '" << fp << "'\n";
    for (i=0; i<N; i++){
        fscanf(fp, "%d", &tmp);
        //std::cout << "Value '" << tmp << "' read from input. \n";
        //ptr_in[i] = tmp;
        arr_in[i] = tmp;
        std::cout << "Input array index " << i << " has value " << arr_in[i] << " \n";
    }
    fclose(fp);
    }

    //set the kernel Arguments
    int narg=0;
    krnl_dct.setArg(narg++, buffer_in);
    krnl_dct.setArg(narg++, buffer_out);

    // Migrate data to kernel space
    q.enqueueMigrateMemObjects({buffer_in},0/* 0 means from host*/);
    //q.enqueueWriteBuffer(buffer_in,CL_TRUE,0,size_in_bytes,arr_in);

    //Launch the Kernel
    q.enqueueTask(krnl_dct);

    // Migrate FPGA data back to host
    q.enqueueMigrateMemObjects({buffer_out},CL_MIGRATE_MEM_OBJECT_HOST);
    q.flush();
    q.finish();


    // Generate Output file
    fp=fopen("/home/randyh/hls_lab_host_code/out.dat","w");
    if (fp == NULL) {
    	printf("Could not create out.dat. \n");
        retval = 1;
    } else {
        std::cout << "Writing '" << fp << "'\n";
		for (i=0; i<N; i++) {
			fprintf(fp, "%d \n", arr_out[i]);
	        std::cout << "Output array index " << i << " has value " << arr_out[i] << " \n";
		}
		fclose(fp);
    }

    // Compare the results file with the golden results
    retval = system("diff --brief -w /home/randyh/hls_lab_host_code/out.dat /home/randyh/hls_lab_host_code/out.golden.dat");
    //retval = system("diff --brief -w /home/randyh/hls_lab_host_code/out.dat /home/randyh/hls_lab_host_code/in.dat");
    if (retval != 0) {
	 printf("Test failed  !!!\n");
	 retval=1;
    } else {
	 printf("Test passed !\n");
    }
    //return retval;

/*
    //Compare arr_in[i] to arr_out[i] and print results
    for (i=0; i<N; i++){
    	if (arr_in[i] == arr_out[i]) {
    		printf("Match at line %d: %d = %d \n", i, arr_in[i], arr_out[i]);
    	} else {
    		printf("No match at line %d: %d != %d \n", i, arr_in[i], arr_out[i]);
    		retval=1;
    	}
    }//End FOR
*/

    // Free memory and garbage collect
    q.enqueueUnmapMemObject(buffer_in , arr_in);
    q.enqueueUnmapMemObject(buffer_out , arr_out);
    q.finish();

    std::cout << "TEST " << (retval ? "FAILED" : "PASSED") << std::endl;
    return (retval ? EXIT_FAILURE :  EXIT_SUCCESS);

}
