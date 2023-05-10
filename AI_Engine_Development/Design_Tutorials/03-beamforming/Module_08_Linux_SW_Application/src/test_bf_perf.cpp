
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "adf.h"

#include "test_bf_app.h"
#include "utils_linux.h"
#include "params.h"
#include "utils_dlbf.h"
#include "utils_ulbf.h"

// from aie app

//DLBF_0: 4 Data Input Ports 
adf::PLIO *attr_i_dlbfid0 = new adf::PLIO("dlbfid0", adf::plio_64_bits, "../data/dlbf_din0.txt", 400);
adf::PLIO *attr_i_dlbfid1 = new adf::PLIO("dlbfid1", adf::plio_64_bits, "../data/dlbf_din1.txt", 400);
adf::PLIO *attr_i_dlbfid2 = new adf::PLIO("dlbfid2", adf::plio_64_bits, "../data/dlbf_din2.txt", 400);
adf::PLIO *attr_i_dlbfid3 = new adf::PLIO("dlbfid3", adf::plio_64_bits, "../data/dlbf_din3.txt", 400);

//DLBF_0, Chain 0: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic00 = new adf::PLIO("dlbfic00", adf::plio_64_bits, "../data/dlbf_cin00.txt", 400);
adf::PLIO *attr_i_dlbfic01 = new adf::PLIO("dlbfic01", adf::plio_64_bits, "../data/dlbf_cin01.txt", 400);
adf::PLIO *attr_i_dlbfic02 = new adf::PLIO("dlbfic02", adf::plio_64_bits, "../data/dlbf_cin02.txt", 400);
adf::PLIO *attr_i_dlbfic03 = new adf::PLIO("dlbfic03", adf::plio_64_bits, "../data/dlbf_cin03.txt", 400);

//DLBF_0, Chain 1: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic10 = new adf::PLIO("dlbfic10", adf::plio_64_bits, "../data/dlbf_cin10.txt", 400);
adf::PLIO *attr_i_dlbfic11 = new adf::PLIO("dlbfic11", adf::plio_64_bits, "../data/dlbf_cin11.txt", 400);
adf::PLIO *attr_i_dlbfic12 = new adf::PLIO("dlbfic12", adf::plio_64_bits, "../data/dlbf_cin12.txt", 400);
adf::PLIO *attr_i_dlbfic13 = new adf::PLIO("dlbfic13", adf::plio_64_bits, "../data/dlbf_cin13.txt", 400);

//DLBF_0, Chain 2: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic20 = new adf::PLIO("dlbfic20", adf::plio_64_bits, "../data/dlbf_cin20.txt", 400);
adf::PLIO *attr_i_dlbfic21 = new adf::PLIO("dlbfic21", adf::plio_64_bits, "../data/dlbf_cin21.txt", 400);
adf::PLIO *attr_i_dlbfic22 = new adf::PLIO("dlbfic22", adf::plio_64_bits, "../data/dlbf_cin22.txt", 400);
adf::PLIO *attr_i_dlbfic23 = new adf::PLIO("dlbfic23", adf::plio_64_bits, "../data/dlbf_cin23.txt", 400);

//DLBF_0, Chain 3: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic30 = new adf::PLIO("dlbfic30", adf::plio_64_bits, "../data/dlbf_cin30.txt", 400);
adf::PLIO *attr_i_dlbfic31 = new adf::PLIO("dlbfic31", adf::plio_64_bits, "../data/dlbf_cin31.txt", 400);
adf::PLIO *attr_i_dlbfic32 = new adf::PLIO("dlbfic32", adf::plio_64_bits, "../data/dlbf_cin32.txt", 400);
adf::PLIO *attr_i_dlbfic33 = new adf::PLIO("dlbfic33", adf::plio_64_bits, "../data/dlbf_cin33.txt", 400);

//DLBF_0, Chain 4: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic40 = new adf::PLIO("dlbfic40", adf::plio_64_bits, "../data/dlbf_cin40.txt", 400);
adf::PLIO *attr_i_dlbfic41 = new adf::PLIO("dlbfic41", adf::plio_64_bits, "../data/dlbf_cin41.txt", 400);
adf::PLIO *attr_i_dlbfic42 = new adf::PLIO("dlbfic42", adf::plio_64_bits, "../data/dlbf_cin42.txt", 400);
adf::PLIO *attr_i_dlbfic43 = new adf::PLIO("dlbfic43", adf::plio_64_bits, "../data/dlbf_cin43.txt", 400);

//DLBF_0, Chain 5: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic50 = new adf::PLIO("dlbfic50", adf::plio_64_bits, "../data/dlbf_cin50.txt", 400);
adf::PLIO *attr_i_dlbfic51 = new adf::PLIO("dlbfic51", adf::plio_64_bits, "../data/dlbf_cin51.txt", 400);
adf::PLIO *attr_i_dlbfic52 = new adf::PLIO("dlbfic52", adf::plio_64_bits, "../data/dlbf_cin52.txt", 400);
adf::PLIO *attr_i_dlbfic53 = new adf::PLIO("dlbfic53", adf::plio_64_bits, "../data/dlbf_cin53.txt", 400);

//DLBF_0, Chain 6: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic60 = new adf::PLIO("dlbfic60", adf::plio_64_bits, "../data/dlbf_cin60.txt", 400);
adf::PLIO *attr_i_dlbfic61 = new adf::PLIO("dlbfic61", adf::plio_64_bits, "../data/dlbf_cin61.txt", 400);
adf::PLIO *attr_i_dlbfic62 = new adf::PLIO("dlbfic62", adf::plio_64_bits, "../data/dlbf_cin62.txt", 400);
adf::PLIO *attr_i_dlbfic63 = new adf::PLIO("dlbfic63", adf::plio_64_bits, "../data/dlbf_cin63.txt", 400);

//DLBF_0, Chain 7: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic70 = new adf::PLIO("dlbfic70", adf::plio_64_bits, "../data/dlbf_cin70.txt", 400);
adf::PLIO *attr_i_dlbfic71 = new adf::PLIO("dlbfic71", adf::plio_64_bits, "../data/dlbf_cin71.txt", 400);
adf::PLIO *attr_i_dlbfic72 = new adf::PLIO("dlbfic72", adf::plio_64_bits, "../data/dlbf_cin72.txt", 400);
adf::PLIO *attr_i_dlbfic73 = new adf::PLIO("dlbfic73", adf::plio_64_bits, "../data/dlbf_cin73.txt", 400);

//DLBF_0, 8 Data Output Ports, for Chains 0-7 
adf::PLIO *attr_o_dlbfo0  = new adf::PLIO("dlbfo0",  adf::plio_64_bits, "data/dlbf_out0.txt", 400);
adf::PLIO *attr_o_dlbfo1  = new adf::PLIO("dlbfo1",  adf::plio_64_bits, "data/dlbf_out1.txt", 400);
adf::PLIO *attr_o_dlbfo2  = new adf::PLIO("dlbfo2",  adf::plio_64_bits, "data/dlbf_out2.txt", 400);
adf::PLIO *attr_o_dlbfo3  = new adf::PLIO("dlbfo3",  adf::plio_64_bits, "data/dlbf_out3.txt", 400);
adf::PLIO *attr_o_dlbfo4  = new adf::PLIO("dlbfo4",  adf::plio_64_bits, "data/dlbf_out4.txt", 400);
adf::PLIO *attr_o_dlbfo5  = new adf::PLIO("dlbfo5",  adf::plio_64_bits, "data/dlbf_out5.txt", 400);
adf::PLIO *attr_o_dlbfo6  = new adf::PLIO("dlbfo6",  adf::plio_64_bits, "data/dlbf_out6.txt", 400);
adf::PLIO *attr_o_dlbfo7  = new adf::PLIO("dlbfo7",  adf::plio_64_bits, "data/dlbf_out7.txt", 400);

