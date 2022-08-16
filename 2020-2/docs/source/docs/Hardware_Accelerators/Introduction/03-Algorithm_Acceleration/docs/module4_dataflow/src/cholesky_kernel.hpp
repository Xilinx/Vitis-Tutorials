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
 * WITHOUT WANCUNCUANTIES ONCU CONDITIONS OF ANY KIND, either express or
 * implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @file potrf.hpp
 * @brief  This files contains implementation of Cholesky Decomposition.
 */

#include <hls_math.h>

namespace internal {

template <typename T, int N, int NCU>
void chol_col(int n, T dataA[(N + NCU - 1) / NCU][N], T dataj[N], T tmp1_i, int num, int j) {
Loop_per_Unit:
for (int p = (j + 1) / NCU; p < (n + NCU - 1) / NCU; p++) {
   #pragma HLS loop_tripcount min = 1 max = 8
   T tmp_i[16] = {0}, tmp3_i, tmp1[8], tmp2[4], tmp3[2];
   #pragma HLS resource variable = tmp_i core = RAM_2P_LUTRAM

   Loop_vec_mul:
   for (int k = 0; k < j; k++) {
   #pragma HLS loop_tripcount min = 1 max = N
   #pragma HLS pipeline
   #pragma HLS dependence variable = tmp_i inter false
   #pragma HLS dependence variable = dataA inter false
      tmp_i[k % 16] += dataA[p][k] * dataj[k];
   }

   Loop_add_1:
   for (int j = 0; j < 8; j++) {
      #pragma HLS pipeline
      tmp1[j] = tmp_i[j] + tmp_i[j + 8];
   }

   Loop_add_2:
   for (int j = 0; j < 4; j++) {
   #pragma HLS pipeline
      tmp2[j] = tmp1[j] + tmp1[j + 4];
   }

   Loop_add_3:
   for (int j = 0; j < 2; j++) {
   #pragma HLS pipeline
      tmp3[j] = tmp2[j] + tmp2[j + 2];
   }

   tmp3_i = tmp3[0] + tmp3[1];

   if (p * NCU + num > j) dataA[p][j] = (dataA[p][j] - tmp3_i) / tmp1_i;
   }
}

template <typename T, int N, int NCU>
void chol_jj(T dataA[NCU][(N + NCU - 1) / NCU][N], T dataj[NCU][N], T& tmp1_j, int& j) {
   T tmp[16] = {0}, tmp3_j, tmp1[8], tmp2[4], tmp3[2];
   #pragma HLS resource variable = tmp core = RAM_2P_LUTRAM

   Loop_vec_mul_jj:
   for (int k = 0; k < j; k++) {
   #pragma HLS pipeline
   #pragma HLS dependence variable = tmp inter false
   #pragma HLS dependence variable = dataA inter false
      T tmp2_j = dataA[j % NCU][j / NCU][k];
      tmp[k % 16] += tmp2_j * tmp2_j;
      for (int p = 0; p < NCU; p++) {
      #pragma HLS unroll
         dataj[p][k] = tmp2_j;
      }
   }

   Loop_add_1:
   for (int j = 0; j < 8; j++) {
   #pragma HLS pipeline
      tmp1[j] = tmp[j] + tmp[j + 8];
   }

   Loop_add_2:
   for (int j = 0; j < 4; j++) {
   #pragma HLS pipeline
      tmp2[j] = tmp1[j] + tmp1[j + 4];
   }

   Loop_add_3:
   for (int j = 0; j < 2; j++) {
   #pragma HLS pipeline
      tmp3[j] = tmp2[j] + tmp2[j + 2];
   }

   tmp3_j = tmp3[0] + tmp3[1];
   tmp1_j = hls::sqrt(dataA[j % NCU][j / NCU][j] - tmp3_j);
   dataA[j % NCU][j / NCU][j] = tmp1_j;
}

template <typename T, int N, int NCU>
void chol_col_wrapper(int n, T dataA[NCU][(N + NCU - 1) / NCU][N], T dataj[NCU][N], T tmp1, int j) {
   #pragma HLS DATAFLOW

   Loop_row:
   for (int num = 0; num < NCU; num++) {
   #pragma HLS unroll factor = NCU

      chol_col<T, N, NCU>(n, dataA[num], dataj[num], tmp1, num, j);
   }
}

template <typename T, int N, int NCU>
void cholesky_core(int n, T dataA[NCU][(N + NCU - 1) / NCU][N]) {
   T tmp1, dataj[NCU][N];
   #pragma HLS array_partition variable = dataj cyclic factor = NCU dim = 1

   Loop_col:
   for (int j = 0; j < n; ++j) {
      chol_jj<T, N, NCU>(dataA, dataj, tmp1, j);
      chol_col_wrapper<T, N, NCU>(n, dataA, dataj, tmp1, j);
   }
}

} // namespace internal

template <typename T, int NMAX, int NCU>
void cholesky(int m, T* A, int lda, int& info) {
   if (NMAX == 1)
      A[0] = hls::sqrt(A[0]);
   else {
      static T matA[NCU][(NMAX + NCU - 1) / NCU][NMAX];
      #pragma HLS array_partition variable = matA cyclic factor = NCU dim = 1
      #pragma HLS resource variable = matA core = XPM_MEMORY uram

      Loop_read:
      for (int r = 0; r < m; r++) {
         for (int c = 0; c < m; c++) {
         #pragma HLS pipeline
         matA[r % NCU][r / NCU][c] = A[r * lda + c];
         }
      }

      internal::cholesky_core<T, NMAX, NCU>(m, matA);

      Loop_write:
      for (int r = 0; r < m; r++) {
         for (int c = 0; c < m; c++) {
         #pragma HLS pipeline
            A[r * lda + c] = matA[r % NCU][r / NCU][c];
         }
      }
   }
}
