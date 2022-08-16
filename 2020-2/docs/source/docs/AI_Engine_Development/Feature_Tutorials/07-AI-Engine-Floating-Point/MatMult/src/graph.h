/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
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
    source(k) = "./matmult_float.cpp";
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


    source(k) = "./matmult_cfloat.cpp";
    location<kernel>(k) = tile(COL,ROW);
    runtime<ratio>(k) = 0.6;

    source(k_conf) = "./matmult_cfloat.cpp";
    location<kernel>(k_conf) = tile(COL,ROW+1);
    runtime<ratio>(k_conf) = 0.6;

    source(k_conj) = "./matmult_cfloat.cpp";
    location<kernel>(k_conj) = tile(COL,ROW+2);
    runtime<ratio>(k_conj) = 0.6;


  }
};

class TopGraph : public graph
{
public:
  port<input> in[4];
  port<output> out[4];

  MatMultFloatGraph<25,0> FG;
  MatMultCFloatGraph<27,0> CFG;

  TopGraph()
  {
    connect <window<NSAMPLES_WINDOW_F_A*NBYTES_FLOAT>> net0(in[0],FG.ina);
    connect <window<NSAMPLES_WINDOW_F_B*NBYTES_FLOAT>> net1(in[1],FG.inb);
    connect <window<NSAMPLES_WINDOW_F_C*NBYTES_FLOAT>> net2(FG.outc,out[0]);

    connect <window<NSAMPLES_WINDOW_CF_A*NBYTES_CFLOAT>> net3(in[2],CFG.ina);
    connect <window<NSAMPLES_WINDOW_CF_B*NBYTES_CFLOAT>> net4(in[3],CFG.inb);
    connect <window<NSAMPLES_WINDOW_CF_C*NBYTES_CFLOAT>> net5(CFG.outc,out[1]);
    connect <window<NSAMPLES_WINDOW_CF_C*NBYTES_CFLOAT>> net6(CFG.outc_conf,out[2]);
    connect <window<NSAMPLES_WINDOW_CF_C*NBYTES_CFLOAT>> net7(CFG.outc_conj,out[3]);

  }
};
