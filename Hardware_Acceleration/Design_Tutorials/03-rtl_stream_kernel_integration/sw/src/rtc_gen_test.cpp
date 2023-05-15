/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <chrono>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <map>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "experimental/xrt_kernel.h"


// centi-counter value
// If the system clock is 200MHz, the cs_count value should be 200,000,000 / 100 = 2,000,000
#define CS_COUNT 2000000

// FONT size in the font library (DONNOT MODIFY)
#define FONT_WIDTH 160
#define FONT_HEIGHT 240

// Please use 'xbutil examine' command to get the device id of the target alveo card if multiple
//   cards are installed in the system.
#define DEVICE_ID   0

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

    // Load xclbin
    std::cout << "Load " << xclbin_file << std::endl;
    xrt::device device = xrt::device(DEVICE_ID);
    xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);

   // create kernel objects
    std::cout << "Create kernels" << std::endl;
    xrt::kernel krnl_rtc_gen = xrt::kernel(device, xclbin_uuid, "rtc_gen", xrt::kernel::cu_access_mode::exclusive);
    xrt::kernel krnl_strm_dump = xrt::kernel(device, xclbin_uuid, "strm_dump");

    // get memory bank groups
    xrtMemoryGroup bank_grp_rtc_gen = krnl_rtc_gen.group_id(6);
    xrtMemoryGroup bank_grp_strm_dump = krnl_strm_dump.group_id(1);

    // create kernel runner instance
    xrt::run run_rtc_gen = xrt::run(krnl_rtc_gen);
    xrt::run run_strm_dump = xrt::run(krnl_strm_dump);
    std::cout << "[MESSAGE] FPGA initialization finished" << std::endl;

    std::cout << "---------------------------- RTC Gen Test Start ----------------------------" << std::endl;   
             
    // read font data
    std::string font_file("font.dat");
    struct stat statbuff;
    stat(font_file.c_str(), &statbuff);
    file_size = statbuff.st_size;
    std::cout << "[MESSAGE] Font library file size : " << file_size << std::endl;

    // read font data
    char *font_buffer;
    font_buffer = new char [file_size];
    read_file(font_file.c_str(), file_size, font_buffer);
    std::cout << "[MESSAGE] Reading font data finished" << std::endl; 

    // create device buffer for fonts
    xrt::bo FPGAFontBuffer = xrt::bo(device, file_size, xrt::bo::flags::normal, bank_grp_rtc_gen);

    // transfer font data to device
    FPGAFontBuffer.write(font_buffer);
    FPGAFontBuffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "[MESSAGE] Font data loaded to Alveo global memory" << std::endl;                                                                                        
    
    uint32_t time_value = 0;
    time_t tt;
    time( &tt );       // get system time
    tm* t= localtime( &tt );
    time_value = (t->tm_hour << 24) + (t->tm_min << 16) + (t->tm_sec << 8);

    // set kernel arguments for font load mode and clock initialization                         
    run_rtc_gen.set_arg(0, 0);              // work_mode
    run_rtc_gen.set_arg(1, CS_COUNT);       // cs_count
    run_rtc_gen.set_arg(2, 0);              // time_format
    run_rtc_gen.set_arg(3, time_value);     // time_set_value
    run_rtc_gen.set_arg(4, 1);              // time_set_en
    run_rtc_gen.set_arg(5, 0);              // time_value (pesudo write)
    run_rtc_gen.set_arg(6, FPGAFontBuffer); // font data address

    // Kernel executionÃ¯Â¼Â load font data
    run_rtc_gen.start();
    run_rtc_gen.wait();
    std::cout << "[MESSAGE] Font data loaded to FPGA on-chip SRAM" << std::endl;      

    uint32_t old_time_value = krnl_rtc_gen.read_register(0x38);

    // -========================================================================-
    //  Test 8-digit clock
    // -========================================================================-
    // create host buffer for output image from FPGA
    void *dout_data_8d;
    dout_data_8d = new char [FONT_HEIGHT * FONT_WIDTH * 8];

    // Create FPGA buffer for output image
    xrt::bo FPGAOutputBuffer_8d = xrt::bo(device, FONT_HEIGHT * FONT_WIDTH * 8, xrt::bo::flags::normal, bank_grp_strm_dump);                                                                    


    // set rtc_gen kernel arguments for RTC generation mode
    run_rtc_gen.set_arg(0, 1);              // work_mode
    run_rtc_gen.set_arg(2, 0);              // time_format    
    run_rtc_gen.set_arg(4, 0);              // time_set_en

    // set strm_dump kernel arguments for data output
    run_strm_dump.set_arg(1, FPGAOutputBuffer_8d);                 // output buffer address
    run_strm_dump.set_arg(2, FONT_HEIGHT * FONT_WIDTH * 8);    // byte_size

    // always loop to display free running RTC in second tick
	// Create 8-digit clock OpenCV display buffer from host data buffer
    cv::Mat output_image_8d(FONT_HEIGHT, FONT_WIDTH*8, CV_8UC1, dout_data_8d);
    while (1) {
        int quit_loop = 0;		
        // trigger the kernels to make the clock image and transfer the data to host OpenCV matrix buffer
        run_rtc_gen.start();
        run_strm_dump.start();
        run_rtc_gen.wait();
        run_strm_dump.wait();

        FPGAOutputBuffer_8d.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        FPGAOutputBuffer_8d.read(dout_data_8d);

        while (1) {
			cv::imshow("Real Time Clock - 8 digits", output_image_8d);
			int temp = cv::waitKey(1);
			if (temp == 27) {	// detect ESD key
                quit_loop = 1;
				break;
            }
            // When the internal clock counter time_val changed, refresh the clock image
            time_value = krnl_rtc_gen.read_register(0x38);
            if ((old_time_value & 0xff00) != (time_value & 0xff00))  // clock second digit changed, refresh the image
            {
                old_time_value = time_value;
			    std::cout << "[MESSAGE] Read-out internel time value = 0x" << std::hex << old_time_value << std::endl;
                break;
            }
		}
		
		if (quit_loop == 1) {
			break;
		}
    }


    // -========================================================================-
    //  Test 11-digit clock
    // -========================================================================-
    // create host buffer for output image from FPGA
    void *dout_data_11d;
    dout_data_11d = new char [FONT_HEIGHT * FONT_WIDTH * 11];
    
    // Create FPGA buffer for output image
    xrt::bo FPGAOutputBuffer_11d = xrt::bo(device, FONT_HEIGHT * FONT_WIDTH * 11, xrt::bo::flags::normal, bank_grp_strm_dump);                                                                    


    // set rtc_gen kernel arguments for RTC generation mode
    run_rtc_gen.set_arg(0, 1);              // work_mode
    run_rtc_gen.set_arg(2, 1);              // time_format    

    // set strm_dump kernel arguments for data output
    run_strm_dump.set_arg(1, FPGAOutputBuffer_11d);                 // output buffer address
    run_strm_dump.set_arg(2, FONT_HEIGHT * FONT_WIDTH * 11);    // byte_size

    // always loop to display free running RTC in second tick
	// Create 11-digit clock OpenCV display buffer from host data buffer
    cv::Mat output_image_11d(FONT_HEIGHT, FONT_WIDTH*11, CV_8UC1, dout_data_11d);
    while (1) {
        int quit_loop = 0;		
        // trigger the kernels to make the clock image and transfer the data to host OpenCV matrix buffer
        run_rtc_gen.start();
        run_strm_dump.start();
        run_rtc_gen.wait();
        run_strm_dump.wait();

        FPGAOutputBuffer_11d.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        FPGAOutputBuffer_11d.read(dout_data_11d);

        while (1) {
			cv::imshow("Real Time Clock - 11 digits", output_image_11d);
			int temp = cv::waitKey(1);
			if (temp == 27) {	// detect ESD key
                quit_loop = 1;
				break;
            }
            // When the internal clock counter time_val changed, refresh the clock image
            time_value = krnl_rtc_gen.read_register(0x38);
            if ((old_time_value & 0xff) != (time_value & 0xff))  // clock second digit changed, refresh the image
            {
                old_time_value = time_value;
				std::cout << "[MESSAGE] Read-out internel time value = 0x" << std::hex << old_time_value << std::endl;
                break;
            }
		}
		if (quit_loop == 1) {
			break;
		}
    }

   std::cout << "[MESSAGE] Program exit normally." << std::endl;
   return EXIT_SUCCESS;
}

