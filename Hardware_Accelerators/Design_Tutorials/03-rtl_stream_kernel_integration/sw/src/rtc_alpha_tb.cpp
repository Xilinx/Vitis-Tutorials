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
#include <map>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Xilinx OCL
#include "xcl2/xcl2.hpp"

// XRT low level API
#include <xclhal2.h>

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


    // Platform name mapping: map development platform name to deployment platform
    // Please note that the HW mode and HW_EMU mode has different mapping presently
    std::map<std::string, std::string> platform_map, platform_map_hw, platform_map_hw_emu;
    
    platform_map_hw["xilinx_u200_gen3x16_xdma_1_1_202020_1"]   = "xilinx_u200_gen3x16_xdma_shell_1_1"; 
    platform_map_hw["xilinx_u200_xdma_201830_2"]               = "xilinx_u200_xdma_201830_2";
    platform_map_hw["xilinx_u250_gen3x16_xdma_3_1_202020_1"]   = "xilinx_u250_gen3x16_xdma_shell_3_1";
    platform_map_hw["xilinx_u250_xdma_201830_2"]               = "xilinx_u250_xdma_201830_2";
    platform_map_hw["xilinx_u280_xdma_201920_3"]               = "xilinx_u280_xdma_201920_3";
    platform_map_hw["xilinx_u50_gen3x16_xdma_201920_3"]        = "xilinx_u50_gen3x16_xdma_201920_3";

    platform_map_hw_emu["xilinx_u200_gen3x16_xdma_1_1_202020_1"]   = "xilinx_u200_gen3x16_xdma_1_1_202020_1"; 
    platform_map_hw_emu["xilinx_u200_xdma_201830_2"]               = "xilinx_u200_xdma_201830_2";
    platform_map_hw_emu["xilinx_u250_gen3x16_xdma_3_1_202020_1"]   = "xilinx_u250_gen3x16_xdma_3_1_202020_1";
    platform_map_hw_emu["xilinx_u250_xdma_201830_2"]               = "xilinx_u250_xdma_201830_2";
    platform_map_hw_emu["xilinx_u280_xdma_201920_3"]               = "xilinx_u280_xdma_201920_3";
    platform_map_hw_emu["xilinx_u50_gen3x16_xdma_201920_3"]        = "xilinx_u50_gen3x16_xdma_201920_3";

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
            platform_map = platform_map_hw_emu;
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
        platform_map = platform_map_hw;
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
	std::string dev_platform(tmp);
	dev_platform.erase(dev_platform.end()-1);   // delete \n in the line end
    std::cout << "[MESSAGE] XCLBIN file for rtc_alpha is built with development platform " << dev_platform << std::endl;
    std::string deploy_platform = platform_map[dev_platform];
    if (deploy_platform.empty()) {
        std::cout << "[ERROR] Unknown development platform. Exit." << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "[MESSAGE] Required deployment platform is " << deploy_platform << std::endl;
    }

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
        std::cout << "[MESSAGE] Found deployment platform in system: " << device_name << std::endl;
        if (device_name == deploy_platform) {
            device_found = 1;
            break;
        }
    }
    if (device_found == 0) {
        std::cout << "[ERROR] No matching deployment platform exists in the system. Exit." << std::endl;
        return EXIT_FAILURE;
    }

    cl::Context context(device);
    cl::CommandQueue q_rtc_gen(context, device);
    cl::CommandQueue q_strm_dump(context, device);
    cl::CommandQueue q_alpha_mix(context, device);                                                  

    std::vector<unsigned char> fileBuf = xcl::read_binary_file(xclbin_file.c_str());
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};

    std::vector<cl::Device> tempdev(1, device);
    cl::Program program(context, tempdev, bins);

    cl::Kernel krnl_rtc_gen;
    cl::Kernel krnl_strm_dump;
    cl::Kernel krnl_alpha_mix;
    
    krnl_rtc_gen = cl::Kernel(program, "rtc_gen:{rtc_gen_1}");
    krnl_strm_dump = cl::Kernel(program, "strm_dump:{strm_dump_1}");
    krnl_alpha_mix = cl::Kernel(program, "alpha_mix:{alpha_mix_1}");

    std::cout << "[MESSAGE] FPGA initialization finished" << std::endl;

    std::cout << "---------------------------- RTC with Alpha Mixing Test Start ----------------------------" << std::endl;  

    // get font file size
    std::string font_file("font.dat");
    struct stat statbuff;
    stat(font_file.c_str(), &statbuff);
    file_size = statbuff.st_size;
    std::cout << "[MESSAGE] Font library file size : " << file_size << std::endl;

    // create host buffer for font buffer reading
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

    // create host buffer for background image
    void *bgr_image_buffer;
    posix_memalign(&bgr_image_buffer, 4096, image.cols * image.rows * 3);

    // read background to host buffer
    cv::Mat bgr_image(image.rows, image.cols, image.type(), bgr_image_buffer);
    image.copyTo(bgr_image);

    // create FPGA buffer for background buffer
    cl::Buffer FPGABgrImageBuffer (context,
                               CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                               (size_t)(image.cols * image.rows * 3),
                               bgr_image_buffer,
                               nullptr);    

    // create host buffer for output image from FPGA
    void *dout_data;
    posix_memalign(&dout_data, 4096, image.cols * image.rows * 3);
    

    // Create FPGA buffer for output image
    cl::Buffer FPGAOutputBuffer  (context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,
                                  (size_t)(image.cols * image.rows * 3),
                                  dout_data,
                                  nullptr);                                                                                           
    
    uint32_t time_value = 0;
    time_t tt;
    time( &tt );       // get system time
    tm* t= localtime( &tt );
    time_value = (t->tm_hour << 24) + (t->tm_min << 16) + (t->tm_sec << 8);

    // set kernel arguments for font load mode and clock initialization                         
    krnl_rtc_gen.setArg(0, 0);              // work_mode
    krnl_rtc_gen.setArg(1, CS_COUNT);       // cs_count
    krnl_rtc_gen.setArg(2, time_format);    // time_format
    krnl_rtc_gen.setArg(3, time_value);     // time_set_value
    krnl_rtc_gen.setArg(4, set_sys_time);   // time_set_en
    krnl_rtc_gen.setArg(5, 0);              // time_value (pesudo write)
    krnl_rtc_gen.setArg(6, FPGAFontBuffer); // font data address

    // Load font data to FPGA buffer of rtc_gen kernel    
    q_rtc_gen.enqueueMigrateMemObjects ({FPGAFontBuffer}, 0);
    q_rtc_gen.finish();
    std::cout << "[MESSAGE] Font data loaded to Alveo global memory" << std::endl; 

    // Kernel execution： load font data
    q_rtc_gen.enqueueTask(krnl_rtc_gen);
    q_rtc_gen.finish();
    std::cout << "[MESSAGE] Font data loaded to FPGA on-chip SRAM" << std::endl;     

    // Load backgound image to FPGA buffer of alpha_mix kernel    
    q_alpha_mix.enqueueMigrateMemObjects ({FPGABgrImageBuffer}, 0);
    q_alpha_mix.finish();
    std::cout << "[MESSAGE] Background image loaded to Alveo global memory" << std::endl; 

    // set kernel arguments to RTC generation mode     
    krnl_rtc_gen.setArg(0, 1);              // work_mode
    krnl_rtc_gen.setArg(4, 0);              // time_set_en

    // set kernel arguments for alpha mix
    krnl_alpha_mix.setArg(1,  FPGABgrImageBuffer);   // background buffer
    krnl_alpha_mix.setArg(3,  FONT_HEIGHT);          // time image height
    if (time_format == 1) {
        krnl_alpha_mix.setArg(4,  FONT_WIDTH * 11);    // time image width (11 digits)
        krnl_alpha_mix.setArg(5,  RTC_IMG_HEIGHT_11D); // time image resized height (11 digits)
        krnl_alpha_mix.setArg(6,  RTC_IMG_WIDTH_11D);  // time image resized width (11 digits)
    } else {
        krnl_alpha_mix.setArg(4,  FONT_WIDTH * 8);     // time image width (8 digits)
        krnl_alpha_mix.setArg(5,  RTC_IMG_HEIGHT_8D);  // time image resized height (8 digits)
        krnl_alpha_mix.setArg(6,  RTC_IMG_WIDTH_8D);   // time image resized width (8 digits)
    }
    krnl_alpha_mix.setArg(7,  RTC_POSITION_ROW);     // time image position - row
    krnl_alpha_mix.setArg(8,  RTC_POSITION_COL);     // time image position - col
    krnl_alpha_mix.setArg(9,  (FONT_COLOR_R<<16) + (FONT_COLOR_G<<8) + FONT_COLOR_B);  // font color           
    krnl_alpha_mix.setArg(10, (BGR_COLOR_R<<16) + (BGR_COLOR_G<<8) + BGR_COLOR_B);    // font backgroud color
    krnl_alpha_mix.setArg(11, BGR_OPA);                 // time background opacity
    krnl_alpha_mix.setArg(12, image.rows);
    krnl_alpha_mix.setArg(13, image.cols);

    // set kernel arguments for strm_dump
    krnl_strm_dump.setArg(1, FPGAOutputBuffer);               // output buffer address
    krnl_strm_dump.setArg(2, image.rows * image.cols * 3);    // byte_size


    // preparation for low level register accessing
    xclDeviceHandle handle; 
    clGetDeviceInfo(device.get(), CL_DEVICE_HANDLE, sizeof(handle), &handle, nullptr);
    
    // get CU id of rtc_gen
    cl_uint cuid_rtc_gen; 
    xclGetComputeUnitInfo(krnl_rtc_gen.get(), 0, XCL_COMPUTE_UNIT_INDEX, sizeof(cuid_rtc_gen), &cuid_rtc_gen, nullptr);
    std::cout << "[MESSAGE] CU index of rtc_gen = " << cuid_rtc_gen << std::endl; 

    // get address offset of time_val register
    size_t time_val_offset = 0;
    clGetKernelArgInfo(krnl_rtc_gen.get(), 5, CL_KERNEL_ARG_OFFSET, sizeof(time_val_offset), &time_val_offset, nullptr);
    std::cout << "[MESSAGE] register time_val offset = 0x" << std::hex << time_val_offset <<std::endl;

    uint32_t old_time_value;
    xclRegRead(handle, cuid_rtc_gen, time_val_offset, &old_time_value);
    std::cout << "[MESSAGE] Read-out initial internel time value = 0x" << std::hex << old_time_value << std::endl;


    // always loop to display free running RTC with mixed background image in second or deci-second tick
    // Create OpenCV display buffer from host data buffer
    cv::Mat output_image(image.rows, image.cols, CV_8UC3, dout_data);
    while (1) {            
        // trigger the kernels to make the clock image and transfer the data to host OpenCV matrix buffer
        q_rtc_gen.enqueueTask(krnl_rtc_gen);
        q_alpha_mix.enqueueTask(krnl_alpha_mix);
        q_strm_dump.enqueueTask(krnl_strm_dump);
        q_rtc_gen.finish();
        q_alpha_mix.finish();
        q_strm_dump.finish();
        q_strm_dump.enqueueMigrateMemObjects({FPGAOutputBuffer},
                               CL_MIGRATE_MEM_OBJECT_HOST);
        q_strm_dump.finish();

        while (1) {
            cv::imshow("Real Time Clock With Alpha Mixing", output_image);
            int temp = cv::waitKey(1);
            if (temp == 27) {
                std::cout << "[MESSAGE] Program exit normally." << std::endl;
                std::cout << "[MESSAGE] Read-out final internal time value = 0x" << std::hex << time_value << std::endl;
                return EXIT_SUCCESS;
            }
            // When the internal clock counter time_val changed, refresh the clock image
            xclRegRead(handle, cuid_rtc_gen, time_val_offset, &time_value);
            if (((time_format == 0) && ((old_time_value & 0xff00) != (time_value & 0xff00))) ||  // clock second digit changed, refresh the image
                ((time_format == 1) && ((old_time_value & 0xff) != (time_value & 0xff))))        // clock deci-second digit changed, refresh the image
            {
                old_time_value = time_value;
                break;
            }
        }
    }
    
}

