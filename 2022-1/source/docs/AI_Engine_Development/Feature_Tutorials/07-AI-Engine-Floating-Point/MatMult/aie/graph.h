/*
* (c) Copyright 2020–2022 Xilinx, Inc. All rights reserved.
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
#include <adf.h>
#include "aie_kernels.h"
#include "system_settings.h"


using namespace adf;

template<int COL,int ROW>
class MatMultFloatGraph : public graph
{
private:
    kernel k;

public:
    port<input> ina,inb;
    port<output> outc;

    MatMultFloatGraph()
    {
        k = kernel::create(matmult_float);

        connect< window<NSAMPLES_WINDOW_F_A*NBYTES_FLOAT> > float_ina(ina, k.in[0]);
        connect< window<NSAMPLES_WINDOW_F_B*NBYTES_FLOAT> > float_inb(inb, k.in[1]);
        connect< window<NSAMPLES_WINDOW_F_C*NBYTES_FLOAT> > float_outc(k.out[0], outc);
        source(k) = "aie_kernels/matmult_float.cpp";
        location<kernel>(k) = tile(COL,ROW);
        runtime<ratio>(k) = 0.6;
    }
};

template<int COL,int ROW>
class MatMultCFloatGraph : public graph
{
private:
    kernel k,k_conf,k_conj;

public:
    port<input> ina,inb;
    port<output> outc,outc_conf,outc_conj;

    MatMultCFloatGraph()
    {
        k = kernel::create(matmult_cfloat);
        k_conf = kernel::create(matmult_cfloat_conf);
        k_conj = kernel::create(matmult_cfloat_conj);

        connect< window<NSAMPLES_WINDOW_CF_A*NBYTES_CFLOAT> > float_ina(ina, k.in[0]);
        connect< window<NSAMPLES_WINDOW_CF_B*NBYTES_CFLOAT> > float_inb(inb, k.in[1]);
        connect< window<NSAMPLES_WINDOW_CF_C*NBYTES_CFLOAT> > float_outc(k.out[0], outc);

        connect< window<NSAMPLES_WINDOW_CF_A*NBYTES_CFLOAT> > float_ina_conf(ina, k_conf.in[0]);
        connect< window<NSAMPLES_WINDOW_CF_B*NBYTES_CFLOAT> > float_inb_conf(inb, k_conf.in[1]);
        connect< window<NSAMPLES_WINDOW_CF_C*NBYTES_CFLOAT> > float_outc_conf(k_conf.out[0], outc_conf);

        connect< window<NSAMPLES_WINDOW_CF_A*NBYTES_CFLOAT> > float_ina_conj(ina, k_conj.in[0]);
        connect< window<NSAMPLES_WINDOW_CF_B*NBYTES_CFLOAT> > float_inb_conj(inb, k_conj.in[1]);
        connect< window<NSAMPLES_WINDOW_CF_C*NBYTES_CFLOAT> > float_outc_conj(k_conj.out[0], outc_conj);


        source(k) = "aie_kernels/matmult_cfloat.cpp";
        location<kernel>(k) = tile(COL,ROW);
        runtime<ratio>(k) = 0.6;

        source(k_conf) = "aie_kernels/matmult_cfloat.cpp";
        location<kernel>(k_conf) = tile(COL,ROW+1);
        runtime<ratio>(k_conf) = 0.6;

        source(k_conj) = "aie_kernels/matmult_cfloat.cpp";
        location<kernel>(k_conj) = tile(COL,ROW+2);
        runtime<ratio>(k_conj) = 0.6;


    }
};

class TopGraph : public graph
{
public:
    input_plio in[4];
    output_plio out[4];

    MatMultFloatGraph<25,0> FG;
    MatMultCFloatGraph<27,0> CFG;

    TopGraph()
    {
        in[0] = input_plio::create("DataIn0", plio_64_bits, "../data/inputa_float.txt");
        in[1] = input_plio::create("DataIn1", plio_64_bits, "../data/inputb_float.txt");
        in[2] = input_plio::create("DataIn2", plio_64_bits, "../data/inputa_cfloat.txt");
        in[3] = input_plio::create("DataIn3", plio_64_bits, "../data/inputb_cfloat.txt");

        out[0] = output_plio::create("DataOut0", plio_64_bits, "float_output.txt");
        out[1] = output_plio::create("DataOut1", plio_64_bits, "cfloat_output.txt");
        out[2] = output_plio::create("DataOut2", plio_64_bits, "cfloat_conf_output.txt");
        out[3] = output_plio::create("DataOut3", plio_64_bits, "cfloat_conj_output.txt");


        connect <> net0(in[0].out[0],FG.ina);
        connect <> net1(in[1].out[0],FG.inb);
        connect <> net2(FG.outc,out[0].in[0]);

        connect <> net3(in[2].out[0],CFG.ina);
        connect <> net4(in[3].out[0],CFG.inb);
        connect <> net5(CFG.outc,out[1].in[0]);
        connect <> net6(CFG.outc_conf,out[2].in[0]);
        connect <> net7(CFG.outc_conj,out[3].in[0]);

    }
};