//DLBF_1: 4 Data Input Ports 
adf::PLIO *attr_i_dlbfid_a0 = new adf::PLIO("dlbfid_a0", adf::plio_64_bits, "../data/dlbf_din0.txt", 400);
adf::PLIO *attr_i_dlbfid_a1 = new adf::PLIO("dlbfid_a1", adf::plio_64_bits, "../data/dlbf_din1.txt", 400);
adf::PLIO *attr_i_dlbfid_a2 = new adf::PLIO("dlbfid_a2", adf::plio_64_bits, "../data/dlbf_din2.txt", 400);
adf::PLIO *attr_i_dlbfid_a3 = new adf::PLIO("dlbfid_a3", adf::plio_64_bits, "../data/dlbf_din3.txt", 400);

//DLBF_1, Chain 0: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a00 = new adf::PLIO("dlbfic_a00", adf::plio_64_bits, "../data/dlbf_cin00.txt", 400);
adf::PLIO *attr_i_dlbfic_a01 = new adf::PLIO("dlbfic_a01", adf::plio_64_bits, "../data/dlbf_cin01.txt", 400);
adf::PLIO *attr_i_dlbfic_a02 = new adf::PLIO("dlbfic_a02", adf::plio_64_bits, "../data/dlbf_cin02.txt", 400);
adf::PLIO *attr_i_dlbfic_a03 = new adf::PLIO("dlbfic_a03", adf::plio_64_bits, "../data/dlbf_cin03.txt", 400);

//DLBF_1, Chain 1: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a10 = new adf::PLIO("dlbfic_a10", adf::plio_64_bits, "../data/dlbf_cin10.txt", 400);
adf::PLIO *attr_i_dlbfic_a11 = new adf::PLIO("dlbfic_a11", adf::plio_64_bits, "../data/dlbf_cin11.txt", 400);
adf::PLIO *attr_i_dlbfic_a12 = new adf::PLIO("dlbfic_a12", adf::plio_64_bits, "../data/dlbf_cin12.txt", 400);
adf::PLIO *attr_i_dlbfic_a13 = new adf::PLIO("dlbfic_a13", adf::plio_64_bits, "../data/dlbf_cin13.txt", 400);

//DLBF_1, Chain 2: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a20 = new adf::PLIO("dlbfic_a20", adf::plio_64_bits, "../data/dlbf_cin20.txt", 400);
adf::PLIO *attr_i_dlbfic_a21 = new adf::PLIO("dlbfic_a21", adf::plio_64_bits, "../data/dlbf_cin21.txt", 400);
adf::PLIO *attr_i_dlbfic_a22 = new adf::PLIO("dlbfic_a22", adf::plio_64_bits, "../data/dlbf_cin22.txt", 400);
adf::PLIO *attr_i_dlbfic_a23 = new adf::PLIO("dlbfic_a23", adf::plio_64_bits, "../data/dlbf_cin23.txt", 400);

//DLBF_1, Chain 3: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a30 = new adf::PLIO("dlbfic_a30", adf::plio_64_bits, "../data/dlbf_cin30.txt", 400);
adf::PLIO *attr_i_dlbfic_a31 = new adf::PLIO("dlbfic_a31", adf::plio_64_bits, "../data/dlbf_cin31.txt", 400);
adf::PLIO *attr_i_dlbfic_a32 = new adf::PLIO("dlbfic_a32", adf::plio_64_bits, "../data/dlbf_cin32.txt", 400);
adf::PLIO *attr_i_dlbfic_a33 = new adf::PLIO("dlbfic_a33", adf::plio_64_bits, "../data/dlbf_cin33.txt", 400);

//DLBF_1, Chain 4: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a40 = new adf::PLIO("dlbfic_a40", adf::plio_64_bits, "../data/dlbf_cin40.txt", 400);
adf::PLIO *attr_i_dlbfic_a41 = new adf::PLIO("dlbfic_a41", adf::plio_64_bits, "../data/dlbf_cin41.txt", 400);
adf::PLIO *attr_i_dlbfic_a42 = new adf::PLIO("dlbfic_a42", adf::plio_64_bits, "../data/dlbf_cin42.txt", 400);
adf::PLIO *attr_i_dlbfic_a43 = new adf::PLIO("dlbfic_a43", adf::plio_64_bits, "../data/dlbf_cin43.txt", 400);

//DLBF_1, Chain 5: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a50 = new adf::PLIO("dlbfic_a50", adf::plio_64_bits, "../data/dlbf_cin50.txt", 400);
adf::PLIO *attr_i_dlbfic_a51 = new adf::PLIO("dlbfic_a51", adf::plio_64_bits, "../data/dlbf_cin51.txt", 400);
adf::PLIO *attr_i_dlbfic_a52 = new adf::PLIO("dlbfic_a52", adf::plio_64_bits, "../data/dlbf_cin52.txt", 400);
adf::PLIO *attr_i_dlbfic_a53 = new adf::PLIO("dlbfic_a53", adf::plio_64_bits, "../data/dlbf_cin53.txt", 400);

//DLBF_1, Chain 6: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a60 = new adf::PLIO("dlbfic_a60", adf::plio_64_bits, "../data/dlbf_cin60.txt", 400);
adf::PLIO *attr_i_dlbfic_a61 = new adf::PLIO("dlbfic_a61", adf::plio_64_bits, "../data/dlbf_cin61.txt", 400);
adf::PLIO *attr_i_dlbfic_a62 = new adf::PLIO("dlbfic_a62", adf::plio_64_bits, "../data/dlbf_cin62.txt", 400);
adf::PLIO *attr_i_dlbfic_a63 = new adf::PLIO("dlbfic_a63", adf::plio_64_bits, "../data/dlbf_cin63.txt", 400);

//DLBF_1, Chain 7: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_a70 = new adf::PLIO("dlbfic_a70", adf::plio_64_bits, "../data/dlbf_cin70.txt", 400);
adf::PLIO *attr_i_dlbfic_a71 = new adf::PLIO("dlbfic_a71", adf::plio_64_bits, "../data/dlbf_cin71.txt", 400);
adf::PLIO *attr_i_dlbfic_a72 = new adf::PLIO("dlbfic_a72", adf::plio_64_bits, "../data/dlbf_cin72.txt", 400);
adf::PLIO *attr_i_dlbfic_a73 = new adf::PLIO("dlbfic_a73", adf::plio_64_bits, "../data/dlbf_cin73.txt", 400);

//DLBF_1, 8 Data Output Ports, for Chains 0-7 
adf::PLIO *attr_o_dlbfo_a0  = new adf::PLIO("dlbfo_a0",  adf::plio_64_bits, "data/dlbf_out8.txt", 400);
adf::PLIO *attr_o_dlbfo_a1  = new adf::PLIO("dlbfo_a1",  adf::plio_64_bits, "data/dlbf_out9.txt", 400);
adf::PLIO *attr_o_dlbfo_a2  = new adf::PLIO("dlbfo_a2",  adf::plio_64_bits, "data/dlbf_out10.txt", 400);
adf::PLIO *attr_o_dlbfo_a3  = new adf::PLIO("dlbfo_a3",  adf::plio_64_bits, "data/dlbf_out11.txt", 400);
adf::PLIO *attr_o_dlbfo_a4  = new adf::PLIO("dlbfo_a4",  adf::plio_64_bits, "data/dlbf_out12.txt", 400);
adf::PLIO *attr_o_dlbfo_a5  = new adf::PLIO("dlbfo_a5",  adf::plio_64_bits, "data/dlbf_out13.txt", 400);
adf::PLIO *attr_o_dlbfo_a6  = new adf::PLIO("dlbfo_a6",  adf::plio_64_bits, "data/dlbf_out14.txt", 400);
adf::PLIO *attr_o_dlbfo_a7  = new adf::PLIO("dlbfo_a7",  adf::plio_64_bits, "data/dlbf_out15.txt", 400);

//DLBF_2: 4 Data Input Ports 
adf::PLIO *attr_i_dlbfid_b0 = new adf::PLIO("dlbfid_b0", adf::plio_64_bits, "../data/dlbf_din0.txt", 400);
adf::PLIO *attr_i_dlbfid_b1 = new adf::PLIO("dlbfid_b1", adf::plio_64_bits, "../data/dlbf_din1.txt", 400);
adf::PLIO *attr_i_dlbfid_b2 = new adf::PLIO("dlbfid_b2", adf::plio_64_bits, "../data/dlbf_din2.txt", 400);
adf::PLIO *attr_i_dlbfid_b3 = new adf::PLIO("dlbfid_b3", adf::plio_64_bits, "../data/dlbf_din3.txt", 400);

