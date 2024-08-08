//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Donato Carlo Giorgio

#pragma once

#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "fft1k_single_kernel.h"

using namespace adf;

#define N_INST 128                       // Number of instances
#define PLIO_WIDTH 2                     // 64 bits PLIOs batch 2 samples together
#define IO_ILV 4                         // IO_ILV is the multiple the sample rate we have as PL clock
#define KER_ILV 4                        // Further ILV to spare kernel resources, does not use addition MEM tile interfaces
#define N_KERS N_INST/(REPS*KER_ILV)     // Number of kernels
#define N_IO N_INST/(IO_ILV*PLIO_WIDTH)  // Number of PLIO channels (considering 64bit-wide PLIOs)

// Define max buf as the minimum between the two formulas result rounded down.
//      #define MAX_BUF 6/(2*N_KERS/N_IO)                             // Port limited, max 3
//      #define MAX_BUF (524288/(BUFFER_SIZE*DATATYPE_BYTES*2*2))     // Memory limited
//  NOTE:   MAX_BUF can be 0. Automatically inferred locks will enforce coherency. 
#define MAX_BUF 3

class fft1k_128_graph : public graph {
    private:
        kernel k_kernel[N_KERS];
        shared_buffer<cint16> in_mem[N_IO], out_mem[N_IO];
    public:
        port<input>     din[N_IO];
        port<output>    dout[N_IO];

        fft1k_128_graph(void)
        {

        // Creating the kernels
            for(int i=0; i<N_KERS; i++){
                k_kernel[i] = kernel::create_object<fft1k_kernel>();
                source(k_kernel[i]) = "fft1k_single_kernel.cpp";
                runtime<ratio>(k_kernel[i]) = 0.9;
                location<stack>(k_kernel[i]) = location<kernel>(k_kernel[i]);
                location<buffer>(k_kernel[i].in[0]) = location<kernel>(k_kernel[i]);
                location<buffer>(k_kernel[i].out[0]) = location<kernel>(k_kernel[i]);
            }

        // Creating the shared buffers and connecting them to the I/Os and kernels
            for(int i=0; i<N_IO; i++){
                // Creating the input and output shared buffers
                in_mem[i] = shared_buffer<cint16>::create({PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS}, 1, int(PLIO_WIDTH*IO_ILV/(KER_ILV*REPS)));
                out_mem[i] = shared_buffer<cint16>::create({PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS}, int(PLIO_WIDTH*IO_ILV/(KER_ILV*REPS)), 1);
                num_buffers(in_mem[i]) = 2;     // Ping-pong buffers needed to pipeline
                num_buffers(out_mem[i]) = 2;
                if(MAX_BUF != 0){
                    if(i%MAX_BUF>0){
                        location<buffer>(in_mem[i]) = location<buffer>(in_mem[i-1]) + relative_offset({.col_offset = 0, .row_offset = 0});
                        location<buffer>(out_mem[i]) = location<buffer>(out_mem[i-1]) + relative_offset({.col_offset = 0, .row_offset = 0});}
                    location<buffer>(out_mem[i]) = location<buffer>(in_mem[i]) + relative_offset({.col_offset = 0, .row_offset = 0});}

                // Input memory management
                write_access(in_mem[i].in[0]) =
                    tiling(
                        {
                            .buffer_dimension = {PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS},      // Full buffer dimensions
                            .tiling_dimension = {PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS},      // Tiling for a single PLIO
                            .offset           = {0, 0, 0, 0},
                            .tile_traversal   = {
                                {.dimension=0, .stride=PLIO_WIDTH, .wrap=1},    // Reading one batch at a time,
                                {.dimension=1, .stride=IO_ILV/KER_ILV, .wrap=1},        // proceeding with IO_ILV, and
                                {.dimension=2, .stride=KER_ILV, .wrap=1},        // with kernel ILV
                                {.dimension=3, .stride=POINTS, .wrap=1}        // row by row
                            } 
                        });
                connect(din[i], in_mem[i].in[0]);

                // Output memory management
                read_access(out_mem[i].out[0]) =
                    tiling(
                        {
                            .buffer_dimension = {PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS},      // Full buffer dimensions
                            .tiling_dimension = {PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS},      // Tiling for a single PLIO
                            .offset           = {0, 0, 0, 0},
                            .tile_traversal   = {
                                {.dimension=0, .stride=PLIO_WIDTH, .wrap=1},         // Reading one batch at a time,
                                {.dimension=1, .stride=IO_ILV/KER_ILV, .wrap=1},             // proceeding with IO_ILV, and
                                {.dimension=2, .stride=KER_ILV, .wrap=1},             // with kernel ILV
                                {.dimension=3, .stride=POINTS, .wrap=1}             // row by row
                            }                        
                        });
                connect(out_mem[i].out[0], dout[i]);
            }

            for(int i=0; i<N_IO; i++){
                int cur = 0;
                for(int k=0; k<IO_ILV/KER_ILV; k++)
                    for(int j=0; j<PLIO_WIDTH/REPS; j++){
                    // Kernel inputs management
                        read_access(in_mem[i].out[cur]) =
                            tiling(                                                                              
                                {
                                    .buffer_dimension = {PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS},      // Full buffer dimension
                                    .tiling_dimension = {1, 1, 1, POINTS},              // 2D Tiling for each kernel
                                    .offset           = {REPS*j, k, 0, 0},                 // Justifying the read pointer
                                    .tile_traversal   = {
                                        {.dimension=3, .stride=POINTS, .wrap=1},        // Reading all the points of
                                        {.dimension=0, .stride=1, .wrap=REPS},          // each instance at a time
                                        {.dimension=2, .stride=1, .wrap=KER_ILV},        
                                        {.dimension=1, .stride=1, .wrap=1}            // IO_ILV managed by offset
                                    } 
                                });
                        connect(in_mem[i].out[cur], k_kernel[int(i*(PLIO_WIDTH*IO_ILV/(REPS*KER_ILV))+cur)].in[0]);

                        // Kernel outputs management
                        write_access(out_mem[i].in[cur]) =
                            tiling(
                                {
                                    .buffer_dimension = {PLIO_WIDTH, IO_ILV/KER_ILV, KER_ILV, POINTS},      // Full buffer dimension
                                    .tiling_dimension = {1, 1, 1, POINTS},              // 2D Tiling for each kernel
                                    .offset           = {REPS*j, k, 0, 0},                 // Justifying the read pointer
                                    .tile_traversal   = {
                                        {.dimension=3, .stride=POINTS, .wrap=1},        // Reading all the points of
                                        {.dimension=0, .stride=1, .wrap=REPS},          // each instance at a time
                                        {.dimension=2, .stride=1, .wrap=KER_ILV},         
                                        {.dimension=1, .stride=1, .wrap=1}            // IO_ILV managed by offset
                                    } 
                                });
                        connect(k_kernel[int(i*(PLIO_WIDTH*IO_ILV/(REPS*KER_ILV))+cur)].out[0], out_mem[i].in[cur]);

                        cur++;
                    }
            }
        }
};