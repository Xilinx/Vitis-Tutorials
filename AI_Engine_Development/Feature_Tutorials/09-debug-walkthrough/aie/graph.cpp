/*
 * Copyright 2020 Xilinx, Inc.
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
 */

#include "graph.h"

#define COL_OFFSET 6

#define UL_INPUT_BA  (4+32)
#define UL_OUTPUT_BA (8)


PLIO *attr_i_dlbfid0 = new PLIO("dlbfid0", plio_64_bits, "data/dlbf_din0.txt");
PLIO *attr_i_dlbfid1 = new PLIO("dlbfid1", plio_64_bits, "data/dlbf_din1.txt");
PLIO *attr_i_dlbfid2 = new PLIO("dlbfid2", plio_64_bits, "data/dlbf_din2.txt");
PLIO *attr_i_dlbfid3 = new PLIO("dlbfid3", plio_64_bits, "data/dlbf_din3.txt");

PLIO *attr_i_dlbfic00 = new PLIO("dlbfic00", plio_64_bits, "data/dlbf_cin00.txt");
PLIO *attr_i_dlbfic01 = new PLIO("dlbfic01", plio_64_bits, "data/dlbf_cin01.txt");
PLIO *attr_i_dlbfic02 = new PLIO("dlbfic02", plio_64_bits, "data/dlbf_cin02.txt");
PLIO *attr_i_dlbfic03 = new PLIO("dlbfic03", plio_64_bits, "data/dlbf_cin03.txt");
PLIO *attr_i_dlbfic10 = new PLIO("dlbfic10", plio_64_bits, "data/dlbf_cin10.txt");
PLIO *attr_i_dlbfic11 = new PLIO("dlbfic11", plio_64_bits, "data/dlbf_cin11.txt");
PLIO *attr_i_dlbfic12 = new PLIO("dlbfic12", plio_64_bits, "data/dlbf_cin12.txt");
PLIO *attr_i_dlbfic13 = new PLIO("dlbfic13", plio_64_bits, "data/dlbf_cin13.txt");
PLIO *attr_i_dlbfic20 = new PLIO("dlbfic20", plio_64_bits, "data/dlbf_cin20.txt");
PLIO *attr_i_dlbfic21 = new PLIO("dlbfic21", plio_64_bits, "data/dlbf_cin21.txt");
PLIO *attr_i_dlbfic22 = new PLIO("dlbfic22", plio_64_bits, "data/dlbf_cin22.txt");
PLIO *attr_i_dlbfic23 = new PLIO("dlbfic23", plio_64_bits, "data/dlbf_cin23.txt");
PLIO *attr_i_dlbfic30 = new PLIO("dlbfic30", plio_64_bits, "data/dlbf_cin30.txt");
PLIO *attr_i_dlbfic31 = new PLIO("dlbfic31", plio_64_bits, "data/dlbf_cin31.txt");
PLIO *attr_i_dlbfic32 = new PLIO("dlbfic32", plio_64_bits, "data/dlbf_cin32.txt");
PLIO *attr_i_dlbfic33 = new PLIO("dlbfic33", plio_64_bits, "data/dlbf_cin33.txt");
PLIO *attr_i_dlbfic40 = new PLIO("dlbfic40", plio_64_bits, "data/dlbf_cin40.txt");
PLIO *attr_i_dlbfic41 = new PLIO("dlbfic41", plio_64_bits, "data/dlbf_cin41.txt");
PLIO *attr_i_dlbfic42 = new PLIO("dlbfic42", plio_64_bits, "data/dlbf_cin42.txt");
PLIO *attr_i_dlbfic43 = new PLIO("dlbfic43", plio_64_bits, "data/dlbf_cin43.txt");
PLIO *attr_i_dlbfic50 = new PLIO("dlbfic50", plio_64_bits, "data/dlbf_cin50.txt");
PLIO *attr_i_dlbfic51 = new PLIO("dlbfic51", plio_64_bits, "data/dlbf_cin51.txt");
PLIO *attr_i_dlbfic52 = new PLIO("dlbfic52", plio_64_bits, "data/dlbf_cin52.txt");
PLIO *attr_i_dlbfic53 = new PLIO("dlbfic53", plio_64_bits, "data/dlbf_cin53.txt");
PLIO *attr_i_dlbfic60 = new PLIO("dlbfic60", plio_64_bits, "data/dlbf_cin60.txt");
PLIO *attr_i_dlbfic61 = new PLIO("dlbfic61", plio_64_bits, "data/dlbf_cin61.txt");
PLIO *attr_i_dlbfic62 = new PLIO("dlbfic62", plio_64_bits, "data/dlbf_cin62.txt");
PLIO *attr_i_dlbfic63 = new PLIO("dlbfic63", plio_64_bits, "data/dlbf_cin63.txt");
PLIO *attr_i_dlbfic70 = new PLIO("dlbfic70", plio_64_bits, "data/dlbf_cin70.txt");
PLIO *attr_i_dlbfic71 = new PLIO("dlbfic71", plio_64_bits, "data/dlbf_cin71.txt");
PLIO *attr_i_dlbfic72 = new PLIO("dlbfic72", plio_64_bits, "data/dlbf_cin72.txt");
PLIO *attr_i_dlbfic73 = new PLIO("dlbfic73", plio_64_bits, "data/dlbf_cin73.txt");
PLIO *attr_o_dlbfo0  = new PLIO("dlbfo0",  plio_64_bits, "data/dlbf_out0.txt");
PLIO *attr_o_dlbfo1  = new PLIO("dlbfo1",  plio_64_bits, "data/dlbf_out1.txt");
PLIO *attr_o_dlbfo2  = new PLIO("dlbfo2",  plio_64_bits, "data/dlbf_out2.txt");
PLIO *attr_o_dlbfo3  = new PLIO("dlbfo3",  plio_64_bits, "data/dlbf_out3.txt");
PLIO *attr_o_dlbfo4  = new PLIO("dlbfo4",  plio_64_bits, "data/dlbf_out4.txt");
PLIO *attr_o_dlbfo5  = new PLIO("dlbfo5",  plio_64_bits, "data/dlbf_out5.txt");
PLIO *attr_o_dlbfo6  = new PLIO("dlbfo6",  plio_64_bits, "data/dlbf_out6.txt");
PLIO *attr_o_dlbfo7  = new PLIO("dlbfo7",  plio_64_bits, "data/dlbf_out7.txt");


