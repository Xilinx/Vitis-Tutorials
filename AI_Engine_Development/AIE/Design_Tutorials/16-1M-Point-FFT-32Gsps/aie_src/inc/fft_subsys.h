//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __FFT_SUBSYS_H__
#define __FFT_SUBSYS_H__

#include <adf.h>

#include "fft_twiddle.h"
#include "fft_phrom.h"

#include "ssr_fft_fp_a.h"
#include "ssr_fft_fp_b.h"
#include "ssr_fft_fp_c.h"
#include "ssr_fft_fp_d.h"
#include "ssr_fft_fp_e.h"
#include "ssr_fft_fp_rotate.h"

using namespace adf;


class ssr_fft_fp_1kpt_a: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out;

  ssr_fft_fp_1kpt_a(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_a>();
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_a.cpp";

    // Make connections
    connect<>(in[0], core.in[0]);
    connect<>(in[1], core.in[1]);
    connect<>(core.out[0], out);
  };
};

class ssr_fft_fp_1kpt_b: public graph {
public:
  kernel core;
  port<input>  in;
  port<output> out;

  ssr_fft_fp_1kpt_b(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_b>(std::vector<int>(2048),
                                               std::vector<int>{TWID0},
                                               std::vector<int>{TWID1});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_b.cpp";
    // Make connections
    connect<>(in,  core.in[0]);
    connect<>(core.out[0], out);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};

class ssr_fft_fp_1kpt_c: public graph {
public:
  kernel core;
  port<input>  in;
  port<output> out;

  ssr_fft_fp_1kpt_c(void) {
    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_c>(std::vector<int>(2048),
                                               std::vector<int>{TWID2},
                                               std::vector<int>{TWID3});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_c.cpp";
    // Make connections
    connect<>(in,  core.in[0]);
    connect<>(core.out[0], out);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};

class ssr_fft_fp_1kpt_d: public graph {
public:
  kernel core;
  port<input> in;
  port<output> out;

  ssr_fft_fp_1kpt_d(void) {

        // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_d>(std::vector<int>{TWID4});
    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_d.cpp";
    // Make connections
    connect<>(in, core.in[0]);
    connect<>(core.out[0], out);
    location<stack>(      core)=location<parameter>(core.param[0]);
  };
};

class ssr_fft_fp_1kpt_e: public graph {
public:
  kernel core;
  port<input> in;
  port<output> out[2];

  ssr_fft_fp_1kpt_e(void) {

    // Create FFT kernels
    core = kernel::create_object<ssr_fft_fp_e>(std::vector<int>(2048),
                                               std::vector<int>{TWID5},
                                               std::vector<int>{TWID6});

    runtime<ratio>(core) = 0.8;
    source(core) = "ssr_fft_fp_e.cpp";

    // Make connections
    connect<>(in, core.in[0]);
    for(int i=0; i<2; i++) connect<>( core.out[i], out[i]);
    location<stack>    (core         )=location<parameter>(core.param[0]);
    location<parameter>(core.param[1])=location<parameter>(core.param[0]);
  };
};


template<int xoff>
class ssr_fft_dds_graph: public graph {
public:
  kernel core[32];
  port<input> in[64];
  port<output> out[64];

  ssr_fft_dds_graph(void) {
    // Create FFT kernels
    core[ 0] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT00},std::vector<int>(2048));
    core[ 1] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT01},std::vector<int>(2048));
    core[ 2] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT02},std::vector<int>(2048));
    core[ 3] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT03},std::vector<int>(2048));
    core[ 4] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT04},std::vector<int>(2048));
    core[ 5] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT05},std::vector<int>(2048));
    core[ 6] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT06},std::vector<int>(2048));
    core[ 7] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT07},std::vector<int>(2048));
    core[ 8] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT08},std::vector<int>(2048));
    core[ 9] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT09},std::vector<int>(2048));
    core[10] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT10},std::vector<int>(2048));
    core[11] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT11},std::vector<int>(2048));
    core[12] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT12},std::vector<int>(2048));
    core[13] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT13},std::vector<int>(2048));
    core[14] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT14},std::vector<int>(2048));
    core[15] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT15},std::vector<int>(2048));

    core[16] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT16},std::vector<int>(2048));
    core[17] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT17},std::vector<int>(2048));
    core[18] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT18},std::vector<int>(2048));
    core[19] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT19},std::vector<int>(2048));
    core[20] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT20},std::vector<int>(2048));
    core[21] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT21},std::vector<int>(2048));
    core[22] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT22},std::vector<int>(2048));
    core[23] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT23},std::vector<int>(2048));
    core[24] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT24},std::vector<int>(2048));
    core[25] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT25},std::vector<int>(2048));
    core[26] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT26},std::vector<int>(2048));
    core[27] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT27},std::vector<int>(2048));
    core[28] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT28},std::vector<int>(2048));
    core[29] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT29},std::vector<int>(2048));
    core[30] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT30},std::vector<int>(2048));
    core[31] = kernel::create_object<ssr_fft_fp_rotate>(std::vector<int>{TWIDROT31},std::vector<int>(2048));

    for(int i=0; i<32; i++){
      runtime<ratio>(core[i]) = 0.8;
      source(core[i]) = "ssr_fft_fp_rotate.cpp";
    }

