// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#pragma once
#include <adf.h>
#include "ddc_include.h"

using namespace adf;

class hbd_chain_kernel {
public:
	static constexpr unsigned HBFD11_BUFF_SIZE = 2;
	static constexpr unsigned OVLP_SIZE = (HBFD11_BUFF_SIZE*DDC_NCH_LTE)*8;
	alignas(32) cint16 (&hbfd_ovlp)[OVLP_SIZE];
	
	hbd_chain_kernel( cint16 (&hbfd_init)[OVLP_SIZE] );
	void run(input_buffer<cint16> & __restrict cb_input,output_buffer<cint16> & __restrict cb_output);
	
	static void registerKernelClass( void )
	{
		REGISTER_FUNCTION( hbd_chain_kernel::run );
		REGISTER_PARAMETER( hbfd_ovlp );
	}
};
