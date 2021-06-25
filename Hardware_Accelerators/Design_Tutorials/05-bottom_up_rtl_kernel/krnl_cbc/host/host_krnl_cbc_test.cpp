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
#include <time.h>
#include <openssl/aes.h>
#include <string.h>
#include <thread>

// Please use 'xbutil list' command to get the device id of the target alveo card if multiple
//   cards are installed in the system.
#define DEVICE_ID   0

// Kernel krnl_cbc argument id
#define krnl_cbc_arg_MODE       0
#define krnl_cbc_arg_CBC_MODE   1
#define krnl_cbc_arg_IV_W3      2
#define krnl_cbc_arg_IV_W2      3
#define krnl_cbc_arg_IV_W1      4
#define krnl_cbc_arg_IV_W0      5
#define krnl_cbc_arg_WORDS_NUM  6
#define krnl_cbc_arg_SRC_ADDR   7
#define krnl_cbc_arg_DEST_ADDR  8

// Kernel krnl_aes argument id
#define krnl_aes_arg_MODE       0 
#define krnl_aes_arg_KEY_LEN    1 
#define krnl_aes_arg_STATUS     2 
#define krnl_aes_arg_KEY_W7     3 
#define krnl_aes_arg_KEY_W6     4 
#define krnl_aes_arg_KEY_W5     5 
#define krnl_aes_arg_KEY_W4     6 
#define krnl_aes_arg_KEY_W3     7 
#define krnl_aes_arg_KEY_W2     8 
#define krnl_aes_arg_KEY_W1     9 
#define krnl_aes_arg_KEY_W0     10