//DLBF_2, Chain 0: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b00 = new adf::PLIO("dlbfic_b00", adf::plio_64_bits, "../data/dlbf_cin00.txt", 400);
adf::PLIO *attr_i_dlbfic_b01 = new adf::PLIO("dlbfic_b01", adf::plio_64_bits, "../data/dlbf_cin01.txt", 400);
adf::PLIO *attr_i_dlbfic_b02 = new adf::PLIO("dlbfic_b02", adf::plio_64_bits, "../data/dlbf_cin02.txt", 400);
adf::PLIO *attr_i_dlbfic_b03 = new adf::PLIO("dlbfic_b03", adf::plio_64_bits, "../data/dlbf_cin03.txt", 400);

//DLBF_2, Chain 1: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b10 = new adf::PLIO("dlbfic_b10", adf::plio_64_bits, "../data/dlbf_cin10.txt", 400);
adf::PLIO *attr_i_dlbfic_b11 = new adf::PLIO("dlbfic_b11", adf::plio_64_bits, "../data/dlbf_cin11.txt", 400);
adf::PLIO *attr_i_dlbfic_b12 = new adf::PLIO("dlbfic_b12", adf::plio_64_bits, "../data/dlbf_cin12.txt", 400);
adf::PLIO *attr_i_dlbfic_b13 = new adf::PLIO("dlbfic_b13", adf::plio_64_bits, "../data/dlbf_cin13.txt", 400);

//DLBF_2, Chain 2: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b20 = new adf::PLIO("dlbfic_b20", adf::plio_64_bits, "../data/dlbf_cin20.txt", 400);
adf::PLIO *attr_i_dlbfic_b21 = new adf::PLIO("dlbfic_b21", adf::plio_64_bits, "../data/dlbf_cin21.txt", 400);
adf::PLIO *attr_i_dlbfic_b22 = new adf::PLIO("dlbfic_b22", adf::plio_64_bits, "../data/dlbf_cin22.txt", 400);
adf::PLIO *attr_i_dlbfic_b23 = new adf::PLIO("dlbfic_b23", adf::plio_64_bits, "../data/dlbf_cin23.txt", 400);

//DLBF_2, Chain 3: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b30 = new adf::PLIO("dlbfic_b30", adf::plio_64_bits, "../data/dlbf_cin30.txt", 400);
adf::PLIO *attr_i_dlbfic_b31 = new adf::PLIO("dlbfic_b31", adf::plio_64_bits, "../data/dlbf_cin31.txt", 400);
adf::PLIO *attr_i_dlbfic_b32 = new adf::PLIO("dlbfic_b32", adf::plio_64_bits, "../data/dlbf_cin32.txt", 400);
adf::PLIO *attr_i_dlbfic_b33 = new adf::PLIO("dlbfic_b33", adf::plio_64_bits, "../data/dlbf_cin33.txt", 400);

//DLBF_2, Chain 4: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b40 = new adf::PLIO("dlbfic_b40", adf::plio_64_bits, "../data/dlbf_cin40.txt", 400);
adf::PLIO *attr_i_dlbfic_b41 = new adf::PLIO("dlbfic_b41", adf::plio_64_bits, "../data/dlbf_cin41.txt", 400);
adf::PLIO *attr_i_dlbfic_b42 = new adf::PLIO("dlbfic_b42", adf::plio_64_bits, "../data/dlbf_cin42.txt", 400);
adf::PLIO *attr_i_dlbfic_b43 = new adf::PLIO("dlbfic_b43", adf::plio_64_bits, "../data/dlbf_cin43.txt", 400);

//DLBF_2, Chain 5: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b50 = new adf::PLIO("dlbfic_b50", adf::plio_64_bits, "../data/dlbf_cin50.txt", 400);
adf::PLIO *attr_i_dlbfic_b51 = new adf::PLIO("dlbfic_b51", adf::plio_64_bits, "../data/dlbf_cin51.txt", 400);
adf::PLIO *attr_i_dlbfic_b52 = new adf::PLIO("dlbfic_b52", adf::plio_64_bits, "../data/dlbf_cin52.txt", 400);
adf::PLIO *attr_i_dlbfic_b53 = new adf::PLIO("dlbfic_b53", adf::plio_64_bits, "../data/dlbf_cin53.txt", 400);

//DLBF_2, Chain 6: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b60 = new adf::PLIO("dlbfic_b60", adf::plio_64_bits, "../data/dlbf_cin60.txt", 400);
adf::PLIO *attr_i_dlbfic_b61 = new adf::PLIO("dlbfic_b61", adf::plio_64_bits, "../data/dlbf_cin61.txt", 400);
adf::PLIO *attr_i_dlbfic_b62 = new adf::PLIO("dlbfic_b62", adf::plio_64_bits, "../data/dlbf_cin62.txt", 400);
adf::PLIO *attr_i_dlbfic_b63 = new adf::PLIO("dlbfic_b63", adf::plio_64_bits, "../data/dlbf_cin63.txt", 400);

//DLBF_2, Chain 7: 4 Data Input Coefficient Ports 
adf::PLIO *attr_i_dlbfic_b70 = new adf::PLIO("dlbfic_b70", adf::plio_64_bits, "../data/dlbf_cin70.txt", 400);
adf::PLIO *attr_i_dlbfic_b71 = new adf::PLIO("dlbfic_b71", adf::plio_64_bits, "../data/dlbf_cin71.txt", 400);
adf::PLIO *attr_i_dlbfic_b72 = new adf::PLIO("dlbfic_b72", adf::plio_64_bits, "../data/dlbf_cin72.txt", 400);
adf::PLIO *attr_i_dlbfic_b73 = new adf::PLIO("dlbfic_b73", adf::plio_64_bits, "../data/dlbf_cin73.txt", 400);

//DLBF_2, 8 Data Output Ports, for Chains 0-7 
adf::PLIO *attr_o_dlbfo_b0  = new adf::PLIO("dlbfo_b0",  adf::plio_64_bits, "data/dlbf_out16.txt", 400);
adf::PLIO *attr_o_dlbfo_b1  = new adf::PLIO("dlbfo_b1",  adf::plio_64_bits, "data/dlbf_out17.txt", 400);
adf::PLIO *attr_o_dlbfo_b2  = new adf::PLIO("dlbfo_b2",  adf::plio_64_bits, "data/dlbf_out18.txt", 400);
adf::PLIO *attr_o_dlbfo_b3  = new adf::PLIO("dlbfo_b3",  adf::plio_64_bits, "data/dlbf_out19.txt", 400);
adf::PLIO *attr_o_dlbfo_b4  = new adf::PLIO("dlbfo_b4",  adf::plio_64_bits, "data/dlbf_out20.txt", 400);
adf::PLIO *attr_o_dlbfo_b5  = new adf::PLIO("dlbfo_b5",  adf::plio_64_bits, "data/dlbf_out21.txt", 400);
adf::PLIO *attr_o_dlbfo_b6  = new adf::PLIO("dlbfo_b6",  adf::plio_64_bits, "data/dlbf_out22.txt", 400);
adf::PLIO *attr_o_dlbfo_b7  = new adf::PLIO("dlbfo_b7",  adf::plio_64_bits, "data/dlbf_out23.txt", 400);

