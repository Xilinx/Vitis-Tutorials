//
// Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_uuid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <cstdlib>
#include <openssl/aes.h>
#include <string.h>

// Please use 'xbutil list' command to get the device id of the target alveo card if multiple
//   cards are installed in the system.
#define DEVICE_ID   0


// software implementation for AES encryption, used for hardware data verification
void ref_aes_cipher (const unsigned char* input, unsigned char* output, const unsigned char* key, int words)
{
    unsigned char *in_data, *out_data;
    AES_KEY aes_key;
    memset(&aes_key, 0x00, sizeof(AES_KEY));

    if (AES_set_encrypt_key(key, 256, &aes_key) < 0) {
        std::cout << "[ERROR] Unable to set encryption key in AES..." << std::endl;
        return;
    }
	
    for (int w = 0; w < words; w++) {
        in_data = (unsigned char*)((unsigned long long)input + w * 16);
        out_data = (unsigned char*)((unsigned long long)output + w * 16);
        AES_ecb_encrypt(in_data, out_data, &aes_key, AES_ENCRYPT);
    }
  
}

// function to write binary file
void write_bin_file(const char *file_name, int file_size, char *write_buffer)
{
    std::ofstream output_file(file_name, std::ios::out | std::ios::binary);
    output_file.write(write_buffer, file_size);
    output_file.close();
}

// function to read binary file
void read_bin_file(const char *file_name, int file_size, char *read_buffer)
{
    std::ifstream input_file(file_name, std::ios::in | std::ios::binary);
    input_file.read(read_buffer, file_size);
    input_file.close();
}

// compare byte data of two arrays
int data_compare (char *out_data, char *ref_data, int byte_num) {
    int err_num = 0;
    for (int i = 0; i < byte_num; i++) {
        if (out_data[i] != ref_data[i]) {
            printf("  MISMATCH at BYTE %d : EXP = %02x, ACT = %02x\n",i, (unsigned char)ref_data[i], (unsigned char)out_data[i]);
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
    const char *optstring = "w:k:";

    int words_num = 1024 * 1024;
    std::string key = "123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0";

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        if ((opt == 'w') && optarg) {
            words_num = std::stoi(std::string(optarg));
        }

        if ((opt == 'k') && optarg) {
            key = std::string(optarg);
        }
        
    }

    std::cout << std::endl;
    std::cout << "------------------------ krnl_aes test program ------------------------" << std::endl;
    std::cout << " 128-bit words number : " << words_num << std::endl;
    std::cout << "           Key length : 256-bit" << std::endl;
    std::cout << "                  Key : " << key <<std::endl;

// generate key and iv byte array for openssl library
    unsigned char key_byte[32], iv_byte[16];
    for (int i = 0; i < 32; i++) {
        key_byte[i] = (unsigned char)std::stoll(key.substr(i*2,2), nullptr, 16);
    }


// --------------------------------------------------------------------------------------
// Generate and read plain/cipher data into host buffer
// --------------------------------------------------------------------------------------
    char *plain_data, *cipher_data;
    plain_data  = new char [words_num * 16];
    cipher_data  = new char [words_num * 16];

    // Call perl script and openssl tool to generate "plain_x.dat", "cipher_ecb_x.dat" and "cipher_cbc_x.dat" files.
    //   Then read the generated bin file into array
    std::cout << std::endl << "Generating plain data" << std::endl;
    srand((unsigned)time(NULL));
    for (int i = 0; i < words_num * 16; i++) {
        plain_data[i] = rand()%256;
    }

    ref_aes_cipher ((unsigned char*) plain_data, (unsigned char*)cipher_data, key_byte, words_num);

    // write out reference data for reference
    write_bin_file("./plain.dat", words_num * 16, plain_data);
    write_bin_file("./cipher.dat", words_num * 16, cipher_data);


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
            std::cout << "[MESSAGE] Program running in hardware emulation mode" << std::endl;
            xclbin_file = "krnl_aes_test_hw_emu.xclbin";
        }
        else
        {
            std::cout << "[ERROR] Unsupported Emulation Mode: " << mode << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        std::cout << "[MESSAGE] Program running in hardware mode" << std::endl;
        xclbin_file = "krnl_aes_test_hw.xclbin";
    }

    // Load xclbin
    std::cout << "Load " << xclbin_file << std::endl;
    xrt::device device = xrt::device(DEVICE_ID);
    xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);
   
    // create kernel objects
    std::cout << "Create kernels" << std::endl;
    xrt::kernel kernel_krnl_aes = xrt::kernel(device, xclbin_uuid, 
                                                "krnl_aes",
                                                xrt::kernel::cu_access_mode::exclusive);

    std::vector <xrt::kernel> kernel_strm_issue;
    kernel_strm_issue.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_issue:{strm_issue_1}", 
                                                xrt::kernel::cu_access_mode::exclusive));
    kernel_strm_issue.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_issue:{strm_issue_2}", 
                                                xrt::kernel::cu_access_mode::exclusive));
    kernel_strm_issue.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_issue:{strm_issue_3}", 
                                                xrt::kernel::cu_access_mode::exclusive));
    kernel_strm_issue.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_issue:{strm_issue_4}", 
                                                xrt::kernel::cu_access_mode::exclusive));

    std::vector <xrt::kernel> kernel_strm_dump;
    kernel_strm_dump.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_dump:{strm_dump_1}", 
                                                xrt::kernel::cu_access_mode::exclusive));
    kernel_strm_dump.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_dump:{strm_dump_2}", 
                                                xrt::kernel::cu_access_mode::exclusive));
    kernel_strm_dump.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_dump:{strm_dump_3}", 
                                                xrt::kernel::cu_access_mode::exclusive));
    kernel_strm_dump.push_back(xrt::kernel(device, xclbin_uuid, 
                                                "strm_dump:{strm_dump_4}", 
                                                xrt::kernel::cu_access_mode::exclusive));

    // get memory bank groups
    xrtMemoryGroup bank_grp_strm_issue = kernel_strm_issue[0].group_id(1);
    xrtMemoryGroup bank_grp_strm_dump = kernel_strm_dump[0].group_id(1);

    // create device buffer objects
    std::cout << "Create input and output device buffers" << std::endl;
    std::vector <xrt::bo> buffer_strm_issue;
    std::vector <xrt::bo> buffer_strm_dump;

    for (int i = 0; i < 4; i++) {
        buffer_strm_issue.push_back(xrt::bo(device, words_num*16, xrt::bo::flags::normal, bank_grp_strm_issue));
        buffer_strm_dump.push_back(xrt::bo(device, words_num*16, xrt::bo::flags::normal, bank_grp_strm_dump));
    }        

    // create kernel runner instance
    xrt::run run_key_exp(kernel_krnl_aes);  // runner for krnl_aes
    std::vector <xrt::run> run_strm_issue;  // runner for strm_issue
    std::vector <xrt::run> run_strm_dump;   // runner for strm_dump
    for (int i = 0; i < 4; i++) {
        run_strm_issue.push_back(xrt::run(kernel_strm_issue[i]));
        run_strm_dump.push_back(xrt::run(kernel_strm_dump[i]));
    }

