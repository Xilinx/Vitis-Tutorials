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

// position of clock image, top-left corner is (0,0)
#define RTC_POSITION_ROW 64
#define RTC_POSITION_COL 400

// resized clock image size for 8-digit font digit size
// ensure RTC_IMG_WIDTH is integer multiple of 8
#define RTC_IMG_WIDTH_8D 480
#define RTC_IMG_HEIGHT_8D 90

// resized clock image size for 11-digit font digit size
// ensure RTC_IMG_WIDTH is integer multiple of 8
#define RTC_IMG_WIDTH_11D 528
#define RTC_IMG_HEIGHT_11D 72

// clock image font color
#define FONT_COLOR_R 255
#define FONT_COLOR_G 255
#define FONT_COLOR_B 255

// clock image background color
#define BGR_COLOR_R 80
#define BGR_COLOR_G 80
#define BGR_COLOR_B 80

// clock image background opacity
#define BGR_OPA 100

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

    int opt;
    const char *optstring = "i:fsh";

    std::string bg_image = "../media/alveo.jpg";
    int time_format = 0;
    int set_sys_time = 0;

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        if (opt == 'h') {
            std::cout << std::endl;
            std::cout << "Usage: rtc_alpha_tb [-i BACK_IMAGE] [-f] [-s]" << std::endl << std::endl;
            std::cout << "  -i BACK_IMAGE: set path to the background image, default is ../media/alveo.jpg" << std::endl;
            std::cout << "  -f : set to use eleven-digit clock, default is eight-digit" << std::endl;
            std::cout << "  -s : use system time to set the clock, default don't set the clock" << std::endl;
            std::cout << std::endl;
            return EXIT_SUCCESS;
        }

        if ((opt == 'i') && optarg) {
            bg_image = std::string(optarg);
        }

        if (opt == 'f') {
            time_format = 1;
        }
        
        if (opt == 's') {
            set_sys_time = 1;
        }

    }

    std::cout << "[Running parameters] " << std::endl;
    std::cout << "background image - " << bg_image << std::endl;
    std::cout << "     time format - " << time_format << std::endl;
    std::cout << " set system time - " << set_sys_time << std::endl;

    // Judge emulation mode accoring to env variable
    std::string xclbin_file;
    char *env_emu;
    if (env_emu = getenv("XCL_EMULATION_MODE"))
    {
        std::string mode(env_emu);
        if (mode == "hw_emu")
        {
            std::cout << "[MESSAGE] Program running in hardware emulation mode" << std::endl;
            xclbin_file = "rtc_alpha_hw_emu.xclbin";
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
        xclbin_file = "rtc_alpha_hw.xclbin";
    }

    // Load xclbin
    std::cout << "Load " << xclbin_file << std::endl;
    xrt::device device = xrt::device(DEVICE_ID);
    xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);

   // create kernel objects
    std::cout << "Create kernels" << std::endl;
    xrt::kernel krnl_rtc_gen = xrt::kernel(device, xclbin_uuid, "rtc_gen", xrt::kernel::cu_access_mode::exclusive);
    xrt::kernel krnl_strm_dump = xrt::kernel(device, xclbin_uuid, "strm_dump");
    xrt::kernel krnl_alpha_mix = xrt::kernel(device, xclbin_uuid, "alpha_mix");

    // get memory bank groups
    xrtMemoryGroup bank_grp_rtc_gen = krnl_rtc_gen.group_id(6);
    xrtMemoryGroup bank_grp_strm_dump = krnl_strm_dump.group_id(1);
    xrtMemoryGroup bank_grp_alpha_mix = krnl_alpha_mix.group_id(1);

    // create kernel runner instance
    xrt::run run_rtc_gen = xrt::run(krnl_rtc_gen);
    xrt::run run_strm_dump = xrt::run(krnl_strm_dump);
    xrt::run run_alpha_mix = xrt::run(krnl_alpha_mix);
    std::cout << "[MESSAGE] FPGA initialization finished" << std::endl;

    std::cout << "---------------------------- RTC with Alpha Mixing Test Start ----------------------------" << std::endl;  

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

    // read background image
    cv::Mat image = cv::imread(bg_image, cv::IMREAD_UNCHANGED);
    std::cout << "[MESSAGE] Background Image " << bg_image << " Read in: " << image.cols << " x " << image.rows << std::endl;

    if (time_format == 1)
    {
        if (((RTC_POSITION_COL + RTC_IMG_WIDTH_11D) > image.cols) || ((RTC_POSITION_ROW + RTC_IMG_HEIGHT_11D) > image.rows))
        {
            std::cout << "ERROR: RTC image exceed background image bound." << std::endl;
            std::cout << "Background image width = " << image.cols << ", height = " << image.rows << std::endl;
            std::cout << "RTC image width = " << RTC_IMG_WIDTH_11D << ", height = " << RTC_IMG_HEIGHT_11D << std::endl;
            std::cout << "RTC image position X = " << RTC_POSITION_COL << ", Y = " << RTC_POSITION_ROW << std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        if (((RTC_POSITION_COL + RTC_IMG_WIDTH_8D) > image.cols) || ((RTC_POSITION_ROW + RTC_IMG_HEIGHT_8D) > image.rows))
        {
            std::cout << "ERROR: RTC image exceed background image bound." << std::endl;
            std::cout << "Background image width = " << image.cols << ", height = " << image.rows << std::endl;
            std::cout << "RTC image width = " << RTC_IMG_WIDTH_8D << ", height = " << RTC_IMG_HEIGHT_8D << std::endl;
            std::cout << "RTC image position X = " << RTC_POSITION_COL << ", Y = " << RTC_POSITION_ROW << std::endl;
            return EXIT_FAILURE;
        }
    }

    // read background image data
    char *bgr_image_buffer;
    bgr_image_buffer = new char [image.cols * image.rows * 3];
    cv::Mat bgr_image(image.rows, image.cols, image.type(), bgr_image_buffer);
    image.copyTo(bgr_image);

    // create device buffer for background image
    xrt::bo FPGABgrImageBuffer = xrt::bo(device, image.cols * image.rows * 3, xrt::bo::flags::normal, bank_grp_alpha_mix);

    // transfer image data to device
    FPGABgrImageBuffer.write(bgr_image_buffer);
    FPGABgrImageBuffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "[MESSAGE] Background image loaded to Alveo global memory" << std::endl; 

    // create output image buffer
    char *dout_data;
    dout_data = new char [image.cols * image.rows * 3];
 
    // create device buffer for output image
    xrt::bo FPGAOutputBuffer = xrt::bo(device, image.cols * image.rows * 3, xrt::bo::flags::normal, bank_grp_strm_dump);                                                                    
    
    uint32_t time_value = 0;
    time_t tt;
    time( &tt );       // get system time
    tm* t= localtime( &tt );
    time_value = (t->tm_hour << 24) + (t->tm_min << 16) + (t->tm_sec << 8);

    // set kernel arguments for font load mode and real time clock initialization                             
    run_rtc_gen.set_arg(0, 0);              // work_mode
    run_rtc_gen.set_arg(1, CS_COUNT);       // cs_count
    run_rtc_gen.set_arg(2, time_format);    // time_format
    run_rtc_gen.set_arg(3, time_value);     // time_set_value
    run_rtc_gen.set_arg(4, set_sys_time);   // time_set_en
    run_rtc_gen.set_arg(5, 0);              // time_value (pesudo write)
    run_rtc_gen.set_arg(6, FPGAFontBuffer); // font data address

    // Kernel executionï¼ load font data
    run_rtc_gen.start();
    run_rtc_gen.wait();
    std::cout << "[MESSAGE] Font data loaded to FPGA on-chip SRAM" << std::endl;     

    // set kernel arguments to RTC generation mode     
    run_rtc_gen.set_arg(0, 1);              // work_mode
    run_rtc_gen.set_arg(4, 0);              // time_set_en

    // set kernel arguments for alpha mix
    run_alpha_mix.set_arg(1,  FPGABgrImageBuffer);   // background buffer
    run_alpha_mix.set_arg(3,  FONT_HEIGHT);          // time image height
    if (time_format == 1) {
        run_alpha_mix.set_arg(4,  FONT_WIDTH * 11);    // time image width (11 digits)
        run_alpha_mix.set_arg(5,  RTC_IMG_HEIGHT_11D); // time image resized height (11 digits)
        run_alpha_mix.set_arg(6,  RTC_IMG_WIDTH_11D);  // time image resized width (11 digits)
    } else {
        run_alpha_mix.set_arg(4,  FONT_WIDTH * 8);     // time image width (8 digits)
        run_alpha_mix.set_arg(5,  RTC_IMG_HEIGHT_8D);  // time image resized height (8 digits)
        run_alpha_mix.set_arg(6,  RTC_IMG_WIDTH_8D);   // time image resized width (8 digits)
    }
    run_alpha_mix.set_arg(7,  RTC_POSITION_ROW);     // time image position - row
    run_alpha_mix.set_arg(8,  RTC_POSITION_COL);     // time image position - col
    run_alpha_mix.set_arg(9,  (FONT_COLOR_R<<16) + (FONT_COLOR_G<<8) + FONT_COLOR_B);  // font color           
    run_alpha_mix.set_arg(10, (BGR_COLOR_R<<16) + (BGR_COLOR_G<<8) + BGR_COLOR_B);    // font backgroud color
    run_alpha_mix.set_arg(11, BGR_OPA);                 // time background opacity
    run_alpha_mix.set_arg(12, image.rows);
    run_alpha_mix.set_arg(13, image.cols);

    // set kernel arguments for strm_dump
    run_strm_dump.set_arg(1, FPGAOutputBuffer);               // output buffer address
    run_strm_dump.set_arg(2, image.rows * image.cols * 3);    // byte_size

    // read real time clock value
    uint32_t old_time_value = krnl_rtc_gen.read_register(0x38);
    std::cout << "[MESSAGE] Read-out initial internel time value = 0x" << std::hex << old_time_value << std::endl;

    // always loop to display free running RTC with mixed background image in second or deci-second tick
    // Create OpenCV display buffer from host data buffer
    cv::Mat output_image(image.rows, image.cols, CV_8UC3, dout_data);
    while (1) {            
        // trigger the kernels to make the clock image and transfer the data to host OpenCV matrix buffer
        run_rtc_gen.start();
        run_alpha_mix.start();
        run_strm_dump.start();
        run_rtc_gen.wait();
        run_alpha_mix.wait();
        run_strm_dump.wait();

        FPGAOutputBuffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        FPGAOutputBuffer.read(dout_data);

        while (1) {
            cv::imshow("Real Time Clock With Alpha Mixing", output_image);
            int temp = cv::waitKey(1);
            if (temp == 27) {
                std::cout << "[MESSAGE] Program exit normally." << std::endl;
                std::cout << "[MESSAGE] Read-out final internal time value = 0x" << std::hex << time_value << std::endl;
                return EXIT_SUCCESS;
            }
            // When the internal clock counter time_val changed, refresh the clock image
            time_value = krnl_rtc_gen.read_register(0x38);
            if (((time_format == 0) && ((old_time_value & 0xff00) != (time_value & 0xff00))) ||  // clock second digit changed, refresh the image
                ((time_format == 1) && ((old_time_value & 0xff) != (time_value & 0xff))))        // clock deci-second digit changed, refresh the image
            {
                old_time_value = time_value;
                break;
            }
        }
    }
    
}