#define	TWD_CONN(coreid, ioid)	connect<>(in[2*(ioid)+0], core[coreid].in[0]); \
                                connect<>(in[2*(ioid)+1], core[coreid].in[1]);         \
				connect<>(core[coreid].out[0], out[2*(ioid)+0]); \
				connect<>(core[coreid].out[1], out[2*(ioid)+1]);

#define TWD_LOC(coreid, xloc, yoff)  location<kernel>   (core[coreid])          =  tile(xloc+2, yoff+1); \
                                     location<stack>    (core[coreid])          =  bank(xloc+2, yoff+1, 3); \
				     location<parameter>(core[coreid].param[1]) =  bank(xloc+2, yoff,   2); \
				     location<parameter>(core[coreid].param[0]) =  bank(xloc+2, yoff,   3); \
				     location<buffer>(core[coreid].out[0])      = {address(xloc+2, yoff, 0x0000), address(xloc+2, yoff, 0x2000)}; \
				     location<buffer>(core[coreid].out[1])      = {address(xloc+2, yoff, 0x1000), address(xloc+2, yoff, 0x3000)};

    TWD_CONN(0, 0);  TWD_CONN(8,  0+4);  TWD_CONN(16, 0+8);  TWD_CONN(24, 0+12);  TWD_CONN(1+0, 16+0);  TWD_CONN(1+8,  16+0+4);  TWD_CONN(1+16, 16+0+8);  TWD_CONN(1+24, 16+0+12);
    TWD_CONN(2, 1);  TWD_CONN(10, 1+4);  TWD_CONN(18, 1+8);  TWD_CONN(26, 1+12);  TWD_CONN(1+2, 16+1);  TWD_CONN(1+10, 16+1+4);  TWD_CONN(1+18, 16+1+8);  TWD_CONN(1+26, 16+1+12);
    TWD_CONN(4, 2);  TWD_CONN(12, 2+4);  TWD_CONN(20, 2+8);  TWD_CONN(28, 2+12);  TWD_CONN(1+4, 16+2);  TWD_CONN(1+12, 16+2+4);  TWD_CONN(1+20, 16+2+8);  TWD_CONN(1+28, 16+2+12);
    TWD_CONN(6, 3);  TWD_CONN(14, 3+4);  TWD_CONN(22, 3+8);  TWD_CONN(30, 3+12);  TWD_CONN(1+6, 16+3);  TWD_CONN(1+14, 16+3+4);  TWD_CONN(1+22, 16+3+8);  TWD_CONN(1+30, 16+3+12);

    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
        TWD_LOC(i*8 + j*2,    xoff + i*3,        j*2);
        TWD_LOC(i*8 + j*2+1,  xoff + i*3 + 3*4,  j*2);
      }

#undef 	TWD_LOC
#undef 	TWD_CONN
  };
};

template<int xoff, int yoff>
class ssr_fft_fp_1kpt_graph: public graph {
public:
  ssr_fft_fp_1kpt_a ka;
  ssr_fft_fp_1kpt_b kb;
  ssr_fft_fp_1kpt_c kc;
  ssr_fft_fp_1kpt_d kd;
  ssr_fft_fp_1kpt_e ke;

  port<input>  in[2];
  port<output> out[2];

