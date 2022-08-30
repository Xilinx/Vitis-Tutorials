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

#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <algorithm>

#include "xcl2.hpp"

#include "matrixUtility.hpp"

// Memory alignment
template <typename T>
T* aligned_alloc(std::size_t num) {
    void* ptr = nullptr;
    if (posix_memalign(&ptr, 4096, num * sizeof(T))) {
        throw std::bad_alloc();
    }
    return reinterpret_cast<T*>(ptr);
}

// Compute time difference
unsigned long diff(const struct timeval* newTime, const struct timeval* oldTime) {
    return (newTime->tv_sec - oldTime->tv_sec) * 1000000 + (newTime->tv_usec - oldTime->tv_usec);
}

// Arguments parser
class ArgParser {
   public:
    ArgParser(int& argc, const char** argv) {
        for (int i = 1; i < argc; ++i) mTokens.push_back(std::string(argv[i]));
    }
    bool getCmdOption(const std::string option, std::string& value) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->mTokens.begin(), this->mTokens.end(), option);
        if (itr != this->mTokens.end() && ++itr != this->mTokens.end()) {
            value = *itr;
            return true;
        }
        return false;
    }

   private:
    std::vector<std::string> mTokens;
};

//! Core function of Cholesky benchmark
int main(int argc, const char* argv[]) {
    // Initialize parser
    ArgParser parser(argc, argv);

    // Initialize paths addresses
    std::string xclbin_path;
    std::string num_str;
    int num_runs, dataAM, dataAN, seed;

    // Read In paths addresses
    if (!parser.getCmdOption("-xclbin", xclbin_path)) {
        std::cout << "INFO:input path is not set!\n";
    }
    if (!parser.getCmdOption("-runs", num_str)) {
        num_runs = 1;
        std::cout << "INFO:number runs is not set!\n";
    } else {
        num_runs = std::stoi(num_str);
    }
    if (!parser.getCmdOption("-M", num_str)) {
        dataAM = 16;
        std::cout << "INFO:row size M is not set!\n";
    } else {
        dataAM = std::stoi(num_str);
    }
    if (!parser.getCmdOption("-N", num_str)) {
        dataAN = 16;
        std::cout << "INFO:column size N is not set!\n";
    } else {
        dataAN = std::stoi(num_str);
    }
    if (!parser.getCmdOption("-seed", num_str)) {
        seed = 12;
        std::cout << "INFO:seed is not set!\n";
    } else {
        seed = std::stoi(num_str);
    }

    // dataAM = dataAN is valid only for symmetric matrix
    dataAM = (dataAM > dataAN) ? dataAN : dataAM;
    dataAN = dataAM;

    // Platform related operations
    std::vector<cl::Device> devices = xcl::get_xil_devices();
    cl::Device device = devices[0];

    // Creating Context and Command Queue for selected Device
    cl::Context context(device);
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
    std::string devName = device.getInfo<CL_DEVICE_NAME>();
    printf("INFO: Found Device=%s\n", devName.c_str());

    cl::Program::Binaries xclBins = xcl::import_binary_file(xclbin_path);
    devices.resize(1);
    cl::Program program(context, devices, xclBins);
    cl::Kernel cholesky_kernel(program, "cholesky_kernel");
    std::cout << "INFO: Kernel has been created" << std::endl;

    // Output the inputs information
    std::cout << "INFO: Number of kernel runs: " << num_runs << std::endl;
    std::cout << "INFO: Matrix Row M: " << dataAM << std::endl;
    std::cout << "INFO: Matrix Col N: " << dataAN << std::endl;

    // Initialization of host buffers

    const int MAXN = dataAN, LDA = dataAN;
    int inout_size = MAXN * MAXN;
    double* dataA;
    dataA = aligned_alloc<double>(inout_size);

    // Generate general matrix dataAM x dataAN
    double** dataC = new double*[dataAM];
    double** dataD = new double*[dataAM];
    double** dataE = new double*[dataAM];
    for (int i = 0; i < dataAM; ++i) {
        dataC[i] = new double[dataAN];
        dataD[i] = new double[dataAN];
        dataE[i] = new double[dataAN];
    }
    triLowerMatGen<double>(dataAN, seed, dataC);
    transposeMat<double>(dataAN, dataC, dataD);
    MulMat<double>(dataAM, dataAN, dataAN, dataC, dataD, dataE);

    for (int i = 0; i < dataAM; ++i) {
        for (int j = 0; j < dataAN; ++j) {
            dataA[i * LDA + j] = dataE[i][j];
        }
    }

    // DDR Settings
    std::vector<cl_mem_ext_ptr_t> mext_io(1);
    mext_io[0].flags = XCL_MEM_DDR_BANK0;
    mext_io[0].obj = dataA;
    mext_io[0].param = 0;

    // Create device buffer and map dev buf to host buf
    std::vector<cl::Buffer> buffer(1);

    buffer[0] = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                           sizeof(double) * inout_size, &mext_io[0]);

    // Data transfer from host buffer to device buffer
    std::vector<std::vector<cl::Event> > kernel_evt(2);
    kernel_evt[0].resize(1);
    kernel_evt[1].resize(1);

    std::vector<cl::Memory> ob_io;
    ob_io.push_back(buffer[0]);

    q.enqueueMigrateMemObjects(ob_io, 0, nullptr, &kernel_evt[0][0]); // 0 : migrate from host to dev
    q.finish();
    std::cout << "INFO: Finish data transfer from host to device" << std::endl;

    // Setup kernel
    cholesky_kernel.setArg(0, dataAN);
    cholesky_kernel.setArg(1, buffer[0]);
    q.finish();
    std::cout << "INFO: Finish kernel setup" << std::endl;

    // Variables to measure time
    struct timeval tstart, tend;

    // Launch kernel and compute kernel execution time
    gettimeofday(&tstart, 0);
    for (int i = 0; i < num_runs; ++i) {
        q.enqueueTask(cholesky_kernel, nullptr, nullptr);
    }
    q.finish();
    gettimeofday(&tend, 0);
    std::cout << "INFO: Finish kernel execution" << std::endl;
    int exec_time = diff(&tend, &tstart);
    std::cout << "INFO: FPGA execution time of " << num_runs << " runs:" << exec_time << " us\n"
              << "INFO: Average execution time per run: " << exec_time / num_runs << " us\n";

    // Data transfer from device buffer to host buffer
    q.enqueueMigrateMemObjects(ob_io, 1, nullptr, nullptr); // 1 : migrate from dev to host
    q.finish();

    // Calculate err between dataA and dataC
    double errA = 0;
    for (int i = 0; i < dataAM; i++) {
        for (int j = 0; j <= i; j++) {
            errA += (dataA[i * LDA + j] - dataC[i][j]) * (dataA[i * LDA + j] - dataC[i][j]);
        }
    }
    errA = std::sqrt(errA);
    std::cout << "errA = " << errA << std::endl;
    std::cout << "dataAN = " << dataAN << std::endl;
    std::cout << "dataAM = " << dataAM << std::endl;
    std::cout << "-------------- " << std::endl;
    if (errA > 0.0001) {
        std::cout << "INFO: Result false" << std::endl;
        std::cout << "-------------- " << std::endl;
        return -1;
    } else {
        std::cout << "INFO: Result correct" << std::endl;
        std::cout << "-------------- " << std::endl;
        return 0;
    }
}