//ULBF_0: 8 Data Input Ports 
adf::PLIO *attr_i_ulbfid0 = new adf::PLIO("ulbfid0", adf::plio_64_bits, "../data/ulbf_din0.txt", 400);
adf::PLIO *attr_i_ulbfid1 = new adf::PLIO("ulbfid1", adf::plio_64_bits, "../data/ulbf_din1.txt", 400);
adf::PLIO *attr_i_ulbfid2 = new adf::PLIO("ulbfid2", adf::plio_64_bits, "../data/ulbf_din2.txt", 400);
adf::PLIO *attr_i_ulbfid3 = new adf::PLIO("ulbfid3", adf::plio_64_bits, "../data/ulbf_din3.txt", 400);
adf::PLIO *attr_i_ulbfid4 = new adf::PLIO("ulbfid4", adf::plio_64_bits, "../data/ulbf_din4.txt", 400);
adf::PLIO *attr_i_ulbfid5 = new adf::PLIO("ulbfid5", adf::plio_64_bits, "../data/ulbf_din5.txt", 400);
adf::PLIO *attr_i_ulbfid6 = new adf::PLIO("ulbfid6", adf::plio_64_bits, "../data/ulbf_din6.txt", 400);
adf::PLIO *attr_i_ulbfid7 = new adf::PLIO("ulbfid7", adf::plio_64_bits, "../data/ulbf_din7.txt", 400);

//ULBF_0, Chain 0: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic00 = new adf::PLIO("ulbfic00", adf::plio_64_bits, "../data/ulbf_cin00.txt", 400);
adf::PLIO *attr_i_ulbfic01 = new adf::PLIO("ulbfic01", adf::plio_64_bits, "../data/ulbf_cin01.txt", 400);
adf::PLIO *attr_i_ulbfic02 = new adf::PLIO("ulbfic02", adf::plio_64_bits, "../data/ulbf_cin02.txt", 400);
adf::PLIO *attr_i_ulbfic03 = new adf::PLIO("ulbfic03", adf::plio_64_bits, "../data/ulbf_cin03.txt", 400);
adf::PLIO *attr_i_ulbfic04 = new adf::PLIO("ulbfic04", adf::plio_64_bits, "../data/ulbf_cin04.txt", 400);
adf::PLIO *attr_i_ulbfic05 = new adf::PLIO("ulbfic05", adf::plio_64_bits, "../data/ulbf_cin05.txt", 400);
adf::PLIO *attr_i_ulbfic06 = new adf::PLIO("ulbfic06", adf::plio_64_bits, "../data/ulbf_cin06.txt", 400);
adf::PLIO *attr_i_ulbfic07 = new adf::PLIO("ulbfic07", adf::plio_64_bits, "../data/ulbf_cin07.txt", 400);

//ULBF_0, Chain 1: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic10 = new adf::PLIO("ulbfic10", adf::plio_64_bits, "../data/ulbf_cin10.txt", 400);
adf::PLIO *attr_i_ulbfic11 = new adf::PLIO("ulbfic11", adf::plio_64_bits, "../data/ulbf_cin11.txt", 400);
adf::PLIO *attr_i_ulbfic12 = new adf::PLIO("ulbfic12", adf::plio_64_bits, "../data/ulbf_cin12.txt", 400);
adf::PLIO *attr_i_ulbfic13 = new adf::PLIO("ulbfic13", adf::plio_64_bits, "../data/ulbf_cin13.txt", 400);
adf::PLIO *attr_i_ulbfic14 = new adf::PLIO("ulbfic14", adf::plio_64_bits, "../data/ulbf_cin14.txt", 400);
adf::PLIO *attr_i_ulbfic15 = new adf::PLIO("ulbfic15", adf::plio_64_bits, "../data/ulbf_cin15.txt", 400);
adf::PLIO *attr_i_ulbfic16 = new adf::PLIO("ulbfic16", adf::plio_64_bits, "../data/ulbf_cin16.txt", 400);
adf::PLIO *attr_i_ulbfic17 = new adf::PLIO("ulbfic17", adf::plio_64_bits, "../data/ulbf_cin17.txt", 400);

//ULBF_0, Chain 2: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic20 = new adf::PLIO("ulbfic20", adf::plio_64_bits, "../data/ulbf_cin20.txt", 400);
adf::PLIO *attr_i_ulbfic21 = new adf::PLIO("ulbfic21", adf::plio_64_bits, "../data/ulbf_cin21.txt", 400);
adf::PLIO *attr_i_ulbfic22 = new adf::PLIO("ulbfic22", adf::plio_64_bits, "../data/ulbf_cin22.txt", 400);
adf::PLIO *attr_i_ulbfic23 = new adf::PLIO("ulbfic23", adf::plio_64_bits, "../data/ulbf_cin23.txt", 400);
adf::PLIO *attr_i_ulbfic24 = new adf::PLIO("ulbfic24", adf::plio_64_bits, "../data/ulbf_cin24.txt", 400);
adf::PLIO *attr_i_ulbfic25 = new adf::PLIO("ulbfic25", adf::plio_64_bits, "../data/ulbf_cin25.txt", 400);
adf::PLIO *attr_i_ulbfic26 = new adf::PLIO("ulbfic26", adf::plio_64_bits, "../data/ulbf_cin26.txt", 400);
adf::PLIO *attr_i_ulbfic27 = new adf::PLIO("ulbfic27", adf::plio_64_bits, "../data/ulbf_cin27.txt", 400);

//ULBF_0, Chain 3: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic30 = new adf::PLIO("ulbfic30", adf::plio_64_bits, "../data/ulbf_cin30.txt", 400);
adf::PLIO *attr_i_ulbfic31 = new adf::PLIO("ulbfic31", adf::plio_64_bits, "../data/ulbf_cin31.txt", 400);
adf::PLIO *attr_i_ulbfic32 = new adf::PLIO("ulbfic32", adf::plio_64_bits, "../data/ulbf_cin32.txt", 400);
adf::PLIO *attr_i_ulbfic33 = new adf::PLIO("ulbfic33", adf::plio_64_bits, "../data/ulbf_cin33.txt", 400);
adf::PLIO *attr_i_ulbfic34 = new adf::PLIO("ulbfic34", adf::plio_64_bits, "../data/ulbf_cin34.txt", 400);
adf::PLIO *attr_i_ulbfic35 = new adf::PLIO("ulbfic35", adf::plio_64_bits, "../data/ulbf_cin35.txt", 400);
adf::PLIO *attr_i_ulbfic36 = new adf::PLIO("ulbfic36", adf::plio_64_bits, "../data/ulbf_cin36.txt", 400);
adf::PLIO *attr_i_ulbfic37 = new adf::PLIO("ulbfic37", adf::plio_64_bits, "../data/ulbf_cin37.txt", 400);

//ULBF_0, 4 Data Output Ports, for Chains 0-3
adf::PLIO *attr_o_ulbfo0  = new adf::PLIO("ulbfo0",  adf::plio_64_bits, "data/ulbf_out0.txt", 400);
adf::PLIO *attr_o_ulbfo1  = new adf::PLIO("ulbfo1",  adf::plio_64_bits, "data/ulbf_out1.txt", 400);
adf::PLIO *attr_o_ulbfo2  = new adf::PLIO("ulbfo2",  adf::plio_64_bits, "data/ulbf_out2.txt", 400);
adf::PLIO *attr_o_ulbfo3  = new adf::PLIO("ulbfo3",  adf::plio_64_bits, "data/ulbf_out3.txt", 400);

//ULBF_1: 8 Data Input Ports 
adf::PLIO *attr_i_ulbfid_a0 = new adf::PLIO("ulbfid_a0", adf::plio_64_bits, "../data/ulbf_din0.txt", 400);
adf::PLIO *attr_i_ulbfid_a1 = new adf::PLIO("ulbfid_a1", adf::plio_64_bits, "../data/ulbf_din1.txt", 400);
adf::PLIO *attr_i_ulbfid_a2 = new adf::PLIO("ulbfid_a2", adf::plio_64_bits, "../data/ulbf_din2.txt", 400);
adf::PLIO *attr_i_ulbfid_a3 = new adf::PLIO("ulbfid_a3", adf::plio_64_bits, "../data/ulbf_din3.txt", 400);
adf::PLIO *attr_i_ulbfid_a4 = new adf::PLIO("ulbfid_a4", adf::plio_64_bits, "../data/ulbf_din4.txt", 400);
adf::PLIO *attr_i_ulbfid_a5 = new adf::PLIO("ulbfid_a5", adf::plio_64_bits, "../data/ulbf_din5.txt", 400);
adf::PLIO *attr_i_ulbfid_a6 = new adf::PLIO("ulbfid_a6", adf::plio_64_bits, "../data/ulbf_din6.txt", 400);
adf::PLIO *attr_i_ulbfid_a7 = new adf::PLIO("ulbfid_a7", adf::plio_64_bits, "../data/ulbf_din7.txt", 400);

