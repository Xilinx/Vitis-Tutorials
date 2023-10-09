// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#ifndef __DDC_CHAIN_TOP__
#define __DDC_CHAIN_TOP__ 


#include "ddc_subsystem.h"

template<int col_offset>
class TEST_32ant: public graph {

private:

	ddc_32ant<col_offset> ddc_top;
	
public:
    input_plio  i_ddc[32];
    
    output_plio o_ddc[32];

    TEST_32ant(){

    
	i_ddc[0]  =  input_plio::create("ddc_in00",    plio_32_bits, "ddc_in00.txt",  400);
	i_ddc[1]  =  input_plio::create("ddc_in01",    plio_32_bits, "ddc_in01.txt",  400);
	i_ddc[2]  =  input_plio::create("ddc_in02",    plio_32_bits, "ddc_in02.txt",  400);
	i_ddc[3]  =  input_plio::create("ddc_in03",    plio_32_bits, "ddc_in03.txt",  400);
	i_ddc[4]  =  input_plio::create("ddc_in04",    plio_32_bits, "ddc_in04.txt",  400);
	i_ddc[5]  =  input_plio::create("ddc_in05",    plio_32_bits, "ddc_in05.txt",  400);
	i_ddc[6]  =  input_plio::create("ddc_in06",    plio_32_bits, "ddc_in06.txt",  400);
	i_ddc[7]  =  input_plio::create("ddc_in07",    plio_32_bits, "ddc_in07.txt",  400);
			    
	i_ddc[8]  =  input_plio::create("ddc_in10",    plio_32_bits, "ddc_in10.txt",  400);
	i_ddc[9]  =  input_plio::create("ddc_in11",    plio_32_bits, "ddc_in11.txt",  400);
	i_ddc[10] =  input_plio::create("ddc_in12",    plio_32_bits, "ddc_in12.txt",  400);
	i_ddc[11] =  input_plio::create("ddc_in13",    plio_32_bits, "ddc_in13.txt",  400);
	i_ddc[12] =  input_plio::create("ddc_in14",    plio_32_bits, "ddc_in14.txt",  400);
	i_ddc[13] =  input_plio::create("ddc_in15",    plio_32_bits, "ddc_in15.txt",  400);
	i_ddc[14] =  input_plio::create("ddc_in16",    plio_32_bits, "ddc_in16.txt",  400);
	i_ddc[15] =  input_plio::create("ddc_in17",    plio_32_bits, "ddc_in17.txt",  400);
			    
	i_ddc[16] =  input_plio::create("ddc_in20",    plio_32_bits, "ddc_in20.txt",  400);
	i_ddc[17] =  input_plio::create("ddc_in21",    plio_32_bits, "ddc_in21.txt",  400);
	i_ddc[18] =  input_plio::create("ddc_in22",    plio_32_bits, "ddc_in22.txt",  400);
	i_ddc[19] =  input_plio::create("ddc_in23",    plio_32_bits, "ddc_in23.txt",  400);
	i_ddc[20] =  input_plio::create("ddc_in24",    plio_32_bits, "ddc_in24.txt",  400);
	i_ddc[21] =  input_plio::create("ddc_in25",    plio_32_bits, "ddc_in25.txt",  400);
	i_ddc[22] =  input_plio::create("ddc_in26",    plio_32_bits, "ddc_in26.txt",  400);
	i_ddc[23] =  input_plio::create("ddc_in27",    plio_32_bits, "ddc_in27.txt",  400);
			    
	i_ddc[24] =  input_plio::create("ddc_in30",    plio_32_bits, "ddc_in30.txt",  400);
	i_ddc[25] =  input_plio::create("ddc_in31",    plio_32_bits, "ddc_in31.txt",  400);
	i_ddc[26] =  input_plio::create("ddc_in32",    plio_32_bits, "ddc_in32.txt",  400);
	i_ddc[27] =  input_plio::create("ddc_in33",    plio_32_bits, "ddc_in33.txt",  400);
	i_ddc[28] =  input_plio::create("ddc_in34",    plio_32_bits, "ddc_in34.txt",  400);
	i_ddc[29] =  input_plio::create("ddc_in35",    plio_32_bits, "ddc_in35.txt",  400);
	i_ddc[30] =  input_plio::create("ddc_in36",    plio_32_bits, "ddc_in36.txt",  400);
	i_ddc[31] =  input_plio::create("ddc_in37",    plio_32_bits, "ddc_in37.txt",  400);
	
	o_ddc[0]   = output_plio::create("ddc_out00",   plio_32_bits, "ddc_out00.txt", 400);
	o_ddc[1]   = output_plio::create("ddc_out01",   plio_32_bits, "ddc_out01.txt", 400);
	o_ddc[2]   = output_plio::create("ddc_out02",   plio_32_bits, "ddc_out02.txt", 400);
	o_ddc[3]   = output_plio::create("ddc_out03",   plio_32_bits, "ddc_out03.txt", 400);
	o_ddc[4]   = output_plio::create("ddc_out04",   plio_32_bits, "ddc_out04.txt", 400);
	o_ddc[5]   = output_plio::create("ddc_out05",   plio_32_bits, "ddc_out05.txt", 400);
	o_ddc[6]   = output_plio::create("ddc_out06",   plio_32_bits, "ddc_out06.txt", 400);
	o_ddc[7]   = output_plio::create("ddc_out07",   plio_32_bits, "ddc_out07.txt", 400);
	
	o_ddc[8]  = output_plio::create("ddc_out10",   plio_32_bits, "ddc_out10.txt", 400);
	o_ddc[9]  = output_plio::create("ddc_out11",   plio_32_bits, "ddc_out11.txt", 400);
	o_ddc[10] = output_plio::create("ddc_out12",   plio_32_bits, "ddc_out12.txt", 400);
	o_ddc[11] = output_plio::create("ddc_out13",   plio_32_bits, "ddc_out13.txt", 400);
	o_ddc[12] = output_plio::create("ddc_out14",   plio_32_bits, "ddc_out14.txt", 400);
	o_ddc[13] = output_plio::create("ddc_out15",   plio_32_bits, "ddc_out15.txt", 400);
	o_ddc[14] = output_plio::create("ddc_out16",   plio_32_bits, "ddc_out16.txt", 400);
	o_ddc[15] = output_plio::create("ddc_out17",   plio_32_bits, "ddc_out17.txt", 400);
  
	o_ddc[16] = output_plio::create("ddc_out20",   plio_32_bits, "ddc_out20.txt", 400);
	o_ddc[17] = output_plio::create("ddc_out21",   plio_32_bits, "ddc_out21.txt", 400);
	o_ddc[18] = output_plio::create("ddc_out22",   plio_32_bits, "ddc_out22.txt", 400);
	o_ddc[19] = output_plio::create("ddc_out23",   plio_32_bits, "ddc_out23.txt", 400);
	o_ddc[20] = output_plio::create("ddc_out24",   plio_32_bits, "ddc_out24.txt", 400);
	o_ddc[21] = output_plio::create("ddc_out25",   plio_32_bits, "ddc_out25.txt", 400);
	o_ddc[22] = output_plio::create("ddc_out26",   plio_32_bits, "ddc_out26.txt", 400);
	o_ddc[23] = output_plio::create("ddc_out27",   plio_32_bits, "ddc_out27.txt", 400);
	  
	o_ddc[24] = output_plio::create("ddc_out30",   plio_32_bits, "ddc_out30.txt", 400);
	o_ddc[25] = output_plio::create("ddc_out31",   plio_32_bits, "ddc_out31.txt", 400);
	o_ddc[26] = output_plio::create("ddc_out32",   plio_32_bits, "ddc_out32.txt", 400);
	o_ddc[27] = output_plio::create("ddc_out33",   plio_32_bits, "ddc_out33.txt", 400);
	o_ddc[28] = output_plio::create("ddc_out34",   plio_32_bits, "ddc_out34.txt", 400);
	o_ddc[29] = output_plio::create("ddc_out35",   plio_32_bits, "ddc_out35.txt", 400);
	o_ddc[30] = output_plio::create("ddc_out36",   plio_32_bits, "ddc_out36.txt", 400);
	o_ddc[31] = output_plio::create("ddc_out37",   plio_32_bits, "ddc_out37.txt", 400);
	
	for(int j=0;j<32;j++) {
		connect<stream>(i_ddc[j].out[0], ddc_top.data_in[j]);
		connect<stream>(ddc_top.data_out[j],  o_ddc[j].in[0]);
	}
		
    };
	
}; // end of class

#endif
