
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
 
#include <adf.h>
#include "subsys.h" 

using namespace adf;

template<int COL0, int COL1, int COL2>
class TEST_BF: public graph{
  
public:

	input_plio  dldin[4*3];
	input_plio  dlcin[32*3];
	output_plio dlout[8*3];	

	input_plio  uldin[8*3];
	input_plio  ulcin[32*3];
	output_plio ulout[4*3];	

	// beamforming, 64 antenna, 32 layers
	DL64A32L<COL0+0, 0>  dlbf_0;
	DL64A32L<COL1+0, 0>  dlbf_1;
	DL64A32L<COL2+0, 0>  dlbf_2;
	UL64A32L<COL0+8, 0>  ulbf_0;
	UL64A32L<COL1+8, 0>  ulbf_1;
	UL64A32L<COL2+8, 0>  ulbf_2;
 TEST_BF(){

	//DLBF_0: 4 Data Input Ports 
	dldin[0] = input_plio::create("dlbfid0", plio_64_bits, "../data/dlbf_din0.txt", 400);
	 dldin[1] = input_plio::create("dlbfid1", plio_64_bits, "../data/dlbf_din1.txt", 400);
	 dldin[2] = input_plio::create("dlbfid2", plio_64_bits, "../data/dlbf_din2.txt", 400);
	 dldin[3] = input_plio::create("dlbfid3", plio_64_bits, "../data/dlbf_din3.txt", 400);

	//DLBF_0, Chain 0: 4 Data Input Coefficient Ports 
	 dlcin[0] = input_plio::create("dlbfic00", plio_64_bits, "../data/dlbf_cin00.txt", 400);
	 dlcin[1] = input_plio::create("dlbfic01", plio_64_bits, "../data/dlbf_cin01.txt", 400);
	 dlcin[2] = input_plio::create("dlbfic02", plio_64_bits, "../data/dlbf_cin02.txt", 400);
	 dlcin[3] = input_plio::create("dlbfic03", plio_64_bits, "../data/dlbf_cin03.txt", 400);

	//DLBF_0, Chain 1: 4 Data Input Coefficient Ports 
	 dlcin[4] = input_plio::create("dlbfic10", plio_64_bits, "../data/dlbf_cin10.txt", 400);
	 dlcin[5]= input_plio::create("dlbfic11", plio_64_bits, "../data/dlbf_cin11.txt", 400);
	 dlcin[6]= input_plio::create("dlbfic12", plio_64_bits, "../data/dlbf_cin12.txt", 400);
	 dlcin[7] = input_plio::create("dlbfic13", plio_64_bits, "../data/dlbf_cin13.txt", 400);

	//DLBF_0, Chain 2: 4 Data Input Coefficient Ports 
	 dlcin[8] = input_plio::create("dlbfic20", plio_64_bits, "../data/dlbf_cin20.txt", 400);
	 dlcin[9] = input_plio::create("dlbfic21", plio_64_bits, "../data/dlbf_cin21.txt", 400);
	 dlcin[10] = input_plio::create("dlbfic22", plio_64_bits, "../data/dlbf_cin22.txt", 400);
	 dlcin[11] = input_plio::create("dlbfic23", plio_64_bits, "../data/dlbf_cin23.txt", 400);

	//DLBF_0, Chain 3: 4 Data Input Coefficient Ports 
	 dlcin[12]= input_plio::create("dlbfic30", plio_64_bits, "../data/dlbf_cin30.txt", 400);
	 dlcin[13] = input_plio::create("dlbfic31", plio_64_bits, "../data/dlbf_cin31.txt", 400);
	 dlcin[14] = input_plio::create("dlbfic32", plio_64_bits, "../data/dlbf_cin32.txt", 400);
	 dlcin[15] = input_plio::create("dlbfic33", plio_64_bits, "../data/dlbf_cin33.txt", 400);

	//DLBF_0, Chain 4: 4 Data Input Coefficient Ports 
	 dlcin[16] = input_plio::create("dlbfic40", plio_64_bits, "../data/dlbf_cin40.txt", 400);
	 dlcin[17] = input_plio::create("dlbfic41", plio_64_bits, "../data/dlbf_cin41.txt", 400);
	 dlcin[18] = input_plio::create("dlbfic42", plio_64_bits, "../data/dlbf_cin42.txt", 400);
	 dlcin[19] = input_plio::create("dlbfic43", plio_64_bits, "../data/dlbf_cin43.txt", 400);

	//DLBF_0, Chain 5: 4 Data Input Coefficient Ports 
	 dlcin[20] = input_plio::create("dlbfic50", plio_64_bits, "../data/dlbf_cin50.txt", 400);
	 dlcin[21] = input_plio::create("dlbfic51", plio_64_bits, "../data/dlbf_cin51.txt", 400);
	 dlcin[22] = input_plio::create("dlbfic52", plio_64_bits, "../data/dlbf_cin52.txt", 400);
	 dlcin[23] = input_plio::create("dlbfic53", plio_64_bits, "../data/dlbf_cin53.txt", 400);

	//DLBF_0, Chain 6: 4 Data Input Coefficient Ports 
	 dlcin[24] = input_plio::create("dlbfic60", plio_64_bits, "../data/dlbf_cin60.txt", 400);
	 dlcin[25] = input_plio::create("dlbfic61", plio_64_bits, "../data/dlbf_cin61.txt", 400);
	 dlcin[26] = input_plio::create("dlbfic62", plio_64_bits, "../data/dlbf_cin62.txt", 400);
	 dlcin[27] = input_plio::create("dlbfic63", plio_64_bits, "../data/dlbf_cin63.txt", 400);

	//DLBF_0, Chain 7: 4 Data Input Coefficient Ports 
	 dlcin[28] = input_plio::create("dlbfic70", plio_64_bits, "../data/dlbf_cin70.txt", 400);
	 dlcin[29] = input_plio::create("dlbfic71", plio_64_bits, "../data/dlbf_cin71.txt", 400);
	 dlcin[30] = input_plio::create("dlbfic72", plio_64_bits, "../data/dlbf_cin72.txt", 400);
	 dlcin[31] = input_plio::create("dlbfic73", plio_64_bits, "../data/dlbf_cin73.txt", 400);

	//DLBF_0, 8 Data Output Ports, for Chains 0-7 
	dlout[0]  = output_plio::create("dlbfo0",  plio_64_bits, "data/dlbf_out0.txt", 400);
	dlout[1]  = output_plio::create("dlbfo1",  plio_64_bits, "data/dlbf_out1.txt", 400);
	dlout[2]  = output_plio::create("dlbfo2",  plio_64_bits, "data/dlbf_out2.txt", 400);
	dlout[3]  = output_plio::create("dlbfo3",  plio_64_bits, "data/dlbf_out3.txt", 400);
	dlout[4]  = output_plio::create("dlbfo4",  plio_64_bits, "data/dlbf_out4.txt", 400);
	dlout[5]  = output_plio::create("dlbfo5",  plio_64_bits, "data/dlbf_out5.txt", 400);
	dlout[6]  = output_plio::create("dlbfo6",  plio_64_bits, "data/dlbf_out6.txt", 400);
	dlout[7]  = output_plio::create("dlbfo7",  plio_64_bits, "data/dlbf_out7.txt", 400);	 
	//DLBF_1: 4 Data Input Ports 
	dldin[4] = input_plio::create("dlbfid_a0", plio_64_bits, "../data/dlbf_din0.txt", 400);
	dldin[5] = input_plio::create("dlbfid_a1", plio_64_bits, "../data/dlbf_din1.txt", 400);
	dldin[6] = input_plio::create("dlbfid_a2", plio_64_bits, "../data/dlbf_din2.txt", 400);
	dldin[7] = input_plio::create("dlbfid_a3", plio_64_bits, "../data/dlbf_din3.txt", 400);

	//DLBF_1, Chain 0: 4 Data Input Coefficient Ports 
	dlcin[32] = input_plio::create("dlbfic_a00", plio_64_bits, "../data/dlbf_cin00.txt", 400);
	dlcin[33] = input_plio::create("dlbfic_a01", plio_64_bits, "../data/dlbf_cin01.txt", 400);
	dlcin[34] = input_plio::create("dlbfic_a02", plio_64_bits, "../data/dlbf_cin02.txt", 400);
	dlcin[35] = input_plio::create("dlbfic_a03", plio_64_bits, "../data/dlbf_cin03.txt", 400);

	//DLBF_1, Chain 1: 4 Data Input Coefficient Ports 
	dlcin[36] = input_plio::create("dlbfic_a10", plio_64_bits, "../data/dlbf_cin10.txt", 400);
	dlcin[37] = input_plio::create("dlbfic_a11", plio_64_bits, "../data/dlbf_cin11.txt", 400);
	dlcin[38] = input_plio::create("dlbfic_a12", plio_64_bits, "../data/dlbf_cin12.txt", 400);
	dlcin[39] = input_plio::create("dlbfic_a13", plio_64_bits, "../data/dlbf_cin13.txt", 400);

	//DLBF_1, Chain 2: 4 Data Input Coefficient Ports 
	dlcin[40] = input_plio::create("dlbfic_a20", plio_64_bits, "../data/dlbf_cin20.txt", 400);
	dlcin[41] = input_plio::create("dlbfic_a21", plio_64_bits, "../data/dlbf_cin21.txt", 400);
	dlcin[42] = input_plio::create("dlbfic_a22", plio_64_bits, "../data/dlbf_cin22.txt", 400);
	dlcin[43] = input_plio::create("dlbfic_a23", plio_64_bits, "../data/dlbf_cin23.txt", 400);

	//DLBF_1, Chain 3: 4 Data Input Coefficient Ports 
	dlcin[44] = input_plio::create("dlbfic_a30", plio_64_bits, "../data/dlbf_cin30.txt", 400);
	dlcin[45] = input_plio::create("dlbfic_a31", plio_64_bits, "../data/dlbf_cin31.txt", 400);
	dlcin[46] = input_plio::create("dlbfic_a32", plio_64_bits, "../data/dlbf_cin32.txt", 400);
	dlcin[47] = input_plio::create("dlbfic_a33", plio_64_bits, "../data/dlbf_cin33.txt", 400);

	//DLBF_1, Chain 4: 4 Data Input Coefficient Ports 
	dlcin[48] = input_plio::create("dlbfic_a40", plio_64_bits, "../data/dlbf_cin40.txt", 400);
	dlcin[49] = input_plio::create("dlbfic_a41", plio_64_bits, "../data/dlbf_cin41.txt", 400);
	dlcin[50] = input_plio::create("dlbfic_a42", plio_64_bits, "../data/dlbf_cin42.txt", 400);
	dlcin[51] = input_plio::create("dlbfic_a43", plio_64_bits, "../data/dlbf_cin43.txt", 400);

	//DLBF_1, Chain 5: 4 Data Input Coefficient Ports 
	dlcin[52] = input_plio::create("dlbfic_a50", plio_64_bits, "../data/dlbf_cin50.txt", 400);
	dlcin[53] = input_plio::create("dlbfic_a51", plio_64_bits, "../data/dlbf_cin51.txt", 400);
	dlcin[54] = input_plio::create("dlbfic_a52", plio_64_bits, "../data/dlbf_cin52.txt", 400);
	dlcin[55] = input_plio::create("dlbfic_a53", plio_64_bits, "../data/dlbf_cin53.txt", 400);

	//DLBF_1, Chain 6: 4 Data Input Coefficient Ports 
	dlcin[56] = input_plio::create("dlbfic_a60", plio_64_bits, "../data/dlbf_cin60.txt", 400);
	dlcin[57] = input_plio::create("dlbfic_a61", plio_64_bits, "../data/dlbf_cin61.txt", 400);
	dlcin[58] = input_plio::create("dlbfic_a62", plio_64_bits, "../data/dlbf_cin62.txt", 400);
	dlcin[59] = input_plio::create("dlbfic_a63", plio_64_bits, "../data/dlbf_cin63.txt", 400);

	//DLBF_1, Chain 7: 4 Data Input Coefficient Ports 
	dlcin[60] = input_plio::create("dlbfic_a70", plio_64_bits, "../data/dlbf_cin70.txt", 400);
	dlcin[61] = input_plio::create("dlbfic_a71", plio_64_bits, "../data/dlbf_cin71.txt", 400);
	dlcin[62] = input_plio::create("dlbfic_a72", plio_64_bits, "../data/dlbf_cin72.txt", 400);
	dlcin[63] = input_plio::create("dlbfic_a73", plio_64_bits, "../data/dlbf_cin73.txt", 400);

	//DLBF_1, 8 Data Output Ports, for Chains 0-7 
	dlout[8]  = output_plio::create("dlbfo_a0",  plio_64_bits, "data/dlbf_out8.txt", 400);
	dlout[9]  = output_plio::create("dlbfo_a1",  plio_64_bits, "data/dlbf_out9.txt", 400);
	dlout[10]  = output_plio::create("dlbfo_a2",  plio_64_bits, "data/dlbf_out10.txt", 400);
	dlout[11]  = output_plio::create("dlbfo_a3",  plio_64_bits, "data/dlbf_out11.txt", 400);
	dlout[12]  = output_plio::create("dlbfo_a4",  plio_64_bits, "data/dlbf_out12.txt", 400);
	dlout[13]  = output_plio::create("dlbfo_a5",  plio_64_bits, "data/dlbf_out13.txt", 400);
	dlout[14]  = output_plio::create("dlbfo_a6",  plio_64_bits, "data/dlbf_out14.txt", 400);
	dlout[15]  = output_plio::create("dlbfo_a7",  plio_64_bits, "data/dlbf_out15.txt", 400);

	//DLBF_2: 4 Data Input Ports 
	dldin[8] = input_plio::create("dlbfid_b0", plio_64_bits, "../data/dlbf_din0.txt", 400);
	dldin[9] = input_plio::create("dlbfid_b1", plio_64_bits, "../data/dlbf_din1.txt", 400);
	dldin[10] = input_plio::create("dlbfid_b2", plio_64_bits, "../data/dlbf_din2.txt", 400);
	dldin[11] = input_plio::create("dlbfid_b3", plio_64_bits, "../data/dlbf_din3.txt", 400);

	//DLBF_2, Chain 0: 4 Data Input Coefficient Ports 
	dlcin[64] = input_plio::create("dlbfic_b00", plio_64_bits, "../data/dlbf_cin00.txt", 400);
	dlcin[65] = input_plio::create("dlbfic_b01", plio_64_bits, "../data/dlbf_cin01.txt", 400);
	dlcin[66] = input_plio::create("dlbfic_b02", plio_64_bits, "../data/dlbf_cin02.txt", 400);
	dlcin[67] = input_plio::create("dlbfic_b03", plio_64_bits, "../data/dlbf_cin03.txt", 400);

	//DLBF_2, Chain 1: 4 Data Input Coefficient Ports 
	dlcin[68] = input_plio::create("dlbfic_b10", plio_64_bits, "../data/dlbf_cin10.txt", 400);
	dlcin[69] = input_plio::create("dlbfic_b11", plio_64_bits, "../data/dlbf_cin11.txt", 400);
	dlcin[70] = input_plio::create("dlbfic_b12", plio_64_bits, "../data/dlbf_cin12.txt", 400);
	dlcin[71] = input_plio::create("dlbfic_b13", plio_64_bits, "../data/dlbf_cin13.txt", 400);

	//DLBF_2, Chain 2: 4 Data Input Coefficient Ports 
	dlcin[72] = input_plio::create("dlbfic_b20", plio_64_bits, "../data/dlbf_cin20.txt", 400);
	dlcin[73] = input_plio::create("dlbfic_b21", plio_64_bits, "../data/dlbf_cin21.txt", 400);
	dlcin[74] = input_plio::create("dlbfic_b22", plio_64_bits, "../data/dlbf_cin22.txt", 400);
	dlcin[75] = input_plio::create("dlbfic_b23", plio_64_bits, "../data/dlbf_cin23.txt", 400);

	//DLBF_2, Chain 3: 4 Data Input Coefficient Ports 
	dlcin[76] = input_plio::create("dlbfic_b30", plio_64_bits, "../data/dlbf_cin30.txt", 400);
	dlcin[77] = input_plio::create("dlbfic_b31", plio_64_bits, "../data/dlbf_cin31.txt", 400);
	dlcin[78] = input_plio::create("dlbfic_b32", plio_64_bits, "../data/dlbf_cin32.txt", 400);
	dlcin[79] = input_plio::create("dlbfic_b33", plio_64_bits, "../data/dlbf_cin33.txt", 400);

	//DLBF_2, Chain 4: 4 Data Input Coefficient Ports 
	dlcin[80] = input_plio::create("dlbfic_b40", plio_64_bits, "../data/dlbf_cin40.txt", 400);
	dlcin[81] = input_plio::create("dlbfic_b41", plio_64_bits, "../data/dlbf_cin41.txt", 400);
	dlcin[82] = input_plio::create("dlbfic_b42", plio_64_bits, "../data/dlbf_cin42.txt", 400);
	dlcin[83] = input_plio::create("dlbfic_b43", plio_64_bits, "../data/dlbf_cin43.txt", 400);

	//DLBF_2, Chain 5: 4 Data Input Coefficient Ports 
	dlcin[84] = input_plio::create("dlbfic_b50", plio_64_bits, "../data/dlbf_cin50.txt", 400);
	dlcin[85] = input_plio::create("dlbfic_b51", plio_64_bits, "../data/dlbf_cin51.txt", 400);
	dlcin[86] = input_plio::create("dlbfic_b52", plio_64_bits, "../data/dlbf_cin52.txt", 400);
	dlcin[87] = input_plio::create("dlbfic_b53", plio_64_bits, "../data/dlbf_cin53.txt", 400);

	//DLBF_2, Chain 6: 4 Data Input Coefficient Ports 
	dlcin[88] = input_plio::create("dlbfic_b60", plio_64_bits, "../data/dlbf_cin60.txt", 400);
	dlcin[89] = input_plio::create("dlbfic_b61", plio_64_bits, "../data/dlbf_cin61.txt", 400);
	dlcin[90] = input_plio::create("dlbfic_b62", plio_64_bits, "../data/dlbf_cin62.txt", 400);
	dlcin[91] = input_plio::create("dlbfic_b63", plio_64_bits, "../data/dlbf_cin63.txt", 400);

	//DLBF_2, Chain 7: 4 Data Input Coefficient Ports 
	dlcin[92] = input_plio::create("dlbfic_b70", plio_64_bits, "../data/dlbf_cin70.txt", 400);
	dlcin[93] = input_plio::create("dlbfic_b71", plio_64_bits, "../data/dlbf_cin71.txt", 400);
	dlcin[94] = input_plio::create("dlbfic_b72", plio_64_bits, "../data/dlbf_cin72.txt", 400);
	dlcin[95] = input_plio::create("dlbfic_b73", plio_64_bits, "../data/dlbf_cin73.txt", 400);

	//DLBF_2, 8 Data Output Ports, for Chains 0-7 
	dlout[16]  = output_plio::create("dlbfo_b0",  plio_64_bits, "data/dlbf_out16.txt", 400);
	dlout[17]  = output_plio::create("dlbfo_b1",  plio_64_bits, "data/dlbf_out17.txt", 400);
	dlout[18]  = output_plio::create("dlbfo_b2",  plio_64_bits, "data/dlbf_out18.txt", 400);
	dlout[19]  = output_plio::create("dlbfo_b3",  plio_64_bits, "data/dlbf_out19.txt", 400);
	dlout[20]  = output_plio::create("dlbfo_b4",  plio_64_bits, "data/dlbf_out20.txt", 400);
	dlout[21]  = output_plio::create("dlbfo_b5",  plio_64_bits, "data/dlbf_out21.txt", 400);
	dlout[22]  = output_plio::create("dlbfo_b6",  plio_64_bits, "data/dlbf_out22.txt", 400);
	dlout[23]  = output_plio::create("dlbfo_b7",  plio_64_bits, "data/dlbf_out23.txt", 400);

	//ULBF_0: 8 Data Input Ports 
	uldin[0] = input_plio::create("ulbfid0", plio_64_bits, "../data/ulbf_din0.txt", 400);
	uldin[1] = input_plio::create("ulbfid1", plio_64_bits, "../data/ulbf_din1.txt", 400);
	uldin[2] = input_plio::create("ulbfid2", plio_64_bits, "../data/ulbf_din2.txt", 400);
	uldin[3] = input_plio::create("ulbfid3", plio_64_bits, "../data/ulbf_din3.txt", 400);
	uldin[4] = input_plio::create("ulbfid4", plio_64_bits, "../data/ulbf_din4.txt", 400);
	uldin[5] = input_plio::create("ulbfid5", plio_64_bits, "../data/ulbf_din5.txt", 400);
	uldin[6] = input_plio::create("ulbfid6", plio_64_bits, "../data/ulbf_din6.txt", 400);
	uldin[7] = input_plio::create("ulbfid7", plio_64_bits, "../data/ulbf_din7.txt", 400);

	//ULBF_0, Chain 0: 8 Data Input Coefficient Ports 
	ulcin[0] = input_plio::create("ulbfic00", plio_64_bits, "../data/ulbf_cin00.txt", 400);
	ulcin[1] = input_plio::create("ulbfic01", plio_64_bits, "../data/ulbf_cin01.txt", 400);
	ulcin[2] = input_plio::create("ulbfic02", plio_64_bits, "../data/ulbf_cin02.txt", 400);
	ulcin[3] = input_plio::create("ulbfic03", plio_64_bits, "../data/ulbf_cin03.txt", 400);
	ulcin[4] = input_plio::create("ulbfic04", plio_64_bits, "../data/ulbf_cin04.txt", 400);
	ulcin[5] = input_plio::create("ulbfic05", plio_64_bits, "../data/ulbf_cin05.txt", 400);
	ulcin[6] = input_plio::create("ulbfic06", plio_64_bits, "../data/ulbf_cin06.txt", 400);
	ulcin[7] = input_plio::create("ulbfic07", plio_64_bits, "../data/ulbf_cin07.txt", 400);

	//ULBF_0, Chain 1: 8 Data Input Coefficient Ports 
	ulcin[8] = input_plio::create("ulbfic10", plio_64_bits, "../data/ulbf_cin10.txt", 400);
	ulcin[9] = input_plio::create("ulbfic11", plio_64_bits, "../data/ulbf_cin11.txt", 400);
	ulcin[10] = input_plio::create("ulbfic12", plio_64_bits, "../data/ulbf_cin12.txt", 400);
	ulcin[11] = input_plio::create("ulbfic13", plio_64_bits, "../data/ulbf_cin13.txt", 400);
	ulcin[12] = input_plio::create("ulbfic14", plio_64_bits, "../data/ulbf_cin14.txt", 400);
	ulcin[13] = input_plio::create("ulbfic15", plio_64_bits, "../data/ulbf_cin15.txt", 400);
	ulcin[14] = input_plio::create("ulbfic16", plio_64_bits, "../data/ulbf_cin16.txt", 400);
	ulcin[15] = input_plio::create("ulbfic17", plio_64_bits, "../data/ulbf_cin17.txt", 400);

	//ULBF_0, Chain 2: 8 Data Input Coefficient Ports 
	ulcin[16] = input_plio::create("ulbfic20", plio_64_bits, "../data/ulbf_cin20.txt", 400);
	ulcin[17] = input_plio::create("ulbfic21", plio_64_bits, "../data/ulbf_cin21.txt", 400);
	ulcin[18] = input_plio::create("ulbfic22", plio_64_bits, "../data/ulbf_cin22.txt", 400);
	ulcin[19] = input_plio::create("ulbfic23", plio_64_bits, "../data/ulbf_cin23.txt", 400);
	ulcin[20] = input_plio::create("ulbfic24", plio_64_bits, "../data/ulbf_cin24.txt", 400);
	ulcin[21] = input_plio::create("ulbfic25", plio_64_bits, "../data/ulbf_cin25.txt", 400);
	ulcin[22] = input_plio::create("ulbfic26", plio_64_bits, "../data/ulbf_cin26.txt", 400);
	ulcin[23] = input_plio::create("ulbfic27", plio_64_bits, "../data/ulbf_cin27.txt", 400);

	//ULBF_0, Chain 3: 8 Data Input Coefficient Ports 
	ulcin[24] = input_plio::create("ulbfic30", plio_64_bits, "../data/ulbf_cin30.txt", 400);
	ulcin[25] = input_plio::create("ulbfic31", plio_64_bits, "../data/ulbf_cin31.txt", 400);
	ulcin[26] = input_plio::create("ulbfic32", plio_64_bits, "../data/ulbf_cin32.txt", 400);
	ulcin[27] = input_plio::create("ulbfic33", plio_64_bits, "../data/ulbf_cin33.txt", 400);
	ulcin[28] = input_plio::create("ulbfic34", plio_64_bits, "../data/ulbf_cin34.txt", 400);
	ulcin[29] = input_plio::create("ulbfic35", plio_64_bits, "../data/ulbf_cin35.txt", 400);
	ulcin[30] = input_plio::create("ulbfic36", plio_64_bits, "../data/ulbf_cin36.txt", 400);
	ulcin[31] = input_plio::create("ulbfic37", plio_64_bits, "../data/ulbf_cin37.txt", 400);

	//ULBF_0, 4 Data Output Ports, for Chains 0-3
	ulout[0]  = output_plio::create("ulbfo0",  plio_64_bits, "data/ulbf_out0.txt", 400);
	ulout[1]  = output_plio::create("ulbfo1",  plio_64_bits, "data/ulbf_out1.txt", 400);
	ulout[2]  = output_plio::create("ulbfo2",  plio_64_bits, "data/ulbf_out2.txt", 400);
	ulout[3]  = output_plio::create("ulbfo3",  plio_64_bits, "data/ulbf_out3.txt", 400);

	//ULBF_1: 8 Data Input Ports 
	uldin[8] = input_plio::create("ulbfid_a0", plio_64_bits, "../data/ulbf_din0.txt", 400);
	uldin[9] = input_plio::create("ulbfid_a1", plio_64_bits, "../data/ulbf_din1.txt", 400);
	uldin[10] = input_plio::create("ulbfid_a2", plio_64_bits, "../data/ulbf_din2.txt", 400);
	uldin[11] = input_plio::create("ulbfid_a3", plio_64_bits, "../data/ulbf_din3.txt", 400);
	uldin[12] = input_plio::create("ulbfid_a4", plio_64_bits, "../data/ulbf_din4.txt", 400);
	uldin[13] = input_plio::create("ulbfid_a5", plio_64_bits, "../data/ulbf_din5.txt", 400);
	uldin[14] = input_plio::create("ulbfid_a6", plio_64_bits, "../data/ulbf_din6.txt", 400);
	uldin[15] = input_plio::create("ulbfid_a7", plio_64_bits, "../data/ulbf_din7.txt", 400);

	//ULBF_1, Chain 0: 8 Data Input Coefficient Ports 
	ulcin[32] = input_plio::create("ulbfic_a00", plio_64_bits, "../data/ulbf_cin00.txt", 400);
	ulcin[33] = input_plio::create("ulbfic_a01", plio_64_bits, "../data/ulbf_cin01.txt", 400);
	ulcin[34] = input_plio::create("ulbfic_a02", plio_64_bits, "../data/ulbf_cin02.txt", 400);
	ulcin[35] = input_plio::create("ulbfic_a03", plio_64_bits, "../data/ulbf_cin03.txt", 400);
	ulcin[36] = input_plio::create("ulbfic_a04", plio_64_bits, "../data/ulbf_cin04.txt", 400);
	ulcin[37] = input_plio::create("ulbfic_a05", plio_64_bits, "../data/ulbf_cin05.txt", 400);
	ulcin[38] = input_plio::create("ulbfic_a06", plio_64_bits, "../data/ulbf_cin06.txt", 400);
	ulcin[39] = input_plio::create("ulbfic_a07", plio_64_bits, "../data/ulbf_cin07.txt", 400);

	//ULBF_1, Chain 1: 8 Data Input Coefficient Ports 
	ulcin[40] = input_plio::create("ulbfic_a10", plio_64_bits, "../data/ulbf_cin10.txt", 400);
	ulcin[41] = input_plio::create("ulbfic_a11", plio_64_bits, "../data/ulbf_cin11.txt", 400);
	ulcin[42] = input_plio::create("ulbfic_a12", plio_64_bits, "../data/ulbf_cin12.txt", 400);
	ulcin[43] = input_plio::create("ulbfic_a13", plio_64_bits, "../data/ulbf_cin13.txt", 400);
	ulcin[44] = input_plio::create("ulbfic_a14", plio_64_bits, "../data/ulbf_cin14.txt", 400);
	ulcin[45] = input_plio::create("ulbfic_a15", plio_64_bits, "../data/ulbf_cin15.txt", 400);
	ulcin[46] = input_plio::create("ulbfic_a16", plio_64_bits, "../data/ulbf_cin16.txt", 400);
	ulcin[47] = input_plio::create("ulbfic_a17", plio_64_bits, "../data/ulbf_cin17.txt", 400);

	//ULBF_1, Chain 2: 8 Data Input Coefficient Ports 
	ulcin[48] = input_plio::create("ulbfic_a20", plio_64_bits, "../data/ulbf_cin20.txt", 400);
	ulcin[49] = input_plio::create("ulbfic_a21", plio_64_bits, "../data/ulbf_cin21.txt", 400);
	ulcin[50] = input_plio::create("ulbfic_a22", plio_64_bits, "../data/ulbf_cin22.txt", 400);
	ulcin[51] = input_plio::create("ulbfic_a23", plio_64_bits, "../data/ulbf_cin23.txt", 400);
	ulcin[52] = input_plio::create("ulbfic_a24", plio_64_bits, "../data/ulbf_cin24.txt", 400);
	ulcin[53] = input_plio::create("ulbfic_a25", plio_64_bits, "../data/ulbf_cin25.txt", 400);
	ulcin[54] = input_plio::create("ulbfic_a26", plio_64_bits, "../data/ulbf_cin26.txt", 400);
	ulcin[55] = input_plio::create("ulbfic_a27", plio_64_bits, "../data/ulbf_cin27.txt", 400);

	//ULBF_1, Chain 3: 8 Data Input Coefficient Ports 
	ulcin[56] = input_plio::create("ulbfic_a30", plio_64_bits, "../data/ulbf_cin30.txt", 400);
	ulcin[57] = input_plio::create("ulbfic_a31", plio_64_bits, "../data/ulbf_cin31.txt", 400);
	ulcin[58] = input_plio::create("ulbfic_a32", plio_64_bits, "../data/ulbf_cin32.txt", 400);
	ulcin[59] = input_plio::create("ulbfic_a33", plio_64_bits, "../data/ulbf_cin33.txt", 400);
	ulcin[60] = input_plio::create("ulbfic_a34", plio_64_bits, "../data/ulbf_cin34.txt", 400);
	ulcin[61] = input_plio::create("ulbfic_a35", plio_64_bits, "../data/ulbf_cin35.txt", 400);
	ulcin[62] = input_plio::create("ulbfic_a36", plio_64_bits, "../data/ulbf_cin36.txt", 400);
	ulcin[63] = input_plio::create("ulbfic_a37", plio_64_bits, "../data/ulbf_cin37.txt", 400);

	//ULBF_1, 4 Data Output Ports, for Chains 0-3
	ulout[4]  = output_plio::create("ulbfo_a0",  plio_64_bits, "data/ulbf_out4.txt", 400);
	ulout[5]  = output_plio::create("ulbfo_a1",  plio_64_bits, "data/ulbf_out5.txt", 400);
	ulout[6]  = output_plio::create("ulbfo_a2",  plio_64_bits, "data/ulbf_out6.txt", 400);
	ulout[7]  = output_plio::create("ulbfo_a3",  plio_64_bits, "data/ulbf_out7.txt", 400);

	//ULBF_2: 8 Data Input Ports 
	uldin[16] = input_plio::create("ulbfid_b0", plio_64_bits, "../data/ulbf_din0.txt", 400);
	uldin[17] = input_plio::create("ulbfid_b1", plio_64_bits, "../data/ulbf_din1.txt", 400);
	uldin[18] = input_plio::create("ulbfid_b2", plio_64_bits, "../data/ulbf_din2.txt", 400);
	uldin[19] = input_plio::create("ulbfid_b3", plio_64_bits, "../data/ulbf_din3.txt", 400);
	uldin[20] = input_plio::create("ulbfid_b4", plio_64_bits, "../data/ulbf_din4.txt", 400);
	uldin[21] = input_plio::create("ulbfid_b5", plio_64_bits, "../data/ulbf_din5.txt", 400);
	uldin[22] = input_plio::create("ulbfid_b6", plio_64_bits, "../data/ulbf_din6.txt", 400);
	uldin[23] = input_plio::create("ulbfid_b7", plio_64_bits, "../data/ulbf_din7.txt", 400);

	//ULBF_2, Chain 0: 8 Data Input Coefficient Ports 
	ulcin[64] = input_plio::create("ulbfic_b00", plio_64_bits, "../data/ulbf_cin00.txt", 400);
	ulcin[65] = input_plio::create("ulbfic_b01", plio_64_bits, "../data/ulbf_cin01.txt", 400);
	ulcin[66] = input_plio::create("ulbfic_b02", plio_64_bits, "../data/ulbf_cin02.txt", 400);
	ulcin[67] = input_plio::create("ulbfic_b03", plio_64_bits, "../data/ulbf_cin03.txt", 400);
	ulcin[68] = input_plio::create("ulbfic_b04", plio_64_bits, "../data/ulbf_cin04.txt", 400);
	ulcin[69] = input_plio::create("ulbfic_b05", plio_64_bits, "../data/ulbf_cin05.txt", 400);
	ulcin[70] = input_plio::create("ulbfic_b06", plio_64_bits, "../data/ulbf_cin06.txt", 400);
	ulcin[71] = input_plio::create("ulbfic_b07", plio_64_bits, "../data/ulbf_cin07.txt", 400);

	//ULBF_2, Chain 1: 8 Data Input Coefficient Ports 
	ulcin[72] = input_plio::create("ulbfic_b10", plio_64_bits, "../data/ulbf_cin10.txt", 400);
	ulcin[73] = input_plio::create("ulbfic_b11", plio_64_bits, "../data/ulbf_cin11.txt", 400);
	ulcin[74] = input_plio::create("ulbfic_b12", plio_64_bits, "../data/ulbf_cin12.txt", 400);
	ulcin[75] = input_plio::create("ulbfic_b13", plio_64_bits, "../data/ulbf_cin13.txt", 400);
	ulcin[76] = input_plio::create("ulbfic_b14", plio_64_bits, "../data/ulbf_cin14.txt", 400);
	ulcin[77] = input_plio::create("ulbfic_b15", plio_64_bits, "../data/ulbf_cin15.txt", 400);
	ulcin[78] = input_plio::create("ulbfic_b16", plio_64_bits, "../data/ulbf_cin16.txt", 400);
	ulcin[79] = input_plio::create("ulbfic_b17", plio_64_bits, "../data/ulbf_cin17.txt", 400);

	//ULBF_2, Chain 2: 8 Data Input Coefficient Ports 
	ulcin[80] = input_plio::create("ulbfic_b20", plio_64_bits, "../data/ulbf_cin20.txt", 400);
	ulcin[81] = input_plio::create("ulbfic_b21", plio_64_bits, "../data/ulbf_cin21.txt", 400);
	ulcin[82] = input_plio::create("ulbfic_b22", plio_64_bits, "../data/ulbf_cin22.txt", 400);
	ulcin[83] = input_plio::create("ulbfic_b23", plio_64_bits, "../data/ulbf_cin23.txt", 400);
	ulcin[84] = input_plio::create("ulbfic_b24", plio_64_bits, "../data/ulbf_cin24.txt", 400);
	ulcin[85] = input_plio::create("ulbfic_b25", plio_64_bits, "../data/ulbf_cin25.txt", 400);
	ulcin[86] = input_plio::create("ulbfic_b26", plio_64_bits, "../data/ulbf_cin26.txt", 400);
	ulcin[87] = input_plio::create("ulbfic_b27", plio_64_bits, "../data/ulbf_cin27.txt", 400);

	//ULBF_2, Chain 3: 8 Data Input Coefficient Ports 
	ulcin[88] = input_plio::create("ulbfic_b30", plio_64_bits, "../data/ulbf_cin30.txt", 400);
	ulcin[89] = input_plio::create("ulbfic_b31", plio_64_bits, "../data/ulbf_cin31.txt", 400);
	ulcin[90] = input_plio::create("ulbfic_b32", plio_64_bits, "../data/ulbf_cin32.txt", 400);
	ulcin[91] = input_plio::create("ulbfic_b33", plio_64_bits, "../data/ulbf_cin33.txt", 400);
	ulcin[92] = input_plio::create("ulbfic_b34", plio_64_bits, "../data/ulbf_cin34.txt", 400);
	ulcin[93] = input_plio::create("ulbfic_b35", plio_64_bits, "../data/ulbf_cin35.txt", 400);
	ulcin[94] = input_plio::create("ulbfic_b36", plio_64_bits, "../data/ulbf_cin36.txt", 400);
	ulcin[95] = input_plio::create("ulbfic_b37", plio_64_bits, "../data/ulbf_cin37.txt", 400);

	//ULBF_2, 4 Data Output Ports, for Chains 0-3
	ulout[8]  = output_plio::create("ulbfo_b0",  plio_64_bits, "data/ulbf_out8.txt", 400);
	ulout[9]  = output_plio::create("ulbfo_b1",  plio_64_bits, "data/ulbf_out9.txt", 400);
	ulout[10]  = output_plio::create("ulbfo_b2",  plio_64_bits, "data/ulbf_out10.txt", 400);
	ulout[11]  = output_plio::create("ulbfo_b3",  plio_64_bits, "data/ulbf_out11.txt", 400);



	
   		
		// DLBF
		for(unsigned k=0;k<4; k++) connect(dldin[k].out[0], dlbf_0.din[k]);
		for(unsigned k=0;k<32;k++) connect(dlcin[k].out[0], dlbf_0.cin[k]);
		for(unsigned k=0;k<8; k++) connect(dlbf_0.out[k], dlout[k].in[0]);

		for(unsigned k=0;k<4; k++) connect(dldin[4+k].out[0], dlbf_1.din[k]);
		for(unsigned k=0;k<32;k++) connect(dlcin[32+k].out[0], dlbf_1.cin[k]);
		for(unsigned k=0;k<8; k++) connect(dlbf_1.out[k], dlout[8+k].in[0]);

		for(unsigned k=0;k<4; k++) connect(dldin[8+k].out[0], dlbf_2.din[k]);
		for(unsigned k=0;k<32;k++) connect(dlcin[64+k].out[0], dlbf_2.cin[k]);
		for(unsigned k=0;k<8; k++) connect(dlbf_2.out[k], dlout[16+k].in[0]);
		
		// ULBF
		for(unsigned k=0;k<8; k++) connect(uldin[k].out[0], ulbf_0.din[k]);
		for(unsigned k=0;k<32;k++) connect(ulcin[k].out[0], ulbf_0.cin[k]);
		for(unsigned k=0;k<4; k++) connect(ulbf_0.out[k], ulout[k].in[0]);

		for(unsigned k=0;k<8; k++) connect(uldin[8+k].out[0], ulbf_1.din[k]);
		for(unsigned k=0;k<32;k++) connect(ulcin[32+k].out[0], ulbf_1.cin[k]);
		for(unsigned k=0;k<4; k++) connect(ulbf_1.out[k], ulout[4+k].in[0]);

		for(unsigned k=0;k<8; k++) connect(uldin[16+k].out[0], ulbf_2.din[k]);
		for(unsigned k=0;k<32;k++) connect(ulcin[64+k].out[0], ulbf_2.cin[k]);
		for(unsigned k=0;k<4; k++) connect(ulbf_2.out[k], ulout[8+k].in[0]);
    };
	
	
};
