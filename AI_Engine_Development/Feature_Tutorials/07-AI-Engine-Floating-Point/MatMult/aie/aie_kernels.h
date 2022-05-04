/*
 * (c) Copyright 2020–2022 Xilinx, Inc.. All rights reserved.
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
 *
 */
#pragma once

void matmult_float(
  input_window_float* __restrict matA,
  input_window_float* __restrict matB,
  output_window_float* __restrict matC);

void matmult_cfloat(
  input_window_cfloat* __restrict matA,
  input_window_cfloat* __restrict matB,
  output_window_cfloat* __restrict matC);

  void matmult_cfloat_conf(
    input_window_cfloat* __restrict matA,
    input_window_cfloat* __restrict matB,
    output_window_cfloat* __restrict matC);

    void matmult_cfloat_conj(
      input_window_cfloat* __restrict matA,
      input_window_cfloat* __restrict matB,
      output_window_cfloat* __restrict matC);

