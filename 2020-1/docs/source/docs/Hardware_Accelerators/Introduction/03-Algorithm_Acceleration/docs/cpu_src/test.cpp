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
#include <vector>
#include <cstdio>

//#include "xcl2.hpp"

#include "matrixUtility.hpp"

void cpu_cholesky(int diagSize, double *matrixA);

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
    int dataAM, dataAN, seed;

    // Read In paths addresses

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
    // Setting maximum size to 2048
    if (dataAM > 2048) {
        dataAM = 2048;
        std::cout << "INFO: Limiting size to 2048" << std::endl;
    } 
    // Setting same size for rows and columns
    dataAN = dataAM;

    // Output the inputs information
    std::cout << "INFO: Matrix Row M: " << dataAM << std::endl;
    std::cout << "INFO: Matrix Col N: " << dataAN << std::endl;

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

	// Variables to measure time
    struct timeval tstart, tend;

	  gettimeofday(&tstart, 0);
    cpu_cholesky(dataAN, dataA);
    gettimeofday(&tend, 0);
    std::cout << "INFO: Finish CPU execution" << std::endl;
	  int exec_time = diff(&tend, &tstart);
    std::cout << "INFO: CPU execution time is:" << exec_time << " us\n";

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