//ULBF_1, Chain 0: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_a00 = new adf::PLIO("ulbfic_a00", adf::plio_64_bits, "../data/ulbf_cin00.txt", 400);
adf::PLIO *attr_i_ulbfic_a01 = new adf::PLIO("ulbfic_a01", adf::plio_64_bits, "../data/ulbf_cin01.txt", 400);
adf::PLIO *attr_i_ulbfic_a02 = new adf::PLIO("ulbfic_a02", adf::plio_64_bits, "../data/ulbf_cin02.txt", 400);
adf::PLIO *attr_i_ulbfic_a03 = new adf::PLIO("ulbfic_a03", adf::plio_64_bits, "../data/ulbf_cin03.txt", 400);
adf::PLIO *attr_i_ulbfic_a04 = new adf::PLIO("ulbfic_a04", adf::plio_64_bits, "../data/ulbf_cin04.txt", 400);
adf::PLIO *attr_i_ulbfic_a05 = new adf::PLIO("ulbfic_a05", adf::plio_64_bits, "../data/ulbf_cin05.txt", 400);
adf::PLIO *attr_i_ulbfic_a06 = new adf::PLIO("ulbfic_a06", adf::plio_64_bits, "../data/ulbf_cin06.txt", 400);
adf::PLIO *attr_i_ulbfic_a07 = new adf::PLIO("ulbfic_a07", adf::plio_64_bits, "../data/ulbf_cin07.txt", 400);

//ULBF_1, Chain 1: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_a10 = new adf::PLIO("ulbfic_a10", adf::plio_64_bits, "../data/ulbf_cin10.txt", 400);
adf::PLIO *attr_i_ulbfic_a11 = new adf::PLIO("ulbfic_a11", adf::plio_64_bits, "../data/ulbf_cin11.txt", 400);
adf::PLIO *attr_i_ulbfic_a12 = new adf::PLIO("ulbfic_a12", adf::plio_64_bits, "../data/ulbf_cin12.txt", 400);
adf::PLIO *attr_i_ulbfic_a13 = new adf::PLIO("ulbfic_a13", adf::plio_64_bits, "../data/ulbf_cin13.txt", 400);
adf::PLIO *attr_i_ulbfic_a14 = new adf::PLIO("ulbfic_a14", adf::plio_64_bits, "../data/ulbf_cin14.txt", 400);
adf::PLIO *attr_i_ulbfic_a15 = new adf::PLIO("ulbfic_a15", adf::plio_64_bits, "../data/ulbf_cin15.txt", 400);
adf::PLIO *attr_i_ulbfic_a16 = new adf::PLIO("ulbfic_a16", adf::plio_64_bits, "../data/ulbf_cin16.txt", 400);
adf::PLIO *attr_i_ulbfic_a17 = new adf::PLIO("ulbfic_a17", adf::plio_64_bits, "../data/ulbf_cin17.txt", 400);

//ULBF_1, Chain 2: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_a20 = new adf::PLIO("ulbfic_a20", adf::plio_64_bits, "../data/ulbf_cin20.txt", 400);
adf::PLIO *attr_i_ulbfic_a21 = new adf::PLIO("ulbfic_a21", adf::plio_64_bits, "../data/ulbf_cin21.txt", 400);
adf::PLIO *attr_i_ulbfic_a22 = new adf::PLIO("ulbfic_a22", adf::plio_64_bits, "../data/ulbf_cin22.txt", 400);
adf::PLIO *attr_i_ulbfic_a23 = new adf::PLIO("ulbfic_a23", adf::plio_64_bits, "../data/ulbf_cin23.txt", 400);
adf::PLIO *attr_i_ulbfic_a24 = new adf::PLIO("ulbfic_a24", adf::plio_64_bits, "../data/ulbf_cin24.txt", 400);
adf::PLIO *attr_i_ulbfic_a25 = new adf::PLIO("ulbfic_a25", adf::plio_64_bits, "../data/ulbf_cin25.txt", 400);
adf::PLIO *attr_i_ulbfic_a26 = new adf::PLIO("ulbfic_a26", adf::plio_64_bits, "../data/ulbf_cin26.txt", 400);
adf::PLIO *attr_i_ulbfic_a27 = new adf::PLIO("ulbfic_a27", adf::plio_64_bits, "../data/ulbf_cin27.txt", 400);

//ULBF_1, Chain 3: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_a30 = new adf::PLIO("ulbfic_a30", adf::plio_64_bits, "../data/ulbf_cin30.txt", 400);
adf::PLIO *attr_i_ulbfic_a31 = new adf::PLIO("ulbfic_a31", adf::plio_64_bits, "../data/ulbf_cin31.txt", 400);
adf::PLIO *attr_i_ulbfic_a32 = new adf::PLIO("ulbfic_a32", adf::plio_64_bits, "../data/ulbf_cin32.txt", 400);
adf::PLIO *attr_i_ulbfic_a33 = new adf::PLIO("ulbfic_a33", adf::plio_64_bits, "../data/ulbf_cin33.txt", 400);
adf::PLIO *attr_i_ulbfic_a34 = new adf::PLIO("ulbfic_a34", adf::plio_64_bits, "../data/ulbf_cin34.txt", 400);
adf::PLIO *attr_i_ulbfic_a35 = new adf::PLIO("ulbfic_a35", adf::plio_64_bits, "../data/ulbf_cin35.txt", 400);
adf::PLIO *attr_i_ulbfic_a36 = new adf::PLIO("ulbfic_a36", adf::plio_64_bits, "../data/ulbf_cin36.txt", 400);
adf::PLIO *attr_i_ulbfic_a37 = new adf::PLIO("ulbfic_a37", adf::plio_64_bits, "../data/ulbf_cin37.txt", 400);

//ULBF_1, 4 Data Output Ports, for Chains 0-3
adf::PLIO *attr_o_ulbfo_a0  = new adf::PLIO("ulbfo_a0",  adf::plio_64_bits, "data/ulbf_out4.txt", 400);
adf::PLIO *attr_o_ulbfo_a1  = new adf::PLIO("ulbfo_a1",  adf::plio_64_bits, "data/ulbf_out5.txt", 400);
adf::PLIO *attr_o_ulbfo_a2  = new adf::PLIO("ulbfo_a2",  adf::plio_64_bits, "data/ulbf_out6.txt", 400);
adf::PLIO *attr_o_ulbfo_a3  = new adf::PLIO("ulbfo_a3",  adf::plio_64_bits, "data/ulbf_out7.txt", 400);

//ULBF_2: 8 Data Input Ports 
adf::PLIO *attr_i_ulbfid_b0 = new adf::PLIO("ulbfid_b0", adf::plio_64_bits, "../data/ulbf_din0.txt", 400);
adf::PLIO *attr_i_ulbfid_b1 = new adf::PLIO("ulbfid_b1", adf::plio_64_bits, "../data/ulbf_din1.txt", 400);
adf::PLIO *attr_i_ulbfid_b2 = new adf::PLIO("ulbfid_b2", adf::plio_64_bits, "../data/ulbf_din2.txt", 400);
adf::PLIO *attr_i_ulbfid_b3 = new adf::PLIO("ulbfid_b3", adf::plio_64_bits, "../data/ulbf_din3.txt", 400);
adf::PLIO *attr_i_ulbfid_b4 = new adf::PLIO("ulbfid_b4", adf::plio_64_bits, "../data/ulbf_din4.txt", 400);
adf::PLIO *attr_i_ulbfid_b5 = new adf::PLIO("ulbfid_b5", adf::plio_64_bits, "../data/ulbf_din5.txt", 400);
adf::PLIO *attr_i_ulbfid_b6 = new adf::PLIO("ulbfid_b6", adf::plio_64_bits, "../data/ulbf_din6.txt", 400);
adf::PLIO *attr_i_ulbfid_b7 = new adf::PLIO("ulbfid_b7", adf::plio_64_bits, "../data/ulbf_din7.txt", 400);