  ssr_fft_fp_1kpt_graph(void) {
    connect<>(in[0], ka.in[0]);
    connect<>(in[1], ka.in[1]);

    connect<>(ka.out, kb.in);
    connect<>(kb.out, kc.in);
    connect<>(kc.out, kd.in);
    connect<>(kd.out, ke.in);

    connect<>(ke.out[0], out[0]);
    connect<>(ke.out[1], out[1]);

    // constraints
    location<kernel>(ka.core) = tile(xoff,   yoff);
    location<kernel>(kb.core) = tile(xoff,   yoff+1);
    location<kernel>(kc.core) = tile(xoff+1, yoff);
    location<kernel>(kd.core) = tile(xoff+1, yoff+1);
    location<kernel>(ke.core) = tile(xoff+2, yoff);

    location<buffer>(ka.core.in[0]) = {address(xoff, yoff+1, 0x0000), address(xoff, yoff+1, 0x2000)};
    location<buffer>(ka.core.in[1]) = {address(xoff, yoff+1, 0x1000), address(xoff, yoff+1, 0x3000)};
    location<stack>( ka.core)       =  bank(xoff, yoff+1, 3);

    location<buffer>(kb.core.in[0]) = {address(xoff, yoff,   0x0000), address(xoff, yoff, 0x2000)};
    location<parameter>(kb.core.param[2])=  address(xoff, yoff+1, 0x4000);
    location<stack>(    kb.core)    =  bank(xoff, yoff+1, 3);

    location<buffer>(kc.core.in[0]) = {address(xoff+1, yoff+1,   0x0000), address(xoff+1, yoff+1, 0x2000)};
    location<parameter>(kc.core.param[2])    =  address(xoff,   yoff,     0x4000);
    location<stack>(    kc.core)    =  bank(xoff, yoff, 3);
    //
    location<buffer>(kd.core.in[0]) = {address(xoff+1, yoff+1,   0x4000), address(xoff+1, yoff+1, 0x6000)};
    location<stack>(    kd.core)    =  bank(xoff+1, yoff, 3);
    //
    location<buffer>(ke.core.in[0]) = {address(xoff+1, yoff, 0x0000), address(xoff+1, yoff, 0x2000)};
    location<parameter>(ke.core.param[2])    =  address(xoff+1, yoff, 0x4000);
    location<stack>(    ke.core)    =  bank(   xoff+2, yoff+1, 2);
    location<buffer>(ke.core.out[0]) = {address(xoff+2, yoff+1,   0x0000), address(xoff+2, yoff+1, 0x2000)};
    location<buffer>(ke.core.out[1]) = {address(xoff+2, yoff+1,   0x1000), address(xoff+2, yoff+1, 0x3000)};

  };
};

template<int xoff>
class ssr_fft_fp_1kpt_graph_x4: public graph {
public:
  ssr_fft_fp_1kpt_graph<xoff,0>  k0;
  ssr_fft_fp_1kpt_graph<xoff,2>  k1;
  ssr_fft_fp_1kpt_graph<xoff,4>  k2;
  ssr_fft_fp_1kpt_graph<xoff,6>  k3;

  port<input>  in[8];
  port<output> out[8];

  ssr_fft_fp_1kpt_graph_x4(void) {

    for(int i=0; i<2; i++){
      connect<>(in[2*0+i], k0.in[i]);
      connect<>(in[2*1+i], k1.in[i]);
      connect<>(in[2*2+i], k2.in[i]);
      connect<>(in[2*3+i], k3.in[i]);
      connect<>(k0.out[i], out[2*0+i]);
      connect<>(k1.out[i], out[2*1+i]);
      connect<>(k2.out[i], out[2*2+i]);
      connect<>(k3.out[i], out[2*3+i]);
    }
  };
};

template<int xoff, int yoff>
class ssr_fft_fp_1kpt_graph_x2: public graph {
public:
  ssr_fft_fp_1kpt_a ka[2];
  ssr_fft_fp_1kpt_b kb[2];
  ssr_fft_fp_1kpt_c kc[2];
  ssr_fft_fp_1kpt_d kd[2];
  ssr_fft_fp_1kpt_e ke[2];

  port<input>  in[4];
  port<output> out[4];

