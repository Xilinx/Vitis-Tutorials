/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_ip.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <thread>

// Please use 'xbutil list' command to get the device id of the target alveo card if multiple
//   cards are installed in the system.
#define DEVICE_ID   0

// function to write binary file
void write_bin_file(const char *file_name, uint32_t file_size, char *write_buffer)
{
    std::ofstream output_file(file_name, std::ios::out | std::ios::binary);
    output_file.write(write_buffer, file_size);
    output_file.close();
}

// function to read binary file
void read_bin_file(const char *file_name, uint32_t file_size, char *read_buffer)
{
    std::ifstream input_file(file_name, std::ios::in | std::ios::binary);
    input_file.read(read_buffer, file_size);
    input_file.close();
}

// compare byte data of two arrays
uint32_t data_compare (char *out_data, char *ref_data, uint32_t byte_num) {
    uint32_t err_num = 0;
    for (uint32_t i = 0; i < byte_num; i++) {
        if (out_data[i] != ref_data[i]) {
            printf("ref_data[%d] = %02x, out_data[%d] = %02x\n", i, (uint8_t)ref_data[i], i, (uint8_t)out_data[i]);
            err_num++;
        }
    }
    return err_num;
}

// compute time duration
float time_dura(timeval start_time, timeval end_time)
{
    return ((end_time.tv_sec - start_time.tv_sec) * 1000 * 1000 + end_time.tv_usec - start_time.tv_usec) / 1000.0;
}


// Main program body
int main(int argc, char *argv[]) {

    int opt;
    const char *optstring = "m:";

    uint32_t mega_bytes = 100;

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        if ((opt == 'm') && optarg) {
            mega_bytes = (uint32_t)(std::stoi(std::string(optarg)));
        }
    }

    uint32_t bytes_num = mega_bytes * 1024 * 1024;

    std::cout << std::endl;
    std::cout << "------------------------ krnl_aurora loopback test ------------------------" << std::endl;
    std::cout << "Transfer size: " << mega_bytes << " MB" << std::endl;

// --------------------------------------------------------------------------------------
// Generate TX data 
// --------------------------------------------------------------------------------------
    char *tx_data, *rx_data;
    tx_data  = new char [bytes_num];
    rx_data  = new char [bytes_num];

    srand((unsigned)time(NULL));

    std::cout << std::endl << "Generate TX data block." << std::endl;
    for (uint32_t i = 0; i < bytes_num; i++) {
        tx_data[i] = rand() % 256;
    }

    // write out TX data for reference
    write_bin_file("./tx_data.dat", bytes_num, tx_data);


// --------------------------------------------------------------------------------------
// Device and kernel initialization
// --------------------------------------------------------------------------------------
    // Judge emulation mode accoring to env variable
    std::string xclbin_file;
    char *env_emu;
    if (env_emu = getenv("XCL_EMULATION_MODE")) {
        std::string mode(env_emu);
        if (mode == "hw_emu")
        {
            std::cout << "Program running in hardware emulation mode" << std::endl;
            xclbin_file = "krnl_aurora_test_hw_emu.xclbin";
        }
        else
        {
            std::cout << "[ERROR] Unsupported Emulation Mode: " << mode << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        std::cout << "Program running in hardware mode" << std::endl;
        xclbin_file = "krnl_aurora_test_hw.xclbin";
    }

    // Load xclbin
    std::cout << "Load " << xclbin_file << std::endl;
    xrt::device device = xrt::device(DEVICE_ID);
    xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);
   
    // create kernel objects
    std::cout << "Create kernels" << std::endl;
    xrt::ip krnl_aurora = xrt::ip(device, xclbin_uuid, "krnl_aurora");
    xrt::kernel krnl_strm_issue = xrt::kernel(device, xclbin_uuid, "strm_issue");
    xrt::kernel krnl_strm_dump = xrt::kernel(device, xclbin_uuid, "strm_dump");

    // get memory bank groups
    xrtMemoryGroup bank_grp_strm_issue = krnl_strm_issue.group_id(1);
    xrtMemoryGroup bank_grp_strm_dump = krnl_strm_dump.group_id(1);

    // create device buffer objects
    std::cout << "Create TX and RX device buffer" << std::endl;
    xrt::bo buffer_strm_issue = xrt::bo(device, bytes_num, xrt::bo::flags::normal, bank_grp_strm_issue);
    xrt::bo buffer_strm_dump = xrt::bo(device, bytes_num, xrt::bo::flags::normal, bank_grp_strm_dump);   

    // create kernel runner instance
    xrt::run run_strm_issue = xrt::run(krnl_strm_issue);
    xrt::run run_strm_dump = xrt::run(krnl_strm_dump);