// software implementation for AES ECB/CBC encryption/decryption, used for hardware data verification
//   cbc_mode = 0, ECB mode; cbc_mode = 1, CBC mode
//   mode = 0, decryption; mode = 1, encryption
void ref_aes_cipher (const unsigned char* input, unsigned char* output, const unsigned char* key, unsigned char* iv, int words, int groups, int cbc_mode, int mode )
{
    unsigned char *in_data, *out_data;
    AES_KEY aes_key;
    memset(&aes_key, 0x00, sizeof(AES_KEY));
	if (mode) {
        if (AES_set_encrypt_key(key, 256, &aes_key) < 0) {
		    std::cout << "[ERROR] Unable to set encryption key in AES..." << std::endl;
		    return;
        }
	} else {
        if (AES_set_decrypt_key(key, 256, &aes_key) < 0) {
		    std::cout << "[ERROR] Unable to set encryption key in AES..." << std::endl;
		    return;
        }
    }

    if (cbc_mode) {
        for (int g = 0; g < groups; g++) {
            unsigned char temp_iv[16];
            memcpy(temp_iv, iv, 16);
            in_data = (unsigned char*)((unsigned long long)input + words * 16 * g);
            out_data = (unsigned char*)((unsigned long long)output + words * 16 * g);
            if (mode) {
                AES_cbc_encrypt(in_data, out_data, words*16, &aes_key, temp_iv, AES_ENCRYPT);
            } else {
                AES_cbc_encrypt(in_data, out_data, words*16, &aes_key, temp_iv, AES_DECRYPT);
            }
        }
    } else {
        for (int g = 0; g < groups; g++) {
            for (int w = 0; w < words; w++) {
                in_data = (unsigned char*)((unsigned long long)input + words * 16 * g + w * 16);
                out_data = (unsigned char*)((unsigned long long)output + words * 16 * g + w * 16);
                if (mode) {
                    AES_ecb_encrypt(in_data, out_data, &aes_key, AES_ENCRYPT);
                } else {
                    AES_ecb_encrypt(in_data, out_data, &aes_key, AES_DECRYPT);
                }
            }
        }
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

void print_help(void) {
    std::cout << std::endl << "    Usage: host_krnl_cbc_test [-g GROUP_NUM] [-w WORD_NUM] [-k KEY] [-v IV] [-v LOOP_NUM] [-s] [-h]" << std::endl;
    std::cout << "      Use -s to emulate ap_ctrl_hs mode, namely wait for ap_done before new ap_start is issued" << std::endl;
    std::cout << "      GROUP_NUM : data group or packet number to be processed, less than 256, default 256" << std::endl;
    std::cout << "      WORD_NUM  : 128-bit word number per group (packet), default 256, should be times of 16" << std::endl;
    std::cout << "      KEY       : 256-bit AES KEY" << std::endl;
    std::cout << "      IV        : 128-bit initial vector for CBC mode" << std::endl;
}

// kernel running thread
void execute_cipher_thread(xrt::run run_krnl_cbc, xrt::bo input_buffer, xrt::bo output_buffer, std::vector<int> iv, int words_num, int mode, int cbc_mode) {
    run_krnl_cbc.set_arg(krnl_cbc_arg_CBC_MODE, cbc_mode);
    run_krnl_cbc.set_arg(krnl_cbc_arg_WORDS_NUM, words_num);
    run_krnl_cbc.set_arg(krnl_cbc_arg_IV_W3, iv[3]);
    run_krnl_cbc.set_arg(krnl_cbc_arg_IV_W2, iv[2]);
    run_krnl_cbc.set_arg(krnl_cbc_arg_IV_W1, iv[1]);
    run_krnl_cbc.set_arg(krnl_cbc_arg_IV_W0, iv[0]);
    run_krnl_cbc.set_arg(krnl_cbc_arg_MODE, mode);
    run_krnl_cbc.set_arg(krnl_cbc_arg_SRC_ADDR, input_buffer);
    run_krnl_cbc.set_arg(krnl_cbc_arg_DEST_ADDR, output_buffer);
    run_krnl_cbc.start();
    run_krnl_cbc.wait();
}

// call kernel with multi-threading
float execute_cipher_mt(xrt::kernel kernel,                     // kernel handle
                        std::vector<xrt::bo> input_sub_buffer,  // input sub buffer array
                        std::vector<xrt::bo> output_sub_buffer, // output sub buffer array
                        std::vector<int> iv,                     // initial vector
                        int groups_num,                          // groups number
                        int words_num,                           // words number
                        int ctrl_mode,                           // 0 - ap_ctrl_hs; 1 - ap_ctrl_chain
                        int encryption_mode,                     // 0 - decryption; 1 - encryption
                        int cbc_mode)                            // 0 - ECB mode; 1 - CBC mode
{
    struct timeval kernels_start_time, kernels_finish_time; // kernel execution time record

    int thread_num = groups_num;    // thread number used to handle all data
    
    std::vector<xrt::run> run_krnl_cbc;
    std::vector<std::thread> t(thread_num);
    
    for (int i = 0; i < thread_num; i++)
    {
        run_krnl_cbc.push_back(xrt::run(kernel));
    }

    gettimeofday(&kernels_start_time, NULL);

    if (ctrl_mode) // ap_ctrl_chain running mode
    {
        int residue = groups_num % thread_num;
        int i, k;
        for (k = 0; k < groups_num / thread_num; k++)
        {
            for (i = 0; i < thread_num; i++)
            {
                t[i] = std::thread(execute_cipher_thread, run_krnl_cbc[i], input_sub_buffer[k * thread_num + i], output_sub_buffer[k * thread_num + i],
                            iv, words_num, encryption_mode, cbc_mode);
            }
            for (i = 0; i < thread_num; i++)
            {
                t[i].join();
            }
        }
        for (i = 0; i < residue; i++)
        {
            //run_krnl_cbc[i].set_arg(krnl_cbc_arg_SRC_ADDR, input_sub_buffer[k * thread_num + i]);   // use sub-buffer for source pointer argument
            //run_krnl_cbc[i].set_arg(krnl_cbc_arg_DEST_ADDR, output_sub_buffer[k * thread_num + i]); // use sub-buffer for source pointer argument
            t[i] = std::thread(execute_cipher_thread, run_krnl_cbc[i], input_sub_buffer[k * thread_num + i], output_sub_buffer[k * thread_num + i],
                            iv, words_num, encryption_mode, cbc_mode);
        }
        for (i = 0; i < residue; i++)
        {
            t[i].join();
        }
    }
    else // emulate ap_ctrl_hs running mode
    {
        for (int i = 0; i < groups_num; i++) {
            execute_cipher_thread(run_krnl_cbc[0], input_sub_buffer[i], output_sub_buffer[i], iv, words_num, encryption_mode, cbc_mode);
        }
    }

    gettimeofday(&kernels_finish_time, NULL);
    
    return time_dura(kernels_start_time, kernels_finish_time);  
}




// Main program body
int main(int argc, char *argv[]) {

    int opt;
    const char *optstring = "g:w:k:v:sh";

    int groups_num = 256; 
    int words_num = 256;
    std::string key = "123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0";
    std::string iv = "FEDCBA9876543210FEDCBA9876543210";
    int chain = 1;      // 0 means ap_ctrl_hs mode, 1 mean ap_ctrl_chain mode

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        if ((opt == 'g') && optarg) {
            groups_num = std::stoi(std::string(optarg));
        }

        if ((opt == 'w') && optarg) {
            words_num = std::stoi(std::string(optarg));
        }

        if ((opt == 'k') && optarg) {
            key = std::string(optarg);
        }

        if ((opt == 'v') && optarg) {
            iv = std::string(optarg);
        }

        if (opt == 's') {
            chain = 0;
        }

        if (opt == 'h') {
            print_help();
            return EXIT_SUCCESS;
        }
    }

    std::cout << std::endl;
    std::cout << "------------------------ krnl_cbc test program ------------------------" << std::endl;
    std::cout << " words number per group : " << words_num << std::endl;
    std::cout << "          groups number : " << groups_num << std::endl;
    std::cout << "             Key length : 256-bit" << std::endl;
    std::cout << "                    Key : " << key << std::endl;
    std::cout << "         initial vector : " << iv << std::endl;
    if (chain) {
        std::cout << "               run mode : ap_ctrl_chain" << std::endl;
    } else {
        std::cout << "               run mode : ap_ctrl_hs" << std::endl;
    }
     
// generate key and iv byte array for openssl library
    unsigned char key_byte[32], iv_byte[16];
    for (int i = 0; i < 32; i++) {
        key_byte[i] = (unsigned char)std::stoll(key.substr(i*2,2), nullptr, 16);
    }

    for (int i = 0; i < 16; i++) {
        iv_byte[i] = (unsigned char)std::stoll(iv.substr(i*2,2), nullptr, 16);
    }


// --------------------------------------------------------------------------------------
// Generate and read plain/cipher data into host buffer
// --------------------------------------------------------------------------------------
    char *plain_data, *cipher_ecb_data, *cipher_cbc_data;
    plain_data  = new char [words_num * 16 * groups_num];
    cipher_ecb_data  = new char [words_num * 16 * groups_num];
    cipher_cbc_data  = new char [words_num * 16 * groups_num];

    char *output_data = new char [words_num * 16 * groups_num];

    // Call perl script and openssl tool to generate "plain_x.dat", "cipher_ecb_x.dat" and "cipher_cbc_x.dat" files.
    //   Then read the generated bin file into array
    std::cout << std::endl << "Generating plain data" << std::endl;
    //srand((unsigned)time(NULL));
    for (int i = 0; i < words_num * 16 * groups_num; i++) {
        plain_data[i] = rand()%256;
    }

    ref_aes_cipher ((unsigned char*) plain_data, (unsigned char*)cipher_ecb_data, key_byte, iv_byte, words_num, groups_num, 0, 1 );
    ref_aes_cipher ((unsigned char*) plain_data, (unsigned char*)cipher_cbc_data, key_byte, iv_byte, words_num, groups_num, 1, 1 );

    // write out reference data for reference
    write_bin_file("./plain.dat", words_num * 16 * groups_num, plain_data);
    write_bin_file("./cipher_ecb.dat", words_num * 16 * groups_num, cipher_ecb_data);
    write_bin_file("./cipher_cbc.dat", words_num * 16 * groups_num, cipher_cbc_data);


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
            xclbin_file = "krnl_cbc_test_hw_emu.xclbin";
        }
        else
        {
            std::cout << "[ERROR] Unsupported Emulation Mode: " << mode << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        std::cout << "Program running in hardware mode" << std::endl;
        xclbin_file = "krnl_cbc_test_hw.xclbin";
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

    xrt::kernel kernel_krnl_cbc = xrt::kernel(device, xclbin_uuid, 
                                                "krnl_cbc",
                                                xrt::kernel::cu_access_mode::exclusive);

    // get memory bank groups for axi read master address pointer argument (same as write master)
    xrtMemoryGroup bank_grp_krnl_cbc_read  = kernel_krnl_cbc.group_id (krnl_cbc_arg_SRC_ADDR);
    xrtMemoryGroup bank_grp_krnl_cbc_write = kernel_krnl_cbc.group_id (krnl_cbc_arg_DEST_ADDR);

    // create device buffer objects
    std::cout << "Create input and output device buffers" << std::endl;
    xrt::bo krnl_cbc_input_buffer = xrt::bo (device, words_num*16*groups_num, xrt::bo::flags::normal, bank_grp_krnl_cbc_read);
    xrt::bo krnl_cbc_output_buffer = xrt::bo (device, words_num*16*groups_num, xrt::bo::flags::normal, bank_grp_krnl_cbc_write);
    
    // create sub-buffer objects
    std::vector<xrt::bo> krnl_cbc_input_sub_buffer;
    std::vector<xrt::bo> krnl_cbc_output_sub_buffer;
    for (int i = 0; i < groups_num; i++) {
        krnl_cbc_input_sub_buffer.push_back(xrt::bo(krnl_cbc_input_buffer, words_num * 16, words_num * 16 * i));
        krnl_cbc_output_sub_buffer.push_back(xrt::bo(krnl_cbc_output_buffer, words_num * 16, words_num * 16 * i));
    }

// --------------------------------------------------------------------------------------
// krnl_aes kernel: finish AES key expansion operation
// --------------------------------------------------------------------------------------
    std::cout << "Run krnl_aes for AES key expansion" << std::endl;

    // create kernel runner instance
    xrt::run run_krnl_aes_key_exp(kernel_krnl_aes);     // runner for krnl_aes (only for key expansion operation)
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_MODE,      1);      // set to encryption mode
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_LEN,   3);      // key length is 256-bit
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W7,    std::stoll(key.substr(0,  8), nullptr, 16));   // KEY word 7
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W6,    std::stoll(key.substr(8,  8), nullptr, 16));   // KEY word 6
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W5,    std::stoll(key.substr(16, 8), nullptr, 16));   // KEY word 5
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W4,    std::stoll(key.substr(24, 8), nullptr, 16));   // KEY word 4
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W3,    std::stoll(key.substr(32, 8), nullptr, 16));   // KEY word 3
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W2,    std::stoll(key.substr(40, 8), nullptr, 16));   // KEY word 2
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W1,    std::stoll(key.substr(48, 8), nullptr, 16));   // KEY word 1
    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_KEY_W0,    std::stoll(key.substr(56, 8), nullptr, 16));   // KEY word 0
    run_krnl_aes_key_exp.start();
    run_krnl_aes_key_exp.wait();
    std::cout << "krnl_aes run finish" << std::endl;


    float total_run_time = 0;
    std::vector <int> iv_int(4);
    iv_int[3] = (int)std::stoll(iv.substr(0, 8),  nullptr, 16);
    iv_int[2] = (int)std::stoll(iv.substr(8, 8),  nullptr, 16);
    iv_int[1] = (int)std::stoll(iv.substr(16, 8), nullptr, 16);
    iv_int[0] = (int)std::stoll(iv.substr(24, 8), nullptr, 16);    

    int fail_count;

