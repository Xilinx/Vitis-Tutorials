/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <math.h>
#include <stdio.h>

#include "cholesky_kernel.hpp"

//! Core function of Cholesky benchmark
int main(int argc, const char* argv[]) {

   const int dataAM = 16, dataAN = 16;
   std::cout << "INFO: Matrix Row M: " << dataAM << std::endl;
   std::cout << "INFO: Matrix Col N: " << dataAN << std::endl;

   double dataA[dataAN*dataAM];
   double dataC[dataAN][dataAM];

   FILE *fp;
   double tmp;

   fp=fopen("tb_data/matrix_input.dat","r");
   for (int i = 0; i < dataAN*dataAM; i++){
      fscanf(fp, "%lf", &tmp);
      dataA[i] = tmp;
   }
   fclose(fp);

   fp=fopen("tb_data/golden_result.dat","r");
   for (int i = 0; i < dataAN; i++){
      for (int j = 0; j < dataAM; j++){
         fscanf(fp, "%lf", &tmp);
         dataC[i][j] = tmp;
      }
   }
   fclose(fp);

   cholesky_kernel(dataAN, dataA);

   // Calculate err between dataA and dataC
   double errA = 0;
   for (int i = 0; i < dataAM; i++) {
      for (int j = 0; j <= i; j++) {
         errA += (dataA[i * dataAN + j] - dataC[i][j]) * (dataA[i * dataAN + j] - dataC[i][j]);
      }
   }
   errA = std::sqrt(errA);
   std::cout << "errA = " << errA << std::endl;
   std::cout << "-------------- " << std::endl;
   if (errA > 0.0001) {
      std::cout << "INFO: Result false" << std::endl;
      std::cout << "-------------- " << std::endl;
      return -1;
   }
   else {
      std::cout << "INFO: Result correct" << std::endl;
      std::cout << "-------------- " << std::endl;
      return 0;
   }
}
