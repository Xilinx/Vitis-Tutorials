/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#ifndef _GRAPH_H_
#define _GRAPH_H_
   
   #include <adf.h>
   #include "matrix_mult_graph.hpp"
   
   using namespace adf;
   #define OPTIMIZED_OVERLAY 1
   #if OPTIMIZED_OVERLAY
   #define SPLIT 3
   #define CASC_LN 8
   #define N_SAMPLES 1
   
   //defining DIM_A, DIM_B, DIM_AB
   #if GEMM_SIZE == 32
   #define DIM_A 16 
   #define DIM_B 12
   #elif GEMM_SIZE == 64
   #define DIM_A 32 
   #define DIM_B 24
   #elif GEMM_SIZE == 128
   #define DIM_A 44 
   #define DIM_B 44 
   #elif GEMM_SIZE == 256
   #define DIM_A 44 
   #define DIM_B 44 
   #elif GEMM_SIZE == 512
   #define DIM_A 16 
   #define DIM_B 16
   #elif GEMM_SIZE == 1024
   #define DIM_A 16 
   #define DIM_B 16
   #endif
   #define DIM_AB (GEMM_SIZE)
   
   //defining GEMM_SIZE_ZP_A
   #if (GEMM_SIZE % DIM_A) == 0 
       #define GEMM_SIZE_ZP_A GEMM_SIZE			
   #else 
       #define GEMM_SIZE_ZP_A (GEMM_SIZE - (GEMM_SIZE % DIM_A) + DIM_A)	
   #endif
   
   //defining GEMM_SIZE_ZP_B
   #if (GEMM_SIZE % (DIM_B*SPLIT)) == 0 
       #define GEMM_SIZE_ZP_B GEMM_SIZE			
   #else 
       #define GEMM_SIZE_ZP_B ((GEMM_SIZE) - ((GEMM_SIZE) % (DIM_B*SPLIT)) + (DIM_B*SPLIT))
   #endif

////   //defining GRAPH_ITER_CNT
//   #if ITER_CNT == -1
//       #define GRAPH_ITER_CNT ITER_CNT
//   #else
//       #if GEMM_SIZE == 32
//           #define GRAPH_ITER_CNT 2*ITER_CNT
//       #elif GEMM_SIZE == 64
//           #define GRAPH_ITER_CNT 2*ITER_CNT
//       #elif GEMM_SIZE == 128
//           #define GRAPH_ITER_CNT 3*ITER_CNT
//       #elif GEMM_SIZE == 256
//           #define GRAPH_ITER_CNT 12*ITER_CNT
//       #elif GEMM_SIZE == 512
//           #define GRAPH_ITER_CNT 352*ITER_CNT
//       #elif GEMM_SIZE == 1024
//           #define GRAPH_ITER_CNT 1408*ITER_CNT
//       #endif
//   #endif
   
   #endif
   
   
   #define WINDOW_SIZE_A (DIM_A * DIM_AB * N_SAMPLES)
   #define WINDOW_SIZE_B (DIM_B * DIM_AB * N_SAMPLES)
   
   extern int base_col, base_row;
   
   class GeMM: public adf::graph
   {
      public:
         input_plio matA_inp[CASC_LN];
         input_plio matB_inp[(SPLIT * CASC_LN)];
         output_plio matC_out[SPLIT];
         
         GeMM() {
            // GeMM Graph Declarations...
            xf::dsp::aie::blas::matrix_mult::matrix_mult_graph<int16, int16, DIM_A, DIM_AB, DIM_B, 0, 0, \
               ROW_MAJOR, ROW_MAJOR, ROW_MAJOR, 0, 0, 0, WINDOW_SIZE_A, WINDOW_SIZE_B, CASC_LN> mmult[SPLIT];
            
            // Mat A PLIO node names...
            for(int j = 0; j < CASC_LN; ++j) {
               std::string matA_plioOut_str = "DataInA" + std::to_string(0) + "_CASC" + std::to_string(j);
               const char *matA_plioOut = matA_plioOut_str.c_str();
               
               std::string matA_Out_file_str = "a" + std::to_string(0) + "_casc" + std::to_string(j) + ".txt";
               const char *matA_Out_file = matA_Out_file_str.c_str();
               
               matA_inp[j] = input_plio::create(matA_plioOut, plio_128_bits, matA_Out_file);
            }
            
            for(int i = 0; i < SPLIT; ++i) {
               // CASC_LN No. of kernels will be created...
               adf::kernel *mmult_kernels = mmult[i].getKernels();
               
               for(int j = 0; j < CASC_LN; ++j) {
                  // Mat B PLIO node names...
                  std::string matB_plioOut_str = "DataInB" + std::to_string(i) + "_CASC" + std::to_string(j);
                  const char *matB_plioOut = matB_plioOut_str.c_str();
                  
                  std::string matB_Out_file_str = "b" + std::to_string(i) + "_casc" + std::to_string(j) + ".txt";
                  const char *matB_Out_file = matB_Out_file_str.c_str();
                  
                  matB_inp[(i * CASC_LN) + j] = input_plio::create(matB_plioOut, plio_128_bits, matB_Out_file);
               } 
               
               // Mat C PLIO node names...
               std::string matC_plioOut_str = "DataOutC" + std::to_string(i);
               const char *matC_plioOut = matC_plioOut_str.c_str();
               
               std::string matC_Out_file_str = "data/c" + std::to_string(i) + ".txt";
               const char *matC_Out_file = matC_Out_file_str.c_str();
               
               // Creating PLIO nodes...
               matC_out[i] = output_plio::create(matC_plioOut, plio_128_bits, matC_Out_file);
               
               // Connecting PLIO Nodes...
               for(int k = 0; k < CASC_LN; ++k) {
                  // Setting runtime ratio...
                  adf::runtime<ratio>(mmult_kernels[k]) = 0.8;
                  
                  // Connecting port IO nodes...
                  adf::connect<>(matA_inp[k].out[0], mmult[i].inA[k]);
                  adf::connect<>(matB_inp[(i * CASC_LN) + k].out[0], mmult[i].inB[k]);
               }
               
               // Connecting port IO nodes...
               adf::connect<>(mmult[i].out[0], matC_out[i].in[0]);
            }
            location<graph>(*this) = area_group({{aie_tile, 14, 0, 13+CASC_LN, SPLIT}});
         }
   };

#endif //ifndef _GRAPH_H_
