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
class FloatGraph : public graph
{
private:
    kernel k;

public:
    port<input> in;
    port<output> out;

    FloatGraph()
    {
        k = kernel::create(firfloat);

        connect< window<NSAMPLES_WINDOW*NBYTES_FLOAT,NSAMPLES_OVERLAP*NBYTES_FLOAT> > float_in(in, k.in[0]);
        connect< window<NSAMPLES_WINDOW*NBYTES_FLOAT> > float_out(k.out[0], out);

        source(k) = "aie_kernels/fir_float.cpp";
        location<kernel>(k) = tile(COL,ROW);
        runtime<ratio>(k) = 0.6;
    }
};

template<int COL,int ROW>
class CFloatGraph : public graph
{
private:
    kernel k;

public:
    port<input> in;
    port<output> out;

    CFloatGraph()
    {
        k = kernel::create(fircfloat);

        connect< window<NSAMPLES_WINDOW*NBYTES_CFLOAT,NSAMPLES_OVERLAP*NBYTES_CFLOAT> > cfloat_in(in, k.in[0]);
        connect< window<NSAMPLES_WINDOW*NBYTES_CFLOAT> > cfloat_out(k.out[0], out);

        source(k) = "aie_kernels/fir_cfloat.cpp";
        location<kernel>(k) = tile(COL,ROW);
        runtime<ratio>(k) = 0.6;
    }
};


class TopGraph : public graph
{
public:
    input_plio in[2];
    output_plio out[2];

    FloatGraph<25,0> FG;
    CFloatGraph<27,0> CFG;
    TopGraph()
    {

        in[0] = input_plio::create("DataIn0", plio_64_bits, "../data/inputf.txt");
        in[1] = input_plio::create("DataIn1", plio_64_bits, "../data/inputcf.txt");
        out[0] = output_plio::create("DataOut0", plio_64_bits, "foutput.txt");
        out[1] = output_plio::create("DataOut1", plio_64_bits, "cfoutput.txt");


        connect <> net0(in[0].out[0],FG.in);
        connect <> net1(FG.out,out[0].in[0]);

        connect <> net2(in[1].out[0],CFG.in);
        connect <> net3(CFG.out,out[1].in[0]);

    }
};