// --------------------------------------------------------------------------------------
// krnl_aes kernel: finish AES key expansion operation
// --------------------------------------------------------------------------------------
    std::cout << "Run krnl_aes for AES key expansion" << std::endl;

    run_key_exp.set_arg(0, 1);      // MODE = 1 (encryption)
    run_key_exp.set_arg(1, 3);      // KEY_LEN = 3 (256-bit)
    run_key_exp.set_arg(3, std::stoll(key.substr(0, 8), nullptr, 16));    // KEY word 7
    run_key_exp.set_arg(4, std::stoll(key.substr(8, 8), nullptr, 16));    // KEY word 6
    run_key_exp.set_arg(5, std::stoll(key.substr(16, 8), nullptr, 16));   // KEY word 5
    run_key_exp.set_arg(6, std::stoll(key.substr(24, 8), nullptr, 16));   // KEY word 4
    run_key_exp.set_arg(7, std::stoll(key.substr(32, 8), nullptr, 16));   // KEY word 3
    run_key_exp.set_arg(8, std::stoll(key.substr(40, 8), nullptr, 16));   // KEY word 2
    run_key_exp.set_arg(9, std::stoll(key.substr(48, 8), nullptr, 16));   // KEY word 1
    run_key_exp.set_arg(10, std::stoll(key.substr(56, 8), nullptr, 16));   // KEY word 0
    run_key_exp.start();
    run_key_exp.wait();
    std::cout << "Kernel run finish" << std::endl;