// --------------------------------------------------------------------------------------
// Aurora loopback test
// --------------------------------------------------------------------------------------
    struct timeval start_time, finish_time;     // kernel execution time record
    std::cout << "Transfer TX data into device buffer" << std::endl;
    buffer_strm_issue.write(tx_data);
    buffer_strm_issue.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // set kernel arguments for strm_issue and strm_dump kernels
    run_strm_issue.set_arg(1, buffer_strm_issue);     // memory pointer
    run_strm_issue.set_arg(2, bytes_num);             // transfer byte size
    run_strm_dump.set_arg(1, buffer_strm_dump);       // memory pointer
    run_strm_dump.set_arg(2, bytes_num);              // transfer byte size

    // wait for aurora kernel to enter ready status (3 seconds timeout)
    std::cout << "Check whether startup status of Aurora kernel is ready..." << std::endl;
    gettimeofday(&start_time, NULL);
    while (1)
    {
        uint32_t reg_read_data = krnl_aurora.read_register(0x10);
        if (reg_read_data == 0x11ff)    // 14'b1_0001_1111_1111
        {
            std::cout << "Aurora kernel startup status is GOOD: " << std::bitset<13>(reg_read_data) << std::endl; 
            std::cout << "[12]channel_up [11]soft_err [10]hard_err [9]mmcm_not_locked_out [8]gt_pll_lock [7:4]line_up [3:0]gt_powergood" <<std:: endl;
            break;
        }
        else 
        {
            gettimeofday(&finish_time, NULL);
            if (time_dura(start_time, finish_time) > 3000)  // 3000 ms timeout value
            {
                std::cout << "Aurora kernel startup status is BAD and waiting timeout: " << std::bitset<13>(reg_read_data) << std::endl; 
                std::cout << "[12]channel_up [11]soft_err [10]hard_err [9]mmcm_not_locked_out [8]gt_pll_lock [7:4]line_up [3:0]gt_powergood" <<std:: endl;
                return EXIT_FAILURE;
            }
        }
    }
   
    std::cout << "Begin data loopback transfer" << std::endl;

    run_strm_dump.start();

    // Give the strm_dump kernel enough time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    gettimeofday(&start_time, NULL);
    
    run_strm_issue.start();
    run_strm_issue.wait();
    run_strm_dump.wait();

    gettimeofday(&finish_time, NULL);

    float kernel_execution_time = time_dura(start_time, finish_time);
    std::cout << "Data loopback transfer finish" << std::endl;
    std::cout << "Transfer time = " << kernel_execution_time << " ms" << std::endl;

    // read RX data from output device buffer of strm_dump kernels and data check
    std::cout << "Fetch RX data from device buffer and verification" << std::endl;

    buffer_strm_dump.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    buffer_strm_dump.read(rx_data);
 
    // write out RX data for reference
    write_bin_file("./rx_data.dat", bytes_num, rx_data);
    std::cout << "Data loopback transfer throughput = " << mega_bytes * 1000 / kernel_execution_time << " MB/s" << std::endl;

    // report aurora error status
    uint32_t reg_read_data = krnl_aurora.read_register(0x10);
    std::cout << "Aurora Error Status:" <<std::endl;
    std::cout << "SOFT_ERR:" << " " <<(1 & (reg_read_data >> (11))) << std::endl;
	std::cout << "HARD_ERR:" << " " <<(1 & (reg_read_data >> (10))) << std::endl;

    std::cout << std::endl;

    uint32_t errors = data_compare(rx_data, tx_data, bytes_num);
    if (errors)
    {
        std::cout << "Data verification FAIL" << std::endl;
        std::cout << "Total mismatched bytes: " << errors << std::endl;        
        std::cout << "Please check tx_data.dat and rx_data.dat files" << std::endl;
    }
    else
    {
        std::cout << "Data verification SUCCEED" << std::endl;
    }
	
    std::cout << std::endl;

}