// --------------------------------------------------------------------------------------
// ECB Mode Encryption test
// --------------------------------------------------------------------------------------
    std::cout << std::endl << "-------------- AES ECB Encryption Test " << std::endl;

    // write plain_data into input device buffer
    krnl_cbc_input_buffer.write(plain_data);
    krnl_cbc_input_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Run the kernel
    total_run_time = execute_cipher_mt(kernel_krnl_cbc, krnl_cbc_input_sub_buffer, krnl_cbc_output_sub_buffer, iv_int,
                                      groups_num, words_num, chain, 1, 0);

    // Dump result data from output device buffer
    krnl_cbc_output_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    krnl_cbc_output_buffer.read(output_data);
    
    // Data validation
    fail_count = data_compare((char*)output_data, cipher_ecb_data, words_num * 16 * groups_num);
    if (fail_count != 0) {
        std::cout << "Data validation FAILED, error num = " << fail_count << std::endl;
    } else {
        std::cout << "Data validation SUCCESS" << std::endl;
    }
    
    std::cout << "Execution time = " << total_run_time << " ms" << std::endl;
    std::cout << "Throughput = " << words_num * groups_num * 16 / total_run_time * 1000 / (1024 * 1024) << " MB/s" << std::endl << std::endl;


// --------------------------------------------------------------------------------------
// ECB Mode Decryption test
// --------------------------------------------------------------------------------------
    std::cout << std::endl << "-------------- AES ECB Decryption Test " << std::endl;

    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_MODE,      0);      // set to decryption mode
    // use kernel run to update the krnl_aes register
    run_krnl_aes_key_exp.start();
    run_krnl_aes_key_exp.wait();

    // write plain_data into input device buffer
    krnl_cbc_input_buffer.write(cipher_ecb_data);
    krnl_cbc_input_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Run the kernel
    total_run_time = execute_cipher_mt(kernel_krnl_cbc, krnl_cbc_input_sub_buffer, krnl_cbc_output_sub_buffer, iv_int,
                                      groups_num, words_num, chain, 0, 0);

    // Dump result data from output device buffer
    krnl_cbc_output_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    krnl_cbc_output_buffer.read(output_data);
    
    // Data validation
    fail_count = data_compare((char*)output_data, plain_data, words_num * 16 * groups_num);
    if (fail_count != 0) {
        std::cout << "Data validation FAILED, error num = " << fail_count << std::endl;
    } else {
        std::cout << "Data validation SUCCESS" << std::endl;
    }
    
    std::cout << "Execution time = " << total_run_time << " ms" << std::endl;
    std::cout << "Throughput = " << words_num * groups_num * 16 / total_run_time * 1000 / (1024 * 1024) << " MB/s" << std::endl << std::endl;