  ssr_fft_fp_1kpt_graph_x2() {

    for(int i=0; i<2; i++){
      connect<>(in[2*i+0], ka[i].in[0]);
      connect<>(in[2*i+1], ka[i].in[1]);

      connect<>(ka[i].out, kb[i].in);
      connect<>(kb[i].out, kc[i].in);
      connect<>(kc[i].out, kd[i].in);
      connect<>(kd[i].out, ke[i].in);

      connect<>(ke[i].out[0], out[2*i+0]);
      connect<>(ke[i].out[1], out[2*i+1]);
    }

    // constraints
    location<kernel>(ka[0].core) = tile(xoff,   yoff);
    location<kernel>(kb[0].core) = tile(xoff,   yoff+1);
    location<kernel>(kc[0].core) = tile(xoff+1, yoff);
    location<kernel>(kd[0].core) = tile(xoff+1, yoff+1);
    location<kernel>(ke[0].core) = tile(xoff+2, yoff);

    location<buffer>(ka[0].core.in[0]) = {address(xoff, yoff+1, 0x0000), address(xoff, yoff+1, 0x2000)};
    location<buffer>(ka[0].core.in[1]) = {address(xoff, yoff+1, 0x1000), address(xoff, yoff+1, 0x3000)};
    location<stack>( ka[0].core)       =  bank(xoff, yoff+1, 3);

    location<buffer>(kb[0].core.in[0]) = {address(xoff, yoff,   0x0000), address(xoff, yoff, 0x2000)};
    location<parameter>(kb[0].core.param[2])=  address(xoff, yoff+1, 0x4000);
    location<stack>(    kb[0].core)    =  bank(xoff, yoff+1, 3);

    location<buffer>(kc[0].core.in[0]) = {address(xoff+1, yoff+1,   0x0000), address(xoff+1, yoff+1, 0x2000)};
    location<parameter>(kc[0].core.param[2])    =  address(xoff,   yoff,     0x4000);
    location<stack>(    kc[0].core)    =  bank(xoff, yoff, 3);
    //
    location<buffer>(kd[0].core.in[0]) = {address(xoff+1, yoff+1,   0x4000), address(xoff+1, yoff+1, 0x6000)};
    location<stack>(    kd[0].core)    =  bank(xoff+1, yoff, 3);
    //
    location<buffer>(ke[0].core.in[0]) = {address(xoff+1, yoff, 0x0000), address(xoff+1, yoff, 0x2000)};
    location<parameter>(ke[0].core.param[2])    =  address(xoff+1, yoff, 0x4000);
    location<stack>(    ke[0].core)    =  bank(   xoff+2, yoff+1, 2);
    location<buffer>(ke[0].core.out[0]) = {address(xoff+2, yoff+1,   0x0000), address(xoff+2, yoff+1, 0x2000)};
    location<buffer>(ke[0].core.out[1]) = {address(xoff+2, yoff+1,   0x1000), address(xoff+2, yoff+1, 0x3000)};

    // constraints
    location<kernel>(ka[1].core) = tile(xoff+2, yoff+1);
    location<kernel>(kb[1].core) = tile(xoff+3, yoff);
    location<kernel>(kc[1].core) = tile(xoff+3, yoff+1);
    location<kernel>(kd[1].core) = tile(xoff+4, yoff);
    location<kernel>(ke[1].core) = tile(xoff+4, yoff+1);

    location<buffer>(ka[1].core.in[0]) = {address(xoff+2, yoff, 0x0000), address(xoff+2, yoff, 0x2000)};
    location<buffer>(ka[1].core.in[1]) = {address(xoff+2, yoff, 0x1000), address(xoff+2, yoff, 0x3000)};
    location<stack>( ka[1].core)       =  bank(xoff+2, yoff, 3);

    location<buffer>(kb[1].core.in[0]) = {address(xoff+3, yoff+1,   0x0000), address(xoff+3, yoff+1, 0x2000)};
    location<parameter>(kb[1].core.param[2])=  address(xoff+2, yoff,    0x4000);
    location<stack>(    kb[1].core)    =  bank(xoff+2, yoff, 3);

    location<buffer>(kc[1].core.in[0]) = {address(xoff+3, yoff,   0x0000), address(xoff+3, yoff, 0x2000)};
    location<parameter>(kc[1].core.param[2])    =  address(xoff+3,   yoff+1,     0x4000);
    location<stack>(    kc[1].core)    =  bank(xoff+3, yoff+1, 3);
    //
    location<buffer>(kd[1].core.in[0]) = {address(xoff+3, yoff,   0x4000), address(xoff+3, yoff, 0x6000)};
    location<stack>(    kd[1].core)    =  bank(xoff+4, yoff+1, 3);
    //
    location<buffer>(ke[1].core.in[0]) = {address(xoff+4, yoff+1, 0x0000), address(xoff+4, yoff+1, 0x2000)};
    location<parameter>(ke[1].core.param[2])    =  address(xoff+4, yoff+1, 0x4000);
    location<stack>(    ke[1].core)    =  bank(   xoff+4, yoff, 2);
    location<buffer>(ke[1].core.out[0]) = {address(xoff+4, yoff,   0x0000), address(xoff+4, yoff, 0x2000)};
    location<buffer>(ke[1].core.out[1]) = {address(xoff+4, yoff,   0x1000), address(xoff+4, yoff, 0x3000)};
  };
};


