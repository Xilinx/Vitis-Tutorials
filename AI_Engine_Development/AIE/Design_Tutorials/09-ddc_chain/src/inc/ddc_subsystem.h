// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#ifndef __DDC_SUBSYSTEM__
#define __DDC_SUBSYSTEM__

#include <adf.h>

#include "ddc_include.h"
#include "ddcinlv_kernel.h"
#include "demixer_5c_kernel0.h"
#include "ddc_chain_kernel.h"
#include "hbd_chain_kernel.h"
#include "fir_47t_sym_hb_2d_intlv.h"

using namespace adf ;


class rx_filter_chain : public graph {

public:
  port<input>  data_in;
  port<output> data_out;
public:
  kernel chanFir;

  rx_filter_chain( std::vector<cint16> ovlp_init, std::vector<cint16> dmBufA_init )
  {
    chanFir = kernel::create_object<ddc_chain_kernel>( ovlp_init, dmBufA_init );

    runtime<ratio>(chanFir)  = 0.8  ;
    source(chanFir) = "ddc_chain_kernel.cc" ;

    // Make connections using the block sizes for the kernels
    connect(data_in, chanFir.in[0]);
    adf::dimensions(chanFir.in[0]) = { DDC_IN_BLOCK_SIZE/4 };
    connect(chanFir.out[0], data_out);
    adf::dimensions(chanFir.out[0]) = { DDC_OUT_BLOCK_SIZE/4 };
  }
};

class rx_hbf47_1a : public graph {

public:
  port<input>  data_in;
  port<output> data_out;
public:
  kernel inlv;
  kernel hbf47d;
  kernel demixer;
  kernel hbf_chain;

  rx_hbf47_1a( std::vector<cint16> hbfd_init ) {

		inlv     = kernel::create_object<ddcinlv_kernel>();
		hbf47d   = kernel::create_object<fir_47t_sym_hb_2d>();
		demixer  = kernel::create_object<demixer_5c_kernel0>();
		hbf_chain= kernel::create_object<hbd_chain_kernel>( hbfd_init );

		runtime<ratio>(inlv)     =0.1;
		runtime<ratio>(hbf47d)   =0.25;
		runtime<ratio>(demixer)  =0.25;
		runtime<ratio>(hbf_chain)=0.25;

		source(inlv)      = "ddcinlv_kernel.cc";
		source(hbf47d)    = "fir_47t_sym_hb_2d_intlv.cc";
		source(demixer)   = "demixer_5c_kernel0.cc";
		source(hbf_chain) = "hbd_chain_kernel.cc";

		connect(data_in, inlv.in[0]);
		adf::dimensions(inlv.in[0]) = { HBF47D_IN_BLOCK_SIZE/4 };
		adf::dimensions(inlv.out[0]) = { HBF47D_IN_BLOCK_SIZE/4 };
		connect(inlv.out[0],hbf47d.in[0]);
		adf::dimensions(hbf47d.in[0]) = { HBF47D_IN_BLOCK_SIZE/4 };
		connect(hbf47d.out[0], demixer.in[0]);
		adf::dimensions(hbf47d.out[0]) = { HBF47D_OUTPUT_SIZE/4 };
		adf::dimensions(demixer.in[0]) = { HBF47D_OUTPUT_SIZE/4 };
		connect(demixer.out[0], hbf_chain.in[0]);
		adf::dimensions(demixer.out[0]) = { HBFD_IN_BLOCK_SIZE/4 };
		adf::dimensions(hbf_chain.in[0]) = { HBFD_IN_BLOCK_SIZE/4 };
		connect(hbf_chain.out[0], data_out);
		adf::dimensions(hbf_chain.out[0]) = { HBFD_OUTPUT_SIZE/4 };

	};
};


template <int xoff, int yoff, int dir>
class ddc : public graph {

    public:
        port<input>  data_in;
		port<output> data_out;
		rx_filter_chain rx_chain_dyn_inst;
		rx_hbf47_1a rx_hbf47;