// --------------------------------------------------------------------------------------
// CBC Mode Encryption test
// --------------------------------------------------------------------------------------
    std::cout << std::endl << "-------------- AES CBC Encryption Test " << std::endl;

    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_MODE,      1);      // set to encryption mode
    // use kernel run to update the krnl_aes register
    run_krnl_aes_key_exp.start();
    run_krnl_aes_key_exp.wait();

    // write plain_data into input device buffer
    krnl_cbc_input_buffer.write(plain_data);
    krnl_cbc_input_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Run the kernel
    total_run_time = execute_cipher_mt(kernel_krnl_cbc, krnl_cbc_input_sub_buffer, krnl_cbc_output_sub_buffer, iv_int,
                                      groups_num, words_num, chain, 1, 1);

    // Dump result data from output device buffer
    krnl_cbc_output_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    krnl_cbc_output_buffer.read(output_data);
    
    // Data validation
    fail_count = data_compare((char*)output_data, cipher_cbc_data, words_num * 16 * groups_num);
    if (fail_count != 0) {
        std::cout << "Data validation FAILED, error num = " << fail_count << std::endl;
    } else {
        std::cout << "Data validation SUCCESS" << std::endl;
    }
    
    std::cout << "Execution time = " << total_run_time << " ms" << std::endl;
    std::cout << "Throughput = " << words_num * groups_num * 16 / total_run_time * 1000 / (1024 * 1024) << " MB/s" << std::endl << std::endl;


