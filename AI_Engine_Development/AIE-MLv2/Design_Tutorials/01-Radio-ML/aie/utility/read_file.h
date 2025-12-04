//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani
#ifndef READ_FILE_H
#define READ_FILE_H
template<typename T, size_t N>
void read_file(std::string file_name, T *data_array) {
  std::ifstream ss_i;
  ss_i.open(file_name,std::ifstream::in);
  if ( ss_i.is_open() == 0 ) {
    std::cout << "ERROR:   failed to open " << file_name << std::endl;
  }
  else {
    std::cout << "PASSED:  Successfully read input file " << file_name << std::endl;
  }
  for (unsigned ss=0; ss < N; ss++) {
    float val;
    ss_i >> val;
    T buff = (T) val;
    data_array[ss] = buff;
  }
  ss_i.close();
}
#endif