  ddc( std::vector<cint16> ovlp_init,
       std::vector<cint16> dmBufA_init,
       std::vector<cint16> hbfd_init )
    : rx_chain_dyn_inst( ovlp_init, dmBufA_init ),  rx_hbf47( hbfd_init )
  {


		// Connections
		connect<>(data_in,rx_hbf47.data_in);
		connect<>(rx_hbf47.data_out,rx_chain_dyn_inst.data_in);
		connect<>(rx_chain_dyn_inst.data_out,data_out);

		//location constraints for HBF47 and demixer
		location<kernel>(rx_hbf47.hbf47d)      = tile(xoff, yoff);
		location<kernel>(rx_hbf47.demixer)     = location<kernel>(rx_hbf47.hbf47d);
		location<kernel>(rx_hbf47.hbf_chain)   = location<kernel>(rx_hbf47.hbf47d);
		location<kernel>(rx_hbf47.inlv)        = location<kernel>(rx_hbf47.hbf47d);
		location<stack>(rx_hbf47.hbf47d)       = bank(xoff, yoff, 0);

		location<buffer>(rx_hbf47.inlv.in[0])= {bank(xoff, yoff,0),bank(xoff, yoff,1)};

		single_buffer(rx_hbf47.hbf47d.in[0]);   location<buffer>(rx_hbf47.hbf47d.in[0])   = bank(xoff, yoff,3);
		single_buffer(rx_hbf47.demixer.in[0]);  location<buffer>(rx_hbf47.demixer.in[0])  = bank(xoff, yoff,2);
		single_buffer(rx_hbf47.hbf_chain.in[0]);location<buffer>(rx_hbf47.hbf_chain.in[0])= bank(xoff, yoff,3);
		location<parameter>(rx_hbf47.hbf_chain.param[0])    = bank(xoff, yoff,2);

		//location constraints for FIRs
		location<kernel>(rx_chain_dyn_inst.chanFir) =tile(xoff, yoff+dir);
		location<stack>(rx_chain_dyn_inst.chanFir)  =bank(xoff, yoff+dir,0);

		location<buffer>(rx_chain_dyn_inst.chanFir.in[0])    = {bank(xoff, yoff+dir,0),bank(xoff, yoff+dir,1)};
		location<buffer>(rx_chain_dyn_inst.chanFir.out[0])   = {bank(xoff, yoff+dir,0),bank(xoff, yoff+dir,1)};
		location<parameter>(rx_chain_dyn_inst.chanFir.param[0]) = bank(xoff, yoff+dir,3);
		location<parameter>(rx_chain_dyn_inst.chanFir.param[1]) = bank(xoff, yoff+dir,2);

	} ;
};