// --------------------------------------------------------------------------------------
// CBC Mode Decryption test
// --------------------------------------------------------------------------------------
    std::cout << std::endl << "-------------- AES CBC Decryption Test " << std::endl;

    run_krnl_aes_key_exp.set_arg(krnl_aes_arg_MODE,      0);      // set to encryption mode
    // use kernel run to update the krnl_aes register
    run_krnl_aes_key_exp.start();
    run_krnl_aes_key_exp.wait();

    // write plain_data into input device buffer
    krnl_cbc_input_buffer.write(cipher_cbc_data);
    krnl_cbc_input_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Run the kernel
    total_run_time = execute_cipher_mt(kernel_krnl_cbc, krnl_cbc_input_sub_buffer, krnl_cbc_output_sub_buffer, iv_int,
                                      groups_num, words_num, chain, 0, 1);

    // Dump result data from output device buffer
    krnl_cbc_output_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    krnl_cbc_output_buffer.read(output_data);
    
    // Data validation
    fail_count = data_compare((char*)output_data, plain_data, words_num * 16 * groups_num);
    if (fail_count != 0) {
        std::cout << "Data validation FAILED, error num = " << fail_count << std::endl;
    } else {
        std::cout << "Data validation SUCCESS" << std::endl;
    }
    
    std::cout << "Execution time = " << total_run_time << " ms" << std::endl;
    std::cout << "Throughput = " << words_num * groups_num * 16 / total_run_time * 1000 / (1024 * 1024) << " MB/s" << std::endl << std::endl;


}
