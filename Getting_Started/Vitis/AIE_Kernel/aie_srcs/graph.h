/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <adf.h>
#include <aie_kernel.h>

using namespace adf;

class Simple_Graph : public adf::graph {
        private:
                kernel k1;
        public:
                input_plio in1,in2;
                output_plio out_put;

                Simple_Graph()
                {
                        k1 = kernel::create(MatMul);
                        source(k1) = "./MatMul.cpp";
                        runtime<ratio>(k1) = 0.9;

                        in1 = adf::input_plio::create("DataIn1", adf::plio_32_bits, "../data_srcs/MatrixA.txt");
                        in2 = adf::input_plio::create("DataIn2", adf::plio_32_bits, "../data_srcs/MatrixB.txt");
                        out_put = adf::output_plio::create("DataOut1", adf::plio_32_bits, "../data_srcs/output.txt");

                connect(in1.out[0],k1.in[0]);
                connect(in2.out[0],k1.in[1]);
                connect(k1.out[0],out_put.in[0]);

                dimensions(k1.in[0])={16};
                dimensions(k1.in[1])={4};
                dimensions(k1.out[0])={4};
    }
};