// --------------------------------------------------------------------------------------
// Encryption test
// strm_issue and strm_dump kernels: transfer input data to device and get output data from device
// --------------------------------------------------------------------------------------
    struct timeval kernels_start_time, kernels_finish_time;     // kernel execution time record

    std::cout << std::endl << "-------------- AES Encryption Test " << std::endl;

    // create host buffer for output data check
    char *output_data;
    output_data = new char [words_num * 16];

    // write plain_data into input device buffer of strm_issue kernels
    std::cout << "Transfer plain data into device buffer" << std::endl;
    for (int i = 0; i < 4; i++) {
        buffer_strm_issue[i].write(plain_data);
        buffer_strm_issue[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }

    // set kernel arguments for strm_issue and strm_dump kernels
    for (int i = 0; i < 4; i++) {
        run_strm_issue[i].set_arg(1, buffer_strm_issue[i]);     // memory pointer
        run_strm_issue[i].set_arg(2, words_num * 16);           // transfer byte size
        run_strm_dump[i].set_arg(1, buffer_strm_dump[i]);       // memory pointer
        run_strm_dump[i].set_arg(2, words_num * 16);            // transfer byte size
    }

    // kernel execution
    std::cout << "Start strm_issue and strm_dump kernels" << std::endl;
    gettimeofday(&kernels_start_time, NULL);
    for (int i = 0; i < 4; i++) {
        run_strm_issue[i].start();
        run_strm_dump[i].start();
    }
    for (int i = 0; i < 4; i++) {
        run_strm_issue[i].wait();
        run_strm_dump[i].wait();
    }
    gettimeofday(&kernels_finish_time, NULL);
    float kernel_execution_time = time_dura(kernels_start_time, kernels_finish_time);
    std::cout << "Kernel run finish" << std::endl;
    printf("AES engines execution time = %f ms\n", kernel_execution_time);

    // read cipher data from output device buffer of strm_dump kernels and data check
    std::cout << "Transfer cipher data from device buffer and verification" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "-- AES Engine " << i << std::endl;
        buffer_strm_dump[i].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        buffer_strm_dump[i].read(output_data);
        int errors = data_compare(output_data, cipher_data, (words_num * 16));
        if (errors) {
            std::cout << "   FAILED" << std::endl;
        } else {
            std::cout << "   SUCCESS" << std::endl;
        }
    }
    printf("AES total encryption throughput = %f MB/s\n", 4 * words_num * 16 / 1000 / kernel_execution_time);


// --------------------------------------------------------------------------------------
// Decryption test
// strm_issue and strm_dump kernels: transfer input data to device and get output data from device
// --------------------------------------------------------------------------------------
    std::cout << std::endl << "-------------- AES Decryption Test " << std::endl;

    // write plain_data into input device buffer of strm_issue kernels
    std::cout << "Transfer cipher data into device buffer" << std::endl;
    for (int i = 0; i < 4; i++) {
        buffer_strm_issue[i].write(cipher_data);
        buffer_strm_issue[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }

    // Method 1 to configure krnl_aes to decryption mode
    //   use run instance to set kernel argument. The kernel register will not be updated until run()
    // ----------------------------------------------------------------------------------------------------
     run_key_exp.set_arg(0,0);                         // index for argument MODE is 0
     run_key_exp.start();
     run_key_exp.wait();
    // ----------------------------------------------------------------------------------------------------
    
    // Method 2 to configrue krnl_aes to decryption mode
    //   use kernel write_register() to access the register directly
    // ----------------------------------------------------------------------------------------------------
    // int reg_offset = kernel_krnl_aes.offset(0);     // reg_offset = 0x10 for krnl_aes kernel register MODE
    // kernel_krnl_aes.write_register(reg_offset, 0);  // set MODE = 0
    // ----------------------------------------------------------------------------------------------------

    // kernel execution
    std::cout << "Start strm_issue and strm_dump kernels" << std::endl;
    gettimeofday(&kernels_start_time, NULL);
    for (int i = 0; i < 4; i++) {
        run_strm_issue[i].start();
        run_strm_dump[i].start();
    }
    for (int i = 0; i < 4; i++) {
        run_strm_issue[i].wait();
        run_strm_dump[i].wait();
    }
    gettimeofday(&kernels_finish_time, NULL);
    kernel_execution_time = time_dura(kernels_start_time, kernels_finish_time);
    std::cout << "Kernel run finish" << std::endl;
    printf("AES engines execution time = %f ms\n", kernel_execution_time);

    // read cipher data from output device buffer of strm_dump kernels and data check
    std::cout << "Transfer plain data from device buffer and verification" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "-- AES Engine " << i << std::endl;
        buffer_strm_dump[i].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        buffer_strm_dump[i].read(output_data);
        int errors = data_compare(output_data, plain_data, (words_num * 16));
        if (errors) {
            std::cout << "   FAILED" << std::endl;
        } else {
            std::cout << "   SUCCESS" << std::endl;
        }

    }
    printf("AES total decryption throughput = %f MB/s\n", 4 * words_num * 16 / 1000 / kernel_execution_time);
    std::cout << std::endl;



}