PLIO *attr_i_ulbfid0 = new PLIO("ulbfid0", plio_64_bits, "data/ulbf_din0.txt");
PLIO *attr_i_ulbfid1 = new PLIO("ulbfid1", plio_64_bits, "data/ulbf_din1.txt");
PLIO *attr_i_ulbfid2 = new PLIO("ulbfid2", plio_64_bits, "data/ulbf_din2.txt");
PLIO *attr_i_ulbfid3 = new PLIO("ulbfid3", plio_64_bits, "data/ulbf_din3.txt");
PLIO *attr_i_ulbfid4 = new PLIO("ulbfid4", plio_64_bits, "data/ulbf_din4.txt");
PLIO *attr_i_ulbfid5 = new PLIO("ulbfid5", plio_64_bits, "data/ulbf_din5.txt");
PLIO *attr_i_ulbfid6 = new PLIO("ulbfid6", plio_64_bits, "data/ulbf_din6.txt");
PLIO *attr_i_ulbfid7 = new PLIO("ulbfid7", plio_64_bits, "data/ulbf_din7.txt");

PLIO *attr_i_ulbfic00 = new PLIO("ulbfic00", plio_64_bits, "data/ulbf_cin00.txt");
PLIO *attr_i_ulbfic01 = new PLIO("ulbfic01", plio_64_bits, "data/ulbf_cin01.txt");
PLIO *attr_i_ulbfic02 = new PLIO("ulbfic02", plio_64_bits, "data/ulbf_cin02.txt");
PLIO *attr_i_ulbfic03 = new PLIO("ulbfic03", plio_64_bits, "data/ulbf_cin03.txt");
PLIO *attr_i_ulbfic04 = new PLIO("ulbfic04", plio_64_bits, "data/ulbf_cin04.txt");
PLIO *attr_i_ulbfic05 = new PLIO("ulbfic05", plio_64_bits, "data/ulbf_cin05.txt");
PLIO *attr_i_ulbfic06 = new PLIO("ulbfic06", plio_64_bits, "data/ulbf_cin06.txt");
PLIO *attr_i_ulbfic07 = new PLIO("ulbfic07", plio_64_bits, "data/ulbf_cin07.txt");
PLIO *attr_i_ulbfic10 = new PLIO("ulbfic10", plio_64_bits, "data/ulbf_cin10.txt");
PLIO *attr_i_ulbfic11 = new PLIO("ulbfic11", plio_64_bits, "data/ulbf_cin11.txt");
PLIO *attr_i_ulbfic12 = new PLIO("ulbfic12", plio_64_bits, "data/ulbf_cin12.txt");
PLIO *attr_i_ulbfic13 = new PLIO("ulbfic13", plio_64_bits, "data/ulbf_cin13.txt");
PLIO *attr_i_ulbfic14 = new PLIO("ulbfic14", plio_64_bits, "data/ulbf_cin14.txt");
PLIO *attr_i_ulbfic15 = new PLIO("ulbfic15", plio_64_bits, "data/ulbf_cin15.txt");
PLIO *attr_i_ulbfic16 = new PLIO("ulbfic16", plio_64_bits, "data/ulbf_cin16.txt");
PLIO *attr_i_ulbfic17 = new PLIO("ulbfic17", plio_64_bits, "data/ulbf_cin17.txt");
PLIO *attr_i_ulbfic20 = new PLIO("ulbfic20", plio_64_bits, "data/ulbf_cin20.txt");
PLIO *attr_i_ulbfic21 = new PLIO("ulbfic21", plio_64_bits, "data/ulbf_cin21.txt");
PLIO *attr_i_ulbfic22 = new PLIO("ulbfic22", plio_64_bits, "data/ulbf_cin22.txt");
PLIO *attr_i_ulbfic23 = new PLIO("ulbfic23", plio_64_bits, "data/ulbf_cin23.txt");
PLIO *attr_i_ulbfic24 = new PLIO("ulbfic24", plio_64_bits, "data/ulbf_cin24.txt");
PLIO *attr_i_ulbfic25 = new PLIO("ulbfic25", plio_64_bits, "data/ulbf_cin25.txt");
PLIO *attr_i_ulbfic26 = new PLIO("ulbfic26", plio_64_bits, "data/ulbf_cin26.txt");
PLIO *attr_i_ulbfic27 = new PLIO("ulbfic27", plio_64_bits, "data/ulbf_cin27.txt");
PLIO *attr_i_ulbfic30 = new PLIO("ulbfic30", plio_64_bits, "data/ulbf_cin30.txt");
PLIO *attr_i_ulbfic31 = new PLIO("ulbfic31", plio_64_bits, "data/ulbf_cin31.txt");
PLIO *attr_i_ulbfic32 = new PLIO("ulbfic32", plio_64_bits, "data/ulbf_cin32.txt");
PLIO *attr_i_ulbfic33 = new PLIO("ulbfic33", plio_64_bits, "data/ulbf_cin33.txt");
PLIO *attr_i_ulbfic34 = new PLIO("ulbfic34", plio_64_bits, "data/ulbf_cin34.txt");
PLIO *attr_i_ulbfic35 = new PLIO("ulbfic35", plio_64_bits, "data/ulbf_cin35.txt");
PLIO *attr_i_ulbfic36 = new PLIO("ulbfic36", plio_64_bits, "data/ulbf_cin36.txt");
PLIO *attr_i_ulbfic37 = new PLIO("ulbfic37", plio_64_bits, "data/ulbf_cin37.txt");

PLIO *attr_o_ulbfo0  = new PLIO("ulbfo0",  plio_64_bits, "data/ulbf_out0.txt");
PLIO *attr_o_ulbfo1  = new PLIO("ulbfo1",  plio_64_bits, "data/ulbf_out1.txt");
PLIO *attr_o_ulbfo2  = new PLIO("ulbfo2",  plio_64_bits, "data/ulbf_out2.txt");
PLIO *attr_o_ulbfo3  = new PLIO("ulbfo3",  plio_64_bits, "data/ulbf_out3.txt");


simulation::platform<4+32+8+32, 8+4> platform(
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
	attr_o_dlbfo0,    attr_o_dlbfo1,   attr_o_dlbfo2,    attr_o_dlbfo3,
	attr_o_dlbfo4,    attr_o_dlbfo5,   attr_o_dlbfo6,    attr_o_dlbfo7,
	attr_o_ulbfo0,    attr_o_ulbfo1,   attr_o_ulbfo2,    attr_o_ulbfo3
);

TEST_BF<COL_OFFSET> dut;


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

#if defined  (__AIESIM__) || defined(__NEW_X86Sim__)
int main(void) {
    dut.init();
    dut.run(100) ;
    dut.end() ;
    return 0 ;
}
#endif