//ULBF_2, Chain 0: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_b00 = new adf::PLIO("ulbfic_b00", adf::plio_64_bits, "../data/ulbf_cin00.txt", 400);
adf::PLIO *attr_i_ulbfic_b01 = new adf::PLIO("ulbfic_b01", adf::plio_64_bits, "../data/ulbf_cin01.txt", 400);
adf::PLIO *attr_i_ulbfic_b02 = new adf::PLIO("ulbfic_b02", adf::plio_64_bits, "../data/ulbf_cin02.txt", 400);
adf::PLIO *attr_i_ulbfic_b03 = new adf::PLIO("ulbfic_b03", adf::plio_64_bits, "../data/ulbf_cin03.txt", 400);
adf::PLIO *attr_i_ulbfic_b04 = new adf::PLIO("ulbfic_b04", adf::plio_64_bits, "../data/ulbf_cin04.txt", 400);
adf::PLIO *attr_i_ulbfic_b05 = new adf::PLIO("ulbfic_b05", adf::plio_64_bits, "../data/ulbf_cin05.txt", 400);
adf::PLIO *attr_i_ulbfic_b06 = new adf::PLIO("ulbfic_b06", adf::plio_64_bits, "../data/ulbf_cin06.txt", 400);
adf::PLIO *attr_i_ulbfic_b07 = new adf::PLIO("ulbfic_b07", adf::plio_64_bits, "../data/ulbf_cin07.txt", 400);

//ULBF_2, Chain 1: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_b10 = new adf::PLIO("ulbfic_b10", adf::plio_64_bits, "../data/ulbf_cin10.txt", 400);
adf::PLIO *attr_i_ulbfic_b11 = new adf::PLIO("ulbfic_b11", adf::plio_64_bits, "../data/ulbf_cin11.txt", 400);
adf::PLIO *attr_i_ulbfic_b12 = new adf::PLIO("ulbfic_b12", adf::plio_64_bits, "../data/ulbf_cin12.txt", 400);
adf::PLIO *attr_i_ulbfic_b13 = new adf::PLIO("ulbfic_b13", adf::plio_64_bits, "../data/ulbf_cin13.txt", 400);
adf::PLIO *attr_i_ulbfic_b14 = new adf::PLIO("ulbfic_b14", adf::plio_64_bits, "../data/ulbf_cin14.txt", 400);
adf::PLIO *attr_i_ulbfic_b15 = new adf::PLIO("ulbfic_b15", adf::plio_64_bits, "../data/ulbf_cin15.txt", 400);
adf::PLIO *attr_i_ulbfic_b16 = new adf::PLIO("ulbfic_b16", adf::plio_64_bits, "../data/ulbf_cin16.txt", 400);
adf::PLIO *attr_i_ulbfic_b17 = new adf::PLIO("ulbfic_b17", adf::plio_64_bits, "../data/ulbf_cin17.txt", 400);

//ULBF_2, Chain 2: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_b20 = new adf::PLIO("ulbfic_b20", adf::plio_64_bits, "../data/ulbf_cin20.txt", 400);
adf::PLIO *attr_i_ulbfic_b21 = new adf::PLIO("ulbfic_b21", adf::plio_64_bits, "../data/ulbf_cin21.txt", 400);
adf::PLIO *attr_i_ulbfic_b22 = new adf::PLIO("ulbfic_b22", adf::plio_64_bits, "../data/ulbf_cin22.txt", 400);
adf::PLIO *attr_i_ulbfic_b23 = new adf::PLIO("ulbfic_b23", adf::plio_64_bits, "../data/ulbf_cin23.txt", 400);
adf::PLIO *attr_i_ulbfic_b24 = new adf::PLIO("ulbfic_b24", adf::plio_64_bits, "../data/ulbf_cin24.txt", 400);
adf::PLIO *attr_i_ulbfic_b25 = new adf::PLIO("ulbfic_b25", adf::plio_64_bits, "../data/ulbf_cin25.txt", 400);
adf::PLIO *attr_i_ulbfic_b26 = new adf::PLIO("ulbfic_b26", adf::plio_64_bits, "../data/ulbf_cin26.txt", 400);
adf::PLIO *attr_i_ulbfic_b27 = new adf::PLIO("ulbfic_b27", adf::plio_64_bits, "../data/ulbf_cin27.txt", 400);

//ULBF_2, Chain 3: 8 Data Input Coefficient Ports 
adf::PLIO *attr_i_ulbfic_b30 = new adf::PLIO("ulbfic_b30", adf::plio_64_bits, "../data/ulbf_cin30.txt", 400);
adf::PLIO *attr_i_ulbfic_b31 = new adf::PLIO("ulbfic_b31", adf::plio_64_bits, "../data/ulbf_cin31.txt", 400);
adf::PLIO *attr_i_ulbfic_b32 = new adf::PLIO("ulbfic_b32", adf::plio_64_bits, "../data/ulbf_cin32.txt", 400);
adf::PLIO *attr_i_ulbfic_b33 = new adf::PLIO("ulbfic_b33", adf::plio_64_bits, "../data/ulbf_cin33.txt", 400);
adf::PLIO *attr_i_ulbfic_b34 = new adf::PLIO("ulbfic_b34", adf::plio_64_bits, "../data/ulbf_cin34.txt", 400);
adf::PLIO *attr_i_ulbfic_b35 = new adf::PLIO("ulbfic_b35", adf::plio_64_bits, "../data/ulbf_cin35.txt", 400);
adf::PLIO *attr_i_ulbfic_b36 = new adf::PLIO("ulbfic_b36", adf::plio_64_bits, "../data/ulbf_cin36.txt", 400);
adf::PLIO *attr_i_ulbfic_b37 = new adf::PLIO("ulbfic_b37", adf::plio_64_bits, "../data/ulbf_cin37.txt", 400);

//ULBF_2, 4 Data Output Ports, for Chains 0-3
adf::PLIO *attr_o_ulbfo_b0  = new adf::PLIO("ulbfo_b0",  adf::plio_64_bits, "data/ulbf_out8.txt", 400);
adf::PLIO *attr_o_ulbfo_b1  = new adf::PLIO("ulbfo_b1",  adf::plio_64_bits, "data/ulbf_out9.txt", 400);
adf::PLIO *attr_o_ulbfo_b2  = new adf::PLIO("ulbfo_b2",  adf::plio_64_bits, "data/ulbf_out10.txt", 400);
adf::PLIO *attr_o_ulbfo_b3  = new adf::PLIO("ulbfo_b3",  adf::plio_64_bits, "data/ulbf_out11.txt", 400);




adf::PLIO dlbf_id[NUM_DLBF_DATA*NUM_DLBF_PORTS] = {*attr_i_dlbfid0, *attr_i_dlbfid1,  *attr_i_dlbfid2, *attr_i_dlbfid3, *attr_i_dlbfid_a0,  *attr_i_dlbfid_a1,  *attr_i_dlbfid_a2, *attr_i_dlbfid_a3, 	*attr_i_dlbfid_b0, *attr_i_dlbfid_b1,  *attr_i_dlbfid_b2, *attr_i_dlbfid_b3};

