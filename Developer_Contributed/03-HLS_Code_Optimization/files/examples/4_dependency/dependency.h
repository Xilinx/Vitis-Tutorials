/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#ifndef H_DEPENDENCY_H
#define H_DEPENDENCY_H

const int nOfSamples = 4*1024;
const int nOfBins    =  256;

void top_histogram(unsigned char din[nOfSamples], unsigned short int dout[nOfBins], const int n_Of_Samples, const int n_Of_Bins);



#endif
