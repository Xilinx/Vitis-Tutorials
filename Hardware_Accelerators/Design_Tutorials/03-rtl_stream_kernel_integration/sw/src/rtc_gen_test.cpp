/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/mman.h>
#include <CL/cl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

// Xilinx OCL
#include "xcl2/xcl2.hpp"

// centi-counter value
// If the system clock is 200MHz, the cs_count value should be 200,000,000 / 100 = 2,000,000
#define CS_COUNT 2000000

#define FONT_WIDTH 160
#define FONT_HEIGHT 240

// function to read binary file
void read_file(const char *file_name, int file_size, char *read_buffer)
{
    // read jpeg image as binary file to host memory
    std::ifstream input_file(file_name, std::ios::in | std::ios::binary);
    input_file.read(read_buffer, file_size);
    input_file.close();
}

int main(int argc, char *argv[])
{

    int file_size;   // font file size in bytes
    int i;

    // Judge emulation mode accoring to env variable
    std::string xclbin_file;
    char *env_emu;
    if (env_emu = getenv("XCL_EMULATION_MODE"))
    {
        std::string mode(env_emu);
        if (mode == "hw_emu")
        {
            std::cout << "[MESSAGE] Program running in hardware emulation mode" << std::endl;
            xclbin_file = "rtc_gen_test_hw_emu.xclbin";
        }
        else
        {
            std::cout << "[ERROR] Unsupported Emulation Mode: " << mode << std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        std::cout << "[MESSAGE] Program running in hardware mode" << std::endl;
        xclbin_file = "rtc_gen_test_hw.xclbin";
    }

    std::string xclbinutil_cmd = "xclbinutil --info -i " + xclbin_file + " | grep VBNV | grep -e xilinx.* -o";

    // XCLBIN file analysis
    FILE *pp = popen(xclbinutil_cmd.c_str(), "r"); // build pipe
	if (!pp) {
		return EXIT_FAILURE;
    }
	char tmp[1024];
	while (fgets(tmp, sizeof(tmp), pp) != NULL);
	pclose(pp);
	std::string platform(tmp);
	platform.erase(platform.end()-1);   // delete \n in the line end
	std::cout << "[MESSAGE] XCLBIN file for rtc_gen_test is built with platform " << platform << std::endl;
	
    // FPGA initialization
    cl::Device device;
    std::string device_name;
    std::vector<cl::Device> devices = xcl::get_xil_devices();
    if (devices.size() == 0)
    {
        std::cout << "[ERROR] no Xilinx devices found" << std::endl;
        return EXIT_FAILURE;
    }
    int device_found = 0;
    for (i = 0; i < devices.size(); i++)
    {
        device = devices[i];
        device_name = device.getInfo<CL_DEVICE_NAME>();
        std::cout << "[MESSAGE] found platform in system: " << device_name << std::endl;
        if (device_name == platform) {
            device_found = 1;
            break;
        }
    }
    if (device_found == 0) {
        std::cout << "[ERROR] no matching platform exists in the system" << std::endl;
        return EXIT_FAILURE;
    }
    
    cl::Context context(device);
    cl::CommandQueue q_rtc_gen(context, device);
    cl::CommandQueue q_strm_dump(context, device);

    std::vector<unsigned char> fileBuf = xcl::read_binary_file(xclbin_file.c_str());
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};

    std::vector<cl::Device> tempdev(1, device);
    cl::Program program(context, tempdev, bins);
    
    cl::Kernel krnl_rtc_gen;
    cl::Kernel krnl_strm_dump;
    
    krnl_rtc_gen = cl::Kernel(program, "rtc_gen:{rtc_gen_1}");
    krnl_strm_dump = cl::Kernel(program, "strm_dump:{strm_dump_1}");

    std::cout << "[MESSAGE] FPGA initialization finished" << std::endl;

    std::cout << "---------------------------- RTC Gen Test Start ----------------------------" << std::endl;   
             
    // get file size and create the read buffer
    std::string font_file("font.dat");
    struct stat statbuff;
    stat(font_file.c_str(), &statbuff);
    file_size = statbuff.st_size;
    std::cout << "[MESSAGE] Font library file size : " << file_size << std::endl;

    // create read buffer and read image file
    void *font_buffer;
    posix_memalign(&font_buffer, 4096, file_size);
    read_file(font_file.c_str(), file_size, (char *)font_buffer);
    std::cout << "[MESSAGE] Reading font data finished" << std::endl; 
    
    // create FPGA buffer for font buffer
    cl::Buffer FPGAFontBuffer (context,
                               CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                               (size_t)file_size,
                               font_buffer,
                               nullptr);                                                                                            
    
    uint32_t time_value;
    time_t tt;
    time( &tt );       // get system time
    tm* t= localtime( &tt );
    time_value = (t->tm_hour << 24) + (t->tm_min << 16) + (t->tm_sec << 8);

    // set kernel arguments for font load mode and clock initialization                         
    krnl_rtc_gen.setArg(0, 0);              // work_mode
    krnl_rtc_gen.setArg(1, CS_COUNT);       // cs_count
    krnl_rtc_gen.setArg(2, 0);              // time_format
    krnl_rtc_gen.setArg(3, time_value);     // time_set_value
    krnl_rtc_gen.setArg(4, 1);              // time_set_en
    krnl_rtc_gen.setArg(5, FPGAFontBuffer); // font data address

    // Load font data to on-chip buffer of rtc_gen kernel    
    q_rtc_gen.enqueueMigrateMemObjects ({FPGAFontBuffer}, 0);     // from host to device
    q_rtc_gen.finish();
    std::cout << "[MESSAGE] Font data loaded to Alveo global memory" << std::endl; 

    // Kernel execution： load font data
    q_rtc_gen.enqueueTask(krnl_rtc_gen);
    q_rtc_gen.finish();
    std::cout << "[MESSAGE] Font data loaded to FPGA on-chip SRAM" << std::endl;     


    // -========================================================================-
    //  Test 8-digit clock
    // -========================================================================-
    // create host buffer for output image from FPGA
    void *dout_data_8d;
    posix_memalign(&dout_data_8d, 4096, FONT_HEIGHT * FONT_WIDTH * 8);
    
    // Create FPGA buffer for output image
    cl::Buffer FPGAOutputBuffer_8d  (context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,
                                  (size_t)(FONT_HEIGHT * FONT_WIDTH * 8),
                                  dout_data_8d,
                                  nullptr);   

    // set rtc_gen kernel arguments for RTC generation mode
    krnl_rtc_gen.setArg(0, 1);              // work_mode
    krnl_rtc_gen.setArg(2, 0);              // time_format    
    krnl_rtc_gen.setArg(4, 0);              // time_set_en

    // set strm_dump kernel arguments for data output
    krnl_strm_dump.setArg(1, FPGAOutputBuffer_8d);                 // output buffer address
    krnl_strm_dump.setArg(2, FONT_HEIGHT * FONT_WIDTH * 8);    // byte_size

    // display 8 digit clock
    cv::Mat output_image_8d(FONT_HEIGHT, FONT_WIDTH*8, CV_8UC1, dout_data_8d);

    while (1) {          

        q_rtc_gen.enqueueTask(krnl_rtc_gen);
        q_strm_dump.enqueueTask(krnl_strm_dump);
        q_rtc_gen.finish();
        q_strm_dump.finish();

        q_strm_dump.enqueueMigrateMemObjects({FPGAOutputBuffer_8d},
                                        CL_MIGRATE_MEM_OBJECT_HOST);
        q_strm_dump.finish();

        cv::imshow("Real Time Clock - 8 digits", output_image_8d);
        int temp = cv::waitKey(500);
        if (temp == 27) {       // detect ESC key
            break;
        }
    }


    // -========================================================================-
    //  Test 11-digit clock
    // -========================================================================-
    // create host buffer for output image from FPGA
    void *dout_data_11d;
    posix_memalign(&dout_data_11d, 4096, FONT_HEIGHT * FONT_WIDTH * 11);
    
    // Create FPGA buffer for output image
    cl::Buffer FPGAOutputBuffer_11d  (context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,
                                  (size_t)(FONT_HEIGHT * FONT_WIDTH * 11),
                                  dout_data_11d,
                                  nullptr);   

    // set rtc_gen kernel arguments for RTC generation mode
    krnl_rtc_gen.setArg(0, 1);              // work_mode
    krnl_rtc_gen.setArg(2, 1);              // time_format    

    // set strm_dump kernel arguments for data output
    krnl_strm_dump.setArg(1, FPGAOutputBuffer_11d);                 // output buffer address
    krnl_strm_dump.setArg(2, FONT_HEIGHT * FONT_WIDTH * 11);    // byte_size

    // display 8 digit clock
    cv::Mat output_image_11d(FONT_HEIGHT, FONT_WIDTH*11, CV_8UC1, dout_data_11d);

    while (1) {          

        q_rtc_gen.enqueueTask(krnl_rtc_gen);
        q_strm_dump.enqueueTask(krnl_strm_dump);
        q_rtc_gen.finish();
        q_strm_dump.finish();

        q_strm_dump.enqueueMigrateMemObjects({FPGAOutputBuffer_11d},
                                        CL_MIGRATE_MEM_OBJECT_HOST);
        q_strm_dump.finish();

        cv::imshow("Real Time Clock - 11 digits", output_image_11d);
        int temp = cv::waitKey(5);
        if (temp == 27) {       // detect ESC key
            break;
        }
    }

   std::cout << "[MESSAGE] Program exit normally." << std::endl;
   return EXIT_SUCCESS;
}