adf::PLIO dlbf_ic[NUM_DLBF_COEFFS*NUM_DLBF_PORTS] = {*attr_i_dlbfic00,  *attr_i_dlbfic01, *attr_i_dlbfic02,  *attr_i_dlbfic03, *attr_i_dlbfic10,  *attr_i_dlbfic11, *attr_i_dlbfic12,  *attr_i_dlbfic13, *attr_i_dlbfic20,  *attr_i_dlbfic21, *attr_i_dlbfic22,  *attr_i_dlbfic23, *attr_i_dlbfic30,  *attr_i_dlbfic31, *attr_i_dlbfic32,  *attr_i_dlbfic33, *attr_i_dlbfic40,  *attr_i_dlbfic41, *attr_i_dlbfic42,  *attr_i_dlbfic43, *attr_i_dlbfic50,  *attr_i_dlbfic51, *attr_i_dlbfic52,  *attr_i_dlbfic53, *attr_i_dlbfic60,  *attr_i_dlbfic61, *attr_i_dlbfic62,  *attr_i_dlbfic63, *attr_i_dlbfic70,  *attr_i_dlbfic71, *attr_i_dlbfic72,  *attr_i_dlbfic73, *attr_i_dlbfic_a00,  *attr_i_dlbfic_a01, *attr_i_dlbfic_a02,  *attr_i_dlbfic_a03, *attr_i_dlbfic_a10,  *attr_i_dlbfic_a11, *attr_i_dlbfic_a12,  *attr_i_dlbfic_a13, *attr_i_dlbfic_a20,  *attr_i_dlbfic_a21, *attr_i_dlbfic_a22,  *attr_i_dlbfic_a23, *attr_i_dlbfic_a30,  *attr_i_dlbfic_a31, *attr_i_dlbfic_a32,  *attr_i_dlbfic_a33, *attr_i_dlbfic_a40,  *attr_i_dlbfic_a41, *attr_i_dlbfic_a42,  *attr_i_dlbfic_a43, *attr_i_dlbfic_a50,  *attr_i_dlbfic_a51, *attr_i_dlbfic_a52,  *attr_i_dlbfic_a53, *attr_i_dlbfic_a60,  *attr_i_dlbfic_a61, *attr_i_dlbfic_a62,  *attr_i_dlbfic_a63, *attr_i_dlbfic_a70,  *attr_i_dlbfic_a71, *attr_i_dlbfic_a72,  *attr_i_dlbfic_a73, *attr_i_dlbfic_b00,  *attr_i_dlbfic_b01, *attr_i_dlbfic_b02,  *attr_i_dlbfic_b03, *attr_i_dlbfic_b10,  *attr_i_dlbfic_b11, *attr_i_dlbfic_b12,  *attr_i_dlbfic_b13, *attr_i_dlbfic_b20,  *attr_i_dlbfic_b21, *attr_i_dlbfic_b22,  *attr_i_dlbfic_b23, *attr_i_dlbfic_b30,  *attr_i_dlbfic_b31, *attr_i_dlbfic_b32,  *attr_i_dlbfic_b33, *attr_i_dlbfic_b40,  *attr_i_dlbfic_b41, *attr_i_dlbfic_b42,  *attr_i_dlbfic_b43, *attr_i_dlbfic_b50,  *attr_i_dlbfic_b51, *attr_i_dlbfic_b52,  *attr_i_dlbfic_b53, *attr_i_dlbfic_b60,  *attr_i_dlbfic_b61, *attr_i_dlbfic_b62,  *attr_i_dlbfic_b63, *attr_i_dlbfic_b70,  *attr_i_dlbfic_b71, *attr_i_dlbfic_b72,  *attr_i_dlbfic_b73};


adf::PLIO dlbf_out[NUM_DLBF_SLAVES] = {*attr_o_dlbfo0,  *attr_o_dlbfo1, *attr_o_dlbfo2,  *attr_o_dlbfo3, *attr_o_dlbfo4,  *attr_o_dlbfo5, *attr_o_dlbfo6,  *attr_o_dlbfo7, 	*attr_o_dlbfo_a0,  *attr_o_dlbfo_a1, *attr_o_dlbfo_a2,  *attr_o_dlbfo_a3, *attr_o_dlbfo_a4,  *attr_o_dlbfo_a5, *attr_o_dlbfo_a6,  *attr_o_dlbfo_a7, *attr_o_dlbfo_b0,  *attr_o_dlbfo_b1, *attr_o_dlbfo_b2,  *attr_o_dlbfo_b3, *attr_o_dlbfo_b4,  *attr_o_dlbfo_b5, *attr_o_dlbfo_b6,  *attr_o_dlbfo_b7};

adf::PLIO ulbf_id[NUM_ULBF_DATA*NUM_ULBF_PORTS] = {*attr_i_ulbfid0, *attr_i_ulbfid1,  *attr_i_ulbfid2, *attr_i_ulbfid3, *attr_i_ulbfid4, *attr_i_ulbfid5,  *attr_i_ulbfid6, *attr_i_ulbfid7, *attr_i_ulbfid_a0, *attr_i_ulbfid_a1,  *attr_i_ulbfid_a2, *attr_i_ulbfid_a3, *attr_i_ulbfid_a4, *attr_i_ulbfid_a5,  *attr_i_ulbfid_a6, *attr_i_ulbfid_a7, *attr_i_ulbfid_b0, *attr_i_ulbfid_b1,  *attr_i_ulbfid_b2, *attr_i_ulbfid_b3, *attr_i_ulbfid_b4, *attr_i_ulbfid_b5,  *attr_i_ulbfid_b6, *attr_i_ulbfid_b7};

adf::PLIO ulbf_ic[NUM_ULBF_COEFFS*NUM_ULBF_PORTS] = {*attr_i_ulbfic00,  *attr_i_ulbfic01, *attr_i_ulbfic02,  *attr_i_ulbfic03, *attr_i_ulbfic04,  *attr_i_ulbfic05, *attr_i_ulbfic06,  *attr_i_ulbfic07, *attr_i_ulbfic10,  *attr_i_ulbfic11, *attr_i_ulbfic12,  *attr_i_ulbfic13, *attr_i_ulbfic14,  *attr_i_ulbfic15, *attr_i_ulbfic16,  *attr_i_ulbfic17, *attr_i_ulbfic20,  *attr_i_ulbfic21, *attr_i_ulbfic22,  *attr_i_ulbfic23, *attr_i_ulbfic24,  *attr_i_ulbfic25, *attr_i_ulbfic26,  *attr_i_ulbfic27, *attr_i_ulbfic30,  *attr_i_ulbfic31, *attr_i_ulbfic32,  *attr_i_ulbfic33, *attr_i_ulbfic34,  *attr_i_ulbfic35, *attr_i_ulbfic36,  *attr_i_ulbfic37, *attr_i_ulbfic_a00,  *attr_i_ulbfic_a01, *attr_i_ulbfic_a02,  *attr_i_ulbfic_a03, *attr_i_ulbfic_a04,  *attr_i_ulbfic_a05, *attr_i_ulbfic_a06,  *attr_i_ulbfic_a07, *attr_i_ulbfic_a10,  *attr_i_ulbfic_a11, *attr_i_ulbfic_a12,  *attr_i_ulbfic_a13, *attr_i_ulbfic_a14,  *attr_i_ulbfic_a15, *attr_i_ulbfic_a16,  *attr_i_ulbfic_a17, *attr_i_ulbfic_a20,  *attr_i_ulbfic_a21, *attr_i_ulbfic_a22,  *attr_i_ulbfic_a23, *attr_i_ulbfic_a24,  *attr_i_ulbfic_a25, *attr_i_ulbfic_a26,  *attr_i_ulbfic_a27, *attr_i_ulbfic_a30,  *attr_i_ulbfic_a31, *attr_i_ulbfic_a32,  *attr_i_ulbfic_a33, *attr_i_ulbfic_a34,  *attr_i_ulbfic_a35, *attr_i_ulbfic_a36,  *attr_i_ulbfic_a37, *attr_i_ulbfic_b00,  *attr_i_ulbfic_b01, *attr_i_ulbfic_b02,  *attr_i_ulbfic_b03, *attr_i_ulbfic_b04,  *attr_i_ulbfic_b05, *attr_i_ulbfic_b06,  *attr_i_ulbfic_b07, *attr_i_ulbfic_b10,  *attr_i_ulbfic_b11, *attr_i_ulbfic_b12,  *attr_i_ulbfic_b13, *attr_i_ulbfic_b14,  *attr_i_ulbfic_b15, *attr_i_ulbfic_b16,  *attr_i_ulbfic_b17, *attr_i_ulbfic_b20,  *attr_i_ulbfic_b21, *attr_i_ulbfic_b22,  *attr_i_ulbfic_b23, *attr_i_ulbfic_b24,  *attr_i_ulbfic_b25, *attr_i_ulbfic_b26,  *attr_i_ulbfic_b27, *attr_i_ulbfic_b30,  *attr_i_ulbfic_b31, *attr_i_ulbfic_b32,  *attr_i_ulbfic_b33, *attr_i_ulbfic_b34,  *attr_i_ulbfic_b35, *attr_i_ulbfic_b36,  *attr_i_ulbfic_b37};

