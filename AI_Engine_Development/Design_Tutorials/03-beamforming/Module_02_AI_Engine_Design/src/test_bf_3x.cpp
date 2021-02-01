/* (c) Copyright 2020 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
 
#include "test_bf.h"

#define COL_OFFSET 0

#define UL_INPUT_BA  (4+32)
#define UL_OUTPUT_BA (8)

//DLBF_0: 4 Data Input Ports 
PLIO *attr_i_dlbfid0 = new PLIO("dlbfid0", plio_64_bits, "../data/dlbf_din0.txt");
PLIO *attr_i_dlbfid1 = new PLIO("dlbfid1", plio_64_bits, "../data/dlbf_din1.txt");
PLIO *attr_i_dlbfid2 = new PLIO("dlbfid2", plio_64_bits, "../data/dlbf_din2.txt");
PLIO *attr_i_dlbfid3 = new PLIO("dlbfid3", plio_64_bits, "../data/dlbf_din3.txt");

//DLBF_0, Chain 0: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic00 = new PLIO("dlbfic00", plio_64_bits, "../data/dlbf_cin00.txt");
PLIO *attr_i_dlbfic01 = new PLIO("dlbfic01", plio_64_bits, "../data/dlbf_cin01.txt");
PLIO *attr_i_dlbfic02 = new PLIO("dlbfic02", plio_64_bits, "../data/dlbf_cin02.txt");
PLIO *attr_i_dlbfic03 = new PLIO("dlbfic03", plio_64_bits, "../data/dlbf_cin03.txt");

//DLBF_0, Chain 1: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic10 = new PLIO("dlbfic10", plio_64_bits, "../data/dlbf_cin10.txt");
PLIO *attr_i_dlbfic11 = new PLIO("dlbfic11", plio_64_bits, "../data/dlbf_cin11.txt");
PLIO *attr_i_dlbfic12 = new PLIO("dlbfic12", plio_64_bits, "../data/dlbf_cin12.txt");
PLIO *attr_i_dlbfic13 = new PLIO("dlbfic13", plio_64_bits, "../data/dlbf_cin13.txt");

//DLBF_0, Chain 2: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic20 = new PLIO("dlbfic20", plio_64_bits, "../data/dlbf_cin20.txt");
PLIO *attr_i_dlbfic21 = new PLIO("dlbfic21", plio_64_bits, "../data/dlbf_cin21.txt");
PLIO *attr_i_dlbfic22 = new PLIO("dlbfic22", plio_64_bits, "../data/dlbf_cin22.txt");
PLIO *attr_i_dlbfic23 = new PLIO("dlbfic23", plio_64_bits, "../data/dlbf_cin23.txt");

//DLBF_0, Chain 3: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic30 = new PLIO("dlbfic30", plio_64_bits, "../data/dlbf_cin30.txt");
PLIO *attr_i_dlbfic31 = new PLIO("dlbfic31", plio_64_bits, "../data/dlbf_cin31.txt");
PLIO *attr_i_dlbfic32 = new PLIO("dlbfic32", plio_64_bits, "../data/dlbf_cin32.txt");
PLIO *attr_i_dlbfic33 = new PLIO("dlbfic33", plio_64_bits, "../data/dlbf_cin33.txt");

//DLBF_0, Chain 4: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic40 = new PLIO("dlbfic40", plio_64_bits, "../data/dlbf_cin40.txt");
PLIO *attr_i_dlbfic41 = new PLIO("dlbfic41", plio_64_bits, "../data/dlbf_cin41.txt");
PLIO *attr_i_dlbfic42 = new PLIO("dlbfic42", plio_64_bits, "../data/dlbf_cin42.txt");
PLIO *attr_i_dlbfic43 = new PLIO("dlbfic43", plio_64_bits, "../data/dlbf_cin43.txt");

//DLBF_0, Chain 5: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic50 = new PLIO("dlbfic50", plio_64_bits, "../data/dlbf_cin50.txt");
PLIO *attr_i_dlbfic51 = new PLIO("dlbfic51", plio_64_bits, "../data/dlbf_cin51.txt");
PLIO *attr_i_dlbfic52 = new PLIO("dlbfic52", plio_64_bits, "../data/dlbf_cin52.txt");
PLIO *attr_i_dlbfic53 = new PLIO("dlbfic53", plio_64_bits, "../data/dlbf_cin53.txt");

//DLBF_0, Chain 6: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic60 = new PLIO("dlbfic60", plio_64_bits, "../data/dlbf_cin60.txt");
PLIO *attr_i_dlbfic61 = new PLIO("dlbfic61", plio_64_bits, "../data/dlbf_cin61.txt");
PLIO *attr_i_dlbfic62 = new PLIO("dlbfic62", plio_64_bits, "../data/dlbf_cin62.txt");
PLIO *attr_i_dlbfic63 = new PLIO("dlbfic63", plio_64_bits, "../data/dlbf_cin63.txt");

//DLBF_0, Chain 7: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic70 = new PLIO("dlbfic70", plio_64_bits, "../data/dlbf_cin70.txt");
PLIO *attr_i_dlbfic71 = new PLIO("dlbfic71", plio_64_bits, "../data/dlbf_cin71.txt");
PLIO *attr_i_dlbfic72 = new PLIO("dlbfic72", plio_64_bits, "../data/dlbf_cin72.txt");
PLIO *attr_i_dlbfic73 = new PLIO("dlbfic73", plio_64_bits, "../data/dlbf_cin73.txt");

//DLBF_0, 8 Data Output Ports, for Chains 0-7 
PLIO *attr_o_dlbfo0  = new PLIO("dlbfo0",  plio_64_bits, "data/dlbf_out0.txt");
PLIO *attr_o_dlbfo1  = new PLIO("dlbfo1",  plio_64_bits, "data/dlbf_out1.txt");
PLIO *attr_o_dlbfo2  = new PLIO("dlbfo2",  plio_64_bits, "data/dlbf_out2.txt");
PLIO *attr_o_dlbfo3  = new PLIO("dlbfo3",  plio_64_bits, "data/dlbf_out3.txt");
PLIO *attr_o_dlbfo4  = new PLIO("dlbfo4",  plio_64_bits, "data/dlbf_out4.txt");
PLIO *attr_o_dlbfo5  = new PLIO("dlbfo5",  plio_64_bits, "data/dlbf_out5.txt");
PLIO *attr_o_dlbfo6  = new PLIO("dlbfo6",  plio_64_bits, "data/dlbf_out6.txt");
PLIO *attr_o_dlbfo7  = new PLIO("dlbfo7",  plio_64_bits, "data/dlbf_out7.txt");

//DLBF_1: 4 Data Input Ports 
PLIO *attr_i_dlbfid_a0 = new PLIO("dlbfid_a0", plio_64_bits, "../data/dlbf_din0.txt");
PLIO *attr_i_dlbfid_a1 = new PLIO("dlbfid_a1", plio_64_bits, "../data/dlbf_din1.txt");
PLIO *attr_i_dlbfid_a2 = new PLIO("dlbfid_a2", plio_64_bits, "../data/dlbf_din2.txt");
PLIO *attr_i_dlbfid_a3 = new PLIO("dlbfid_a3", plio_64_bits, "../data/dlbf_din3.txt");

//DLBF_1, Chain 0: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a00 = new PLIO("dlbfic_a00", plio_64_bits, "../data/dlbf_cin00.txt");
PLIO *attr_i_dlbfic_a01 = new PLIO("dlbfic_a01", plio_64_bits, "../data/dlbf_cin01.txt");
PLIO *attr_i_dlbfic_a02 = new PLIO("dlbfic_a02", plio_64_bits, "../data/dlbf_cin02.txt");
PLIO *attr_i_dlbfic_a03 = new PLIO("dlbfic_a03", plio_64_bits, "../data/dlbf_cin03.txt");

//DLBF_1, Chain 1: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a10 = new PLIO("dlbfic_a10", plio_64_bits, "../data/dlbf_cin10.txt");
PLIO *attr_i_dlbfic_a11 = new PLIO("dlbfic_a11", plio_64_bits, "../data/dlbf_cin11.txt");
PLIO *attr_i_dlbfic_a12 = new PLIO("dlbfic_a12", plio_64_bits, "../data/dlbf_cin12.txt");
PLIO *attr_i_dlbfic_a13 = new PLIO("dlbfic_a13", plio_64_bits, "../data/dlbf_cin13.txt");

//DLBF_1, Chain 2: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a20 = new PLIO("dlbfic_a20", plio_64_bits, "../data/dlbf_cin20.txt");
PLIO *attr_i_dlbfic_a21 = new PLIO("dlbfic_a21", plio_64_bits, "../data/dlbf_cin21.txt");
PLIO *attr_i_dlbfic_a22 = new PLIO("dlbfic_a22", plio_64_bits, "../data/dlbf_cin22.txt");
PLIO *attr_i_dlbfic_a23 = new PLIO("dlbfic_a23", plio_64_bits, "../data/dlbf_cin23.txt");

//DLBF_1, Chain 3: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a30 = new PLIO("dlbfic_a30", plio_64_bits, "../data/dlbf_cin30.txt");
PLIO *attr_i_dlbfic_a31 = new PLIO("dlbfic_a31", plio_64_bits, "../data/dlbf_cin31.txt");
PLIO *attr_i_dlbfic_a32 = new PLIO("dlbfic_a32", plio_64_bits, "../data/dlbf_cin32.txt");
PLIO *attr_i_dlbfic_a33 = new PLIO("dlbfic_a33", plio_64_bits, "../data/dlbf_cin33.txt");

//DLBF_1, Chain 4: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a40 = new PLIO("dlbfic_a40", plio_64_bits, "../data/dlbf_cin40.txt");
PLIO *attr_i_dlbfic_a41 = new PLIO("dlbfic_a41", plio_64_bits, "../data/dlbf_cin41.txt");
PLIO *attr_i_dlbfic_a42 = new PLIO("dlbfic_a42", plio_64_bits, "../data/dlbf_cin42.txt");
PLIO *attr_i_dlbfic_a43 = new PLIO("dlbfic_a43", plio_64_bits, "../data/dlbf_cin43.txt");

//DLBF_1, Chain 5: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a50 = new PLIO("dlbfic_a50", plio_64_bits, "../data/dlbf_cin50.txt");
PLIO *attr_i_dlbfic_a51 = new PLIO("dlbfic_a51", plio_64_bits, "../data/dlbf_cin51.txt");
PLIO *attr_i_dlbfic_a52 = new PLIO("dlbfic_a52", plio_64_bits, "../data/dlbf_cin52.txt");
PLIO *attr_i_dlbfic_a53 = new PLIO("dlbfic_a53", plio_64_bits, "../data/dlbf_cin53.txt");

//DLBF_1, Chain 6: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a60 = new PLIO("dlbfic_a60", plio_64_bits, "../data/dlbf_cin60.txt");
PLIO *attr_i_dlbfic_a61 = new PLIO("dlbfic_a61", plio_64_bits, "../data/dlbf_cin61.txt");
PLIO *attr_i_dlbfic_a62 = new PLIO("dlbfic_a62", plio_64_bits, "../data/dlbf_cin62.txt");
PLIO *attr_i_dlbfic_a63 = new PLIO("dlbfic_a63", plio_64_bits, "../data/dlbf_cin63.txt");

//DLBF_1, Chain 7: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_a70 = new PLIO("dlbfic_a70", plio_64_bits, "../data/dlbf_cin70.txt");
PLIO *attr_i_dlbfic_a71 = new PLIO("dlbfic_a71", plio_64_bits, "../data/dlbf_cin71.txt");
PLIO *attr_i_dlbfic_a72 = new PLIO("dlbfic_a72", plio_64_bits, "../data/dlbf_cin72.txt");
PLIO *attr_i_dlbfic_a73 = new PLIO("dlbfic_a73", plio_64_bits, "../data/dlbf_cin73.txt");

//DLBF_1, 8 Data Output Ports, for Chains 0-7 
PLIO *attr_o_dlbfo_a0  = new PLIO("dlbfo_a0",  plio_64_bits, "data/dlbf_out8.txt");
PLIO *attr_o_dlbfo_a1  = new PLIO("dlbfo_a1",  plio_64_bits, "data/dlbf_out9.txt");
PLIO *attr_o_dlbfo_a2  = new PLIO("dlbfo_a2",  plio_64_bits, "data/dlbf_out10.txt");
PLIO *attr_o_dlbfo_a3  = new PLIO("dlbfo_a3",  plio_64_bits, "data/dlbf_out11.txt");
PLIO *attr_o_dlbfo_a4  = new PLIO("dlbfo_a4",  plio_64_bits, "data/dlbf_out12.txt");
PLIO *attr_o_dlbfo_a5  = new PLIO("dlbfo_a5",  plio_64_bits, "data/dlbf_out13.txt");
PLIO *attr_o_dlbfo_a6  = new PLIO("dlbfo_a6",  plio_64_bits, "data/dlbf_out14.txt");
PLIO *attr_o_dlbfo_a7  = new PLIO("dlbfo_a7",  plio_64_bits, "data/dlbf_out15.txt");

//DLBF_2: 4 Data Input Ports 
PLIO *attr_i_dlbfid_b0 = new PLIO("dlbfid_b0", plio_64_bits, "../data/dlbf_din0.txt");
PLIO *attr_i_dlbfid_b1 = new PLIO("dlbfid_b1", plio_64_bits, "../data/dlbf_din1.txt");
PLIO *attr_i_dlbfid_b2 = new PLIO("dlbfid_b2", plio_64_bits, "../data/dlbf_din2.txt");
PLIO *attr_i_dlbfid_b3 = new PLIO("dlbfid_b3", plio_64_bits, "../data/dlbf_din3.txt");

//DLBF_2, Chain 0: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b00 = new PLIO("dlbfic_b00", plio_64_bits, "../data/dlbf_cin00.txt");
PLIO *attr_i_dlbfic_b01 = new PLIO("dlbfic_b01", plio_64_bits, "../data/dlbf_cin01.txt");
PLIO *attr_i_dlbfic_b02 = new PLIO("dlbfic_b02", plio_64_bits, "../data/dlbf_cin02.txt");
PLIO *attr_i_dlbfic_b03 = new PLIO("dlbfic_b03", plio_64_bits, "../data/dlbf_cin03.txt");

//DLBF_2, Chain 1: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b10 = new PLIO("dlbfic_b10", plio_64_bits, "../data/dlbf_cin10.txt");
PLIO *attr_i_dlbfic_b11 = new PLIO("dlbfic_b11", plio_64_bits, "../data/dlbf_cin11.txt");
PLIO *attr_i_dlbfic_b12 = new PLIO("dlbfic_b12", plio_64_bits, "../data/dlbf_cin12.txt");
PLIO *attr_i_dlbfic_b13 = new PLIO("dlbfic_b13", plio_64_bits, "../data/dlbf_cin13.txt");

//DLBF_2, Chain 2: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b20 = new PLIO("dlbfic_b20", plio_64_bits, "../data/dlbf_cin20.txt");
PLIO *attr_i_dlbfic_b21 = new PLIO("dlbfic_b21", plio_64_bits, "../data/dlbf_cin21.txt");
PLIO *attr_i_dlbfic_b22 = new PLIO("dlbfic_b22", plio_64_bits, "../data/dlbf_cin22.txt");
PLIO *attr_i_dlbfic_b23 = new PLIO("dlbfic_b23", plio_64_bits, "../data/dlbf_cin23.txt");

//DLBF_2, Chain 3: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b30 = new PLIO("dlbfic_b30", plio_64_bits, "../data/dlbf_cin30.txt");
PLIO *attr_i_dlbfic_b31 = new PLIO("dlbfic_b31", plio_64_bits, "../data/dlbf_cin31.txt");
PLIO *attr_i_dlbfic_b32 = new PLIO("dlbfic_b32", plio_64_bits, "../data/dlbf_cin32.txt");
PLIO *attr_i_dlbfic_b33 = new PLIO("dlbfic_b33", plio_64_bits, "../data/dlbf_cin33.txt");

//DLBF_2, Chain 4: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b40 = new PLIO("dlbfic_b40", plio_64_bits, "../data/dlbf_cin40.txt");
PLIO *attr_i_dlbfic_b41 = new PLIO("dlbfic_b41", plio_64_bits, "../data/dlbf_cin41.txt");
PLIO *attr_i_dlbfic_b42 = new PLIO("dlbfic_b42", plio_64_bits, "../data/dlbf_cin42.txt");
PLIO *attr_i_dlbfic_b43 = new PLIO("dlbfic_b43", plio_64_bits, "../data/dlbf_cin43.txt");

//DLBF_2, Chain 5: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b50 = new PLIO("dlbfic_b50", plio_64_bits, "../data/dlbf_cin50.txt");
PLIO *attr_i_dlbfic_b51 = new PLIO("dlbfic_b51", plio_64_bits, "../data/dlbf_cin51.txt");
PLIO *attr_i_dlbfic_b52 = new PLIO("dlbfic_b52", plio_64_bits, "../data/dlbf_cin52.txt");
PLIO *attr_i_dlbfic_b53 = new PLIO("dlbfic_b53", plio_64_bits, "../data/dlbf_cin53.txt");

//DLBF_2, Chain 6: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b60 = new PLIO("dlbfic_b60", plio_64_bits, "../data/dlbf_cin60.txt");
PLIO *attr_i_dlbfic_b61 = new PLIO("dlbfic_b61", plio_64_bits, "../data/dlbf_cin61.txt");
PLIO *attr_i_dlbfic_b62 = new PLIO("dlbfic_b62", plio_64_bits, "../data/dlbf_cin62.txt");
PLIO *attr_i_dlbfic_b63 = new PLIO("dlbfic_b63", plio_64_bits, "../data/dlbf_cin63.txt");

//DLBF_2, Chain 7: 4 Data Input Coefficient Ports 
PLIO *attr_i_dlbfic_b70 = new PLIO("dlbfic_b70", plio_64_bits, "../data/dlbf_cin70.txt");
PLIO *attr_i_dlbfic_b71 = new PLIO("dlbfic_b71", plio_64_bits, "../data/dlbf_cin71.txt");
PLIO *attr_i_dlbfic_b72 = new PLIO("dlbfic_b72", plio_64_bits, "../data/dlbf_cin72.txt");
PLIO *attr_i_dlbfic_b73 = new PLIO("dlbfic_b73", plio_64_bits, "../data/dlbf_cin73.txt");

//DLBF_2, 8 Data Output Ports, for Chains 0-7 
PLIO *attr_o_dlbfo_b0  = new PLIO("dlbfo_b0",  plio_64_bits, "data/dlbf_out16.txt");
PLIO *attr_o_dlbfo_b1  = new PLIO("dlbfo_b1",  plio_64_bits, "data/dlbf_out17.txt");
PLIO *attr_o_dlbfo_b2  = new PLIO("dlbfo_b2",  plio_64_bits, "data/dlbf_out18.txt");
PLIO *attr_o_dlbfo_b3  = new PLIO("dlbfo_b3",  plio_64_bits, "data/dlbf_out19.txt");
PLIO *attr_o_dlbfo_b4  = new PLIO("dlbfo_b4",  plio_64_bits, "data/dlbf_out20.txt");
PLIO *attr_o_dlbfo_b5  = new PLIO("dlbfo_b5",  plio_64_bits, "data/dlbf_out21.txt");
PLIO *attr_o_dlbfo_b6  = new PLIO("dlbfo_b6",  plio_64_bits, "data/dlbf_out22.txt");
PLIO *attr_o_dlbfo_b7  = new PLIO("dlbfo_b7",  plio_64_bits, "data/dlbf_out23.txt");

//ULBF_0: 8 Data Input Ports 
PLIO *attr_i_ulbfid0 = new PLIO("ulbfid0", plio_64_bits, "../data/ulbf_din0.txt");
PLIO *attr_i_ulbfid1 = new PLIO("ulbfid1", plio_64_bits, "../data/ulbf_din1.txt");
PLIO *attr_i_ulbfid2 = new PLIO("ulbfid2", plio_64_bits, "../data/ulbf_din2.txt");
PLIO *attr_i_ulbfid3 = new PLIO("ulbfid3", plio_64_bits, "../data/ulbf_din3.txt");
PLIO *attr_i_ulbfid4 = new PLIO("ulbfid4", plio_64_bits, "../data/ulbf_din4.txt");
PLIO *attr_i_ulbfid5 = new PLIO("ulbfid5", plio_64_bits, "../data/ulbf_din5.txt");
PLIO *attr_i_ulbfid6 = new PLIO("ulbfid6", plio_64_bits, "../data/ulbf_din6.txt");
PLIO *attr_i_ulbfid7 = new PLIO("ulbfid7", plio_64_bits, "../data/ulbf_din7.txt");

//ULBF_0, Chain 0: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic00 = new PLIO("ulbfic00", plio_64_bits, "../data/ulbf_cin00.txt");
PLIO *attr_i_ulbfic01 = new PLIO("ulbfic01", plio_64_bits, "../data/ulbf_cin01.txt");
PLIO *attr_i_ulbfic02 = new PLIO("ulbfic02", plio_64_bits, "../data/ulbf_cin02.txt");
PLIO *attr_i_ulbfic03 = new PLIO("ulbfic03", plio_64_bits, "../data/ulbf_cin03.txt");
PLIO *attr_i_ulbfic04 = new PLIO("ulbfic04", plio_64_bits, "../data/ulbf_cin04.txt");
PLIO *attr_i_ulbfic05 = new PLIO("ulbfic05", plio_64_bits, "../data/ulbf_cin05.txt");
PLIO *attr_i_ulbfic06 = new PLIO("ulbfic06", plio_64_bits, "../data/ulbf_cin06.txt");
PLIO *attr_i_ulbfic07 = new PLIO("ulbfic07", plio_64_bits, "../data/ulbf_cin07.txt");

//ULBF_0, Chain 1: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic10 = new PLIO("ulbfic10", plio_64_bits, "../data/ulbf_cin10.txt");
PLIO *attr_i_ulbfic11 = new PLIO("ulbfic11", plio_64_bits, "../data/ulbf_cin11.txt");
PLIO *attr_i_ulbfic12 = new PLIO("ulbfic12", plio_64_bits, "../data/ulbf_cin12.txt");
PLIO *attr_i_ulbfic13 = new PLIO("ulbfic13", plio_64_bits, "../data/ulbf_cin13.txt");
PLIO *attr_i_ulbfic14 = new PLIO("ulbfic14", plio_64_bits, "../data/ulbf_cin14.txt");
PLIO *attr_i_ulbfic15 = new PLIO("ulbfic15", plio_64_bits, "../data/ulbf_cin15.txt");
PLIO *attr_i_ulbfic16 = new PLIO("ulbfic16", plio_64_bits, "../data/ulbf_cin16.txt");
PLIO *attr_i_ulbfic17 = new PLIO("ulbfic17", plio_64_bits, "../data/ulbf_cin17.txt");

//ULBF_0, Chain 2: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic20 = new PLIO("ulbfic20", plio_64_bits, "../data/ulbf_cin20.txt");
PLIO *attr_i_ulbfic21 = new PLIO("ulbfic21", plio_64_bits, "../data/ulbf_cin21.txt");
PLIO *attr_i_ulbfic22 = new PLIO("ulbfic22", plio_64_bits, "../data/ulbf_cin22.txt");
PLIO *attr_i_ulbfic23 = new PLIO("ulbfic23", plio_64_bits, "../data/ulbf_cin23.txt");
PLIO *attr_i_ulbfic24 = new PLIO("ulbfic24", plio_64_bits, "../data/ulbf_cin24.txt");
PLIO *attr_i_ulbfic25 = new PLIO("ulbfic25", plio_64_bits, "../data/ulbf_cin25.txt");
PLIO *attr_i_ulbfic26 = new PLIO("ulbfic26", plio_64_bits, "../data/ulbf_cin26.txt");
PLIO *attr_i_ulbfic27 = new PLIO("ulbfic27", plio_64_bits, "../data/ulbf_cin27.txt");

//ULBF_0, Chain 3: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic30 = new PLIO("ulbfic30", plio_64_bits, "../data/ulbf_cin30.txt");
PLIO *attr_i_ulbfic31 = new PLIO("ulbfic31", plio_64_bits, "../data/ulbf_cin31.txt");
PLIO *attr_i_ulbfic32 = new PLIO("ulbfic32", plio_64_bits, "../data/ulbf_cin32.txt");
PLIO *attr_i_ulbfic33 = new PLIO("ulbfic33", plio_64_bits, "../data/ulbf_cin33.txt");
PLIO *attr_i_ulbfic34 = new PLIO("ulbfic34", plio_64_bits, "../data/ulbf_cin34.txt");
PLIO *attr_i_ulbfic35 = new PLIO("ulbfic35", plio_64_bits, "../data/ulbf_cin35.txt");
PLIO *attr_i_ulbfic36 = new PLIO("ulbfic36", plio_64_bits, "../data/ulbf_cin36.txt");
PLIO *attr_i_ulbfic37 = new PLIO("ulbfic37", plio_64_bits, "../data/ulbf_cin37.txt");

//ULBF_0, 4 Data Output Ports, for Chains 0-3
PLIO *attr_o_ulbfo0  = new PLIO("ulbfo0",  plio_64_bits, "data/ulbf_out0.txt");
PLIO *attr_o_ulbfo1  = new PLIO("ulbfo1",  plio_64_bits, "data/ulbf_out1.txt");
PLIO *attr_o_ulbfo2  = new PLIO("ulbfo2",  plio_64_bits, "data/ulbf_out2.txt");
PLIO *attr_o_ulbfo3  = new PLIO("ulbfo3",  plio_64_bits, "data/ulbf_out3.txt");

//ULBF_1: 8 Data Input Ports 
PLIO *attr_i_ulbfid_a0 = new PLIO("ulbfid_a0", plio_64_bits, "../data/ulbf_din0.txt");
PLIO *attr_i_ulbfid_a1 = new PLIO("ulbfid_a1", plio_64_bits, "../data/ulbf_din1.txt");
PLIO *attr_i_ulbfid_a2 = new PLIO("ulbfid_a2", plio_64_bits, "../data/ulbf_din2.txt");
PLIO *attr_i_ulbfid_a3 = new PLIO("ulbfid_a3", plio_64_bits, "../data/ulbf_din3.txt");
PLIO *attr_i_ulbfid_a4 = new PLIO("ulbfid_a4", plio_64_bits, "../data/ulbf_din4.txt");
PLIO *attr_i_ulbfid_a5 = new PLIO("ulbfid_a5", plio_64_bits, "../data/ulbf_din5.txt");
PLIO *attr_i_ulbfid_a6 = new PLIO("ulbfid_a6", plio_64_bits, "../data/ulbf_din6.txt");
PLIO *attr_i_ulbfid_a7 = new PLIO("ulbfid_a7", plio_64_bits, "../data/ulbf_din7.txt");

//ULBF_1, Chain 0: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_a00 = new PLIO("ulbfic_a00", plio_64_bits, "../data/ulbf_cin00.txt");
PLIO *attr_i_ulbfic_a01 = new PLIO("ulbfic_a01", plio_64_bits, "../data/ulbf_cin01.txt");
PLIO *attr_i_ulbfic_a02 = new PLIO("ulbfic_a02", plio_64_bits, "../data/ulbf_cin02.txt");
PLIO *attr_i_ulbfic_a03 = new PLIO("ulbfic_a03", plio_64_bits, "../data/ulbf_cin03.txt");
PLIO *attr_i_ulbfic_a04 = new PLIO("ulbfic_a04", plio_64_bits, "../data/ulbf_cin04.txt");
PLIO *attr_i_ulbfic_a05 = new PLIO("ulbfic_a05", plio_64_bits, "../data/ulbf_cin05.txt");
PLIO *attr_i_ulbfic_a06 = new PLIO("ulbfic_a06", plio_64_bits, "../data/ulbf_cin06.txt");
PLIO *attr_i_ulbfic_a07 = new PLIO("ulbfic_a07", plio_64_bits, "../data/ulbf_cin07.txt");

//ULBF_1, Chain 1: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_a10 = new PLIO("ulbfic_a10", plio_64_bits, "../data/ulbf_cin10.txt");
PLIO *attr_i_ulbfic_a11 = new PLIO("ulbfic_a11", plio_64_bits, "../data/ulbf_cin11.txt");
PLIO *attr_i_ulbfic_a12 = new PLIO("ulbfic_a12", plio_64_bits, "../data/ulbf_cin12.txt");
PLIO *attr_i_ulbfic_a13 = new PLIO("ulbfic_a13", plio_64_bits, "../data/ulbf_cin13.txt");
PLIO *attr_i_ulbfic_a14 = new PLIO("ulbfic_a14", plio_64_bits, "../data/ulbf_cin14.txt");
PLIO *attr_i_ulbfic_a15 = new PLIO("ulbfic_a15", plio_64_bits, "../data/ulbf_cin15.txt");
PLIO *attr_i_ulbfic_a16 = new PLIO("ulbfic_a16", plio_64_bits, "../data/ulbf_cin16.txt");
PLIO *attr_i_ulbfic_a17 = new PLIO("ulbfic_a17", plio_64_bits, "../data/ulbf_cin17.txt");

//ULBF_1, Chain 2: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_a20 = new PLIO("ulbfic_a20", plio_64_bits, "../data/ulbf_cin20.txt");
PLIO *attr_i_ulbfic_a21 = new PLIO("ulbfic_a21", plio_64_bits, "../data/ulbf_cin21.txt");
PLIO *attr_i_ulbfic_a22 = new PLIO("ulbfic_a22", plio_64_bits, "../data/ulbf_cin22.txt");
PLIO *attr_i_ulbfic_a23 = new PLIO("ulbfic_a23", plio_64_bits, "../data/ulbf_cin23.txt");
PLIO *attr_i_ulbfic_a24 = new PLIO("ulbfic_a24", plio_64_bits, "../data/ulbf_cin24.txt");
PLIO *attr_i_ulbfic_a25 = new PLIO("ulbfic_a25", plio_64_bits, "../data/ulbf_cin25.txt");
PLIO *attr_i_ulbfic_a26 = new PLIO("ulbfic_a26", plio_64_bits, "../data/ulbf_cin26.txt");
PLIO *attr_i_ulbfic_a27 = new PLIO("ulbfic_a27", plio_64_bits, "../data/ulbf_cin27.txt");

//ULBF_1, Chain 3: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_a30 = new PLIO("ulbfic_a30", plio_64_bits, "../data/ulbf_cin30.txt");
PLIO *attr_i_ulbfic_a31 = new PLIO("ulbfic_a31", plio_64_bits, "../data/ulbf_cin31.txt");
PLIO *attr_i_ulbfic_a32 = new PLIO("ulbfic_a32", plio_64_bits, "../data/ulbf_cin32.txt");
PLIO *attr_i_ulbfic_a33 = new PLIO("ulbfic_a33", plio_64_bits, "../data/ulbf_cin33.txt");
PLIO *attr_i_ulbfic_a34 = new PLIO("ulbfic_a34", plio_64_bits, "../data/ulbf_cin34.txt");
PLIO *attr_i_ulbfic_a35 = new PLIO("ulbfic_a35", plio_64_bits, "../data/ulbf_cin35.txt");
PLIO *attr_i_ulbfic_a36 = new PLIO("ulbfic_a36", plio_64_bits, "../data/ulbf_cin36.txt");
PLIO *attr_i_ulbfic_a37 = new PLIO("ulbfic_a37", plio_64_bits, "../data/ulbf_cin37.txt");

//ULBF_1, 4 Data Output Ports, for Chains 0-3
PLIO *attr_o_ulbfo_a0  = new PLIO("ulbfo_a0",  plio_64_bits, "data/ulbf_out4.txt");
PLIO *attr_o_ulbfo_a1  = new PLIO("ulbfo_a1",  plio_64_bits, "data/ulbf_out5.txt");
PLIO *attr_o_ulbfo_a2  = new PLIO("ulbfo_a2",  plio_64_bits, "data/ulbf_out6.txt");
PLIO *attr_o_ulbfo_a3  = new PLIO("ulbfo_a3",  plio_64_bits, "data/ulbf_out7.txt");

//ULBF_2: 8 Data Input Ports 
PLIO *attr_i_ulbfid_b0 = new PLIO("ulbfid_b0", plio_64_bits, "../data/ulbf_din0.txt");
PLIO *attr_i_ulbfid_b1 = new PLIO("ulbfid_b1", plio_64_bits, "../data/ulbf_din1.txt");
PLIO *attr_i_ulbfid_b2 = new PLIO("ulbfid_b2", plio_64_bits, "../data/ulbf_din2.txt");
PLIO *attr_i_ulbfid_b3 = new PLIO("ulbfid_b3", plio_64_bits, "../data/ulbf_din3.txt");
PLIO *attr_i_ulbfid_b4 = new PLIO("ulbfid_b4", plio_64_bits, "../data/ulbf_din4.txt");
PLIO *attr_i_ulbfid_b5 = new PLIO("ulbfid_b5", plio_64_bits, "../data/ulbf_din5.txt");
PLIO *attr_i_ulbfid_b6 = new PLIO("ulbfid_b6", plio_64_bits, "../data/ulbf_din6.txt");
PLIO *attr_i_ulbfid_b7 = new PLIO("ulbfid_b7", plio_64_bits, "../data/ulbf_din7.txt");

//ULBF_2, Chain 0: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_b00 = new PLIO("ulbfic_b00", plio_64_bits, "../data/ulbf_cin00.txt");
PLIO *attr_i_ulbfic_b01 = new PLIO("ulbfic_b01", plio_64_bits, "../data/ulbf_cin01.txt");
PLIO *attr_i_ulbfic_b02 = new PLIO("ulbfic_b02", plio_64_bits, "../data/ulbf_cin02.txt");
PLIO *attr_i_ulbfic_b03 = new PLIO("ulbfic_b03", plio_64_bits, "../data/ulbf_cin03.txt");
PLIO *attr_i_ulbfic_b04 = new PLIO("ulbfic_b04", plio_64_bits, "../data/ulbf_cin04.txt");
PLIO *attr_i_ulbfic_b05 = new PLIO("ulbfic_b05", plio_64_bits, "../data/ulbf_cin05.txt");
PLIO *attr_i_ulbfic_b06 = new PLIO("ulbfic_b06", plio_64_bits, "../data/ulbf_cin06.txt");
PLIO *attr_i_ulbfic_b07 = new PLIO("ulbfic_b07", plio_64_bits, "../data/ulbf_cin07.txt");

//ULBF_2, Chain 1: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_b10 = new PLIO("ulbfic_b10", plio_64_bits, "../data/ulbf_cin10.txt");
PLIO *attr_i_ulbfic_b11 = new PLIO("ulbfic_b11", plio_64_bits, "../data/ulbf_cin11.txt");
PLIO *attr_i_ulbfic_b12 = new PLIO("ulbfic_b12", plio_64_bits, "../data/ulbf_cin12.txt");
PLIO *attr_i_ulbfic_b13 = new PLIO("ulbfic_b13", plio_64_bits, "../data/ulbf_cin13.txt");
PLIO *attr_i_ulbfic_b14 = new PLIO("ulbfic_b14", plio_64_bits, "../data/ulbf_cin14.txt");
PLIO *attr_i_ulbfic_b15 = new PLIO("ulbfic_b15", plio_64_bits, "../data/ulbf_cin15.txt");
PLIO *attr_i_ulbfic_b16 = new PLIO("ulbfic_b16", plio_64_bits, "../data/ulbf_cin16.txt");
PLIO *attr_i_ulbfic_b17 = new PLIO("ulbfic_b17", plio_64_bits, "../data/ulbf_cin17.txt");

//ULBF_2, Chain 2: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_b20 = new PLIO("ulbfic_b20", plio_64_bits, "../data/ulbf_cin20.txt");
PLIO *attr_i_ulbfic_b21 = new PLIO("ulbfic_b21", plio_64_bits, "../data/ulbf_cin21.txt");
PLIO *attr_i_ulbfic_b22 = new PLIO("ulbfic_b22", plio_64_bits, "../data/ulbf_cin22.txt");
PLIO *attr_i_ulbfic_b23 = new PLIO("ulbfic_b23", plio_64_bits, "../data/ulbf_cin23.txt");
PLIO *attr_i_ulbfic_b24 = new PLIO("ulbfic_b24", plio_64_bits, "../data/ulbf_cin24.txt");
PLIO *attr_i_ulbfic_b25 = new PLIO("ulbfic_b25", plio_64_bits, "../data/ulbf_cin25.txt");
PLIO *attr_i_ulbfic_b26 = new PLIO("ulbfic_b26", plio_64_bits, "../data/ulbf_cin26.txt");
PLIO *attr_i_ulbfic_b27 = new PLIO("ulbfic_b27", plio_64_bits, "../data/ulbf_cin27.txt");

//ULBF_2, Chain 3: 8 Data Input Coefficient Ports 
PLIO *attr_i_ulbfic_b30 = new PLIO("ulbfic_b30", plio_64_bits, "../data/ulbf_cin30.txt");
PLIO *attr_i_ulbfic_b31 = new PLIO("ulbfic_b31", plio_64_bits, "../data/ulbf_cin31.txt");
PLIO *attr_i_ulbfic_b32 = new PLIO("ulbfic_b32", plio_64_bits, "../data/ulbf_cin32.txt");
PLIO *attr_i_ulbfic_b33 = new PLIO("ulbfic_b33", plio_64_bits, "../data/ulbf_cin33.txt");
PLIO *attr_i_ulbfic_b34 = new PLIO("ulbfic_b34", plio_64_bits, "../data/ulbf_cin34.txt");
PLIO *attr_i_ulbfic_b35 = new PLIO("ulbfic_b35", plio_64_bits, "../data/ulbf_cin35.txt");
PLIO *attr_i_ulbfic_b36 = new PLIO("ulbfic_b36", plio_64_bits, "../data/ulbf_cin36.txt");
PLIO *attr_i_ulbfic_b37 = new PLIO("ulbfic_b37", plio_64_bits, "../data/ulbf_cin37.txt");

//ULBF_2, 4 Data Output Ports, for Chains 0-3
PLIO *attr_o_ulbfo_b0  = new PLIO("ulbfo_b0",  plio_64_bits, "data/ulbf_out8.txt");
PLIO *attr_o_ulbfo_b1  = new PLIO("ulbfo_b1",  plio_64_bits, "data/ulbf_out9.txt");
PLIO *attr_o_ulbfo_b2  = new PLIO("ulbfo_b2",  plio_64_bits, "data/ulbf_out10.txt");
PLIO *attr_o_ulbfo_b3  = new PLIO("ulbfo_b3",  plio_64_bits, "data/ulbf_out11.txt");


simulation::platform<(4+32+8+32)*3, (8+4)*3> platform(
	attr_i_dlbfid0,   attr_i_dlbfid1,  attr_i_dlbfid2,   attr_i_dlbfid3, 
	attr_i_dlbfic00,  attr_i_dlbfic01, attr_i_dlbfic02,  attr_i_dlbfic03,
	attr_i_dlbfic10,  attr_i_dlbfic11, attr_i_dlbfic12,  attr_i_dlbfic13,
	attr_i_dlbfic20,  attr_i_dlbfic21, attr_i_dlbfic22,  attr_i_dlbfic23,
	attr_i_dlbfic30,  attr_i_dlbfic31, attr_i_dlbfic32,  attr_i_dlbfic33,
	attr_i_dlbfic40,  attr_i_dlbfic41, attr_i_dlbfic42,  attr_i_dlbfic43,
	attr_i_dlbfic50,  attr_i_dlbfic51, attr_i_dlbfic52,  attr_i_dlbfic53,
	attr_i_dlbfic60,  attr_i_dlbfic61, attr_i_dlbfic62,  attr_i_dlbfic63,
	attr_i_dlbfic70,  attr_i_dlbfic71, attr_i_dlbfic72,  attr_i_dlbfic73,
    attr_i_ulbfid0,   attr_i_ulbfid1,  attr_i_ulbfid2,   attr_i_ulbfid3, 
	attr_i_ulbfid4,   attr_i_ulbfid5,  attr_i_ulbfid6,   attr_i_ulbfid7, 
	attr_i_ulbfic00,  attr_i_ulbfic01, attr_i_ulbfic02,  attr_i_ulbfic03,
	attr_i_ulbfic04,  attr_i_ulbfic05, attr_i_ulbfic06,  attr_i_ulbfic07,
	attr_i_ulbfic10,  attr_i_ulbfic11, attr_i_ulbfic12,  attr_i_ulbfic13,
	attr_i_ulbfic14,  attr_i_ulbfic15, attr_i_ulbfic16,  attr_i_ulbfic17,
	attr_i_ulbfic20,  attr_i_ulbfic21, attr_i_ulbfic22,  attr_i_ulbfic23,
	attr_i_ulbfic24,  attr_i_ulbfic25, attr_i_ulbfic26,  attr_i_ulbfic27,
	attr_i_ulbfic30,  attr_i_ulbfic31, attr_i_ulbfic32,  attr_i_ulbfic33,
	attr_i_ulbfic34,  attr_i_ulbfic35, attr_i_ulbfic36,  attr_i_ulbfic37,


	attr_i_dlbfid_a0,   attr_i_dlbfid_a1,  attr_i_dlbfid_a2,   attr_i_dlbfid_a3, 
	attr_i_dlbfic_a00,  attr_i_dlbfic_a01, attr_i_dlbfic_a02,  attr_i_dlbfic_a03,
	attr_i_dlbfic_a10,  attr_i_dlbfic_a11, attr_i_dlbfic_a12,  attr_i_dlbfic_a13,
	attr_i_dlbfic_a20,  attr_i_dlbfic_a21, attr_i_dlbfic_a22,  attr_i_dlbfic_a23,
	attr_i_dlbfic_a30,  attr_i_dlbfic_a31, attr_i_dlbfic_a32,  attr_i_dlbfic_a33,
	attr_i_dlbfic_a40,  attr_i_dlbfic_a41, attr_i_dlbfic_a42,  attr_i_dlbfic_a43,
	attr_i_dlbfic_a50,  attr_i_dlbfic_a51, attr_i_dlbfic_a52,  attr_i_dlbfic_a53,
	attr_i_dlbfic_a60,  attr_i_dlbfic_a61, attr_i_dlbfic_a62,  attr_i_dlbfic_a63,
	attr_i_dlbfic_a70,  attr_i_dlbfic_a71, attr_i_dlbfic_a72,  attr_i_dlbfic_a73,
    attr_i_ulbfid_a0,   attr_i_ulbfid_a1,  attr_i_ulbfid_a2,   attr_i_ulbfid_a3, 
	attr_i_ulbfid_a4,   attr_i_ulbfid_a5,  attr_i_ulbfid_a6,   attr_i_ulbfid_a7, 
	attr_i_ulbfic_a00,  attr_i_ulbfic_a01, attr_i_ulbfic_a02,  attr_i_ulbfic_a03,
	attr_i_ulbfic_a04,  attr_i_ulbfic_a05, attr_i_ulbfic_a06,  attr_i_ulbfic_a07,
	attr_i_ulbfic_a10,  attr_i_ulbfic_a11, attr_i_ulbfic_a12,  attr_i_ulbfic_a13,
	attr_i_ulbfic_a14,  attr_i_ulbfic_a15, attr_i_ulbfic_a16,  attr_i_ulbfic_a17,
	attr_i_ulbfic_a20,  attr_i_ulbfic_a21, attr_i_ulbfic_a22,  attr_i_ulbfic_a23,
	attr_i_ulbfic_a24,  attr_i_ulbfic_a25, attr_i_ulbfic_a26,  attr_i_ulbfic_a27,
	attr_i_ulbfic_a30,  attr_i_ulbfic_a31, attr_i_ulbfic_a32,  attr_i_ulbfic_a33,
	attr_i_ulbfic_a34,  attr_i_ulbfic_a35, attr_i_ulbfic_a36,  attr_i_ulbfic_a37,


	attr_i_dlbfid_b0,   attr_i_dlbfid_b1,  attr_i_dlbfid_b2,   attr_i_dlbfid_b3, 
	attr_i_dlbfic_b00,  attr_i_dlbfic_b01, attr_i_dlbfic_b02,  attr_i_dlbfic_b03,
	attr_i_dlbfic_b10,  attr_i_dlbfic_b11, attr_i_dlbfic_b12,  attr_i_dlbfic_b13,
	attr_i_dlbfic_b20,  attr_i_dlbfic_b21, attr_i_dlbfic_b22,  attr_i_dlbfic_b23,
	attr_i_dlbfic_b30,  attr_i_dlbfic_b31, attr_i_dlbfic_b32,  attr_i_dlbfic_b33,
	attr_i_dlbfic_b40,  attr_i_dlbfic_b41, attr_i_dlbfic_b42,  attr_i_dlbfic_b43,
	attr_i_dlbfic_b50,  attr_i_dlbfic_b51, attr_i_dlbfic_b52,  attr_i_dlbfic_b53,
	attr_i_dlbfic_b60,  attr_i_dlbfic_b61, attr_i_dlbfic_b62,  attr_i_dlbfic_b63,
	attr_i_dlbfic_b70,  attr_i_dlbfic_b71, attr_i_dlbfic_b72,  attr_i_dlbfic_b73,
    attr_i_ulbfid_b0,   attr_i_ulbfid_b1,  attr_i_ulbfid_b2,   attr_i_ulbfid_b3, 
	attr_i_ulbfid_b4,   attr_i_ulbfid_b5,  attr_i_ulbfid_b6,   attr_i_ulbfid_b7, 
	attr_i_ulbfic_b00,  attr_i_ulbfic_b01, attr_i_ulbfic_b02,  attr_i_ulbfic_b03,
	attr_i_ulbfic_b04,  attr_i_ulbfic_b05, attr_i_ulbfic_b06,  attr_i_ulbfic_b07,
	attr_i_ulbfic_b10,  attr_i_ulbfic_b11, attr_i_ulbfic_b12,  attr_i_ulbfic_b13,
	attr_i_ulbfic_b14,  attr_i_ulbfic_b15, attr_i_ulbfic_b16,  attr_i_ulbfic_b17,
	attr_i_ulbfic_b20,  attr_i_ulbfic_b21, attr_i_ulbfic_b22,  attr_i_ulbfic_b23,
	attr_i_ulbfic_b24,  attr_i_ulbfic_b25, attr_i_ulbfic_b26,  attr_i_ulbfic_b27,
	attr_i_ulbfic_b30,  attr_i_ulbfic_b31, attr_i_ulbfic_b32,  attr_i_ulbfic_b33,
	attr_i_ulbfic_b34,  attr_i_ulbfic_b35, attr_i_ulbfic_b36,  attr_i_ulbfic_b37,

	attr_o_dlbfo0,    attr_o_dlbfo1,   attr_o_dlbfo2,    attr_o_dlbfo3,
	attr_o_dlbfo4,    attr_o_dlbfo5,   attr_o_dlbfo6,    attr_o_dlbfo7,
	attr_o_ulbfo0,    attr_o_ulbfo1,   attr_o_ulbfo2,    attr_o_ulbfo3,

	attr_o_dlbfo_a0,    attr_o_dlbfo_a1,   attr_o_dlbfo_a2,    attr_o_dlbfo_a3,
	attr_o_dlbfo_a4,    attr_o_dlbfo_a5,   attr_o_dlbfo_a6,    attr_o_dlbfo_a7,
	attr_o_ulbfo_a0,    attr_o_ulbfo_a1,   attr_o_ulbfo_a2,    attr_o_ulbfo_a3,

	attr_o_dlbfo_b0,    attr_o_dlbfo_b1,   attr_o_dlbfo_b2,    attr_o_dlbfo_b3,
	attr_o_dlbfo_b4,    attr_o_dlbfo_b5,   attr_o_dlbfo_b6,    attr_o_dlbfo_b7,
	attr_o_ulbfo_b0,    attr_o_ulbfo_b1,   attr_o_ulbfo_b2,    attr_o_ulbfo_b3



);

TEST_BF<COL_OFFSET, COL_OFFSET+16, COL_OFFSET+32> dut;


//---------------------------------------
// DLBF Source and Sink port connections
//---------------------------------------
connect<> dd00(platform.src[0], dut.dldin[0]);
connect<> dd01(platform.src[1], dut.dldin[1]);
connect<> dd02(platform.src[2], dut.dldin[2]);
connect<> dd03(platform.src[3], dut.dldin[3]);
connect<> dc00(platform.src[4*1 + 0], dut.dlcin[4*0 + 0]);
connect<> dc01(platform.src[4*1 + 1], dut.dlcin[4*0 + 1]);
connect<> dc02(platform.src[4*1 + 2], dut.dlcin[4*0 + 2]);
connect<> dc03(platform.src[4*1 + 3], dut.dlcin[4*0 + 3]);
connect<> dc10(platform.src[4*2 + 0], dut.dlcin[4*1 + 0]);
connect<> dc11(platform.src[4*2 + 1], dut.dlcin[4*1 + 1]);
connect<> dc12(platform.src[4*2 + 2], dut.dlcin[4*1 + 2]);
connect<> dc13(platform.src[4*2 + 3], dut.dlcin[4*1 + 3]);
connect<> dc20(platform.src[4*3 + 0], dut.dlcin[4*2 + 0]);
connect<> dc21(platform.src[4*3 + 1], dut.dlcin[4*2 + 1]);
connect<> dc22(platform.src[4*3 + 2], dut.dlcin[4*2 + 2]);
connect<> dc23(platform.src[4*3 + 3], dut.dlcin[4*2 + 3]);
connect<> dc30(platform.src[4*4 + 0], dut.dlcin[4*3 + 0]);
connect<> dc31(platform.src[4*4 + 1], dut.dlcin[4*3 + 1]);
connect<> dc32(platform.src[4*4 + 2], dut.dlcin[4*3 + 2]);
connect<> dc33(platform.src[4*4 + 3], dut.dlcin[4*3 + 3]);
connect<> dc40(platform.src[4*5 + 0], dut.dlcin[4*4 + 0]);
connect<> dc41(platform.src[4*5 + 1], dut.dlcin[4*4 + 1]);
connect<> dc42(platform.src[4*5 + 2], dut.dlcin[4*4 + 2]);
connect<> dc43(platform.src[4*5 + 3], dut.dlcin[4*4 + 3]);
connect<> dc50(platform.src[4*6 + 0], dut.dlcin[4*5 + 0]);
connect<> dc51(platform.src[4*6 + 1], dut.dlcin[4*5 + 1]);
connect<> dc52(platform.src[4*6 + 2], dut.dlcin[4*5 + 2]);
connect<> dc53(platform.src[4*6 + 3], dut.dlcin[4*5 + 3]);
connect<> dc60(platform.src[4*7 + 0], dut.dlcin[4*6 + 0]);
connect<> dc61(platform.src[4*7 + 1], dut.dlcin[4*6 + 1]);
connect<> dc62(platform.src[4*7 + 2], dut.dlcin[4*6 + 2]);
connect<> dc63(platform.src[4*7 + 3], dut.dlcin[4*6 + 3]);
connect<> dc70(platform.src[4*8 + 0], dut.dlcin[4*7 + 0]);
connect<> dc71(platform.src[4*8 + 1], dut.dlcin[4*7 + 1]);
connect<> dc72(platform.src[4*8 + 2], dut.dlcin[4*7 + 2]);
connect<> dc73(platform.src[4*8 + 3], dut.dlcin[4*7 + 3]);
connect<> dnet0(dut.dlout[0],  platform.sink[0]);
connect<> dnet1(dut.dlout[1],  platform.sink[1]);
connect<> dnet2(dut.dlout[2],  platform.sink[2]);
connect<> dnet3(dut.dlout[3],  platform.sink[3]);
connect<> dnet4(dut.dlout[4],  platform.sink[4]);
connect<> dnet5(dut.dlout[5],  platform.sink[5]);
connect<> dnet6(dut.dlout[6],  platform.sink[6]);
connect<> dnet7(dut.dlout[7],  platform.sink[7]);

connect<> dda00(platform.src[76+0], dut.dldin[4 + 0]);
connect<> dda01(platform.src[76+1], dut.dldin[4 + 1]);
connect<> dda02(platform.src[76+2], dut.dldin[4 + 2]);
connect<> dda03(platform.src[76+3], dut.dldin[4 + 3]);
connect<> dca00(platform.src[76+4*1 + 0], dut.dlcin[32 + 4*0 + 0]);
connect<> dca01(platform.src[76+4*1 + 1], dut.dlcin[32 + 4*0 + 1]);
connect<> dca02(platform.src[76+4*1 + 2], dut.dlcin[32 + 4*0 + 2]);
connect<> dca03(platform.src[76+4*1 + 3], dut.dlcin[32 + 4*0 + 3]);
connect<> dca10(platform.src[76+4*2 + 0], dut.dlcin[32 + 4*1 + 0]);
connect<> dca11(platform.src[76+4*2 + 1], dut.dlcin[32 + 4*1 + 1]);
connect<> dca12(platform.src[76+4*2 + 2], dut.dlcin[32 + 4*1 + 2]);
connect<> dca13(platform.src[76+4*2 + 3], dut.dlcin[32 + 4*1 + 3]);
connect<> dca20(platform.src[76+4*3 + 0], dut.dlcin[32 + 4*2 + 0]);
connect<> dca21(platform.src[76+4*3 + 1], dut.dlcin[32 + 4*2 + 1]);
connect<> dca22(platform.src[76+4*3 + 2], dut.dlcin[32 + 4*2 + 2]);
connect<> dca23(platform.src[76+4*3 + 3], dut.dlcin[32 + 4*2 + 3]);
connect<> dca30(platform.src[76+4*4 + 0], dut.dlcin[32 + 4*3 + 0]);
connect<> dca31(platform.src[76+4*4 + 1], dut.dlcin[32 + 4*3 + 1]);
connect<> dca32(platform.src[76+4*4 + 2], dut.dlcin[32 + 4*3 + 2]);
connect<> dca33(platform.src[76+4*4 + 3], dut.dlcin[32 + 4*3 + 3]);
connect<> dca40(platform.src[76+4*5 + 0], dut.dlcin[32 + 4*4 + 0]);
connect<> dca41(platform.src[76+4*5 + 1], dut.dlcin[32 + 4*4 + 1]);
connect<> dca42(platform.src[76+4*5 + 2], dut.dlcin[32 + 4*4 + 2]);
connect<> dca43(platform.src[76+4*5 + 3], dut.dlcin[32 + 4*4 + 3]);
connect<> dca50(platform.src[76+4*6 + 0], dut.dlcin[32 + 4*5 + 0]);
connect<> dca51(platform.src[76+4*6 + 1], dut.dlcin[32 + 4*5 + 1]);
connect<> dca52(platform.src[76+4*6 + 2], dut.dlcin[32 + 4*5 + 2]);
connect<> dca53(platform.src[76+4*6 + 3], dut.dlcin[32 + 4*5 + 3]);
connect<> dca60(platform.src[76+4*7 + 0], dut.dlcin[32 + 4*6 + 0]);
connect<> dca61(platform.src[76+4*7 + 1], dut.dlcin[32 + 4*6 + 1]);
connect<> dca62(platform.src[76+4*7 + 2], dut.dlcin[32 + 4*6 + 2]);
connect<> dca63(platform.src[76+4*7 + 3], dut.dlcin[32 + 4*6 + 3]);
connect<> dca70(platform.src[76+4*8 + 0], dut.dlcin[32 + 4*7 + 0]);
connect<> dca71(platform.src[76+4*8 + 1], dut.dlcin[32 + 4*7 + 1]);
connect<> dca72(platform.src[76+4*8 + 2], dut.dlcin[32 + 4*7 + 2]);
connect<> dca73(platform.src[76+4*8 + 3], dut.dlcin[32 + 4*7 + 3]);
connect<> dneta0(dut.dlout[8+0],  platform.sink[12+0]);
connect<> dneta1(dut.dlout[8+1],  platform.sink[12+1]);
connect<> dneta2(dut.dlout[8+2],  platform.sink[12+2]);
connect<> dneta3(dut.dlout[8+3],  platform.sink[12+3]);
connect<> dneta4(dut.dlout[8+4],  platform.sink[12+4]);
connect<> dneta5(dut.dlout[8+5],  platform.sink[12+5]);
connect<> dneta6(dut.dlout[8+6],  platform.sink[12+6]);
connect<> dneta7(dut.dlout[8+7],  platform.sink[12+7]);

connect<> ddb00(platform.src[152+0], dut.dldin[4*2 + 0]);
connect<> ddb01(platform.src[152+1], dut.dldin[4*2 + 1]);
connect<> ddb02(platform.src[152+2], dut.dldin[4*2 + 2]);
connect<> ddb03(platform.src[152+3], dut.dldin[4*2 + 3]);
connect<> dcb00(platform.src[152+4*1 + 0], dut.dlcin[32*2 + 4*0 + 0]);
connect<> dcb01(platform.src[152+4*1 + 1], dut.dlcin[32*2 + 4*0 + 1]);
connect<> dcb02(platform.src[152+4*1 + 2], dut.dlcin[32*2 + 4*0 + 2]);
connect<> dcb03(platform.src[152+4*1 + 3], dut.dlcin[32*2 + 4*0 + 3]);
connect<> dcb10(platform.src[152+4*2 + 0], dut.dlcin[32*2 + 4*1 + 0]);
connect<> dcb11(platform.src[152+4*2 + 1], dut.dlcin[32*2 + 4*1 + 1]);
connect<> dcb12(platform.src[152+4*2 + 2], dut.dlcin[32*2 + 4*1 + 2]);
connect<> dcb13(platform.src[152+4*2 + 3], dut.dlcin[32*2 + 4*1 + 3]);
connect<> dcb20(platform.src[152+4*3 + 0], dut.dlcin[32*2 + 4*2 + 0]);
connect<> dcb21(platform.src[152+4*3 + 1], dut.dlcin[32*2 + 4*2 + 1]);
connect<> dcb22(platform.src[152+4*3 + 2], dut.dlcin[32*2 + 4*2 + 2]);
connect<> dcb23(platform.src[152+4*3 + 3], dut.dlcin[32*2 + 4*2 + 3]);
connect<> dcb30(platform.src[152+4*4 + 0], dut.dlcin[32*2 + 4*3 + 0]);
connect<> dcb31(platform.src[152+4*4 + 1], dut.dlcin[32*2 + 4*3 + 1]);
connect<> dcb32(platform.src[152+4*4 + 2], dut.dlcin[32*2 + 4*3 + 2]);
connect<> dcb33(platform.src[152+4*4 + 3], dut.dlcin[32*2 + 4*3 + 3]);
connect<> dcb40(platform.src[152+4*5 + 0], dut.dlcin[32*2 + 4*4 + 0]);
connect<> dcb41(platform.src[152+4*5 + 1], dut.dlcin[32*2 + 4*4 + 1]);
connect<> dcb42(platform.src[152+4*5 + 2], dut.dlcin[32*2 + 4*4 + 2]);
connect<> dcb43(platform.src[152+4*5 + 3], dut.dlcin[32*2 + 4*4 + 3]);
connect<> dcb50(platform.src[152+4*6 + 0], dut.dlcin[32*2 + 4*5 + 0]);
connect<> dcb51(platform.src[152+4*6 + 1], dut.dlcin[32*2 + 4*5 + 1]);
connect<> dcb52(platform.src[152+4*6 + 2], dut.dlcin[32*2 + 4*5 + 2]);
connect<> dcb53(platform.src[152+4*6 + 3], dut.dlcin[32*2 + 4*5 + 3]);
connect<> dcb60(platform.src[152+4*7 + 0], dut.dlcin[32*2 + 4*6 + 0]);
connect<> dcb61(platform.src[152+4*7 + 1], dut.dlcin[32*2 + 4*6 + 1]);
connect<> dcb62(platform.src[152+4*7 + 2], dut.dlcin[32*2 + 4*6 + 2]);
connect<> dcb63(platform.src[152+4*7 + 3], dut.dlcin[32*2 + 4*6 + 3]);
connect<> dcb70(platform.src[152+4*8 + 0], dut.dlcin[32*2 + 4*7 + 0]);
connect<> dcb71(platform.src[152+4*8 + 1], dut.dlcin[32*2 + 4*7 + 1]);
connect<> dcb72(platform.src[152+4*8 + 2], dut.dlcin[32*2 + 4*7 + 2]);
connect<> dcb73(platform.src[152+4*8 + 3], dut.dlcin[32*2 + 4*7 + 3]);
connect<> dnetb0(dut.dlout[8*2+0],  platform.sink[24+0]);
connect<> dnetb1(dut.dlout[8*2+1],  platform.sink[24+1]);
connect<> dnetb2(dut.dlout[8*2+2],  platform.sink[24+2]);
connect<> dnetb3(dut.dlout[8*2+3],  platform.sink[24+3]);
connect<> dnetb4(dut.dlout[8*2+4],  platform.sink[24+4]);
connect<> dnetb5(dut.dlout[8*2+5],  platform.sink[24+5]);
connect<> dnetb6(dut.dlout[8*2+6],  platform.sink[24+6]);
connect<> dnetb7(dut.dlout[8*2+7],  platform.sink[24+7]);


//---------------------------------------
// ULBF Source and Sink port connections
//---------------------------------------
connect<> ud0(platform.src[UL_INPUT_BA+0], dut.uldin[0]);
connect<> ud1(platform.src[UL_INPUT_BA+1], dut.uldin[1]);
connect<> ud2(platform.src[UL_INPUT_BA+2], dut.uldin[2]);
connect<> ud3(platform.src[UL_INPUT_BA+3], dut.uldin[3]);
connect<> ud4(platform.src[UL_INPUT_BA+4], dut.uldin[4]);
connect<> ud5(platform.src[UL_INPUT_BA+5], dut.uldin[5]);
connect<> ud6(platform.src[UL_INPUT_BA+6], dut.uldin[6]);
connect<> ud7(platform.src[UL_INPUT_BA+7], dut.uldin[7]);

connect<> uc00(platform.src[UL_INPUT_BA+8*1 + 0], dut.ulcin[8*0 + 0]);
connect<> uc01(platform.src[UL_INPUT_BA+8*1 + 1], dut.ulcin[8*0 + 1]);
connect<> uc02(platform.src[UL_INPUT_BA+8*1 + 2], dut.ulcin[8*0 + 2]);
connect<> uc03(platform.src[UL_INPUT_BA+8*1 + 3], dut.ulcin[8*0 + 3]);
connect<> uc04(platform.src[UL_INPUT_BA+8*1 + 4], dut.ulcin[8*0 + 4]);
connect<> uc05(platform.src[UL_INPUT_BA+8*1 + 5], dut.ulcin[8*0 + 5]);
connect<> uc06(platform.src[UL_INPUT_BA+8*1 + 6], dut.ulcin[8*0 + 6]);
connect<> uc07(platform.src[UL_INPUT_BA+8*1 + 7], dut.ulcin[8*0 + 7]);
connect<> uc10(platform.src[UL_INPUT_BA+8*2 + 0], dut.ulcin[8*1 + 0]);
connect<> uc11(platform.src[UL_INPUT_BA+8*2 + 1], dut.ulcin[8*1 + 1]);
connect<> uc12(platform.src[UL_INPUT_BA+8*2 + 2], dut.ulcin[8*1 + 2]);
connect<> uc13(platform.src[UL_INPUT_BA+8*2 + 3], dut.ulcin[8*1 + 3]);
connect<> uc14(platform.src[UL_INPUT_BA+8*2 + 4], dut.ulcin[8*1 + 4]);
connect<> uc15(platform.src[UL_INPUT_BA+8*2 + 5], dut.ulcin[8*1 + 5]);
connect<> uc16(platform.src[UL_INPUT_BA+8*2 + 6], dut.ulcin[8*1 + 6]);
connect<> uc17(platform.src[UL_INPUT_BA+8*2 + 7], dut.ulcin[8*1 + 7]);
connect<> uc20(platform.src[UL_INPUT_BA+8*3 + 0], dut.ulcin[8*2 + 0]);
connect<> uc21(platform.src[UL_INPUT_BA+8*3 + 1], dut.ulcin[8*2 + 1]);
connect<> uc22(platform.src[UL_INPUT_BA+8*3 + 2], dut.ulcin[8*2 + 2]);
connect<> uc23(platform.src[UL_INPUT_BA+8*3 + 3], dut.ulcin[8*2 + 3]);
connect<> uc24(platform.src[UL_INPUT_BA+8*3 + 4], dut.ulcin[8*2 + 4]);
connect<> uc25(platform.src[UL_INPUT_BA+8*3 + 5], dut.ulcin[8*2 + 5]);
connect<> uc26(platform.src[UL_INPUT_BA+8*3 + 6], dut.ulcin[8*2 + 6]);
connect<> uc27(platform.src[UL_INPUT_BA+8*3 + 7], dut.ulcin[8*2 + 7]);
connect<> uc30(platform.src[UL_INPUT_BA+8*4 + 0], dut.ulcin[8*3 + 0]);
connect<> uc31(platform.src[UL_INPUT_BA+8*4 + 1], dut.ulcin[8*3 + 1]);
connect<> uc32(platform.src[UL_INPUT_BA+8*4 + 2], dut.ulcin[8*3 + 2]);
connect<> uc33(platform.src[UL_INPUT_BA+8*4 + 3], dut.ulcin[8*3 + 3]);
connect<> uc34(platform.src[UL_INPUT_BA+8*4 + 4], dut.ulcin[8*3 + 4]);
connect<> uc35(platform.src[UL_INPUT_BA+8*4 + 5], dut.ulcin[8*3 + 5]);
connect<> uc36(platform.src[UL_INPUT_BA+8*4 + 6], dut.ulcin[8*3 + 6]);
connect<> uc37(platform.src[UL_INPUT_BA+8*4 + 7], dut.ulcin[8*3 + 7]);

connect<> unet0(dut.ulout[0],  platform.sink[UL_OUTPUT_BA+0]);
connect<> unet1(dut.ulout[1],  platform.sink[UL_OUTPUT_BA+1]);
connect<> unet2(dut.ulout[2],  platform.sink[UL_OUTPUT_BA+2]);
connect<> unet3(dut.ulout[3],  platform.sink[UL_OUTPUT_BA+3]);


connect<> uda0(platform.src[76+UL_INPUT_BA+0], dut.uldin[8+0]);
connect<> uda1(platform.src[76+UL_INPUT_BA+1], dut.uldin[8+1]);
connect<> uda2(platform.src[76+UL_INPUT_BA+2], dut.uldin[8+2]);
connect<> uda3(platform.src[76+UL_INPUT_BA+3], dut.uldin[8+3]);
connect<> uda4(platform.src[76+UL_INPUT_BA+4], dut.uldin[8+4]);
connect<> uda5(platform.src[76+UL_INPUT_BA+5], dut.uldin[8+5]);
connect<> uda6(platform.src[76+UL_INPUT_BA+6], dut.uldin[8+6]);
connect<> uda7(platform.src[76+UL_INPUT_BA+7], dut.uldin[8+7]);

connect<> uca00(platform.src[76+UL_INPUT_BA+8*1 + 0], dut.ulcin[32 + 8*0 + 0]);
connect<> uca01(platform.src[76+UL_INPUT_BA+8*1 + 1], dut.ulcin[32 + 8*0 + 1]);
connect<> uca02(platform.src[76+UL_INPUT_BA+8*1 + 2], dut.ulcin[32 + 8*0 + 2]);
connect<> uca03(platform.src[76+UL_INPUT_BA+8*1 + 3], dut.ulcin[32 + 8*0 + 3]);
connect<> uca04(platform.src[76+UL_INPUT_BA+8*1 + 4], dut.ulcin[32 + 8*0 + 4]);
connect<> uca05(platform.src[76+UL_INPUT_BA+8*1 + 5], dut.ulcin[32 + 8*0 + 5]);
connect<> uca06(platform.src[76+UL_INPUT_BA+8*1 + 6], dut.ulcin[32 + 8*0 + 6]);
connect<> uca07(platform.src[76+UL_INPUT_BA+8*1 + 7], dut.ulcin[32 + 8*0 + 7]);
connect<> uca10(platform.src[76+UL_INPUT_BA+8*2 + 0], dut.ulcin[32 + 8*1 + 0]);
connect<> uca11(platform.src[76+UL_INPUT_BA+8*2 + 1], dut.ulcin[32 + 8*1 + 1]);
connect<> uca12(platform.src[76+UL_INPUT_BA+8*2 + 2], dut.ulcin[32 + 8*1 + 2]);
connect<> uca13(platform.src[76+UL_INPUT_BA+8*2 + 3], dut.ulcin[32 + 8*1 + 3]);
connect<> uca14(platform.src[76+UL_INPUT_BA+8*2 + 4], dut.ulcin[32 + 8*1 + 4]);
connect<> uca15(platform.src[76+UL_INPUT_BA+8*2 + 5], dut.ulcin[32 + 8*1 + 5]);
connect<> uca16(platform.src[76+UL_INPUT_BA+8*2 + 6], dut.ulcin[32 + 8*1 + 6]);
connect<> uca17(platform.src[76+UL_INPUT_BA+8*2 + 7], dut.ulcin[32 + 8*1 + 7]);
connect<> uca20(platform.src[76+UL_INPUT_BA+8*3 + 0], dut.ulcin[32 + 8*2 + 0]);
connect<> uca21(platform.src[76+UL_INPUT_BA+8*3 + 1], dut.ulcin[32 + 8*2 + 1]);
connect<> uca22(platform.src[76+UL_INPUT_BA+8*3 + 2], dut.ulcin[32 + 8*2 + 2]);
connect<> uca23(platform.src[76+UL_INPUT_BA+8*3 + 3], dut.ulcin[32 + 8*2 + 3]);
connect<> uca24(platform.src[76+UL_INPUT_BA+8*3 + 4], dut.ulcin[32 + 8*2 + 4]);
connect<> uca25(platform.src[76+UL_INPUT_BA+8*3 + 5], dut.ulcin[32 + 8*2 + 5]);
connect<> uca26(platform.src[76+UL_INPUT_BA+8*3 + 6], dut.ulcin[32 + 8*2 + 6]);
connect<> uca27(platform.src[76+UL_INPUT_BA+8*3 + 7], dut.ulcin[32 + 8*2 + 7]);
connect<> uca30(platform.src[76+UL_INPUT_BA+8*4 + 0], dut.ulcin[32 + 8*3 + 0]);
connect<> uca31(platform.src[76+UL_INPUT_BA+8*4 + 1], dut.ulcin[32 + 8*3 + 1]);
connect<> uca32(platform.src[76+UL_INPUT_BA+8*4 + 2], dut.ulcin[32 + 8*3 + 2]);
connect<> uca33(platform.src[76+UL_INPUT_BA+8*4 + 3], dut.ulcin[32 + 8*3 + 3]);
connect<> uca34(platform.src[76+UL_INPUT_BA+8*4 + 4], dut.ulcin[32 + 8*3 + 4]);
connect<> uca35(platform.src[76+UL_INPUT_BA+8*4 + 5], dut.ulcin[32 + 8*3 + 5]);
connect<> uca36(platform.src[76+UL_INPUT_BA+8*4 + 6], dut.ulcin[32 + 8*3 + 6]);
connect<> uca37(platform.src[76+UL_INPUT_BA+8*4 + 7], dut.ulcin[32 + 8*3 + 7]);

connect<> uneta0(dut.ulout[4+0],  platform.sink[12+UL_OUTPUT_BA+0]);
connect<> uneta1(dut.ulout[4+1],  platform.sink[12+UL_OUTPUT_BA+1]);
connect<> uneta2(dut.ulout[4+2],  platform.sink[12+UL_OUTPUT_BA+2]);
connect<> uneta3(dut.ulout[4+3],  platform.sink[12+UL_OUTPUT_BA+3]);


connect<> udb0(platform.src[152+UL_INPUT_BA+0], dut.uldin[8*2+0]);
connect<> udb1(platform.src[152+UL_INPUT_BA+1], dut.uldin[8*2+1]);
connect<> udb2(platform.src[152+UL_INPUT_BA+2], dut.uldin[8*2+2]);
connect<> udb3(platform.src[152+UL_INPUT_BA+3], dut.uldin[8*2+3]);
connect<> udb4(platform.src[152+UL_INPUT_BA+4], dut.uldin[8*2+4]);
connect<> udb5(platform.src[152+UL_INPUT_BA+5], dut.uldin[8*2+5]);
connect<> udb6(platform.src[152+UL_INPUT_BA+6], dut.uldin[8*2+6]);
connect<> udb7(platform.src[152+UL_INPUT_BA+7], dut.uldin[8*2+7]);

connect<> ucb00(platform.src[152+UL_INPUT_BA+8*1 + 0], dut.ulcin[32*2 + 8*0 + 0]);
connect<> ucb01(platform.src[152+UL_INPUT_BA+8*1 + 1], dut.ulcin[32*2 + 8*0 + 1]);
connect<> ucb02(platform.src[152+UL_INPUT_BA+8*1 + 2], dut.ulcin[32*2 + 8*0 + 2]);
connect<> ucb03(platform.src[152+UL_INPUT_BA+8*1 + 3], dut.ulcin[32*2 + 8*0 + 3]);
connect<> ucb04(platform.src[152+UL_INPUT_BA+8*1 + 4], dut.ulcin[32*2 + 8*0 + 4]);
connect<> ucb05(platform.src[152+UL_INPUT_BA+8*1 + 5], dut.ulcin[32*2 + 8*0 + 5]);
connect<> ucb06(platform.src[152+UL_INPUT_BA+8*1 + 6], dut.ulcin[32*2 + 8*0 + 6]);
connect<> ucb07(platform.src[152+UL_INPUT_BA+8*1 + 7], dut.ulcin[32*2 + 8*0 + 7]);
connect<> ucb10(platform.src[152+UL_INPUT_BA+8*2 + 0], dut.ulcin[32*2 + 8*1 + 0]);
connect<> ucb11(platform.src[152+UL_INPUT_BA+8*2 + 1], dut.ulcin[32*2 + 8*1 + 1]);
connect<> ucb12(platform.src[152+UL_INPUT_BA+8*2 + 2], dut.ulcin[32*2 + 8*1 + 2]);
connect<> ucb13(platform.src[152+UL_INPUT_BA+8*2 + 3], dut.ulcin[32*2 + 8*1 + 3]);
connect<> ucb14(platform.src[152+UL_INPUT_BA+8*2 + 4], dut.ulcin[32*2 + 8*1 + 4]);
connect<> ucb15(platform.src[152+UL_INPUT_BA+8*2 + 5], dut.ulcin[32*2 + 8*1 + 5]);
connect<> ucb16(platform.src[152+UL_INPUT_BA+8*2 + 6], dut.ulcin[32*2 + 8*1 + 6]);
connect<> ucb17(platform.src[152+UL_INPUT_BA+8*2 + 7], dut.ulcin[32*2 + 8*1 + 7]);
connect<> ucb20(platform.src[152+UL_INPUT_BA+8*3 + 0], dut.ulcin[32*2 + 8*2 + 0]);
connect<> ucb21(platform.src[152+UL_INPUT_BA+8*3 + 1], dut.ulcin[32*2 + 8*2 + 1]);
connect<> ucb22(platform.src[152+UL_INPUT_BA+8*3 + 2], dut.ulcin[32*2 + 8*2 + 2]);
connect<> ucb23(platform.src[152+UL_INPUT_BA+8*3 + 3], dut.ulcin[32*2 + 8*2 + 3]);
connect<> ucb24(platform.src[152+UL_INPUT_BA+8*3 + 4], dut.ulcin[32*2 + 8*2 + 4]);
connect<> ucb25(platform.src[152+UL_INPUT_BA+8*3 + 5], dut.ulcin[32*2 + 8*2 + 5]);
connect<> ucb26(platform.src[152+UL_INPUT_BA+8*3 + 6], dut.ulcin[32*2 + 8*2 + 6]);
connect<> ucb27(platform.src[152+UL_INPUT_BA+8*3 + 7], dut.ulcin[32*2 + 8*2 + 7]);
connect<> ucb30(platform.src[152+UL_INPUT_BA+8*4 + 0], dut.ulcin[32*2 + 8*3 + 0]);
connect<> ucb31(platform.src[152+UL_INPUT_BA+8*4 + 1], dut.ulcin[32*2 + 8*3 + 1]);
connect<> ucb32(platform.src[152+UL_INPUT_BA+8*4 + 2], dut.ulcin[32*2 + 8*3 + 2]);
connect<> ucb33(platform.src[152+UL_INPUT_BA+8*4 + 3], dut.ulcin[32*2 + 8*3 + 3]);
connect<> ucb34(platform.src[152+UL_INPUT_BA+8*4 + 4], dut.ulcin[32*2 + 8*3 + 4]);
connect<> ucb35(platform.src[152+UL_INPUT_BA+8*4 + 5], dut.ulcin[32*2 + 8*3 + 5]);
connect<> ucb36(platform.src[152+UL_INPUT_BA+8*4 + 6], dut.ulcin[32*2 + 8*3 + 6]);
connect<> ucb37(platform.src[152+UL_INPUT_BA+8*4 + 7], dut.ulcin[32*2 + 8*3 + 7]);

connect<> unetb0(dut.ulout[4*2+0],  platform.sink[24+UL_OUTPUT_BA+0]);
connect<> unetb1(dut.ulout[4*2+1],  platform.sink[24+UL_OUTPUT_BA+1]);
connect<> unetb2(dut.ulout[4*2+2],  platform.sink[24+UL_OUTPUT_BA+2]);
connect<> unetb3(dut.ulout[4*2+3],  platform.sink[24+UL_OUTPUT_BA+3]);

#ifdef __AIESIM__
int main(void) {

	dut.init();
	const int NITER = 20;
	const int totalWords = 	NITER*PDSCH_OUT_DATA_BLKSZ;

	event::handle handle0 = event::start_profiling(*attr_o_dlbfo0, event::io_stream_start_to_bytes_transferred_cycles, totalWords*sizeof(cint16));
	event::handle handle1 = event::start_profiling(*attr_o_ulbfo0, event::io_stream_start_to_bytes_transferred_cycles, totalWords*sizeof(cint16));

	dut.run(NITER);
	dut.wait();
	
	long long cycle_count0 = event::read_profiling(handle0);
	long long cycle_count1 = event::read_profiling(handle1);

	event::stop_profiling(handle0);
	event::stop_profiling(handle1);

	double throughput0 = (double)totalWords * sizeof(cint16) / (cycle_count0 * 1e-9); //byte per second
	double throughput1 = (double)totalWords * sizeof(cint16) / (cycle_count1 * 1e-9); //byte per second
   
	std::cout << " DLBF Output0: Throughput " << throughput0/4.0 << " Samples/s, i.e. " << throughput0 << " Bytes/s" <<std::endl;
	std::cout << " ULBF Output0: Throughput " << throughput1/4.0 << " Samples/s, i.e. " << throughput1 << " Bytes/s" <<std::endl;

	dut.end();
	return 0;

}
#endif