static std::vector<cint16> ovlp_init0(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init1(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init2(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init3(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init4(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init5(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init6(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init7(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init8(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init9(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init10(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init11(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init12(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init13(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init14(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init15(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init16(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init17(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init18(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init19(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init20(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init21(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init22(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init23(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init24(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init25(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init26(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init27(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init28(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init29(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init30(ddc_chain_kernel::OVLP_SIZE);
static std::vector<cint16> ovlp_init31(ddc_chain_kernel::OVLP_SIZE);

static std::vector<cint16> dmBufA_init0(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init1(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init2(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init3(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init4(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init5(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init6(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init7(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init8(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init9(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init10(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init11(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init12(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init13(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init14(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init15(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init16(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init17(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init18(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init19(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init20(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init21(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init22(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init23(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init24(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init25(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init26(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init27(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init28(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init29(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init30(DDC_LTE_SAMPLES_PERCH4D);
static std::vector<cint16> dmBufA_init31(DDC_LTE_SAMPLES_PERCH4D);

static std::vector<cint16> hbfd_init0(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init1(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init2(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init3(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init4(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init5(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init6(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init7(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init8(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init9(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init10(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init11(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init12(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init13(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init14(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init15(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init16(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init17(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init18(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init19(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init20(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init21(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init22(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init23(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init24(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init25(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init26(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init27(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init28(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init29(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init30(hbd_chain_kernel::OVLP_SIZE);
static std::vector<cint16> hbfd_init31(hbd_chain_kernel::OVLP_SIZE);

template<int col_offset>
class ddc_32ant : public graph {

    public:
		port<input>  data_in[32];
		port<output> data_out[32];

		ddc<col_offset+0,0,1> ddc_inst0;
	    ddc<col_offset+0,2,1> ddc_inst1;
	    ddc<col_offset+0,4,1> ddc_inst2;
	    ddc<col_offset+0,6,1> ddc_inst3;

	    ddc<col_offset+1,0,1> ddc_inst4;
	    ddc<col_offset+1,2,1> ddc_inst5;
	    ddc<col_offset+1,4,1> ddc_inst6;
	    ddc<col_offset+1,6,1> ddc_inst7;

	    ddc<col_offset+2,0,1> ddc_inst8;
	    ddc<col_offset+2,2,1> ddc_inst9;
	    ddc<col_offset+2,4,1> ddc_inst10;
	    ddc<col_offset+2,6,1> ddc_inst11;

	    ddc<col_offset+3,0,1> ddc_inst12;
	    ddc<col_offset+3,2,1> ddc_inst13;
	    ddc<col_offset+3,4,1> ddc_inst14;
	    ddc<col_offset+3,6,1> ddc_inst15;

	    ddc<col_offset+4,0,1> ddc_inst16;
	    ddc<col_offset+4,2,1> ddc_inst17;
	    ddc<col_offset+4,4,1> ddc_inst18;
	    ddc<col_offset+4,6,1> ddc_inst19;

	    ddc<col_offset+5,0,1> ddc_inst20;
	    ddc<col_offset+5,2,1> ddc_inst21;
	    ddc<col_offset+5,4,1> ddc_inst22;
	    ddc<col_offset+5,6,1> ddc_inst23;

	    ddc<col_offset+6,0,1> ddc_inst24;
	    ddc<col_offset+6,2,1> ddc_inst25;
	    ddc<col_offset+6,4,1> ddc_inst26;
	    ddc<col_offset+6,6,1> ddc_inst27;

	    ddc<col_offset+7,0,1> ddc_inst28;
	    ddc<col_offset+7,2,1> ddc_inst29;
	    ddc<col_offset+7,4,1> ddc_inst30;
	    ddc<col_offset+7,6,1> ddc_inst31;

  ddc_32ant( void ) : ddc_inst0( ovlp_init0, dmBufA_init0, hbfd_init0 ),
                      ddc_inst1( ovlp_init1, dmBufA_init1, hbfd_init1 ),
                      ddc_inst2( ovlp_init2, dmBufA_init2, hbfd_init2 ),
                      ddc_inst3( ovlp_init3, dmBufA_init3, hbfd_init3 ),
                      ddc_inst4( ovlp_init4, dmBufA_init4, hbfd_init4 ),
                      ddc_inst5( ovlp_init5, dmBufA_init5, hbfd_init5 ),
                      ddc_inst6( ovlp_init6, dmBufA_init6, hbfd_init6 ),
                      ddc_inst7( ovlp_init7, dmBufA_init7, hbfd_init7 ),
                      ddc_inst8( ovlp_init8, dmBufA_init8, hbfd_init8 ),
                      ddc_inst9( ovlp_init9, dmBufA_init9, hbfd_init9 ),
                      ddc_inst10( ovlp_init10, dmBufA_init10, hbfd_init10 ),
                      ddc_inst11( ovlp_init11, dmBufA_init11, hbfd_init11 ),
                      ddc_inst12( ovlp_init12, dmBufA_init12, hbfd_init12 ),
                      ddc_inst13( ovlp_init13, dmBufA_init13, hbfd_init13 ),
                      ddc_inst14( ovlp_init14, dmBufA_init14, hbfd_init14 ),
                      ddc_inst15( ovlp_init15, dmBufA_init15, hbfd_init15 ),
                      ddc_inst16( ovlp_init16, dmBufA_init16, hbfd_init16 ),
                      ddc_inst17( ovlp_init17, dmBufA_init17, hbfd_init17 ),
                      ddc_inst18( ovlp_init18, dmBufA_init18, hbfd_init18 ),
                      ddc_inst19( ovlp_init19, dmBufA_init19, hbfd_init19 ),
                      ddc_inst20( ovlp_init20, dmBufA_init20, hbfd_init20 ),
                      ddc_inst21( ovlp_init21, dmBufA_init21, hbfd_init21 ),
                      ddc_inst22( ovlp_init22, dmBufA_init22, hbfd_init22 ),
                      ddc_inst23( ovlp_init23, dmBufA_init23, hbfd_init23 ),
                      ddc_inst24( ovlp_init24, dmBufA_init24, hbfd_init24 ),
                      ddc_inst25( ovlp_init25, dmBufA_init25, hbfd_init25 ),
                      ddc_inst26( ovlp_init26, dmBufA_init26, hbfd_init26 ),
                      ddc_inst27( ovlp_init27, dmBufA_init27, hbfd_init27 ),
                      ddc_inst28( ovlp_init28, dmBufA_init28, hbfd_init28 ),
                      ddc_inst29( ovlp_init29, dmBufA_init29, hbfd_init29 ),
                      ddc_inst30( ovlp_init30, dmBufA_init30, hbfd_init30 ),
                      ddc_inst31( ovlp_init31, dmBufA_init31, hbfd_init31 )
  {


		// Connections
		connect<>(data_in[0], ddc_inst0.data_in);
	    connect<>(data_in[1], ddc_inst1.data_in);
	    connect<>(data_in[2], ddc_inst2.data_in);
	    connect<>(data_in[3], ddc_inst3.data_in);

	    connect<>(data_in[4], ddc_inst4.data_in);
	    connect<>(data_in[5], ddc_inst5.data_in);
	    connect<>(data_in[6], ddc_inst6.data_in);
	    connect<>(data_in[7], ddc_inst7.data_in);

	    connect<>(data_in[8], ddc_inst8.data_in);
	    connect<>(data_in[9], ddc_inst9.data_in);
	    connect<>(data_in[10], ddc_inst10.data_in);
	    connect<>(data_in[11], ddc_inst11.data_in);

	    connect<>(data_in[12], ddc_inst12.data_in);
	    connect<>(data_in[13], ddc_inst13.data_in);
	    connect<>(data_in[14], ddc_inst14.data_in);
	    connect<>(data_in[15], ddc_inst15.data_in);

	    connect<>(data_in[16], ddc_inst16.data_in);
	    connect<>(data_in[17], ddc_inst17.data_in);
	    connect<>(data_in[18], ddc_inst18.data_in);
	    connect<>(data_in[19], ddc_inst19.data_in);

	    connect<>(data_in[20], ddc_inst20.data_in);
	    connect<>(data_in[21], ddc_inst21.data_in);
	    connect<>(data_in[22], ddc_inst22.data_in);
	    connect<>(data_in[23], ddc_inst23.data_in);

	    connect<>(data_in[24], ddc_inst24.data_in);
	    connect<>(data_in[25], ddc_inst25.data_in);
	    connect<>(data_in[26], ddc_inst26.data_in);
	    connect<>(data_in[27], ddc_inst27.data_in);

	    connect<>(data_in[28], ddc_inst28.data_in);
	    connect<>(data_in[29], ddc_inst29.data_in);
	    connect<>(data_in[30], ddc_inst30.data_in);
	    connect<>(data_in[31], ddc_inst31.data_in);

	    connect<>(ddc_inst0.data_out,data_out[0]);
	    connect<>(ddc_inst1.data_out,data_out[1]);
	    connect<>(ddc_inst2.data_out,data_out[2]);
	    connect<>(ddc_inst3.data_out,data_out[3]);

	    connect<>(ddc_inst4.data_out,data_out[4]);
	    connect<>(ddc_inst5.data_out,data_out[5]);
	    connect<>(ddc_inst6.data_out,data_out[6]);
	    connect<>(ddc_inst7.data_out,data_out[7]);

	    connect<>(ddc_inst8.data_out,data_out[8]);
	    connect<>(ddc_inst9.data_out,data_out[9]);
	    connect<>(ddc_inst10.data_out,data_out[10]);
	    connect<>(ddc_inst11.data_out,data_out[11]);

	    connect<>(ddc_inst12.data_out,data_out[12]);
	    connect<>(ddc_inst13.data_out,data_out[13]);
	    connect<>(ddc_inst14.data_out,data_out[14]);
	    connect<>(ddc_inst15.data_out,data_out[15]);

	    connect<>(ddc_inst16.data_out,data_out[16]);
	    connect<>(ddc_inst17.data_out,data_out[17]);
	    connect<>(ddc_inst18.data_out,data_out[18]);
	    connect<>(ddc_inst19.data_out,data_out[19]);

	    connect<>(ddc_inst20.data_out,data_out[20]);
	    connect<>(ddc_inst21.data_out,data_out[21]);
	    connect<>(ddc_inst22.data_out,data_out[22]);
	    connect<>(ddc_inst23.data_out,data_out[23]);

	    connect<>(ddc_inst24.data_out,data_out[24]);
	    connect<>(ddc_inst25.data_out,data_out[25]);
	    connect<>(ddc_inst26.data_out,data_out[26]);
	    connect<>(ddc_inst27.data_out,data_out[27]);

	    connect<>(ddc_inst28.data_out,data_out[28]);
	    connect<>(ddc_inst29.data_out,data_out[29]);
	    connect<>(ddc_inst30.data_out,data_out[30]);
	    connect<>(ddc_inst31.data_out,data_out[31]);

	} ;
};
#endif