template<int xoff>
class ssr_fft_fp_1kpt_graph_x8: public graph {
public:
  ssr_fft_fp_1kpt_graph_x2<xoff,0>  k0;
  ssr_fft_fp_1kpt_graph_x2<xoff,2>  k1;
  ssr_fft_fp_1kpt_graph_x2<xoff,4>  k2;
  ssr_fft_fp_1kpt_graph_x2<xoff,6>  k3;

  port<input>  in[16];
  port<output> out[16];


  ssr_fft_fp_1kpt_graph_x8() {

    for(int i=0; i<4; i++){
      connect<>(in[4*0+i], k0.in[i]);
      connect<>(in[4*1+i], k1.in[i]);
      connect<>(in[4*2+i], k2.in[i]);
      connect<>(in[4*3+i], k3.in[i]);
      connect<>(k0.out[i], out[4*0+i]);
      connect<>(k1.out[i], out[4*1+i]);
      connect<>(k2.out[i], out[4*2+i]);
      connect<>(k3.out[i], out[4*3+i]);
    }
  };
};


template<int xoff>
class ssr_fft_fp_1kpt_graph_x32_left: public graph {
public:

  ssr_fft_fp_1kpt_graph_x4<xoff+3*0>  k0;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*1>  k1;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*2>  k2;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*3>  k3;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*4>  k4;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*5>  k5;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*6>  k6;
  ssr_fft_fp_1kpt_graph_x4<xoff+3*7>  k7;
  ssr_fft_dds_graph<xoff>             dds;

  port<input>  in[8*8];
  port<output> out[8*8];


  ssr_fft_fp_1kpt_graph_x32_left() {

    for(int i=0; i<8; i++){
      connect<>(in[8*0+i], k0.in[i]);
      connect<>(in[8*1+i], k1.in[i]);
      connect<>(in[8*2+i], k2.in[i]);
      connect<>(in[8*3+i], k3.in[i]);
      connect<>(in[8*4+i], k4.in[i]);
      connect<>(in[8*5+i], k5.in[i]);
      connect<>(in[8*6+i], k6.in[i]);
      connect<>(in[8*7+i], k7.in[i]);

      connect<>(k0.out[i], dds.in[8*0+i]);
      connect<>(k1.out[i], dds.in[8*1+i]);
      connect<>(k2.out[i], dds.in[8*2+i]);
      connect<>(k3.out[i], dds.in[8*3+i]);
      connect<>(k4.out[i], dds.in[8*4+i]);
      connect<>(k5.out[i], dds.in[8*5+i]);
      connect<>(k6.out[i], dds.in[8*6+i]);
      connect<>(k7.out[i], dds.in[8*7+i]);
    }

    for(int i=0; i<64; i++) connect<>(dds.out[i], out[i]);

  };
};

template<int xoff>
class ssr_fft_fp_1kpt_graph_x32_right: public graph {
public:

  ssr_fft_fp_1kpt_graph_x8<xoff+5*0>  k0;
  ssr_fft_fp_1kpt_graph_x8<xoff+5*1>  k1;
  ssr_fft_fp_1kpt_graph_x8<xoff+5*2>  k2;
  ssr_fft_fp_1kpt_graph_x8<xoff+5*3>  k3;


  port<input>  in[8*8];
  port<output> out[8*8];


  ssr_fft_fp_1kpt_graph_x32_right() {

    for(int i=0; i<16; i++){
      connect<>(in[16*0+i], k0.in[i]);
      connect<>(in[16*1+i], k1.in[i]);
      connect<>(in[16*2+i], k2.in[i]);
      connect<>(in[16*3+i], k3.in[i]);

      connect<>(k0.out[i], out[16*0+i]);
      connect<>(k1.out[i], out[16*1+i]);
      connect<>(k2.out[i], out[16*2+i]);
      connect<>(k3.out[i], out[16*3+i]);
    }
  };
};

template<int xoff>
class ssr_fft_fp_1mpt_graph: public graph {
public:

  ssr_fft_fp_1kpt_graph_x32_left<xoff>      ffta;
  ssr_fft_fp_1kpt_graph_x32_right<xoff+3*8> fftb;

  port<input>        in[8*8];
  port<output>  mid_out[8*8];
  port<input>   mid_in[ 8*8];
  port<output>     out[ 8*8];

  ssr_fft_fp_1mpt_graph() {

    for(int i=0; i<64; i++){
      connect<>(in[i], ffta.in[i]);
      connect<>(ffta.out[i], mid_out[i]);
      connect<>(mid_in[i],  fftb.in[i]);
      connect<>(fftb.out[i], out[i]);
    }
  };
};




#endif //__FFT_SUBSYS_H__
