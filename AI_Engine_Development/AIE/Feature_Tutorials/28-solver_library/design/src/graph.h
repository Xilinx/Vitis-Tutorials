//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved. Portions of this file consist of AI-generated content.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#ifndef __LINEAR_EQU_SOLVER_GRAPH_H__
#define __LINEAR_EQU_SOLVER_GRAPH_H__

#include <adf.h>
#include "cholesky_graph.hpp"
#include "substitution_graph.hpp"

using namespace adf;

// Cholesky graph parameters
#define TT_DATA cfloat
#define TP_DIM 32
#define TP_NUM_FRAMES 1
#define TP_GRID_DIM 1
#define TP_CASC_LEN 1

// Substitution graph parameters
#define DIM_SIZE_SUBSTITUTION 32
#define TP_L_LEADING 1 // Backward substitution previous value = 0
#define TP_L_LEADING_TRANSPOSE 1
#define GRID_DIM_SUBSTITUTION 1

// Substitution type: 0 = forward, 1 = backward
#define FWD_SUBST_TYPE 0
#define BCK_SUBST_TYPE 1

class LinearEquSolverGraph : public adf::graph {
public:
    input_plio A_matrix;
    input_plio b_input;
    output_plio x_output;

    // Step 1: Cholesky decomposition  A = L * L^T
    xf::solver::aie::cholesky::cholesky_graph<TT_DATA, TP_DIM, TP_NUM_FRAMES, TP_GRID_DIM, TP_CASC_LEN>
        choleskyGraph;

    // Step 2: Forward substitution  L * y = b
    xf::solver::aie::substitution::substitution_graph<TT_DATA, DIM_SIZE_SUBSTITUTION, FWD_SUBST_TYPE, TP_L_LEADING_TRANSPOSE, TP_GRID_DIM>
        fwd_substitutionGraph;

    // Step 3: Backward substitution  L^T * x = y
    xf::solver::aie::substitution::substitution_graph<TT_DATA, DIM_SIZE_SUBSTITUTION, BCK_SUBST_TYPE, TP_L_LEADING, TP_GRID_DIM>
        bck_substitutionGraph;

    LinearEquSolverGraph() {
        // Input PLIOs: A (32x32 matrix), b (32x1 vector)
        A_matrix = input_plio::create("A_matrix", plio_64_bits, "data/A_matrix.txt");
        b_input  = input_plio::create("b_input",  plio_64_bits, "data/b_input.txt");

        // Output PLIO: x (32x1 solution vector)
        x_output = output_plio::create("x_output", plio_64_bits, "data/x_output.txt");

        // Cholesky: A_matrix -> choleskyGraph -> matL_output
        connect<> net_A(A_matrix.out[0], choleskyGraph.in[0]);

        // Forward substitution: matL_output and b_input -> y_output
        connect<> net_L_fwd(choleskyGraph.out[0], fwd_substitutionGraph.L_in[0]);
        connect<> net_b(b_input.out[0], fwd_substitutionGraph.y_in[0]);
        
        // Backward substitution: L from Cholesky and y from forward substitution -> x_output
        connect<> net_L_bck(choleskyGraph.out[0], bck_substitutionGraph.L_in[0]);
        connect<> net_y(fwd_substitutionGraph.x_out[0], bck_substitutionGraph.y_in[0]);

        // Output: x_output
        connect<> net_x(bck_substitutionGraph.x_out[0], x_output.in[0]);
    }
};

#endif // __LINEAR_EQU_SOLVER_GRAPH_H__