adf::PLIO ulbf_out[NUM_ULBF_SLAVES] = {*attr_o_ulbfo0,  *attr_o_ulbfo1, *attr_o_ulbfo2,  *attr_o_ulbfo3, *attr_o_ulbfo_a0,  *attr_o_ulbfo_a1, *attr_o_ulbfo_a2,  *attr_o_ulbfo_a3, *attr_o_ulbfo_b0,  *attr_o_ulbfo_b1, *attr_o_ulbfo_b2,  *attr_o_ulbfo_b3};

int test_perf(std::string str, int samples, adf::PLIO inp, int d, long long throughput_exp){

    adf::event::handle handle_in_0;
	int flag = 0;
	long long throughput_min = 900000000; // initial value to some high number (>800000000) as 800MSps is expected throughput of inputs and outputs
        long long throughput_max = 0;
        int iter=0;
        while(iter < samples) {
            long long count_start_in_0, count_end_in_0;
            long long throughput;
            handle_in_0  = adf::event::start_profiling(inp,     adf::event::io_stream_running_event_count);
            count_start_in_0   = adf::event::read_profiling(handle_in_0);
            //precision of usleep is dependent on linux system call
            usleep(1000000); //1s
            count_end_in_0     = adf::event::read_profiling(handle_in_0);
            adf::event::stop_profiling(handle_in_0);
            if (count_end_in_0 > count_start_in_0)    throughput = (count_end_in_0-count_start_in_0);
            else                                      throughput = (count_end_in_0-count_start_in_0+0x100000000);	//roll over correction for 32b performance counter
            if (throughput<throughput_min)  throughput_min = throughput;
            if (throughput>throughput_max)  throughput_max = throughput;
            iter++;
        }
        std::cout << "[throughput] "<<str<<" " << std::dec << d << "   Min:" << throughput_min << "   Max:" << throughput_max << "    Range:" << throughput_max-throughput_min << std::endl;
		log_plnx << "[throughput] "<<str<<" " << std::dec << d << "   Min:" << throughput_min << "   Max:" << throughput_max << "    Range:" << throughput_max-throughput_min << std::endl;
		if ((throughput_max < throughput_exp)||((throughput_min == 900000000)&&((throughput_max-throughput_min) > 60000))) flag = 1;

	return flag;
}

int dlbf_perf(int samples, int flag_all, int mid, int mic, int sid){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    long long throughput_out_exp = 800000000;
    int flag = 0;

	if (!flag_all){
		test_perf("DLBF-data", samples, dlbf_id[mid], mid, 0);
		test_perf("DLBF-coeffs", samples, dlbf_ic[mic], mic, 0);
		flag = test_perf("DLBF-slaves", samples, dlbf_out[sid], sid, throughput_out_exp);
	} else {
		for(int d =0; d < NUM_DLBF_DATA; d++)
			test_perf("DLBF-data", samples, dlbf_id[d], d, 0);
		for(int d =0; d < NUM_DLBF_COEFFS; d++)
			test_perf("DLBF-coeffs", samples, dlbf_ic[d], d, 0);
		for(int d =0; d < NUM_DLBF_SLAVES; d++)
			flag = flag + test_perf("DLBF-slaves", samples, dlbf_out[d], d, throughput_out_exp);
	}

    if (flag>0){
        printf("[DLBF-PERF] PERFORMANCE TEST FAILED\n");
        log_plnx << "[DLBF-PERF] PERFORMANCE TEST FAILED" << std::endl;
    } else {
        printf("[DLBF-PERF] PERFORMANCE TEST PASSED\n");
        log_plnx << "DLBF-PERF] PERFORMANCE TEST PASSED" << std::endl;
    } 
    return !flag;
}

int ulbf_perf(int samples, int flag_all, int mid, int mic, int sid){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    long long throughput_out_exp = 800000000;
    int flag = 0;

	if (!flag_all){
		test_perf("ULBF-data", samples, ulbf_id[mid], mid, 0);
		test_perf("ULBF-coeffs", samples, ulbf_ic[mic], mic, 0);
		flag = test_perf("ULBF-slaves", samples, ulbf_out[sid], sid, throughput_out_exp);
	} else {
		for(int d =0; d < NUM_ULBF_DATA; d++)
			test_perf("ULBF-data", samples, ulbf_id[d], d, 0);
		for(int d =0; d < NUM_ULBF_COEFFS; d++)
			test_perf("ULBF-coeffs", samples, ulbf_ic[d], d, 0);
		for(int d =0; d < NUM_ULBF_SLAVES; d++)
			flag = flag + test_perf("ULBF-slaves", samples, ulbf_out[d], d, throughput_out_exp);
	}

    if (flag>0){
        printf("[ULBF-PERF] PERFORMANCE TEST FAILED\n");
        log_plnx << "[ULBF-PERF] PERFORMANCE TEST FAILED" << std::endl;
    } else {
        printf("[ULBF-PERF] PERFORMANCE TEST PASSED\n");
        log_plnx << "[ULBF-PERF] PERFORMANCE TEST PASSED" << std::endl;
    } 
    return !flag;
}


void enable_graphs(int niter, int debug){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    printf("[Beamformer-PERF] Restarting masters and slaves for Performace Test\n");
    log_plnx << "[Beamformer-PERF] Restarting masters and slaves for Performace Test"  << std::endl ;

    dlbf_reset(debug);    ulbf_reset(debug);
    dlbf_data_config_ips(niter, debug);    dlbf_coeffs_config_ips(niter, debug);    dlbf_slave_config_ips(niter, debug);
    ulbf_data_config_ips(niter, debug);    ulbf_coeffs_config_ips(niter, debug);    ulbf_slave_config_ips(niter, debug);
    dlbf_start(debug);    ulbf_start(debug);

}

void test_perf_all(int flag_all, int samples){

    int dlbf_result = 1;
    int ulbf_result = 1;

    int dlbf_mid = rand()% NUM_DLBF_DATA;
    int dlbf_mic = rand()% NUM_DLBF_COEFFS;
    int dlbf_sid = rand()% NUM_DLBF_SLAVES;

    //DLBF Performance test
    dlbf_result = dlbf_perf(samples, flag_all, dlbf_mid, dlbf_mic, dlbf_sid);
    printf("[DLBF-PERF] DLBF Performance measurements Done ... \n");
    log_plnx << "[DLBF-PERF] DLBF Performance measurements Done ... " << std::endl ;

    int ulbf_mid = rand()% NUM_ULBF_DATA;
    int ulbf_mic = rand()% NUM_ULBF_COEFFS;
    int ulbf_sid = rand()% NUM_ULBF_SLAVES;

    //ULBF Performance test
    ulbf_result = ulbf_perf(samples, flag_all, ulbf_mid, ulbf_mic, ulbf_sid);
    printf("[ULBF-PERF] ULBF Performance measurements Done ... \n");
    log_plnx << "[ULBF-PERF] ULBF Performance measurements Done ... " << std::endl ;


    if ((dlbf_result==0)||(ulbf_result==0)){
        printf("[Beamformer-PERF] - ***** TEST FAILED ***** \n");
        log_plnx << "[Beamformer-PERF] - ***** TEST FAILED ***** " << std::endl;
    } else {
        printf("[Beamformer-PERF] - ***** TEST PASSED ***** \n");
        log_plnx << "[Beamformer-PERF] - ***** TEST PASSED ***** " << std::endl;
    }
}

void stop_run(){

    int debug = 0;
    int niter = 1;

    dlbf_data_config_ips(niter, debug);
    dlbf_coeffs_config_ips(niter, debug);
    dlbf_stop(debug);
    ulbf_data_config_ips(niter, debug);
	ulbf_coeffs_config_ips(niter, debug);
    ulbf_stop(debug);

}


