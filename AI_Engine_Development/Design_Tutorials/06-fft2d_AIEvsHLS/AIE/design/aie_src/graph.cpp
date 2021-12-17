// © Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "graph.h"

FFTrows_graph fft_rows[FFT2D_INSTS];
FFTcols_graph fft_cols[FFT2D_INSTS];

#if FFT2D_INSTS == 1

   PLIO *attr_i0 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i1 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   
   PLIO *attr_o0 = new PLIO("DataOut0", plio_128_bits, "data/output0.txt");
   PLIO *attr_o1 = new PLIO("DataOut1", plio_128_bits, "data/output1.txt");
   
   simulation::platform<2, 2> plat( attr_i0,
   											attr_i1,
   
   											attr_o0,
   											attr_o1
   									    );
   
   connect<> neti0(plat.src[0], fft_rows[0].in);
   connect<> neti1(plat.src[1], fft_cols[0].in);
   
   connect<> neto0(fft_rows[0].out, plat.sink[0]);
   connect<> neto1(fft_cols[0].out, plat.sink[1]);

#elif FFT2D_INSTS == 5

   PLIO *attr_i0 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i1 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i2 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i3 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i4 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i5 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i6 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i7 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i8 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i9 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   
   PLIO *attr_o0 = new PLIO("DataOut0", plio_128_bits, "data/output0.txt");
   PLIO *attr_o1 = new PLIO("DataOut1", plio_128_bits, "data/output1.txt");
   PLIO *attr_o2 = new PLIO("DataOut2", plio_128_bits, "data/output2.txt");
   PLIO *attr_o3 = new PLIO("DataOut3", plio_128_bits, "data/output3.txt");
   PLIO *attr_o4 = new PLIO("DataOut4", plio_128_bits, "data/output4.txt");
   PLIO *attr_o5 = new PLIO("DataOut5", plio_128_bits, "data/output5.txt");
   PLIO *attr_o6 = new PLIO("DataOut6", plio_128_bits, "data/output6.txt");
   PLIO *attr_o7 = new PLIO("DataOut7", plio_128_bits, "data/output7.txt");
   PLIO *attr_o8 = new PLIO("DataOut8", plio_128_bits, "data/output8.txt");
   PLIO *attr_o9 = new PLIO("DataOut9", plio_128_bits, "data/output9.txt");
   
   simulation::platform<10, 10> plat( attr_i0,
   											  attr_i1,
   											  attr_i2,
   											  attr_i3,
   											  attr_i4,
   											  attr_i5,
   											  attr_i6,
   											  attr_i7,
   											  attr_i8,
   											  attr_i9,
   
   											  attr_o0,
   											  attr_o1,
   											  attr_o2,
   											  attr_o3,
   											  attr_o4,
   											  attr_o5,
   											  attr_o6,
   											  attr_o7,
   											  attr_o8,
   											  attr_o9
   											);
   
   connect<> neti0(plat.src[0], fft_rows[0].in);
   connect<> neti1(plat.src[1], fft_cols[0].in);
   connect<> neti2(plat.src[2], fft_rows[1].in);
   connect<> neti3(plat.src[3], fft_cols[1].in);
   connect<> neti4(plat.src[4], fft_rows[2].in);
   connect<> neti5(plat.src[5], fft_cols[2].in);
   connect<> neti6(plat.src[6], fft_rows[3].in);
   connect<> neti7(plat.src[7], fft_cols[3].in);
   connect<> neti8(plat.src[8], fft_rows[4].in);
   connect<> neti9(plat.src[9], fft_cols[4].in);
   
   connect<> neto0(fft_rows[0].out, plat.sink[0]);
   connect<> neto1(fft_cols[0].out, plat.sink[1]);
   connect<> neto2(fft_rows[1].out, plat.sink[2]);
   connect<> neto3(fft_cols[1].out, plat.sink[3]);
   connect<> neto4(fft_rows[2].out, plat.sink[4]);
   connect<> neto5(fft_cols[2].out, plat.sink[5]);
   connect<> neto6(fft_rows[3].out, plat.sink[6]);
   connect<> neto7(fft_cols[3].out, plat.sink[7]);
   connect<> neto8(fft_rows[4].out, plat.sink[8]);
   connect<> neto9(fft_cols[4].out, plat.sink[9]);

#elif FFT2D_INSTS == 10

   PLIO *attr_i0 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i1 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i2 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i3 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i4 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i5 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i6 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i7 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i8 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i9 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   
   PLIO *attr_o0 = new PLIO("DataOut0", plio_128_bits, "data/output0.txt");
   PLIO *attr_o1 = new PLIO("DataOut1", plio_128_bits, "data/output1.txt");
   PLIO *attr_o2 = new PLIO("DataOut2", plio_128_bits, "data/output2.txt");
   PLIO *attr_o3 = new PLIO("DataOut3", plio_128_bits, "data/output3.txt");
   PLIO *attr_o4 = new PLIO("DataOut4", plio_128_bits, "data/output4.txt");
   PLIO *attr_o5 = new PLIO("DataOut5", plio_128_bits, "data/output5.txt");
   PLIO *attr_o6 = new PLIO("DataOut6", plio_128_bits, "data/output6.txt");
   PLIO *attr_o7 = new PLIO("DataOut7", plio_128_bits, "data/output7.txt");
   PLIO *attr_o8 = new PLIO("DataOut8", plio_128_bits, "data/output8.txt");
   PLIO *attr_o9 = new PLIO("DataOut9", plio_128_bits, "data/output9.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   
   simulation::platform<20, 20> plat( attr_i0,
   											  attr_i1,
   											  attr_i2,
   											  attr_i3,
   											  attr_i4,
   											  attr_i5,
   											  attr_i6,
   											  attr_i7,
   											  attr_i8,
   											  attr_i9,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   
   											  attr_o0,
   											  attr_o1,
   											  attr_o2,
   											  attr_o3,
   											  attr_o4,
   											  attr_o5,
   											  attr_o6,
   											  attr_o7,
   											  attr_o8,
   											  attr_o9,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19
   											);
   
   connect<> neti00(plat.src[ 0], fft_rows[0].in);
   connect<> neti01(plat.src[ 1], fft_cols[0].in);
   connect<> neti02(plat.src[ 2], fft_rows[1].in);
   connect<> neti03(plat.src[ 3], fft_cols[1].in);
   connect<> neti04(plat.src[ 4], fft_rows[2].in);
   connect<> neti05(plat.src[ 5], fft_cols[2].in);
   connect<> neti06(plat.src[ 6], fft_rows[3].in);
   connect<> neti07(plat.src[ 7], fft_cols[3].in);
   connect<> neti08(plat.src[ 8], fft_rows[4].in);
   connect<> neti09(plat.src[ 9], fft_cols[4].in);
   connect<> neti10(plat.src[10], fft_rows[5].in);
   connect<> neti11(plat.src[11], fft_cols[5].in);
   connect<> neti12(plat.src[12], fft_rows[6].in);
   connect<> neti13(plat.src[13], fft_cols[6].in);
   connect<> neti14(plat.src[14], fft_rows[7].in);
   connect<> neti15(plat.src[15], fft_cols[7].in);
   connect<> neti16(plat.src[16], fft_rows[8].in);
   connect<> neti17(plat.src[17], fft_cols[8].in);
   connect<> neti18(plat.src[18], fft_rows[9].in);
   connect<> neti19(plat.src[19], fft_cols[9].in);
   
   connect<> neto00(fft_rows[0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[5].out, plat.sink[10]);
   connect<> neto11(fft_cols[5].out, plat.sink[11]);
   connect<> neto12(fft_rows[6].out, plat.sink[12]);
   connect<> neto13(fft_cols[6].out, plat.sink[13]);
   connect<> neto14(fft_rows[7].out, plat.sink[14]);
   connect<> neto15(fft_cols[7].out, plat.sink[15]);
   connect<> neto16(fft_rows[8].out, plat.sink[16]);
   connect<> neto17(fft_cols[8].out, plat.sink[17]);
   connect<> neto18(fft_rows[9].out, plat.sink[18]);
   connect<> neto19(fft_cols[9].out, plat.sink[19]);

#elif FFT2D_INSTS == 15

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   
   simulation::platform<30, 30> plat( attr_i00,
   											  attr_i01,
   											  attr_i02,
   											  attr_i03,
   											  attr_i04,
   											  attr_i05,
   											  attr_i06,
   											  attr_i07,
   											  attr_i08,
   											  attr_i09,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   											  attr_i20,
   											  attr_i21,
   											  attr_i22,
   											  attr_i23,
   											  attr_i24,
   											  attr_i25,
   											  attr_i26,
   											  attr_i27,
   											  attr_i28,
   											  attr_i29,
                                      
   											  attr_o00,
   											  attr_o01,
   											  attr_o02,
   											  attr_o03,
   											  attr_o04,
   											  attr_o05,
   											  attr_o06,
   											  attr_o07,
   											  attr_o08,
   											  attr_o09,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19,
   											  attr_o20,
   											  attr_o21,
   											  attr_o22,
   											  attr_o23,
   											  attr_o24,
   											  attr_o25,
   											  attr_o26,
   											  attr_o27,
   											  attr_o28,
   											  attr_o29
   											 );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);

#elif FFT2D_INSTS == 20

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i30 = new PLIO("DataIn30", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i31 = new PLIO("DataIn31", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i32 = new PLIO("DataIn32", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i33 = new PLIO("DataIn33", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i34 = new PLIO("DataIn34", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i35 = new PLIO("DataIn35", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i36 = new PLIO("DataIn36", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i37 = new PLIO("DataIn37", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i38 = new PLIO("DataIn38", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i39 = new PLIO("DataIn39", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   PLIO *attr_o30 = new PLIO("DataOut30", plio_128_bits, "data/output30.txt");
   PLIO *attr_o31 = new PLIO("DataOut31", plio_128_bits, "data/output31.txt");
   PLIO *attr_o32 = new PLIO("DataOut32", plio_128_bits, "data/output32.txt");
   PLIO *attr_o33 = new PLIO("DataOut33", plio_128_bits, "data/output33.txt");
   PLIO *attr_o34 = new PLIO("DataOut34", plio_128_bits, "data/output34.txt");
   PLIO *attr_o35 = new PLIO("DataOut35", plio_128_bits, "data/output35.txt");
   PLIO *attr_o36 = new PLIO("DataOut36", plio_128_bits, "data/output36.txt");
   PLIO *attr_o37 = new PLIO("DataOut37", plio_128_bits, "data/output37.txt");
   PLIO *attr_o38 = new PLIO("DataOut38", plio_128_bits, "data/output38.txt");
   PLIO *attr_o39 = new PLIO("DataOut39", plio_128_bits, "data/output39.txt");
   
   simulation::platform<40, 40> plat( attr_i00,
   											  attr_i01,
   											  attr_i02,
   											  attr_i03,
   											  attr_i04,
   											  attr_i05,
   											  attr_i06,
   											  attr_i07,
   											  attr_i08,
   											  attr_i09,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   											  attr_i20,
   											  attr_i21,
   											  attr_i22,
   											  attr_i23,
   											  attr_i24,
   											  attr_i25,
   											  attr_i26,
   											  attr_i27,
   											  attr_i28,
   											  attr_i29,
   											  attr_i30,
   											  attr_i31,
   											  attr_i32,
   											  attr_i33,
   											  attr_i34,
   											  attr_i35,
   											  attr_i36,
   											  attr_i37,
   											  attr_i38,
   											  attr_i39,
                                      
   											  attr_o00,
   											  attr_o01,
   											  attr_o02,
   											  attr_o03,
   											  attr_o04,
   											  attr_o05,
   											  attr_o06,
   											  attr_o07,
   											  attr_o08,
   											  attr_o09,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19,
   											  attr_o20,
   											  attr_o21,
   											  attr_o22,
   											  attr_o23,
   											  attr_o24,
   											  attr_o25,
   											  attr_o26,
   											  attr_o27,
   											  attr_o28,
   											  attr_o29,
   											  attr_o30,
   											  attr_o31,
   											  attr_o32,
   											  attr_o33,
   											  attr_o34,
   											  attr_o35,
   											  attr_o36,
   											  attr_o37,
   											  attr_o38,
   											  attr_o39
   											 );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   connect<> neti30(plat.src[30], fft_rows[15].in);
   connect<> neti31(plat.src[31], fft_cols[15].in);
   connect<> neti32(plat.src[32], fft_rows[16].in);
   connect<> neti33(plat.src[33], fft_cols[16].in);
   connect<> neti34(plat.src[34], fft_rows[17].in);
   connect<> neti35(plat.src[35], fft_cols[17].in);
   connect<> neti36(plat.src[36], fft_rows[18].in);
   connect<> neti37(plat.src[37], fft_cols[18].in);
   connect<> neti38(plat.src[38], fft_rows[19].in);
   connect<> neti39(plat.src[39], fft_cols[19].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);
   connect<> neto30(fft_rows[15].out, plat.sink[30]);
   connect<> neto31(fft_cols[15].out, plat.sink[31]);
   connect<> neto32(fft_rows[16].out, plat.sink[32]);
   connect<> neto33(fft_cols[16].out, plat.sink[33]);
   connect<> neto34(fft_rows[17].out, plat.sink[34]);
   connect<> neto35(fft_cols[17].out, plat.sink[35]);
   connect<> neto36(fft_rows[18].out, plat.sink[36]);
   connect<> neto37(fft_cols[18].out, plat.sink[37]);
   connect<> neto38(fft_rows[19].out, plat.sink[38]);
   connect<> neto39(fft_cols[19].out, plat.sink[39]);

#elif FFT2D_INSTS == 25

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i30 = new PLIO("DataIn30", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i31 = new PLIO("DataIn31", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i32 = new PLIO("DataIn32", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i33 = new PLIO("DataIn33", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i34 = new PLIO("DataIn34", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i35 = new PLIO("DataIn35", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i36 = new PLIO("DataIn36", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i37 = new PLIO("DataIn37", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i38 = new PLIO("DataIn38", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i39 = new PLIO("DataIn39", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i40 = new PLIO("DataIn40", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i41 = new PLIO("DataIn41", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i42 = new PLIO("DataIn42", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i43 = new PLIO("DataIn43", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i44 = new PLIO("DataIn44", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i45 = new PLIO("DataIn45", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i46 = new PLIO("DataIn46", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i47 = new PLIO("DataIn47", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i48 = new PLIO("DataIn48", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i49 = new PLIO("DataIn49", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   PLIO *attr_o30 = new PLIO("DataOut30", plio_128_bits, "data/output30.txt");
   PLIO *attr_o31 = new PLIO("DataOut31", plio_128_bits, "data/output31.txt");
   PLIO *attr_o32 = new PLIO("DataOut32", plio_128_bits, "data/output32.txt");
   PLIO *attr_o33 = new PLIO("DataOut33", plio_128_bits, "data/output33.txt");
   PLIO *attr_o34 = new PLIO("DataOut34", plio_128_bits, "data/output34.txt");
   PLIO *attr_o35 = new PLIO("DataOut35", plio_128_bits, "data/output35.txt");
   PLIO *attr_o36 = new PLIO("DataOut36", plio_128_bits, "data/output36.txt");
   PLIO *attr_o37 = new PLIO("DataOut37", plio_128_bits, "data/output37.txt");
   PLIO *attr_o38 = new PLIO("DataOut38", plio_128_bits, "data/output38.txt");
   PLIO *attr_o39 = new PLIO("DataOut39", plio_128_bits, "data/output39.txt");
   PLIO *attr_o40 = new PLIO("DataOut40", plio_128_bits, "data/output40.txt");
   PLIO *attr_o41 = new PLIO("DataOut41", plio_128_bits, "data/output41.txt");
   PLIO *attr_o42 = new PLIO("DataOut42", plio_128_bits, "data/output42.txt");
   PLIO *attr_o43 = new PLIO("DataOut43", plio_128_bits, "data/output43.txt");
   PLIO *attr_o44 = new PLIO("DataOut44", plio_128_bits, "data/output44.txt");
   PLIO *attr_o45 = new PLIO("DataOut45", plio_128_bits, "data/output45.txt");
   PLIO *attr_o46 = new PLIO("DataOut46", plio_128_bits, "data/output46.txt");
   PLIO *attr_o47 = new PLIO("DataOut47", plio_128_bits, "data/output47.txt");
   PLIO *attr_o48 = new PLIO("DataOut48", plio_128_bits, "data/output48.txt");
   PLIO *attr_o49 = new PLIO("DataOut49", plio_128_bits, "data/output49.txt");
   
   simulation::platform<50, 50> plat( attr_i00,
   											  attr_i01,
   											  attr_i02,
   											  attr_i03,
   											  attr_i04,
   											  attr_i05,
   											  attr_i06,
   											  attr_i07,
   											  attr_i08,
   											  attr_i09,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   											  attr_i20,
   											  attr_i21,
   											  attr_i22,
   											  attr_i23,
   											  attr_i24,
   											  attr_i25,
   											  attr_i26,
   											  attr_i27,
   											  attr_i28,
   											  attr_i29,
   											  attr_i30,
   											  attr_i31,
   											  attr_i32,
   											  attr_i33,
   											  attr_i34,
   											  attr_i35,
   											  attr_i36,
   											  attr_i37,
   											  attr_i38,
   											  attr_i39,
   											  attr_i40,
   											  attr_i41,
   											  attr_i42,
   											  attr_i43,
   											  attr_i44,
   											  attr_i45,
   											  attr_i46,
   											  attr_i47,
   											  attr_i48,
   											  attr_i49,
   
   											  attr_o00,
   											  attr_o01,
   											  attr_o02,
   											  attr_o03,
   											  attr_o04,
   											  attr_o05,
   											  attr_o06,
   											  attr_o07,
   											  attr_o08,
   											  attr_o09,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19,
   											  attr_o20,
   											  attr_o21,
   											  attr_o22,
   											  attr_o23,
   											  attr_o24,
   											  attr_o25,
   											  attr_o26,
   											  attr_o27,
   											  attr_o28,
   											  attr_o29,
   											  attr_o30,
   											  attr_o31,
   											  attr_o32,
   											  attr_o33,
   											  attr_o34,
   											  attr_o35,
   											  attr_o36,
   											  attr_o37,
   											  attr_o38,
   											  attr_o39,
   											  attr_o40,
   											  attr_o41,
   											  attr_o42,
   											  attr_o43,
   											  attr_o44,
   											  attr_o45,
   											  attr_o46,
   											  attr_o47,
   											  attr_o48,
   											  attr_o49
   											 );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   connect<> neti30(plat.src[30], fft_rows[15].in);
   connect<> neti31(plat.src[31], fft_cols[15].in);
   connect<> neti32(plat.src[32], fft_rows[16].in);
   connect<> neti33(plat.src[33], fft_cols[16].in);
   connect<> neti34(plat.src[34], fft_rows[17].in);
   connect<> neti35(plat.src[35], fft_cols[17].in);
   connect<> neti36(plat.src[36], fft_rows[18].in);
   connect<> neti37(plat.src[37], fft_cols[18].in);
   connect<> neti38(plat.src[38], fft_rows[19].in);
   connect<> neti39(plat.src[39], fft_cols[19].in);
   connect<> neti40(plat.src[40], fft_rows[20].in);
   connect<> neti41(plat.src[41], fft_cols[20].in);
   connect<> neti42(plat.src[42], fft_rows[21].in);
   connect<> neti43(plat.src[43], fft_cols[21].in);
   connect<> neti44(plat.src[44], fft_rows[22].in);
   connect<> neti45(plat.src[45], fft_cols[22].in);
   connect<> neti46(plat.src[46], fft_rows[23].in);
   connect<> neti47(plat.src[47], fft_cols[23].in);
   connect<> neti48(plat.src[48], fft_rows[24].in);
   connect<> neti49(plat.src[49], fft_cols[24].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);
   connect<> neto30(fft_rows[15].out, plat.sink[30]);
   connect<> neto31(fft_cols[15].out, plat.sink[31]);
   connect<> neto32(fft_rows[16].out, plat.sink[32]);
   connect<> neto33(fft_cols[16].out, plat.sink[33]);
   connect<> neto34(fft_rows[17].out, plat.sink[34]);
   connect<> neto35(fft_cols[17].out, plat.sink[35]);
   connect<> neto36(fft_rows[18].out, plat.sink[36]);
   connect<> neto37(fft_cols[18].out, plat.sink[37]);
   connect<> neto38(fft_rows[19].out, plat.sink[38]);
   connect<> neto39(fft_cols[19].out, plat.sink[39]);
   connect<> neto40(fft_rows[20].out, plat.sink[40]);
   connect<> neto41(fft_cols[20].out, plat.sink[41]);
   connect<> neto42(fft_rows[21].out, plat.sink[42]);
   connect<> neto43(fft_cols[21].out, plat.sink[43]);
   connect<> neto44(fft_rows[22].out, plat.sink[44]);
   connect<> neto45(fft_cols[22].out, plat.sink[45]);
   connect<> neto46(fft_rows[23].out, plat.sink[46]);
   connect<> neto47(fft_cols[23].out, plat.sink[47]);
   connect<> neto48(fft_rows[24].out, plat.sink[48]);
   connect<> neto49(fft_cols[24].out, plat.sink[49]);

#elif FFT2D_INSTS == 30

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i30 = new PLIO("DataIn30", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i31 = new PLIO("DataIn31", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i32 = new PLIO("DataIn32", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i33 = new PLIO("DataIn33", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i34 = new PLIO("DataIn34", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i35 = new PLIO("DataIn35", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i36 = new PLIO("DataIn36", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i37 = new PLIO("DataIn37", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i38 = new PLIO("DataIn38", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i39 = new PLIO("DataIn39", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i40 = new PLIO("DataIn40", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i41 = new PLIO("DataIn41", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i42 = new PLIO("DataIn42", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i43 = new PLIO("DataIn43", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i44 = new PLIO("DataIn44", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i45 = new PLIO("DataIn45", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i46 = new PLIO("DataIn46", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i47 = new PLIO("DataIn47", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i48 = new PLIO("DataIn48", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i49 = new PLIO("DataIn49", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i50 = new PLIO("DataIn50", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i51 = new PLIO("DataIn51", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i52 = new PLIO("DataIn52", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i53 = new PLIO("DataIn53", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i54 = new PLIO("DataIn54", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i55 = new PLIO("DataIn55", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i56 = new PLIO("DataIn56", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i57 = new PLIO("DataIn57", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i58 = new PLIO("DataIn58", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i59 = new PLIO("DataIn59", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   PLIO *attr_o30 = new PLIO("DataOut30", plio_128_bits, "data/output30.txt");
   PLIO *attr_o31 = new PLIO("DataOut31", plio_128_bits, "data/output31.txt");
   PLIO *attr_o32 = new PLIO("DataOut32", plio_128_bits, "data/output32.txt");
   PLIO *attr_o33 = new PLIO("DataOut33", plio_128_bits, "data/output33.txt");
   PLIO *attr_o34 = new PLIO("DataOut34", plio_128_bits, "data/output34.txt");
   PLIO *attr_o35 = new PLIO("DataOut35", plio_128_bits, "data/output35.txt");
   PLIO *attr_o36 = new PLIO("DataOut36", plio_128_bits, "data/output36.txt");
   PLIO *attr_o37 = new PLIO("DataOut37", plio_128_bits, "data/output37.txt");
   PLIO *attr_o38 = new PLIO("DataOut38", plio_128_bits, "data/output38.txt");
   PLIO *attr_o39 = new PLIO("DataOut39", plio_128_bits, "data/output39.txt");
   PLIO *attr_o40 = new PLIO("DataOut40", plio_128_bits, "data/output40.txt");
   PLIO *attr_o41 = new PLIO("DataOut41", plio_128_bits, "data/output41.txt");
   PLIO *attr_o42 = new PLIO("DataOut42", plio_128_bits, "data/output42.txt");
   PLIO *attr_o43 = new PLIO("DataOut43", plio_128_bits, "data/output43.txt");
   PLIO *attr_o44 = new PLIO("DataOut44", plio_128_bits, "data/output44.txt");
   PLIO *attr_o45 = new PLIO("DataOut45", plio_128_bits, "data/output45.txt");
   PLIO *attr_o46 = new PLIO("DataOut46", plio_128_bits, "data/output46.txt");
   PLIO *attr_o47 = new PLIO("DataOut47", plio_128_bits, "data/output47.txt");
   PLIO *attr_o48 = new PLIO("DataOut48", plio_128_bits, "data/output48.txt");
   PLIO *attr_o49 = new PLIO("DataOut49", plio_128_bits, "data/output49.txt");
   PLIO *attr_o50 = new PLIO("DataOut50", plio_128_bits, "data/output50.txt");
   PLIO *attr_o51 = new PLIO("DataOut51", plio_128_bits, "data/output51.txt");
   PLIO *attr_o52 = new PLIO("DataOut52", plio_128_bits, "data/output52.txt");
   PLIO *attr_o53 = new PLIO("DataOut53", plio_128_bits, "data/output53.txt");
   PLIO *attr_o54 = new PLIO("DataOut54", plio_128_bits, "data/output54.txt");
   PLIO *attr_o55 = new PLIO("DataOut55", plio_128_bits, "data/output55.txt");
   PLIO *attr_o56 = new PLIO("DataOut56", plio_128_bits, "data/output56.txt");
   PLIO *attr_o57 = new PLIO("DataOut57", plio_128_bits, "data/output57.txt");
   PLIO *attr_o58 = new PLIO("DataOut58", plio_128_bits, "data/output58.txt");
   PLIO *attr_o59 = new PLIO("DataOut59", plio_128_bits, "data/output59.txt");
   
   simulation::platform<60, 60> plat( attr_i00,
   											  attr_i01,
   											  attr_i02,
   											  attr_i03,
   											  attr_i04,
   											  attr_i05,
   											  attr_i06,
   											  attr_i07,
   											  attr_i08,
   											  attr_i09,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   											  attr_i20,
   											  attr_i21,
   											  attr_i22,
   											  attr_i23,
   											  attr_i24,
   											  attr_i25,
   											  attr_i26,
   											  attr_i27,
   											  attr_i28,
   											  attr_i29,
   											  attr_i30,
   											  attr_i31,
   											  attr_i32,
   											  attr_i33,
   											  attr_i34,
   											  attr_i35,
   											  attr_i36,
   											  attr_i37,
   											  attr_i38,
   											  attr_i39,
   											  attr_i40,
   											  attr_i41,
   											  attr_i42,
   											  attr_i43,
   											  attr_i44,
   											  attr_i45,
   											  attr_i46,
   											  attr_i47,
   											  attr_i48,
   											  attr_i49,
   											  attr_i50,
   											  attr_i51,
   											  attr_i52,
   											  attr_i53,
   											  attr_i54,
   											  attr_i55,
   											  attr_i56,
   											  attr_i57,
   											  attr_i58,
   											  attr_i59,
   
   											  attr_o00,
   											  attr_o01,
   											  attr_o02,
   											  attr_o03,
   											  attr_o04,
   											  attr_o05,
   											  attr_o06,
   											  attr_o07,
   											  attr_o08,
   											  attr_o09,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19,
   											  attr_o20,
   											  attr_o21,
   											  attr_o22,
   											  attr_o23,
   											  attr_o24,
   											  attr_o25,
   											  attr_o26,
   											  attr_o27,
   											  attr_o28,
   											  attr_o29,
   											  attr_o30,
   											  attr_o31,
   											  attr_o32,
   											  attr_o33,
   											  attr_o34,
   											  attr_o35,
   											  attr_o36,
   											  attr_o37,
   											  attr_o38,
   											  attr_o39,
   											  attr_o40,
   											  attr_o41,
   											  attr_o42,
   											  attr_o43,
   											  attr_o44,
   											  attr_o45,
   											  attr_o46,
   											  attr_o47,
   											  attr_o48,
   											  attr_o49,
   											  attr_o50,
   											  attr_o51,
   											  attr_o52,
   											  attr_o53,
   											  attr_o54,
   											  attr_o55,
   											  attr_o56,
   											  attr_o57,
   											  attr_o58,
   											  attr_o59
   											 );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   connect<> neti30(plat.src[30], fft_rows[15].in);
   connect<> neti31(plat.src[31], fft_cols[15].in);
   connect<> neti32(plat.src[32], fft_rows[16].in);
   connect<> neti33(plat.src[33], fft_cols[16].in);
   connect<> neti34(plat.src[34], fft_rows[17].in);
   connect<> neti35(plat.src[35], fft_cols[17].in);
   connect<> neti36(plat.src[36], fft_rows[18].in);
   connect<> neti37(plat.src[37], fft_cols[18].in);
   connect<> neti38(plat.src[38], fft_rows[19].in);
   connect<> neti39(plat.src[39], fft_cols[19].in);
   connect<> neti40(plat.src[40], fft_rows[20].in);
   connect<> neti41(plat.src[41], fft_cols[20].in);
   connect<> neti42(plat.src[42], fft_rows[21].in);
   connect<> neti43(plat.src[43], fft_cols[21].in);
   connect<> neti44(plat.src[44], fft_rows[22].in);
   connect<> neti45(plat.src[45], fft_cols[22].in);
   connect<> neti46(plat.src[46], fft_rows[23].in);
   connect<> neti47(plat.src[47], fft_cols[23].in);
   connect<> neti48(plat.src[48], fft_rows[24].in);
   connect<> neti49(plat.src[49], fft_cols[24].in);
   connect<> neti50(plat.src[50], fft_rows[25].in);
   connect<> neti51(plat.src[51], fft_cols[25].in);
   connect<> neti52(plat.src[52], fft_rows[26].in);
   connect<> neti53(plat.src[53], fft_cols[26].in);
   connect<> neti54(plat.src[54], fft_rows[27].in);
   connect<> neti55(plat.src[55], fft_cols[27].in);
   connect<> neti56(plat.src[56], fft_rows[28].in);
   connect<> neti57(plat.src[57], fft_cols[28].in);
   connect<> neti58(plat.src[58], fft_rows[29].in);
   connect<> neti59(plat.src[59], fft_cols[29].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);
   connect<> neto30(fft_rows[15].out, plat.sink[30]);
   connect<> neto31(fft_cols[15].out, plat.sink[31]);
   connect<> neto32(fft_rows[16].out, plat.sink[32]);
   connect<> neto33(fft_cols[16].out, plat.sink[33]);
   connect<> neto34(fft_rows[17].out, plat.sink[34]);
   connect<> neto35(fft_cols[17].out, plat.sink[35]);
   connect<> neto36(fft_rows[18].out, plat.sink[36]);
   connect<> neto37(fft_cols[18].out, plat.sink[37]);
   connect<> neto38(fft_rows[19].out, plat.sink[38]);
   connect<> neto39(fft_cols[19].out, plat.sink[39]);
   connect<> neto40(fft_rows[20].out, plat.sink[40]);
   connect<> neto41(fft_cols[20].out, plat.sink[41]);
   connect<> neto42(fft_rows[21].out, plat.sink[42]);
   connect<> neto43(fft_cols[21].out, plat.sink[43]);
   connect<> neto44(fft_rows[22].out, plat.sink[44]);
   connect<> neto45(fft_cols[22].out, plat.sink[45]);
   connect<> neto46(fft_rows[23].out, plat.sink[46]);
   connect<> neto47(fft_cols[23].out, plat.sink[47]);
   connect<> neto48(fft_rows[24].out, plat.sink[48]);
   connect<> neto49(fft_cols[24].out, plat.sink[49]);
   connect<> neto50(fft_rows[25].out, plat.sink[50]);
   connect<> neto51(fft_cols[25].out, plat.sink[51]);
   connect<> neto52(fft_rows[26].out, plat.sink[52]);
   connect<> neto53(fft_cols[26].out, plat.sink[53]);
   connect<> neto54(fft_rows[27].out, plat.sink[54]);
   connect<> neto55(fft_cols[27].out, plat.sink[55]);
   connect<> neto56(fft_rows[28].out, plat.sink[56]);
   connect<> neto57(fft_cols[28].out, plat.sink[57]);
   connect<> neto58(fft_rows[29].out, plat.sink[58]);
   connect<> neto59(fft_cols[29].out, plat.sink[59]);

#elif FFT2D_INSTS == 36

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i30 = new PLIO("DataIn30", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i31 = new PLIO("DataIn31", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i32 = new PLIO("DataIn32", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i33 = new PLIO("DataIn33", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i34 = new PLIO("DataIn34", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i35 = new PLIO("DataIn35", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i36 = new PLIO("DataIn36", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i37 = new PLIO("DataIn37", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i38 = new PLIO("DataIn38", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i39 = new PLIO("DataIn39", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i40 = new PLIO("DataIn40", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i41 = new PLIO("DataIn41", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i42 = new PLIO("DataIn42", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i43 = new PLIO("DataIn43", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i44 = new PLIO("DataIn44", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i45 = new PLIO("DataIn45", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i46 = new PLIO("DataIn46", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i47 = new PLIO("DataIn47", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i48 = new PLIO("DataIn48", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i49 = new PLIO("DataIn49", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i50 = new PLIO("DataIn50", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i51 = new PLIO("DataIn51", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i52 = new PLIO("DataIn52", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i53 = new PLIO("DataIn53", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i54 = new PLIO("DataIn54", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i55 = new PLIO("DataIn55", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i56 = new PLIO("DataIn56", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i57 = new PLIO("DataIn57", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i58 = new PLIO("DataIn58", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i59 = new PLIO("DataIn59", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i60 = new PLIO("DataIn60", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i61 = new PLIO("DataIn61", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i62 = new PLIO("DataIn62", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i63 = new PLIO("DataIn63", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i64 = new PLIO("DataIn64", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i65 = new PLIO("DataIn65", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i66 = new PLIO("DataIn66", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i67 = new PLIO("DataIn67", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i68 = new PLIO("DataIn68", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i69 = new PLIO("DataIn69", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i70 = new PLIO("DataIn70", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i71 = new PLIO("DataIn71", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   
   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   PLIO *attr_o30 = new PLIO("DataOut30", plio_128_bits, "data/output30.txt");
   PLIO *attr_o31 = new PLIO("DataOut31", plio_128_bits, "data/output31.txt");
   PLIO *attr_o32 = new PLIO("DataOut32", plio_128_bits, "data/output32.txt");
   PLIO *attr_o33 = new PLIO("DataOut33", plio_128_bits, "data/output33.txt");
   PLIO *attr_o34 = new PLIO("DataOut34", plio_128_bits, "data/output34.txt");
   PLIO *attr_o35 = new PLIO("DataOut35", plio_128_bits, "data/output35.txt");
   PLIO *attr_o36 = new PLIO("DataOut36", plio_128_bits, "data/output36.txt");
   PLIO *attr_o37 = new PLIO("DataOut37", plio_128_bits, "data/output37.txt");
   PLIO *attr_o38 = new PLIO("DataOut38", plio_128_bits, "data/output38.txt");
   PLIO *attr_o39 = new PLIO("DataOut39", plio_128_bits, "data/output39.txt");
   PLIO *attr_o40 = new PLIO("DataOut40", plio_128_bits, "data/output40.txt");
   PLIO *attr_o41 = new PLIO("DataOut41", plio_128_bits, "data/output41.txt");
   PLIO *attr_o42 = new PLIO("DataOut42", plio_128_bits, "data/output42.txt");
   PLIO *attr_o43 = new PLIO("DataOut43", plio_128_bits, "data/output43.txt");
   PLIO *attr_o44 = new PLIO("DataOut44", plio_128_bits, "data/output44.txt");
   PLIO *attr_o45 = new PLIO("DataOut45", plio_128_bits, "data/output45.txt");
   PLIO *attr_o46 = new PLIO("DataOut46", plio_128_bits, "data/output46.txt");
   PLIO *attr_o47 = new PLIO("DataOut47", plio_128_bits, "data/output47.txt");
   PLIO *attr_o48 = new PLIO("DataOut48", plio_128_bits, "data/output48.txt");
   PLIO *attr_o49 = new PLIO("DataOut49", plio_128_bits, "data/output49.txt");
   PLIO *attr_o50 = new PLIO("DataOut50", plio_128_bits, "data/output50.txt");
   PLIO *attr_o51 = new PLIO("DataOut51", plio_128_bits, "data/output51.txt");
   PLIO *attr_o52 = new PLIO("DataOut52", plio_128_bits, "data/output52.txt");
   PLIO *attr_o53 = new PLIO("DataOut53", plio_128_bits, "data/output53.txt");
   PLIO *attr_o54 = new PLIO("DataOut54", plio_128_bits, "data/output54.txt");
   PLIO *attr_o55 = new PLIO("DataOut55", plio_128_bits, "data/output55.txt");
   PLIO *attr_o56 = new PLIO("DataOut56", plio_128_bits, "data/output56.txt");
   PLIO *attr_o57 = new PLIO("DataOut57", plio_128_bits, "data/output57.txt");
   PLIO *attr_o58 = new PLIO("DataOut58", plio_128_bits, "data/output58.txt");
   PLIO *attr_o59 = new PLIO("DataOut59", plio_128_bits, "data/output59.txt");
   PLIO *attr_o60 = new PLIO("DataOut60", plio_128_bits, "data/output60.txt");
   PLIO *attr_o61 = new PLIO("DataOut61", plio_128_bits, "data/output61.txt");
   PLIO *attr_o62 = new PLIO("DataOut62", plio_128_bits, "data/output62.txt");
   PLIO *attr_o63 = new PLIO("DataOut63", plio_128_bits, "data/output63.txt");
   PLIO *attr_o64 = new PLIO("DataOut64", plio_128_bits, "data/output64.txt");
   PLIO *attr_o65 = new PLIO("DataOut65", plio_128_bits, "data/output65.txt");
   PLIO *attr_o66 = new PLIO("DataOut66", plio_128_bits, "data/output66.txt");
   PLIO *attr_o67 = new PLIO("DataOut67", plio_128_bits, "data/output67.txt");
   PLIO *attr_o68 = new PLIO("DataOut68", plio_128_bits, "data/output68.txt");
   PLIO *attr_o69 = new PLIO("DataOut69", plio_128_bits, "data/output69.txt");
   PLIO *attr_o70 = new PLIO("DataOut70", plio_128_bits, "data/output70.txt");
   PLIO *attr_o71 = new PLIO("DataOut71", plio_128_bits, "data/output71.txt");
   
   simulation::platform<72, 72> plat( attr_i00,
   											  attr_i01,
   											  attr_i02,
   											  attr_i03,
   											  attr_i04,
   											  attr_i05,
   											  attr_i06,
   											  attr_i07,
   											  attr_i08,
   											  attr_i09,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   											  attr_i20,
   											  attr_i21,
   											  attr_i22,
   											  attr_i23,
   											  attr_i24,
   											  attr_i25,
   											  attr_i26,
   											  attr_i27,
   											  attr_i28,
   											  attr_i29,
   											  attr_i30,
   											  attr_i31,
   											  attr_i32,
   											  attr_i33,
   											  attr_i34,
   											  attr_i35,
   											  attr_i36,
   											  attr_i37,
   											  attr_i38,
   											  attr_i39,
   											  attr_i40,
   											  attr_i41,
   											  attr_i42,
   											  attr_i43,
   											  attr_i44,
   											  attr_i45,
   											  attr_i46,
   											  attr_i47,
   											  attr_i48,
   											  attr_i49,
   											  attr_i50,
   											  attr_i51,
   											  attr_i52,
   											  attr_i53,
   											  attr_i54,
   											  attr_i55,
   											  attr_i56,
   											  attr_i57,
   											  attr_i58,
   											  attr_i59,
   											  attr_i60,
   											  attr_i61,
   											  attr_i62,
   											  attr_i63,
   											  attr_i64,
   											  attr_i65,
   											  attr_i66,
   											  attr_i67,
   											  attr_i68,
   											  attr_i69,
   											  attr_i70,
   											  attr_i71,
                                      
   											  attr_o00,
   											  attr_o01,
   											  attr_o02,
   											  attr_o03,
   											  attr_o04,
   											  attr_o05,
   											  attr_o06,
   											  attr_o07,
   											  attr_o08,
   											  attr_o09,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19,
   											  attr_o20,
   											  attr_o21,
   											  attr_o22,
   											  attr_o23,
   											  attr_o24,
   											  attr_o25,
   											  attr_o26,
   											  attr_o27,
   											  attr_o28,
   											  attr_o29,
   											  attr_o30,
   											  attr_o31,
   											  attr_o32,
   											  attr_o33,
   											  attr_o34,
   											  attr_o35,
   											  attr_o36,
   											  attr_o37,
   											  attr_o38,
   											  attr_o39,
   											  attr_o40,
   											  attr_o41,
   											  attr_o42,
   											  attr_o43,
   											  attr_o44,
   											  attr_o45,
   											  attr_o46,
   											  attr_o47,
   											  attr_o48,
   											  attr_o49,
   											  attr_o50,
   											  attr_o51,
   											  attr_o52,
   											  attr_o53,
   											  attr_o54,
   											  attr_o55,
   											  attr_o56,
   											  attr_o57,
   											  attr_o58,
   											  attr_o59,
   											  attr_o60,
   											  attr_o61,
   											  attr_o62,
   											  attr_o63,
   											  attr_o64,
   											  attr_o65,
   											  attr_o66,
   											  attr_o67,
   											  attr_o68,
   											  attr_o69,
   											  attr_o70,
   											  attr_o71
   											 );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   connect<> neti30(plat.src[30], fft_rows[15].in);
   connect<> neti31(plat.src[31], fft_cols[15].in);
   connect<> neti32(plat.src[32], fft_rows[16].in);
   connect<> neti33(plat.src[33], fft_cols[16].in);
   connect<> neti34(plat.src[34], fft_rows[17].in);
   connect<> neti35(plat.src[35], fft_cols[17].in);
   connect<> neti36(plat.src[36], fft_rows[18].in);
   connect<> neti37(plat.src[37], fft_cols[18].in);
   connect<> neti38(plat.src[38], fft_rows[19].in);
   connect<> neti39(plat.src[39], fft_cols[19].in);
   connect<> neti40(plat.src[40], fft_rows[20].in);
   connect<> neti41(plat.src[41], fft_cols[20].in);
   connect<> neti42(plat.src[42], fft_rows[21].in);
   connect<> neti43(plat.src[43], fft_cols[21].in);
   connect<> neti44(plat.src[44], fft_rows[22].in);
   connect<> neti45(plat.src[45], fft_cols[22].in);
   connect<> neti46(plat.src[46], fft_rows[23].in);
   connect<> neti47(plat.src[47], fft_cols[23].in);
   connect<> neti48(plat.src[48], fft_rows[24].in);
   connect<> neti49(plat.src[49], fft_cols[24].in);
   connect<> neti50(plat.src[50], fft_rows[25].in);
   connect<> neti51(plat.src[51], fft_cols[25].in);
   connect<> neti52(plat.src[52], fft_rows[26].in);
   connect<> neti53(plat.src[53], fft_cols[26].in);
   connect<> neti54(plat.src[54], fft_rows[27].in);
   connect<> neti55(plat.src[55], fft_cols[27].in);
   connect<> neti56(plat.src[56], fft_rows[28].in);
   connect<> neti57(plat.src[57], fft_cols[28].in);
   connect<> neti58(plat.src[58], fft_rows[29].in);
   connect<> neti59(plat.src[59], fft_cols[29].in);
   connect<> neti60(plat.src[60], fft_rows[30].in);
   connect<> neti61(plat.src[61], fft_cols[30].in);
   connect<> neti62(plat.src[62], fft_rows[31].in);
   connect<> neti63(plat.src[63], fft_cols[31].in);
   connect<> neti64(plat.src[64], fft_rows[32].in);
   connect<> neti65(plat.src[65], fft_cols[32].in);
   connect<> neti66(plat.src[66], fft_rows[33].in);
   connect<> neti67(plat.src[67], fft_cols[33].in);
   connect<> neti68(plat.src[68], fft_rows[34].in);
   connect<> neti69(plat.src[69], fft_cols[34].in);
   connect<> neti70(plat.src[70], fft_rows[35].in);
   connect<> neti71(plat.src[71], fft_cols[35].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);
   connect<> neto30(fft_rows[15].out, plat.sink[30]);
   connect<> neto31(fft_cols[15].out, plat.sink[31]);
   connect<> neto32(fft_rows[16].out, plat.sink[32]);
   connect<> neto33(fft_cols[16].out, plat.sink[33]);
   connect<> neto34(fft_rows[17].out, plat.sink[34]);
   connect<> neto35(fft_cols[17].out, plat.sink[35]);
   connect<> neto36(fft_rows[18].out, plat.sink[36]);
   connect<> neto37(fft_cols[18].out, plat.sink[37]);
   connect<> neto38(fft_rows[19].out, plat.sink[38]);
   connect<> neto39(fft_cols[19].out, plat.sink[39]);
   connect<> neto40(fft_rows[20].out, plat.sink[40]);
   connect<> neto41(fft_cols[20].out, plat.sink[41]);
   connect<> neto42(fft_rows[21].out, plat.sink[42]);
   connect<> neto43(fft_cols[21].out, plat.sink[43]);
   connect<> neto44(fft_rows[22].out, plat.sink[44]);
   connect<> neto45(fft_cols[22].out, plat.sink[45]);
   connect<> neto46(fft_rows[23].out, plat.sink[46]);
   connect<> neto47(fft_cols[23].out, plat.sink[47]);
   connect<> neto48(fft_rows[24].out, plat.sink[48]);
   connect<> neto49(fft_cols[24].out, plat.sink[49]);
   connect<> neto50(fft_rows[25].out, plat.sink[50]);
   connect<> neto51(fft_cols[25].out, plat.sink[51]);
   connect<> neto52(fft_rows[26].out, plat.sink[52]);
   connect<> neto53(fft_cols[26].out, plat.sink[53]);
   connect<> neto54(fft_rows[27].out, plat.sink[54]);
   connect<> neto55(fft_cols[27].out, plat.sink[55]);
   connect<> neto56(fft_rows[28].out, plat.sink[56]);
   connect<> neto57(fft_cols[28].out, plat.sink[57]);
   connect<> neto58(fft_rows[29].out, plat.sink[58]);
   connect<> neto59(fft_cols[29].out, plat.sink[59]);
   connect<> neto60(fft_rows[30].out, plat.sink[60]);
   connect<> neto61(fft_cols[30].out, plat.sink[61]);
   connect<> neto62(fft_rows[31].out, plat.sink[62]);
   connect<> neto63(fft_cols[31].out, plat.sink[63]);
   connect<> neto64(fft_rows[32].out, plat.sink[64]);
   connect<> neto65(fft_cols[32].out, plat.sink[65]);
   connect<> neto66(fft_rows[33].out, plat.sink[66]);
   connect<> neto67(fft_cols[33].out, plat.sink[67]);
   connect<> neto68(fft_rows[34].out, plat.sink[68]);
   connect<> neto69(fft_cols[34].out, plat.sink[69]);
   connect<> neto70(fft_rows[35].out, plat.sink[70]);
   connect<> neto71(fft_cols[35].out, plat.sink[71]);

#elif FFT2D_INSTS == 40

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i30 = new PLIO("DataIn30", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i31 = new PLIO("DataIn31", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i32 = new PLIO("DataIn32", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i33 = new PLIO("DataIn33", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i34 = new PLIO("DataIn34", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i35 = new PLIO("DataIn35", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i36 = new PLIO("DataIn36", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i37 = new PLIO("DataIn37", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i38 = new PLIO("DataIn38", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i39 = new PLIO("DataIn39", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i40 = new PLIO("DataIn40", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i41 = new PLIO("DataIn41", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i42 = new PLIO("DataIn42", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i43 = new PLIO("DataIn43", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i44 = new PLIO("DataIn44", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i45 = new PLIO("DataIn45", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i46 = new PLIO("DataIn46", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i47 = new PLIO("DataIn47", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i48 = new PLIO("DataIn48", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i49 = new PLIO("DataIn49", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i50 = new PLIO("DataIn50", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i51 = new PLIO("DataIn51", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i52 = new PLIO("DataIn52", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i53 = new PLIO("DataIn53", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i54 = new PLIO("DataIn54", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i55 = new PLIO("DataIn55", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i56 = new PLIO("DataIn56", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i57 = new PLIO("DataIn57", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i58 = new PLIO("DataIn58", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i59 = new PLIO("DataIn59", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i60 = new PLIO("DataIn60", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i61 = new PLIO("DataIn61", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i62 = new PLIO("DataIn62", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i63 = new PLIO("DataIn63", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i64 = new PLIO("DataIn64", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i65 = new PLIO("DataIn65", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i66 = new PLIO("DataIn66", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i67 = new PLIO("DataIn67", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i68 = new PLIO("DataIn68", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i69 = new PLIO("DataIn69", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i70 = new PLIO("DataIn70", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i71 = new PLIO("DataIn71", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i72 = new PLIO("DataIn72", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i73 = new PLIO("DataIn73", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i74 = new PLIO("DataIn74", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i75 = new PLIO("DataIn75", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i76 = new PLIO("DataIn76", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i77 = new PLIO("DataIn77", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i78 = new PLIO("DataIn78", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i79 = new PLIO("DataIn79", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   PLIO *attr_o30 = new PLIO("DataOut30", plio_128_bits, "data/output30.txt");
   PLIO *attr_o31 = new PLIO("DataOut31", plio_128_bits, "data/output31.txt");
   PLIO *attr_o32 = new PLIO("DataOut32", plio_128_bits, "data/output32.txt");
   PLIO *attr_o33 = new PLIO("DataOut33", plio_128_bits, "data/output33.txt");
   PLIO *attr_o34 = new PLIO("DataOut34", plio_128_bits, "data/output34.txt");
   PLIO *attr_o35 = new PLIO("DataOut35", plio_128_bits, "data/output35.txt");
   PLIO *attr_o36 = new PLIO("DataOut36", plio_128_bits, "data/output36.txt");
   PLIO *attr_o37 = new PLIO("DataOut37", plio_128_bits, "data/output37.txt");
   PLIO *attr_o38 = new PLIO("DataOut38", plio_128_bits, "data/output38.txt");
   PLIO *attr_o39 = new PLIO("DataOut39", plio_128_bits, "data/output39.txt");
   PLIO *attr_o40 = new PLIO("DataOut40", plio_128_bits, "data/output40.txt");
   PLIO *attr_o41 = new PLIO("DataOut41", plio_128_bits, "data/output41.txt");
   PLIO *attr_o42 = new PLIO("DataOut42", plio_128_bits, "data/output42.txt");
   PLIO *attr_o43 = new PLIO("DataOut43", plio_128_bits, "data/output43.txt");
   PLIO *attr_o44 = new PLIO("DataOut44", plio_128_bits, "data/output44.txt");
   PLIO *attr_o45 = new PLIO("DataOut45", plio_128_bits, "data/output45.txt");
   PLIO *attr_o46 = new PLIO("DataOut46", plio_128_bits, "data/output46.txt");
   PLIO *attr_o47 = new PLIO("DataOut47", plio_128_bits, "data/output47.txt");
   PLIO *attr_o48 = new PLIO("DataOut48", plio_128_bits, "data/output48.txt");
   PLIO *attr_o49 = new PLIO("DataOut49", plio_128_bits, "data/output49.txt");
   PLIO *attr_o50 = new PLIO("DataOut50", plio_128_bits, "data/output50.txt");
   PLIO *attr_o51 = new PLIO("DataOut51", plio_128_bits, "data/output51.txt");
   PLIO *attr_o52 = new PLIO("DataOut52", plio_128_bits, "data/output52.txt");
   PLIO *attr_o53 = new PLIO("DataOut53", plio_128_bits, "data/output53.txt");
   PLIO *attr_o54 = new PLIO("DataOut54", plio_128_bits, "data/output54.txt");
   PLIO *attr_o55 = new PLIO("DataOut55", plio_128_bits, "data/output55.txt");
   PLIO *attr_o56 = new PLIO("DataOut56", plio_128_bits, "data/output56.txt");
   PLIO *attr_o57 = new PLIO("DataOut57", plio_128_bits, "data/output57.txt");
   PLIO *attr_o58 = new PLIO("DataOut58", plio_128_bits, "data/output58.txt");
   PLIO *attr_o59 = new PLIO("DataOut59", plio_128_bits, "data/output59.txt");
   PLIO *attr_o60 = new PLIO("DataOut60", plio_128_bits, "data/output60.txt");
   PLIO *attr_o61 = new PLIO("DataOut61", plio_128_bits, "data/output61.txt");
   PLIO *attr_o62 = new PLIO("DataOut62", plio_128_bits, "data/output62.txt");
   PLIO *attr_o63 = new PLIO("DataOut63", plio_128_bits, "data/output63.txt");
   PLIO *attr_o64 = new PLIO("DataOut64", plio_128_bits, "data/output64.txt");
   PLIO *attr_o65 = new PLIO("DataOut65", plio_128_bits, "data/output65.txt");
   PLIO *attr_o66 = new PLIO("DataOut66", plio_128_bits, "data/output66.txt");
   PLIO *attr_o67 = new PLIO("DataOut67", plio_128_bits, "data/output67.txt");
   PLIO *attr_o68 = new PLIO("DataOut68", plio_128_bits, "data/output68.txt");
   PLIO *attr_o69 = new PLIO("DataOut69", plio_128_bits, "data/output69.txt");
   PLIO *attr_o70 = new PLIO("DataOut70", plio_128_bits, "data/output70.txt");
   PLIO *attr_o71 = new PLIO("DataOut71", plio_128_bits, "data/output71.txt");
   PLIO *attr_o72 = new PLIO("DataOut72", plio_128_bits, "data/output72.txt");
   PLIO *attr_o73 = new PLIO("DataOut73", plio_128_bits, "data/output73.txt");
   PLIO *attr_o74 = new PLIO("DataOut74", plio_128_bits, "data/output74.txt");
   PLIO *attr_o75 = new PLIO("DataOut75", plio_128_bits, "data/output75.txt");
   PLIO *attr_o76 = new PLIO("DataOut76", plio_128_bits, "data/output76.txt");
   PLIO *attr_o77 = new PLIO("DataOut77", plio_128_bits, "data/output77.txt");
   PLIO *attr_o78 = new PLIO("DataOut78", plio_128_bits, "data/output78.txt");
   PLIO *attr_o79 = new PLIO("DataOut79", plio_128_bits, "data/output79.txt");
   
   simulation::platform<80, 80> plat( attr_i00,
   											  attr_i01,
   											  attr_i02,
   											  attr_i03,
   											  attr_i04,
   											  attr_i05,
   											  attr_i06,
   											  attr_i07,
   											  attr_i08,
   											  attr_i09,
   											  attr_i10,
   											  attr_i11,
   											  attr_i12,
   											  attr_i13,
   											  attr_i14,
   											  attr_i15,
   											  attr_i16,
   											  attr_i17,
   											  attr_i18,
   											  attr_i19,
   											  attr_i20,
   											  attr_i21,
   											  attr_i22,
   											  attr_i23,
   											  attr_i24,
   											  attr_i25,
   											  attr_i26,
   											  attr_i27,
   											  attr_i28,
   											  attr_i29,
   											  attr_i30,
   											  attr_i31,
   											  attr_i32,
   											  attr_i33,
   											  attr_i34,
   											  attr_i35,
   											  attr_i36,
   											  attr_i37,
   											  attr_i38,
   											  attr_i39,
   											  attr_i40,
   											  attr_i41,
   											  attr_i42,
   											  attr_i43,
   											  attr_i44,
   											  attr_i45,
   											  attr_i46,
   											  attr_i47,
   											  attr_i48,
   											  attr_i49,
   											  attr_i50,
   											  attr_i51,
   											  attr_i52,
   											  attr_i53,
   											  attr_i54,
   											  attr_i55,
   											  attr_i56,
   											  attr_i57,
   											  attr_i58,
   											  attr_i59,
   											  attr_i60,
   											  attr_i61,
   											  attr_i62,
   											  attr_i63,
   											  attr_i64,
   											  attr_i65,
   											  attr_i66,
   											  attr_i67,
   											  attr_i68,
   											  attr_i69,
   											  attr_i70,
   											  attr_i71,
   											  attr_i72,
   											  attr_i73,
   											  attr_i74,
   											  attr_i75,
   											  attr_i76,
   											  attr_i77,
   											  attr_i78,
   											  attr_i79,
   
   											  attr_o00,
   											  attr_o01,
   											  attr_o02,
   											  attr_o03,
   											  attr_o04,
   											  attr_o05,
   											  attr_o06,
   											  attr_o07,
   											  attr_o08,
   											  attr_o09,
   											  attr_o10,
   											  attr_o11,
   											  attr_o12,
   											  attr_o13,
   											  attr_o14,
   											  attr_o15,
   											  attr_o16,
   											  attr_o17,
   											  attr_o18,
   											  attr_o19,
   											  attr_o20,
   											  attr_o21,
   											  attr_o22,
   											  attr_o23,
   											  attr_o24,
   											  attr_o25,
   											  attr_o26,
   											  attr_o27,
   											  attr_o28,
   											  attr_o29,
   											  attr_o30,
   											  attr_o31,
   											  attr_o32,
   											  attr_o33,
   											  attr_o34,
   											  attr_o35,
   											  attr_o36,
   											  attr_o37,
   											  attr_o38,
   											  attr_o39,
   											  attr_o40,
   											  attr_o41,
   											  attr_o42,
   											  attr_o43,
   											  attr_o44,
   											  attr_o45,
   											  attr_o46,
   											  attr_o47,
   											  attr_o48,
   											  attr_o49,
   											  attr_o50,
   											  attr_o51,
   											  attr_o52,
   											  attr_o53,
   											  attr_o54,
   											  attr_o55,
   											  attr_o56,
   											  attr_o57,
   											  attr_o58,
   											  attr_o59,
   											  attr_o60,
   											  attr_o61,
   											  attr_o62,
   											  attr_o63,
   											  attr_o64,
   											  attr_o65,
   											  attr_o66,
   											  attr_o67,
   											  attr_o68,
   											  attr_o69,
   											  attr_o70,
   											  attr_o71,
   											  attr_o72,
   											  attr_o73,
   											  attr_o74,
   											  attr_o75,
   											  attr_o76,
   											  attr_o77,
   											  attr_o78,
   											  attr_o79
   											 );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   connect<> neti30(plat.src[30], fft_rows[15].in);
   connect<> neti31(plat.src[31], fft_cols[15].in);
   connect<> neti32(plat.src[32], fft_rows[16].in);
   connect<> neti33(plat.src[33], fft_cols[16].in);
   connect<> neti34(plat.src[34], fft_rows[17].in);
   connect<> neti35(plat.src[35], fft_cols[17].in);
   connect<> neti36(plat.src[36], fft_rows[18].in);
   connect<> neti37(plat.src[37], fft_cols[18].in);
   connect<> neti38(plat.src[38], fft_rows[19].in);
   connect<> neti39(plat.src[39], fft_cols[19].in);
   connect<> neti40(plat.src[40], fft_rows[20].in);
   connect<> neti41(plat.src[41], fft_cols[20].in);
   connect<> neti42(plat.src[42], fft_rows[21].in);
   connect<> neti43(plat.src[43], fft_cols[21].in);
   connect<> neti44(plat.src[44], fft_rows[22].in);
   connect<> neti45(plat.src[45], fft_cols[22].in);
   connect<> neti46(plat.src[46], fft_rows[23].in);
   connect<> neti47(plat.src[47], fft_cols[23].in);
   connect<> neti48(plat.src[48], fft_rows[24].in);
   connect<> neti49(plat.src[49], fft_cols[24].in);
   connect<> neti50(plat.src[50], fft_rows[25].in);
   connect<> neti51(plat.src[51], fft_cols[25].in);
   connect<> neti52(plat.src[52], fft_rows[26].in);
   connect<> neti53(plat.src[53], fft_cols[26].in);
   connect<> neti54(plat.src[54], fft_rows[27].in);
   connect<> neti55(plat.src[55], fft_cols[27].in);
   connect<> neti56(plat.src[56], fft_rows[28].in);
   connect<> neti57(plat.src[57], fft_cols[28].in);
   connect<> neti58(plat.src[58], fft_rows[29].in);
   connect<> neti59(plat.src[59], fft_cols[29].in);
   connect<> neti60(plat.src[60], fft_rows[30].in);
   connect<> neti61(plat.src[61], fft_cols[30].in);
   connect<> neti62(plat.src[62], fft_rows[31].in);
   connect<> neti63(plat.src[63], fft_cols[31].in);
   connect<> neti64(plat.src[64], fft_rows[32].in);
   connect<> neti65(plat.src[65], fft_cols[32].in);
   connect<> neti66(plat.src[66], fft_rows[33].in);
   connect<> neti67(plat.src[67], fft_cols[33].in);
   connect<> neti68(plat.src[68], fft_rows[34].in);
   connect<> neti69(plat.src[69], fft_cols[34].in);
   connect<> neti70(plat.src[70], fft_rows[35].in);
   connect<> neti71(plat.src[71], fft_cols[35].in);
   connect<> neti72(plat.src[72], fft_rows[36].in);
   connect<> neti73(plat.src[73], fft_cols[36].in);
   connect<> neti74(plat.src[74], fft_rows[37].in);
   connect<> neti75(plat.src[75], fft_cols[37].in);
   connect<> neti76(plat.src[76], fft_rows[38].in);
   connect<> neti77(plat.src[77], fft_cols[38].in);
   connect<> neti78(plat.src[78], fft_rows[39].in);
   connect<> neti79(plat.src[79], fft_cols[39].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);
   connect<> neto30(fft_rows[15].out, plat.sink[30]);
   connect<> neto31(fft_cols[15].out, plat.sink[31]);
   connect<> neto32(fft_rows[16].out, plat.sink[32]);
   connect<> neto33(fft_cols[16].out, plat.sink[33]);
   connect<> neto34(fft_rows[17].out, plat.sink[34]);
   connect<> neto35(fft_cols[17].out, plat.sink[35]);
   connect<> neto36(fft_rows[18].out, plat.sink[36]);
   connect<> neto37(fft_cols[18].out, plat.sink[37]);
   connect<> neto38(fft_rows[19].out, plat.sink[38]);
   connect<> neto39(fft_cols[19].out, plat.sink[39]);
   connect<> neto40(fft_rows[20].out, plat.sink[40]);
   connect<> neto41(fft_cols[20].out, plat.sink[41]);
   connect<> neto42(fft_rows[21].out, plat.sink[42]);
   connect<> neto43(fft_cols[21].out, plat.sink[43]);
   connect<> neto44(fft_rows[22].out, plat.sink[44]);
   connect<> neto45(fft_cols[22].out, plat.sink[45]);
   connect<> neto46(fft_rows[23].out, plat.sink[46]);
   connect<> neto47(fft_cols[23].out, plat.sink[47]);
   connect<> neto48(fft_rows[24].out, plat.sink[48]);
   connect<> neto49(fft_cols[24].out, plat.sink[49]);
   connect<> neto50(fft_rows[25].out, plat.sink[50]);
   connect<> neto51(fft_cols[25].out, plat.sink[51]);
   connect<> neto52(fft_rows[26].out, plat.sink[52]);
   connect<> neto53(fft_cols[26].out, plat.sink[53]);
   connect<> neto54(fft_rows[27].out, plat.sink[54]);
   connect<> neto55(fft_cols[27].out, plat.sink[55]);
   connect<> neto56(fft_rows[28].out, plat.sink[56]);
   connect<> neto57(fft_cols[28].out, plat.sink[57]);
   connect<> neto58(fft_rows[29].out, plat.sink[58]);
   connect<> neto59(fft_cols[29].out, plat.sink[59]);
   connect<> neto60(fft_rows[30].out, plat.sink[60]);
   connect<> neto61(fft_cols[30].out, plat.sink[61]);
   connect<> neto62(fft_rows[31].out, plat.sink[62]);
   connect<> neto63(fft_cols[31].out, plat.sink[63]);
   connect<> neto64(fft_rows[32].out, plat.sink[64]);
   connect<> neto65(fft_cols[32].out, plat.sink[65]);
   connect<> neto66(fft_rows[33].out, plat.sink[66]);
   connect<> neto67(fft_cols[33].out, plat.sink[67]);
   connect<> neto68(fft_rows[34].out, plat.sink[68]);
   connect<> neto69(fft_cols[34].out, plat.sink[69]);
   connect<> neto70(fft_rows[35].out, plat.sink[70]);
   connect<> neto71(fft_cols[35].out, plat.sink[71]);
   connect<> neto72(fft_rows[36].out, plat.sink[72]);
   connect<> neto73(fft_cols[36].out, plat.sink[73]);
   connect<> neto74(fft_rows[37].out, plat.sink[74]);
   connect<> neto75(fft_cols[37].out, plat.sink[75]);
   connect<> neto76(fft_rows[38].out, plat.sink[76]);
   connect<> neto77(fft_cols[38].out, plat.sink[77]);
   connect<> neto78(fft_rows[39].out, plat.sink[78]);
   connect<> neto79(fft_cols[39].out, plat.sink[79]);

#elif FFT2D_INSTS == 50

   PLIO *attr_i00 = new PLIO("DataIn0",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i01 = new PLIO("DataIn1",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i02 = new PLIO("DataIn2",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i03 = new PLIO("DataIn3",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i04 = new PLIO("DataIn4",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i05 = new PLIO("DataIn5",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i06 = new PLIO("DataIn6",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i07 = new PLIO("DataIn7",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i08 = new PLIO("DataIn8",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i09 = new PLIO("DataIn9",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i10 = new PLIO("DataIn10", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i11 = new PLIO("DataIn11", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i12 = new PLIO("DataIn12", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i13 = new PLIO("DataIn13", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i14 = new PLIO("DataIn14", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i15 = new PLIO("DataIn15", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i16 = new PLIO("DataIn16", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i17 = new PLIO("DataIn17", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i18 = new PLIO("DataIn18", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i19 = new PLIO("DataIn19", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i20 = new PLIO("DataIn20", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i21 = new PLIO("DataIn21", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i22 = new PLIO("DataIn22", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i23 = new PLIO("DataIn23", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i24 = new PLIO("DataIn24", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i25 = new PLIO("DataIn25", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i26 = new PLIO("DataIn26", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i27 = new PLIO("DataIn27", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i28 = new PLIO("DataIn28", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i29 = new PLIO("DataIn29", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i30 = new PLIO("DataIn30", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i31 = new PLIO("DataIn31", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i32 = new PLIO("DataIn32", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i33 = new PLIO("DataIn33", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i34 = new PLIO("DataIn34", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i35 = new PLIO("DataIn35", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i36 = new PLIO("DataIn36", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i37 = new PLIO("DataIn37", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i38 = new PLIO("DataIn38", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i39 = new PLIO("DataIn39", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i40 = new PLIO("DataIn40", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i41 = new PLIO("DataIn41", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i42 = new PLIO("DataIn42", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i43 = new PLIO("DataIn43", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i44 = new PLIO("DataIn44", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i45 = new PLIO("DataIn45", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i46 = new PLIO("DataIn46", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i47 = new PLIO("DataIn47", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i48 = new PLIO("DataIn48", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i49 = new PLIO("DataIn49", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i50 = new PLIO("DataIn50", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i51 = new PLIO("DataIn51", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i52 = new PLIO("DataIn52", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i53 = new PLIO("DataIn53", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i54 = new PLIO("DataIn54", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i55 = new PLIO("DataIn55", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i56 = new PLIO("DataIn56", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i57 = new PLIO("DataIn57", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i58 = new PLIO("DataIn58", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i59 = new PLIO("DataIn59", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i60 = new PLIO("DataIn60", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i61 = new PLIO("DataIn61", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i62 = new PLIO("DataIn62", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i63 = new PLIO("DataIn63", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i64 = new PLIO("DataIn64", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i65 = new PLIO("DataIn65", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i66 = new PLIO("DataIn66", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i67 = new PLIO("DataIn67", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i68 = new PLIO("DataIn68", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i69 = new PLIO("DataIn69", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i70 = new PLIO("DataIn70", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i71 = new PLIO("DataIn71", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i72 = new PLIO("DataIn72", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i73 = new PLIO("DataIn73", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i74 = new PLIO("DataIn74", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i75 = new PLIO("DataIn75", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i76 = new PLIO("DataIn76", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i77 = new PLIO("DataIn77", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i78 = new PLIO("DataIn78", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i79 = new PLIO("DataIn79", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i80 = new PLIO("DataIn80", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i81 = new PLIO("DataIn81", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i82 = new PLIO("DataIn82", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i83 = new PLIO("DataIn83", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i84 = new PLIO("DataIn84", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i85 = new PLIO("DataIn85", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i86 = new PLIO("DataIn86", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i87 = new PLIO("DataIn87", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i88 = new PLIO("DataIn88", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i89 = new PLIO("DataIn89", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i90 = new PLIO("DataIn90", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i91 = new PLIO("DataIn91", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i92 = new PLIO("DataIn92", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i93 = new PLIO("DataIn93", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i94 = new PLIO("DataIn94", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i95 = new PLIO("DataIn95", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i96 = new PLIO("DataIn96", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i97 = new PLIO("DataIn97", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i98 = new PLIO("DataIn98", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i99 = new PLIO("DataIn99", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o00 = new PLIO("DataOut0",  plio_128_bits, "data/output00.txt");
   PLIO *attr_o01 = new PLIO("DataOut1",  plio_128_bits, "data/output01.txt");
   PLIO *attr_o02 = new PLIO("DataOut2",  plio_128_bits, "data/output02.txt");
   PLIO *attr_o03 = new PLIO("DataOut3",  plio_128_bits, "data/output03.txt");
   PLIO *attr_o04 = new PLIO("DataOut4",  plio_128_bits, "data/output04.txt");
   PLIO *attr_o05 = new PLIO("DataOut5",  plio_128_bits, "data/output05.txt");
   PLIO *attr_o06 = new PLIO("DataOut6",  plio_128_bits, "data/output06.txt");
   PLIO *attr_o07 = new PLIO("DataOut7",  plio_128_bits, "data/output07.txt");
   PLIO *attr_o08 = new PLIO("DataOut8",  plio_128_bits, "data/output08.txt");
   PLIO *attr_o09 = new PLIO("DataOut9",  plio_128_bits, "data/output09.txt");
   PLIO *attr_o10 = new PLIO("DataOut10", plio_128_bits, "data/output10.txt");
   PLIO *attr_o11 = new PLIO("DataOut11", plio_128_bits, "data/output11.txt");
   PLIO *attr_o12 = new PLIO("DataOut12", plio_128_bits, "data/output12.txt");
   PLIO *attr_o13 = new PLIO("DataOut13", plio_128_bits, "data/output13.txt");
   PLIO *attr_o14 = new PLIO("DataOut14", plio_128_bits, "data/output14.txt");
   PLIO *attr_o15 = new PLIO("DataOut15", plio_128_bits, "data/output15.txt");
   PLIO *attr_o16 = new PLIO("DataOut16", plio_128_bits, "data/output16.txt");
   PLIO *attr_o17 = new PLIO("DataOut17", plio_128_bits, "data/output17.txt");
   PLIO *attr_o18 = new PLIO("DataOut18", plio_128_bits, "data/output18.txt");
   PLIO *attr_o19 = new PLIO("DataOut19", plio_128_bits, "data/output19.txt");
   PLIO *attr_o20 = new PLIO("DataOut20", plio_128_bits, "data/output20.txt");
   PLIO *attr_o21 = new PLIO("DataOut21", plio_128_bits, "data/output21.txt");
   PLIO *attr_o22 = new PLIO("DataOut22", plio_128_bits, "data/output22.txt");
   PLIO *attr_o23 = new PLIO("DataOut23", plio_128_bits, "data/output23.txt");
   PLIO *attr_o24 = new PLIO("DataOut24", plio_128_bits, "data/output24.txt");
   PLIO *attr_o25 = new PLIO("DataOut25", plio_128_bits, "data/output25.txt");
   PLIO *attr_o26 = new PLIO("DataOut26", plio_128_bits, "data/output26.txt");
   PLIO *attr_o27 = new PLIO("DataOut27", plio_128_bits, "data/output27.txt");
   PLIO *attr_o28 = new PLIO("DataOut28", plio_128_bits, "data/output28.txt");
   PLIO *attr_o29 = new PLIO("DataOut29", plio_128_bits, "data/output29.txt");
   PLIO *attr_o30 = new PLIO("DataOut30", plio_128_bits, "data/output30.txt");
   PLIO *attr_o31 = new PLIO("DataOut31", plio_128_bits, "data/output31.txt");
   PLIO *attr_o32 = new PLIO("DataOut32", plio_128_bits, "data/output32.txt");
   PLIO *attr_o33 = new PLIO("DataOut33", plio_128_bits, "data/output33.txt");
   PLIO *attr_o34 = new PLIO("DataOut34", plio_128_bits, "data/output34.txt");
   PLIO *attr_o35 = new PLIO("DataOut35", plio_128_bits, "data/output35.txt");
   PLIO *attr_o36 = new PLIO("DataOut36", plio_128_bits, "data/output36.txt");
   PLIO *attr_o37 = new PLIO("DataOut37", plio_128_bits, "data/output37.txt");
   PLIO *attr_o38 = new PLIO("DataOut38", plio_128_bits, "data/output38.txt");
   PLIO *attr_o39 = new PLIO("DataOut39", plio_128_bits, "data/output39.txt");
   PLIO *attr_o40 = new PLIO("DataOut40", plio_128_bits, "data/output40.txt");
   PLIO *attr_o41 = new PLIO("DataOut41", plio_128_bits, "data/output41.txt");
   PLIO *attr_o42 = new PLIO("DataOut42", plio_128_bits, "data/output42.txt");
   PLIO *attr_o43 = new PLIO("DataOut43", plio_128_bits, "data/output43.txt");
   PLIO *attr_o44 = new PLIO("DataOut44", plio_128_bits, "data/output44.txt");
   PLIO *attr_o45 = new PLIO("DataOut45", plio_128_bits, "data/output45.txt");
   PLIO *attr_o46 = new PLIO("DataOut46", plio_128_bits, "data/output46.txt");
   PLIO *attr_o47 = new PLIO("DataOut47", plio_128_bits, "data/output47.txt");
   PLIO *attr_o48 = new PLIO("DataOut48", plio_128_bits, "data/output48.txt");
   PLIO *attr_o49 = new PLIO("DataOut49", plio_128_bits, "data/output49.txt");
   PLIO *attr_o50 = new PLIO("DataOut50", plio_128_bits, "data/output50.txt");
   PLIO *attr_o51 = new PLIO("DataOut51", plio_128_bits, "data/output51.txt");
   PLIO *attr_o52 = new PLIO("DataOut52", plio_128_bits, "data/output52.txt");
   PLIO *attr_o53 = new PLIO("DataOut53", plio_128_bits, "data/output53.txt");
   PLIO *attr_o54 = new PLIO("DataOut54", plio_128_bits, "data/output54.txt");
   PLIO *attr_o55 = new PLIO("DataOut55", plio_128_bits, "data/output55.txt");
   PLIO *attr_o56 = new PLIO("DataOut56", plio_128_bits, "data/output56.txt");
   PLIO *attr_o57 = new PLIO("DataOut57", plio_128_bits, "data/output57.txt");
   PLIO *attr_o58 = new PLIO("DataOut58", plio_128_bits, "data/output58.txt");
   PLIO *attr_o59 = new PLIO("DataOut59", plio_128_bits, "data/output59.txt");
   PLIO *attr_o60 = new PLIO("DataOut60", plio_128_bits, "data/output60.txt");
   PLIO *attr_o61 = new PLIO("DataOut61", plio_128_bits, "data/output61.txt");
   PLIO *attr_o62 = new PLIO("DataOut62", plio_128_bits, "data/output62.txt");
   PLIO *attr_o63 = new PLIO("DataOut63", plio_128_bits, "data/output63.txt");
   PLIO *attr_o64 = new PLIO("DataOut64", plio_128_bits, "data/output64.txt");
   PLIO *attr_o65 = new PLIO("DataOut65", plio_128_bits, "data/output65.txt");
   PLIO *attr_o66 = new PLIO("DataOut66", plio_128_bits, "data/output66.txt");
   PLIO *attr_o67 = new PLIO("DataOut67", plio_128_bits, "data/output67.txt");
   PLIO *attr_o68 = new PLIO("DataOut68", plio_128_bits, "data/output68.txt");
   PLIO *attr_o69 = new PLIO("DataOut69", plio_128_bits, "data/output69.txt");
   PLIO *attr_o70 = new PLIO("DataOut70", plio_128_bits, "data/output70.txt");
   PLIO *attr_o71 = new PLIO("DataOut71", plio_128_bits, "data/output71.txt");
   PLIO *attr_o72 = new PLIO("DataOut72", plio_128_bits, "data/output72.txt");
   PLIO *attr_o73 = new PLIO("DataOut73", plio_128_bits, "data/output73.txt");
   PLIO *attr_o74 = new PLIO("DataOut74", plio_128_bits, "data/output74.txt");
   PLIO *attr_o75 = new PLIO("DataOut75", plio_128_bits, "data/output75.txt");
   PLIO *attr_o76 = new PLIO("DataOut76", plio_128_bits, "data/output76.txt");
   PLIO *attr_o77 = new PLIO("DataOut77", plio_128_bits, "data/output77.txt");
   PLIO *attr_o78 = new PLIO("DataOut78", plio_128_bits, "data/output78.txt");
   PLIO *attr_o79 = new PLIO("DataOut79", plio_128_bits, "data/output79.txt");
   PLIO *attr_o80 = new PLIO("DataOut80", plio_128_bits, "data/output80.txt");
   PLIO *attr_o81 = new PLIO("DataOut81", plio_128_bits, "data/output81.txt");
   PLIO *attr_o82 = new PLIO("DataOut82", plio_128_bits, "data/output82.txt");
   PLIO *attr_o83 = new PLIO("DataOut83", plio_128_bits, "data/output83.txt");
   PLIO *attr_o84 = new PLIO("DataOut84", plio_128_bits, "data/output84.txt");
   PLIO *attr_o85 = new PLIO("DataOut85", plio_128_bits, "data/output85.txt");
   PLIO *attr_o86 = new PLIO("DataOut86", plio_128_bits, "data/output86.txt");
   PLIO *attr_o87 = new PLIO("DataOut87", plio_128_bits, "data/output87.txt");
   PLIO *attr_o88 = new PLIO("DataOut88", plio_128_bits, "data/output88.txt");
   PLIO *attr_o89 = new PLIO("DataOut89", plio_128_bits, "data/output89.txt");
   PLIO *attr_o90 = new PLIO("DataOut90", plio_128_bits, "data/output90.txt");
   PLIO *attr_o91 = new PLIO("DataOut91", plio_128_bits, "data/output91.txt");
   PLIO *attr_o92 = new PLIO("DataOut92", plio_128_bits, "data/output92.txt");
   PLIO *attr_o93 = new PLIO("DataOut93", plio_128_bits, "data/output93.txt");
   PLIO *attr_o94 = new PLIO("DataOut94", plio_128_bits, "data/output94.txt");
   PLIO *attr_o95 = new PLIO("DataOut95", plio_128_bits, "data/output95.txt");
   PLIO *attr_o96 = new PLIO("DataOut96", plio_128_bits, "data/output96.txt");
   PLIO *attr_o97 = new PLIO("DataOut97", plio_128_bits, "data/output97.txt");
   PLIO *attr_o98 = new PLIO("DataOut98", plio_128_bits, "data/output98.txt");
   PLIO *attr_o99 = new PLIO("DataOut99", plio_128_bits, "data/output99.txt");
   
   simulation::platform<100, 100> plat( attr_i00,
   											    attr_i01,
   											    attr_i02,
   											    attr_i03,
   											    attr_i04,
   											    attr_i05,
   											    attr_i06,
   											    attr_i07,
   											    attr_i08,
   											    attr_i09,
   											    attr_i10,
   											    attr_i11,
   											    attr_i12,
   											    attr_i13,
   											    attr_i14,
   											    attr_i15,
   											    attr_i16,
   											    attr_i17,
   											    attr_i18,
   											    attr_i19,
   											    attr_i20,
   											    attr_i21,
   											    attr_i22,
   											    attr_i23,
   											    attr_i24,
   											    attr_i25,
   											    attr_i26,
   											    attr_i27,
   											    attr_i28,
   											    attr_i29,
   											    attr_i30,
   											    attr_i31,
   											    attr_i32,
   											    attr_i33,
   											    attr_i34,
   											    attr_i35,
   											    attr_i36,
   											    attr_i37,
   											    attr_i38,
   											    attr_i39,
   											    attr_i40,
   											    attr_i41,
   											    attr_i42,
   											    attr_i43,
   											    attr_i44,
   											    attr_i45,
   											    attr_i46,
   											    attr_i47,
   											    attr_i48,
   											    attr_i49,
   											    attr_i50,
   											    attr_i51,
   											    attr_i52,
   											    attr_i53,
   											    attr_i54,
   											    attr_i55,
   											    attr_i56,
   											    attr_i57,
   											    attr_i58,
   											    attr_i59,
   											    attr_i60,
   											    attr_i61,
   											    attr_i62,
   											    attr_i63,
   											    attr_i64,
   											    attr_i65,
   											    attr_i66,
   											    attr_i67,
   											    attr_i68,
   											    attr_i69,
   											    attr_i70,
   											    attr_i71,
   											    attr_i72,
   											    attr_i73,
   											    attr_i74,
   											    attr_i75,
   											    attr_i76,
   											    attr_i77,
   											    attr_i78,
   											    attr_i79,
   											    attr_i80,
   											    attr_i81,
   											    attr_i82,
   											    attr_i83,
   											    attr_i84,
   											    attr_i85,
   											    attr_i86,
   											    attr_i87,
   											    attr_i88,
   											    attr_i89,
   											    attr_i90,
   											    attr_i91,
   											    attr_i92,
   											    attr_i93,
   											    attr_i94,
   											    attr_i95,
   											    attr_i96,
   											    attr_i97,
   											    attr_i98,
   											    attr_i99,
   
   											    attr_o00,
   											    attr_o01,
   											    attr_o02,
   											    attr_o03,
   											    attr_o04,
   											    attr_o05,
   											    attr_o06,
   											    attr_o07,
   											    attr_o08,
   											    attr_o09,
   											    attr_o10,
   											    attr_o11,
   											    attr_o12,
   											    attr_o13,
   											    attr_o14,
   											    attr_o15,
   											    attr_o16,
   											    attr_o17,
   											    attr_o18,
   											    attr_o19,
   											    attr_o20,
   											    attr_o21,
   											    attr_o22,
   											    attr_o23,
   											    attr_o24,
   											    attr_o25,
   											    attr_o26,
   											    attr_o27,
   											    attr_o28,
   											    attr_o29,
   											    attr_o30,
   											    attr_o31,
   											    attr_o32,
   											    attr_o33,
   											    attr_o34,
   											    attr_o35,
   											    attr_o36,
   											    attr_o37,
   											    attr_o38,
   											    attr_o39,
   											    attr_o40,
   											    attr_o41,
   											    attr_o42,
   											    attr_o43,
   											    attr_o44,
   											    attr_o45,
   											    attr_o46,
   											    attr_o47,
   											    attr_o48,
   											    attr_o49,
   											    attr_o50,
   											    attr_o51,
   											    attr_o52,
   											    attr_o53,
   											    attr_o54,
   											    attr_o55,
   											    attr_o56,
   											    attr_o57,
   											    attr_o58,
   											    attr_o59,
   											    attr_o60,
   											    attr_o61,
   											    attr_o62,
   											    attr_o63,
   											    attr_o64,
   											    attr_o65,
   											    attr_o66,
   											    attr_o67,
   											    attr_o68,
   											    attr_o69,
   											    attr_o70,
   											    attr_o71,
   											    attr_o72,
   											    attr_o73,
   											    attr_o74,
   											    attr_o75,
   											    attr_o76,
   											    attr_o77,
   											    attr_o78,
   											    attr_o79,
   											    attr_o80,
   											    attr_o81,
   											    attr_o82,
   											    attr_o83,
   											    attr_o84,
   											    attr_o85,
   											    attr_o86,
   											    attr_o87,
   											    attr_o88,
   											    attr_o89,
   											    attr_o90,
   											    attr_o91,
   											    attr_o92,
   											    attr_o93,
   											    attr_o94,
   											    attr_o95,
   											    attr_o96,
   											    attr_o97,
   											    attr_o98,
   											    attr_o99
   											  );
   
   connect<> neti00(plat.src[ 0], fft_rows[ 0].in);
   connect<> neti01(plat.src[ 1], fft_cols[ 0].in);
   connect<> neti02(plat.src[ 2], fft_rows[ 1].in);
   connect<> neti03(plat.src[ 3], fft_cols[ 1].in);
   connect<> neti04(plat.src[ 4], fft_rows[ 2].in);
   connect<> neti05(plat.src[ 5], fft_cols[ 2].in);
   connect<> neti06(plat.src[ 6], fft_rows[ 3].in);
   connect<> neti07(plat.src[ 7], fft_cols[ 3].in);
   connect<> neti08(plat.src[ 8], fft_rows[ 4].in);
   connect<> neti09(plat.src[ 9], fft_cols[ 4].in);
   connect<> neti10(plat.src[10], fft_rows[ 5].in);
   connect<> neti11(plat.src[11], fft_cols[ 5].in);
   connect<> neti12(plat.src[12], fft_rows[ 6].in);
   connect<> neti13(plat.src[13], fft_cols[ 6].in);
   connect<> neti14(plat.src[14], fft_rows[ 7].in);
   connect<> neti15(plat.src[15], fft_cols[ 7].in);
   connect<> neti16(plat.src[16], fft_rows[ 8].in);
   connect<> neti17(plat.src[17], fft_cols[ 8].in);
   connect<> neti18(plat.src[18], fft_rows[ 9].in);
   connect<> neti19(plat.src[19], fft_cols[ 9].in);
   connect<> neti20(plat.src[20], fft_rows[10].in);
   connect<> neti21(plat.src[21], fft_cols[10].in);
   connect<> neti22(plat.src[22], fft_rows[11].in);
   connect<> neti23(plat.src[23], fft_cols[11].in);
   connect<> neti24(plat.src[24], fft_rows[12].in);
   connect<> neti25(plat.src[25], fft_cols[12].in);
   connect<> neti26(plat.src[26], fft_rows[13].in);
   connect<> neti27(plat.src[27], fft_cols[13].in);
   connect<> neti28(plat.src[28], fft_rows[14].in);
   connect<> neti29(plat.src[29], fft_cols[14].in);
   connect<> neti30(plat.src[30], fft_rows[15].in);
   connect<> neti31(plat.src[31], fft_cols[15].in);
   connect<> neti32(plat.src[32], fft_rows[16].in);
   connect<> neti33(plat.src[33], fft_cols[16].in);
   connect<> neti34(plat.src[34], fft_rows[17].in);
   connect<> neti35(plat.src[35], fft_cols[17].in);
   connect<> neti36(plat.src[36], fft_rows[18].in);
   connect<> neti37(plat.src[37], fft_cols[18].in);
   connect<> neti38(plat.src[38], fft_rows[19].in);
   connect<> neti39(plat.src[39], fft_cols[19].in);
   connect<> neti40(plat.src[40], fft_rows[20].in);
   connect<> neti41(plat.src[41], fft_cols[20].in);
   connect<> neti42(plat.src[42], fft_rows[21].in);
   connect<> neti43(plat.src[43], fft_cols[21].in);
   connect<> neti44(plat.src[44], fft_rows[22].in);
   connect<> neti45(plat.src[45], fft_cols[22].in);
   connect<> neti46(plat.src[46], fft_rows[23].in);
   connect<> neti47(plat.src[47], fft_cols[23].in);
   connect<> neti48(plat.src[48], fft_rows[24].in);
   connect<> neti49(plat.src[49], fft_cols[24].in);
   connect<> neti50(plat.src[50], fft_rows[25].in);
   connect<> neti51(plat.src[51], fft_cols[25].in);
   connect<> neti52(plat.src[52], fft_rows[26].in);
   connect<> neti53(plat.src[53], fft_cols[26].in);
   connect<> neti54(plat.src[54], fft_rows[27].in);
   connect<> neti55(plat.src[55], fft_cols[27].in);
   connect<> neti56(plat.src[56], fft_rows[28].in);
   connect<> neti57(plat.src[57], fft_cols[28].in);
   connect<> neti58(plat.src[58], fft_rows[29].in);
   connect<> neti59(plat.src[59], fft_cols[29].in);
   connect<> neti60(plat.src[60], fft_rows[30].in);
   connect<> neti61(plat.src[61], fft_cols[30].in);
   connect<> neti62(plat.src[62], fft_rows[31].in);
   connect<> neti63(plat.src[63], fft_cols[31].in);
   connect<> neti64(plat.src[64], fft_rows[32].in);
   connect<> neti65(plat.src[65], fft_cols[32].in);
   connect<> neti66(plat.src[66], fft_rows[33].in);
   connect<> neti67(plat.src[67], fft_cols[33].in);
   connect<> neti68(plat.src[68], fft_rows[34].in);
   connect<> neti69(plat.src[69], fft_cols[34].in);
   connect<> neti70(plat.src[70], fft_rows[35].in);
   connect<> neti71(plat.src[71], fft_cols[35].in);
   connect<> neti72(plat.src[72], fft_rows[36].in);
   connect<> neti73(plat.src[73], fft_cols[36].in);
   connect<> neti74(plat.src[74], fft_rows[37].in);
   connect<> neti75(plat.src[75], fft_cols[37].in);
   connect<> neti76(plat.src[76], fft_rows[38].in);
   connect<> neti77(plat.src[77], fft_cols[38].in);
   connect<> neti78(plat.src[78], fft_rows[39].in);
   connect<> neti79(plat.src[79], fft_cols[39].in);
   connect<> neti80(plat.src[80], fft_rows[40].in);
   connect<> neti81(plat.src[81], fft_cols[40].in);
   connect<> neti82(plat.src[82], fft_rows[41].in);
   connect<> neti83(plat.src[83], fft_cols[41].in);
   connect<> neti84(plat.src[84], fft_rows[42].in);
   connect<> neti85(plat.src[85], fft_cols[42].in);
   connect<> neti86(plat.src[86], fft_rows[43].in);
   connect<> neti87(plat.src[87], fft_cols[43].in);
   connect<> neti88(plat.src[88], fft_rows[44].in);
   connect<> neti89(plat.src[89], fft_cols[44].in);
   connect<> neti90(plat.src[90], fft_rows[45].in);
   connect<> neti91(plat.src[91], fft_cols[45].in);
   connect<> neti92(plat.src[92], fft_rows[46].in);
   connect<> neti93(plat.src[93], fft_cols[46].in);
   connect<> neti94(plat.src[94], fft_rows[47].in);
   connect<> neti95(plat.src[95], fft_cols[47].in);
   connect<> neti96(plat.src[96], fft_rows[48].in);
   connect<> neti97(plat.src[97], fft_cols[48].in);
   connect<> neti98(plat.src[98], fft_rows[49].in);
   connect<> neti99(plat.src[99], fft_cols[49].in);
   
   connect<> neto00(fft_rows[ 0].out, plat.sink[ 0]);
   connect<> neto01(fft_cols[ 0].out, plat.sink[ 1]);
   connect<> neto02(fft_rows[ 1].out, plat.sink[ 2]);
   connect<> neto03(fft_cols[ 1].out, plat.sink[ 3]);
   connect<> neto04(fft_rows[ 2].out, plat.sink[ 4]);
   connect<> neto05(fft_cols[ 2].out, plat.sink[ 5]);
   connect<> neto06(fft_rows[ 3].out, plat.sink[ 6]);
   connect<> neto07(fft_cols[ 3].out, plat.sink[ 7]);
   connect<> neto08(fft_rows[ 4].out, plat.sink[ 8]);
   connect<> neto09(fft_cols[ 4].out, plat.sink[ 9]);
   connect<> neto10(fft_rows[ 5].out, plat.sink[10]);
   connect<> neto11(fft_cols[ 5].out, plat.sink[11]);
   connect<> neto12(fft_rows[ 6].out, plat.sink[12]);
   connect<> neto13(fft_cols[ 6].out, plat.sink[13]);
   connect<> neto14(fft_rows[ 7].out, plat.sink[14]);
   connect<> neto15(fft_cols[ 7].out, plat.sink[15]);
   connect<> neto16(fft_rows[ 8].out, plat.sink[16]);
   connect<> neto17(fft_cols[ 8].out, plat.sink[17]);
   connect<> neto18(fft_rows[ 9].out, plat.sink[18]);
   connect<> neto19(fft_cols[ 9].out, plat.sink[19]);
   connect<> neto20(fft_rows[10].out, plat.sink[20]);
   connect<> neto21(fft_cols[10].out, plat.sink[21]);
   connect<> neto22(fft_rows[11].out, plat.sink[22]);
   connect<> neto23(fft_cols[11].out, plat.sink[23]);
   connect<> neto24(fft_rows[12].out, plat.sink[24]);
   connect<> neto25(fft_cols[12].out, plat.sink[25]);
   connect<> neto26(fft_rows[13].out, plat.sink[26]);
   connect<> neto27(fft_cols[13].out, plat.sink[27]);
   connect<> neto28(fft_rows[14].out, plat.sink[28]);
   connect<> neto29(fft_cols[14].out, plat.sink[29]);
   connect<> neto30(fft_rows[15].out, plat.sink[30]);
   connect<> neto31(fft_cols[15].out, plat.sink[31]);
   connect<> neto32(fft_rows[16].out, plat.sink[32]);
   connect<> neto33(fft_cols[16].out, plat.sink[33]);
   connect<> neto34(fft_rows[17].out, plat.sink[34]);
   connect<> neto35(fft_cols[17].out, plat.sink[35]);
   connect<> neto36(fft_rows[18].out, plat.sink[36]);
   connect<> neto37(fft_cols[18].out, plat.sink[37]);
   connect<> neto38(fft_rows[19].out, plat.sink[38]);
   connect<> neto39(fft_cols[19].out, plat.sink[39]);
   connect<> neto40(fft_rows[20].out, plat.sink[40]);
   connect<> neto41(fft_cols[20].out, plat.sink[41]);
   connect<> neto42(fft_rows[21].out, plat.sink[42]);
   connect<> neto43(fft_cols[21].out, plat.sink[43]);
   connect<> neto44(fft_rows[22].out, plat.sink[44]);
   connect<> neto45(fft_cols[22].out, plat.sink[45]);
   connect<> neto46(fft_rows[23].out, plat.sink[46]);
   connect<> neto47(fft_cols[23].out, plat.sink[47]);
   connect<> neto48(fft_rows[24].out, plat.sink[48]);
   connect<> neto49(fft_cols[24].out, plat.sink[49]);
   connect<> neto50(fft_rows[25].out, plat.sink[50]);
   connect<> neto51(fft_cols[25].out, plat.sink[51]);
   connect<> neto52(fft_rows[26].out, plat.sink[52]);
   connect<> neto53(fft_cols[26].out, plat.sink[53]);
   connect<> neto54(fft_rows[27].out, plat.sink[54]);
   connect<> neto55(fft_cols[27].out, plat.sink[55]);
   connect<> neto56(fft_rows[28].out, plat.sink[56]);
   connect<> neto57(fft_cols[28].out, plat.sink[57]);
   connect<> neto58(fft_rows[29].out, plat.sink[58]);
   connect<> neto59(fft_cols[29].out, plat.sink[59]);
   connect<> neto60(fft_rows[30].out, plat.sink[60]);
   connect<> neto61(fft_cols[30].out, plat.sink[61]);
   connect<> neto62(fft_rows[31].out, plat.sink[62]);
   connect<> neto63(fft_cols[31].out, plat.sink[63]);
   connect<> neto64(fft_rows[32].out, plat.sink[64]);
   connect<> neto65(fft_cols[32].out, plat.sink[65]);
   connect<> neto66(fft_rows[33].out, plat.sink[66]);
   connect<> neto67(fft_cols[33].out, plat.sink[67]);
   connect<> neto68(fft_rows[34].out, plat.sink[68]);
   connect<> neto69(fft_cols[34].out, plat.sink[69]);
   connect<> neto70(fft_rows[35].out, plat.sink[70]);
   connect<> neto71(fft_cols[35].out, plat.sink[71]);
   connect<> neto72(fft_rows[36].out, plat.sink[72]);
   connect<> neto73(fft_cols[36].out, plat.sink[73]);
   connect<> neto74(fft_rows[37].out, plat.sink[74]);
   connect<> neto75(fft_cols[37].out, plat.sink[75]);
   connect<> neto76(fft_rows[38].out, plat.sink[76]);
   connect<> neto77(fft_cols[38].out, plat.sink[77]);
   connect<> neto78(fft_rows[39].out, plat.sink[78]);
   connect<> neto79(fft_cols[39].out, plat.sink[79]);
   connect<> neto80(fft_rows[40].out, plat.sink[80]);
   connect<> neto81(fft_cols[40].out, plat.sink[81]);
   connect<> neto82(fft_rows[41].out, plat.sink[82]);
   connect<> neto83(fft_cols[41].out, plat.sink[83]);
   connect<> neto84(fft_rows[42].out, plat.sink[84]);
   connect<> neto85(fft_cols[42].out, plat.sink[85]);
   connect<> neto86(fft_rows[43].out, plat.sink[86]);
   connect<> neto87(fft_cols[43].out, plat.sink[87]);
   connect<> neto88(fft_rows[44].out, plat.sink[88]);
   connect<> neto89(fft_cols[44].out, plat.sink[89]);
   connect<> neto90(fft_rows[45].out, plat.sink[90]);
   connect<> neto91(fft_cols[45].out, plat.sink[91]);
   connect<> neto92(fft_rows[46].out, plat.sink[92]);
   connect<> neto93(fft_cols[46].out, plat.sink[93]);
   connect<> neto94(fft_rows[47].out, plat.sink[94]);
   connect<> neto95(fft_cols[47].out, plat.sink[95]);
   connect<> neto96(fft_rows[48].out, plat.sink[96]);
   connect<> neto97(fft_cols[48].out, plat.sink[97]);
   connect<> neto98(fft_rows[49].out, plat.sink[98]);
   connect<> neto99(fft_cols[49].out, plat.sink[99]);

#elif FFT2D_INSTS == 60

   PLIO *attr_i000 = new PLIO("DataIn0",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i001 = new PLIO("DataIn1",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i002 = new PLIO("DataIn2",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i003 = new PLIO("DataIn3",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i004 = new PLIO("DataIn4",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i005 = new PLIO("DataIn5",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i006 = new PLIO("DataIn6",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i007 = new PLIO("DataIn7",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i008 = new PLIO("DataIn8",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i009 = new PLIO("DataIn9",   plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i010 = new PLIO("DataIn10",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i011 = new PLIO("DataIn11",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i012 = new PLIO("DataIn12",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i013 = new PLIO("DataIn13",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i014 = new PLIO("DataIn14",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i015 = new PLIO("DataIn15",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i016 = new PLIO("DataIn16",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i017 = new PLIO("DataIn17",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i018 = new PLIO("DataIn18",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i019 = new PLIO("DataIn19",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i020 = new PLIO("DataIn20",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i021 = new PLIO("DataIn21",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i022 = new PLIO("DataIn22",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i023 = new PLIO("DataIn23",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i024 = new PLIO("DataIn24",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i025 = new PLIO("DataIn25",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i026 = new PLIO("DataIn26",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i027 = new PLIO("DataIn27",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i028 = new PLIO("DataIn28",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i029 = new PLIO("DataIn29",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i030 = new PLIO("DataIn30",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i031 = new PLIO("DataIn31",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i032 = new PLIO("DataIn32",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i033 = new PLIO("DataIn33",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i034 = new PLIO("DataIn34",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i035 = new PLIO("DataIn35",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i036 = new PLIO("DataIn36",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i037 = new PLIO("DataIn37",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i038 = new PLIO("DataIn38",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i039 = new PLIO("DataIn39",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i040 = new PLIO("DataIn40",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i041 = new PLIO("DataIn41",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i042 = new PLIO("DataIn42",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i043 = new PLIO("DataIn43",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i044 = new PLIO("DataIn44",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i045 = new PLIO("DataIn45",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i046 = new PLIO("DataIn46",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i047 = new PLIO("DataIn47",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i048 = new PLIO("DataIn48",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i049 = new PLIO("DataIn49",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i050 = new PLIO("DataIn50",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i051 = new PLIO("DataIn51",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i052 = new PLIO("DataIn52",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i053 = new PLIO("DataIn53",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i054 = new PLIO("DataIn54",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i055 = new PLIO("DataIn55",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i056 = new PLIO("DataIn56",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i057 = new PLIO("DataIn57",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i058 = new PLIO("DataIn58",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i059 = new PLIO("DataIn59",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i060 = new PLIO("DataIn60",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i061 = new PLIO("DataIn61",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i062 = new PLIO("DataIn62",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i063 = new PLIO("DataIn63",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i064 = new PLIO("DataIn64",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i065 = new PLIO("DataIn65",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i066 = new PLIO("DataIn66",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i067 = new PLIO("DataIn67",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i068 = new PLIO("DataIn68",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i069 = new PLIO("DataIn69",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i070 = new PLIO("DataIn70",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i071 = new PLIO("DataIn71",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i072 = new PLIO("DataIn72",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i073 = new PLIO("DataIn73",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i074 = new PLIO("DataIn74",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i075 = new PLIO("DataIn75",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i076 = new PLIO("DataIn76",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i077 = new PLIO("DataIn77",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i078 = new PLIO("DataIn78",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i079 = new PLIO("DataIn79",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i080 = new PLIO("DataIn80",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i081 = new PLIO("DataIn81",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i082 = new PLIO("DataIn82",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i083 = new PLIO("DataIn83",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i084 = new PLIO("DataIn84",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i085 = new PLIO("DataIn85",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i086 = new PLIO("DataIn86",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i087 = new PLIO("DataIn87",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i088 = new PLIO("DataIn88",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i089 = new PLIO("DataIn89",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i090 = new PLIO("DataIn90",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i091 = new PLIO("DataIn91",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i092 = new PLIO("DataIn92",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i093 = new PLIO("DataIn93",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i094 = new PLIO("DataIn94",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i095 = new PLIO("DataIn95",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i096 = new PLIO("DataIn96",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i097 = new PLIO("DataIn97",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i098 = new PLIO("DataIn98",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i099 = new PLIO("DataIn99",  plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i100 = new PLIO("DataIn100", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i101 = new PLIO("DataIn101", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i102 = new PLIO("DataIn102", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i103 = new PLIO("DataIn103", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i104 = new PLIO("DataIn104", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i105 = new PLIO("DataIn105", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i106 = new PLIO("DataIn106", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i107 = new PLIO("DataIn107", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i108 = new PLIO("DataIn108", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i109 = new PLIO("DataIn109", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i110 = new PLIO("DataIn110", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i111 = new PLIO("DataIn111", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i112 = new PLIO("DataIn112", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i113 = new PLIO("DataIn113", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i114 = new PLIO("DataIn114", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i115 = new PLIO("DataIn115", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i116 = new PLIO("DataIn116", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i117 = new PLIO("DataIn117", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");
   PLIO *attr_i118 = new PLIO("DataIn118", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input0.txt");
   PLIO *attr_i119 = new PLIO("DataIn119", plio_128_bits, "../../../../../design/aie_src/aiesim_data/input1.txt");

   PLIO *attr_o000 = new PLIO("DataOut0",   plio_128_bits, "data/output000.txt");
   PLIO *attr_o001 = new PLIO("DataOut1",   plio_128_bits, "data/output001.txt");
   PLIO *attr_o002 = new PLIO("DataOut2",   plio_128_bits, "data/output002.txt");
   PLIO *attr_o003 = new PLIO("DataOut3",   plio_128_bits, "data/output003.txt");
   PLIO *attr_o004 = new PLIO("DataOut4",   plio_128_bits, "data/output004.txt");
   PLIO *attr_o005 = new PLIO("DataOut5",   plio_128_bits, "data/output005.txt");
   PLIO *attr_o006 = new PLIO("DataOut6",   plio_128_bits, "data/output006.txt");
   PLIO *attr_o007 = new PLIO("DataOut7",   plio_128_bits, "data/output007.txt");
   PLIO *attr_o008 = new PLIO("DataOut8",   plio_128_bits, "data/output008.txt");
   PLIO *attr_o009 = new PLIO("DataOut9",   plio_128_bits, "data/output009.txt");
   PLIO *attr_o010 = new PLIO("DataOut10",  plio_128_bits, "data/output010.txt");
   PLIO *attr_o011 = new PLIO("DataOut11",  plio_128_bits, "data/output011.txt");
   PLIO *attr_o012 = new PLIO("DataOut12",  plio_128_bits, "data/output012.txt");
   PLIO *attr_o013 = new PLIO("DataOut13",  plio_128_bits, "data/output013.txt");
   PLIO *attr_o014 = new PLIO("DataOut14",  plio_128_bits, "data/output014.txt");
   PLIO *attr_o015 = new PLIO("DataOut15",  plio_128_bits, "data/output015.txt");
   PLIO *attr_o016 = new PLIO("DataOut16",  plio_128_bits, "data/output016.txt");
   PLIO *attr_o017 = new PLIO("DataOut17",  plio_128_bits, "data/output017.txt");
   PLIO *attr_o018 = new PLIO("DataOut18",  plio_128_bits, "data/output018.txt");
   PLIO *attr_o019 = new PLIO("DataOut19",  plio_128_bits, "data/output019.txt");
   PLIO *attr_o020 = new PLIO("DataOut20",  plio_128_bits, "data/output020.txt");
   PLIO *attr_o021 = new PLIO("DataOut21",  plio_128_bits, "data/output021.txt");
   PLIO *attr_o022 = new PLIO("DataOut22",  plio_128_bits, "data/output022.txt");
   PLIO *attr_o023 = new PLIO("DataOut23",  plio_128_bits, "data/output023.txt");
   PLIO *attr_o024 = new PLIO("DataOut24",  plio_128_bits, "data/output024.txt");
   PLIO *attr_o025 = new PLIO("DataOut25",  plio_128_bits, "data/output025.txt");
   PLIO *attr_o026 = new PLIO("DataOut26",  plio_128_bits, "data/output026.txt");
   PLIO *attr_o027 = new PLIO("DataOut27",  plio_128_bits, "data/output027.txt");
   PLIO *attr_o028 = new PLIO("DataOut28",  plio_128_bits, "data/output028.txt");
   PLIO *attr_o029 = new PLIO("DataOut29",  plio_128_bits, "data/output029.txt");
   PLIO *attr_o030 = new PLIO("DataOut30",  plio_128_bits, "data/output030.txt");
   PLIO *attr_o031 = new PLIO("DataOut31",  plio_128_bits, "data/output031.txt");
   PLIO *attr_o032 = new PLIO("DataOut32",  plio_128_bits, "data/output032.txt");
   PLIO *attr_o033 = new PLIO("DataOut33",  plio_128_bits, "data/output033.txt");
   PLIO *attr_o034 = new PLIO("DataOut34",  plio_128_bits, "data/output034.txt");
   PLIO *attr_o035 = new PLIO("DataOut35",  plio_128_bits, "data/output035.txt");
   PLIO *attr_o036 = new PLIO("DataOut36",  plio_128_bits, "data/output036.txt");
   PLIO *attr_o037 = new PLIO("DataOut37",  plio_128_bits, "data/output037.txt");
   PLIO *attr_o038 = new PLIO("DataOut38",  plio_128_bits, "data/output038.txt");
   PLIO *attr_o039 = new PLIO("DataOut39",  plio_128_bits, "data/output039.txt");
   PLIO *attr_o040 = new PLIO("DataOut40",  plio_128_bits, "data/output040.txt");
   PLIO *attr_o041 = new PLIO("DataOut41",  plio_128_bits, "data/output041.txt");
   PLIO *attr_o042 = new PLIO("DataOut42",  plio_128_bits, "data/output042.txt");
   PLIO *attr_o043 = new PLIO("DataOut43",  plio_128_bits, "data/output043.txt");
   PLIO *attr_o044 = new PLIO("DataOut44",  plio_128_bits, "data/output044.txt");
   PLIO *attr_o045 = new PLIO("DataOut45",  plio_128_bits, "data/output045.txt");
   PLIO *attr_o046 = new PLIO("DataOut46",  plio_128_bits, "data/output046.txt");
   PLIO *attr_o047 = new PLIO("DataOut47",  plio_128_bits, "data/output047.txt");
   PLIO *attr_o048 = new PLIO("DataOut48",  plio_128_bits, "data/output048.txt");
   PLIO *attr_o049 = new PLIO("DataOut49",  plio_128_bits, "data/output049.txt");
   PLIO *attr_o050 = new PLIO("DataOut50",  plio_128_bits, "data/output050.txt");
   PLIO *attr_o051 = new PLIO("DataOut51",  plio_128_bits, "data/output051.txt");
   PLIO *attr_o052 = new PLIO("DataOut52",  plio_128_bits, "data/output052.txt");
   PLIO *attr_o053 = new PLIO("DataOut53",  plio_128_bits, "data/output053.txt");
   PLIO *attr_o054 = new PLIO("DataOut54",  plio_128_bits, "data/output054.txt");
   PLIO *attr_o055 = new PLIO("DataOut55",  plio_128_bits, "data/output055.txt");
   PLIO *attr_o056 = new PLIO("DataOut56",  plio_128_bits, "data/output056.txt");
   PLIO *attr_o057 = new PLIO("DataOut57",  plio_128_bits, "data/output057.txt");
   PLIO *attr_o058 = new PLIO("DataOut58",  plio_128_bits, "data/output058.txt");
   PLIO *attr_o059 = new PLIO("DataOut59",  plio_128_bits, "data/output059.txt");
   PLIO *attr_o060 = new PLIO("DataOut60",  plio_128_bits, "data/output060.txt");
   PLIO *attr_o061 = new PLIO("DataOut61",  plio_128_bits, "data/output061.txt");
   PLIO *attr_o062 = new PLIO("DataOut62",  plio_128_bits, "data/output062.txt");
   PLIO *attr_o063 = new PLIO("DataOut63",  plio_128_bits, "data/output063.txt");
   PLIO *attr_o064 = new PLIO("DataOut64",  plio_128_bits, "data/output064.txt");
   PLIO *attr_o065 = new PLIO("DataOut65",  plio_128_bits, "data/output065.txt");
   PLIO *attr_o066 = new PLIO("DataOut66",  plio_128_bits, "data/output066.txt");
   PLIO *attr_o067 = new PLIO("DataOut67",  plio_128_bits, "data/output067.txt");
   PLIO *attr_o068 = new PLIO("DataOut68",  plio_128_bits, "data/output068.txt");
   PLIO *attr_o069 = new PLIO("DataOut69",  plio_128_bits, "data/output069.txt");
   PLIO *attr_o070 = new PLIO("DataOut70",  plio_128_bits, "data/output070.txt");
   PLIO *attr_o071 = new PLIO("DataOut71",  plio_128_bits, "data/output071.txt");
   PLIO *attr_o072 = new PLIO("DataOut72",  plio_128_bits, "data/output072.txt");
   PLIO *attr_o073 = new PLIO("DataOut73",  plio_128_bits, "data/output073.txt");
   PLIO *attr_o074 = new PLIO("DataOut74",  plio_128_bits, "data/output074.txt");
   PLIO *attr_o075 = new PLIO("DataOut75",  plio_128_bits, "data/output075.txt");
   PLIO *attr_o076 = new PLIO("DataOut76",  plio_128_bits, "data/output076.txt");
   PLIO *attr_o077 = new PLIO("DataOut77",  plio_128_bits, "data/output077.txt");
   PLIO *attr_o078 = new PLIO("DataOut78",  plio_128_bits, "data/output078.txt");
   PLIO *attr_o079 = new PLIO("DataOut79",  plio_128_bits, "data/output079.txt");
   PLIO *attr_o080 = new PLIO("DataOut80",  plio_128_bits, "data/output080.txt");
   PLIO *attr_o081 = new PLIO("DataOut81",  plio_128_bits, "data/output081.txt");
   PLIO *attr_o082 = new PLIO("DataOut82",  plio_128_bits, "data/output082.txt");
   PLIO *attr_o083 = new PLIO("DataOut83",  plio_128_bits, "data/output083.txt");
   PLIO *attr_o084 = new PLIO("DataOut84",  plio_128_bits, "data/output084.txt");
   PLIO *attr_o085 = new PLIO("DataOut85",  plio_128_bits, "data/output085.txt");
   PLIO *attr_o086 = new PLIO("DataOut86",  plio_128_bits, "data/output086.txt");
   PLIO *attr_o087 = new PLIO("DataOut87",  plio_128_bits, "data/output087.txt");
   PLIO *attr_o088 = new PLIO("DataOut88",  plio_128_bits, "data/output088.txt");
   PLIO *attr_o089 = new PLIO("DataOut89",  plio_128_bits, "data/output089.txt");
   PLIO *attr_o090 = new PLIO("DataOut90",  plio_128_bits, "data/output090.txt");
   PLIO *attr_o091 = new PLIO("DataOut91",  plio_128_bits, "data/output091.txt");
   PLIO *attr_o092 = new PLIO("DataOut92",  plio_128_bits, "data/output092.txt");
   PLIO *attr_o093 = new PLIO("DataOut93",  plio_128_bits, "data/output093.txt");
   PLIO *attr_o094 = new PLIO("DataOut94",  plio_128_bits, "data/output094.txt");
   PLIO *attr_o095 = new PLIO("DataOut95",  plio_128_bits, "data/output095.txt");
   PLIO *attr_o096 = new PLIO("DataOut96",  plio_128_bits, "data/output096.txt");
   PLIO *attr_o097 = new PLIO("DataOut97",  plio_128_bits, "data/output097.txt");
   PLIO *attr_o098 = new PLIO("DataOut98",  plio_128_bits, "data/output098.txt");
   PLIO *attr_o099 = new PLIO("DataOut99",  plio_128_bits, "data/output099.txt");
   PLIO *attr_o100 = new PLIO("DataOut100", plio_128_bits, "data/output100.txt");
   PLIO *attr_o101 = new PLIO("DataOut101", plio_128_bits, "data/output101.txt");
   PLIO *attr_o102 = new PLIO("DataOut102", plio_128_bits, "data/output102.txt");
   PLIO *attr_o103 = new PLIO("DataOut103", plio_128_bits, "data/output103.txt");
   PLIO *attr_o104 = new PLIO("DataOut104", plio_128_bits, "data/output104.txt");
   PLIO *attr_o105 = new PLIO("DataOut105", plio_128_bits, "data/output105.txt");
   PLIO *attr_o106 = new PLIO("DataOut106", plio_128_bits, "data/output106.txt");
   PLIO *attr_o107 = new PLIO("DataOut107", plio_128_bits, "data/output107.txt");
   PLIO *attr_o108 = new PLIO("DataOut108", plio_128_bits, "data/output108.txt");
   PLIO *attr_o109 = new PLIO("DataOut109", plio_128_bits, "data/output109.txt");
   PLIO *attr_o110 = new PLIO("DataOut110", plio_128_bits, "data/output110.txt");
   PLIO *attr_o111 = new PLIO("DataOut111", plio_128_bits, "data/output111.txt");
   PLIO *attr_o112 = new PLIO("DataOut112", plio_128_bits, "data/output112.txt");
   PLIO *attr_o113 = new PLIO("DataOut113", plio_128_bits, "data/output113.txt");
   PLIO *attr_o114 = new PLIO("DataOut114", plio_128_bits, "data/output114.txt");
   PLIO *attr_o115 = new PLIO("DataOut115", plio_128_bits, "data/output115.txt");
   PLIO *attr_o116 = new PLIO("DataOut116", plio_128_bits, "data/output116.txt");
   PLIO *attr_o117 = new PLIO("DataOut117", plio_128_bits, "data/output117.txt");
   PLIO *attr_o118 = new PLIO("DataOut118", plio_128_bits, "data/output118.txt");
   PLIO *attr_o119 = new PLIO("DataOut119", plio_128_bits, "data/output119.txt");
   
   simulation::platform<120, 120> plat( attr_i000,
   											    attr_i001,
   											    attr_i002,
   											    attr_i003,
   											    attr_i004,
   											    attr_i005,
   											    attr_i006,
   											    attr_i007,
   											    attr_i008,
   											    attr_i009,
   											    attr_i010,
   											    attr_i011,
   											    attr_i012,
   											    attr_i013,
   											    attr_i014,
   											    attr_i015,
   											    attr_i016,
   											    attr_i017,
   											    attr_i018,
   											    attr_i019,
   											    attr_i020,
   											    attr_i021,
   											    attr_i022,
   											    attr_i023,
   											    attr_i024,
   											    attr_i025,
   											    attr_i026,
   											    attr_i027,
   											    attr_i028,
   											    attr_i029,
   											    attr_i030,
   											    attr_i031,
   											    attr_i032,
   											    attr_i033,
   											    attr_i034,
   											    attr_i035,
   											    attr_i036,
   											    attr_i037,
   											    attr_i038,
   											    attr_i039,
   											    attr_i040,
   											    attr_i041,
   											    attr_i042,
   											    attr_i043,
   											    attr_i044,
   											    attr_i045,
   											    attr_i046,
   											    attr_i047,
   											    attr_i048,
   											    attr_i049,
   											    attr_i050,
   											    attr_i051,
   											    attr_i052,
   											    attr_i053,
   											    attr_i054,
   											    attr_i055,
   											    attr_i056,
   											    attr_i057,
   											    attr_i058,
   											    attr_i059,
   											    attr_i060,
   											    attr_i061,
   											    attr_i062,
   											    attr_i063,
   											    attr_i064,
   											    attr_i065,
   											    attr_i066,
   											    attr_i067,
   											    attr_i068,
   											    attr_i069,
   											    attr_i070,
   											    attr_i071,
   											    attr_i072,
   											    attr_i073,
   											    attr_i074,
   											    attr_i075,
   											    attr_i076,
   											    attr_i077,
   											    attr_i078,
   											    attr_i079,
   											    attr_i080,
   											    attr_i081,
   											    attr_i082,
   											    attr_i083,
   											    attr_i084,
   											    attr_i085,
   											    attr_i086,
   											    attr_i087,
   											    attr_i088,
   											    attr_i089,
   											    attr_i090,
   											    attr_i091,
   											    attr_i092,
   											    attr_i093,
   											    attr_i094,
   											    attr_i095,
   											    attr_i096,
   											    attr_i097,
   											    attr_i098,
   											    attr_i099,
   											    attr_i100,
   											    attr_i101,
   											    attr_i102,
   											    attr_i103,
   											    attr_i104,
   											    attr_i105,
   											    attr_i106,
   											    attr_i107,
   											    attr_i108,
   											    attr_i109,
   											    attr_i110,
   											    attr_i111,
   											    attr_i112,
   											    attr_i113,
   											    attr_i114,
   											    attr_i115,
   											    attr_i116,
   											    attr_i117,
   											    attr_i118,
   											    attr_i119,
   
   											    attr_o000,
   											    attr_o001,
   											    attr_o002,
   											    attr_o003,
   											    attr_o004,
   											    attr_o005,
   											    attr_o006,
   											    attr_o007,
   											    attr_o008,
   											    attr_o009,
   											    attr_o010,
   											    attr_o011,
   											    attr_o012,
   											    attr_o013,
   											    attr_o014,
   											    attr_o015,
   											    attr_o016,
   											    attr_o017,
   											    attr_o018,
   											    attr_o019,
   											    attr_o020,
   											    attr_o021,
   											    attr_o022,
   											    attr_o023,
   											    attr_o024,
   											    attr_o025,
   											    attr_o026,
   											    attr_o027,
   											    attr_o028,
   											    attr_o029,
   											    attr_o030,
   											    attr_o031,
   											    attr_o032,
   											    attr_o033,
   											    attr_o034,
   											    attr_o035,
   											    attr_o036,
   											    attr_o037,
   											    attr_o038,
   											    attr_o039,
   											    attr_o040,
   											    attr_o041,
   											    attr_o042,
   											    attr_o043,
   											    attr_o044,
   											    attr_o045,
   											    attr_o046,
   											    attr_o047,
   											    attr_o048,
   											    attr_o049,
   											    attr_o050,
   											    attr_o051,
   											    attr_o052,
   											    attr_o053,
   											    attr_o054,
   											    attr_o055,
   											    attr_o056,
   											    attr_o057,
   											    attr_o058,
   											    attr_o059,
   											    attr_o060,
   											    attr_o061,
   											    attr_o062,
   											    attr_o063,
   											    attr_o064,
   											    attr_o065,
   											    attr_o066,
   											    attr_o067,
   											    attr_o068,
   											    attr_o069,
   											    attr_o070,
   											    attr_o071,
   											    attr_o072,
   											    attr_o073,
   											    attr_o074,
   											    attr_o075,
   											    attr_o076,
   											    attr_o077,
   											    attr_o078,
   											    attr_o079,
   											    attr_o080,
   											    attr_o081,
   											    attr_o082,
   											    attr_o083,
   											    attr_o084,
   											    attr_o085,
   											    attr_o086,
   											    attr_o087,
   											    attr_o088,
   											    attr_o089,
   											    attr_o090,
   											    attr_o091,
   											    attr_o092,
   											    attr_o093,
   											    attr_o094,
   											    attr_o095,
   											    attr_o096,
   											    attr_o097,
   											    attr_o098,
   											    attr_o099,
   											    attr_o100,
   											    attr_o101,
   											    attr_o102,
   											    attr_o103,
   											    attr_o104,
   											    attr_o105,
   											    attr_o106,
   											    attr_o107,
   											    attr_o108,
   											    attr_o109,
   											    attr_o110,
   											    attr_o111,
   											    attr_o112,
   											    attr_o113,
   											    attr_o114,
   											    attr_o115,
   											    attr_o116,
   											    attr_o117,
   											    attr_o118,
   											    attr_o119
   											  );
   
   connect<> neti000(plat.src[  0], fft_rows[ 0].in);
   connect<> neti001(plat.src[  1], fft_cols[ 0].in);
   connect<> neti002(plat.src[  2], fft_rows[ 1].in);
   connect<> neti003(plat.src[  3], fft_cols[ 1].in);
   connect<> neti004(plat.src[  4], fft_rows[ 2].in);
   connect<> neti005(plat.src[  5], fft_cols[ 2].in);
   connect<> neti006(plat.src[  6], fft_rows[ 3].in);
   connect<> neti007(plat.src[  7], fft_cols[ 3].in);
   connect<> neti008(plat.src[  8], fft_rows[ 4].in);
   connect<> neti009(plat.src[  9], fft_cols[ 4].in);
   connect<> neti010(plat.src[ 10], fft_rows[ 5].in);
   connect<> neti011(plat.src[ 11], fft_cols[ 5].in);
   connect<> neti012(plat.src[ 12], fft_rows[ 6].in);
   connect<> neti013(plat.src[ 13], fft_cols[ 6].in);
   connect<> neti014(plat.src[ 14], fft_rows[ 7].in);
   connect<> neti015(plat.src[ 15], fft_cols[ 7].in);
   connect<> neti016(plat.src[ 16], fft_rows[ 8].in);
   connect<> neti017(plat.src[ 17], fft_cols[ 8].in);
   connect<> neti018(plat.src[ 18], fft_rows[ 9].in);
   connect<> neti019(plat.src[ 19], fft_cols[ 9].in);
   connect<> neti020(plat.src[ 20], fft_rows[10].in);
   connect<> neti021(plat.src[ 21], fft_cols[10].in);
   connect<> neti022(plat.src[ 22], fft_rows[11].in);
   connect<> neti023(plat.src[ 23], fft_cols[11].in);
   connect<> neti024(plat.src[ 24], fft_rows[12].in);
   connect<> neti025(plat.src[ 25], fft_cols[12].in);
   connect<> neti026(plat.src[ 26], fft_rows[13].in);
   connect<> neti027(plat.src[ 27], fft_cols[13].in);
   connect<> neti028(plat.src[ 28], fft_rows[14].in);
   connect<> neti029(plat.src[ 29], fft_cols[14].in);
   connect<> neti030(plat.src[ 30], fft_rows[15].in);
   connect<> neti031(plat.src[ 31], fft_cols[15].in);
   connect<> neti032(plat.src[ 32], fft_rows[16].in);
   connect<> neti033(plat.src[ 33], fft_cols[16].in);
   connect<> neti034(plat.src[ 34], fft_rows[17].in);
   connect<> neti035(plat.src[ 35], fft_cols[17].in);
   connect<> neti036(plat.src[ 36], fft_rows[18].in);
   connect<> neti037(plat.src[ 37], fft_cols[18].in);
   connect<> neti038(plat.src[ 38], fft_rows[19].in);
   connect<> neti039(plat.src[ 39], fft_cols[19].in);
   connect<> neti040(plat.src[ 40], fft_rows[20].in);
   connect<> neti041(plat.src[ 41], fft_cols[20].in);
   connect<> neti042(plat.src[ 42], fft_rows[21].in);
   connect<> neti043(plat.src[ 43], fft_cols[21].in);
   connect<> neti044(plat.src[ 44], fft_rows[22].in);
   connect<> neti045(plat.src[ 45], fft_cols[22].in);
   connect<> neti046(plat.src[ 46], fft_rows[23].in);
   connect<> neti047(plat.src[ 47], fft_cols[23].in);
   connect<> neti048(plat.src[ 48], fft_rows[24].in);
   connect<> neti049(plat.src[ 49], fft_cols[24].in);
   connect<> neti050(plat.src[ 50], fft_rows[25].in);
   connect<> neti051(plat.src[ 51], fft_cols[25].in);
   connect<> neti052(plat.src[ 52], fft_rows[26].in);
   connect<> neti053(plat.src[ 53], fft_cols[26].in);
   connect<> neti054(plat.src[ 54], fft_rows[27].in);
   connect<> neti055(plat.src[ 55], fft_cols[27].in);
   connect<> neti056(plat.src[ 56], fft_rows[28].in);
   connect<> neti057(plat.src[ 57], fft_cols[28].in);
   connect<> neti058(plat.src[ 58], fft_rows[29].in);
   connect<> neti059(plat.src[ 59], fft_cols[29].in);
   connect<> neti060(plat.src[ 60], fft_rows[30].in);
   connect<> neti061(plat.src[ 61], fft_cols[30].in);
   connect<> neti062(plat.src[ 62], fft_rows[31].in);
   connect<> neti063(plat.src[ 63], fft_cols[31].in);
   connect<> neti064(plat.src[ 64], fft_rows[32].in);
   connect<> neti065(plat.src[ 65], fft_cols[32].in);
   connect<> neti066(plat.src[ 66], fft_rows[33].in);
   connect<> neti067(plat.src[ 67], fft_cols[33].in);
   connect<> neti068(plat.src[ 68], fft_rows[34].in);
   connect<> neti069(plat.src[ 69], fft_cols[34].in);
   connect<> neti070(plat.src[ 70], fft_rows[35].in);
   connect<> neti071(plat.src[ 71], fft_cols[35].in);
   connect<> neti072(plat.src[ 72], fft_rows[36].in);
   connect<> neti073(plat.src[ 73], fft_cols[36].in);
   connect<> neti074(plat.src[ 74], fft_rows[37].in);
   connect<> neti075(plat.src[ 75], fft_cols[37].in);
   connect<> neti076(plat.src[ 76], fft_rows[38].in);
   connect<> neti077(plat.src[ 77], fft_cols[38].in);
   connect<> neti078(plat.src[ 78], fft_rows[39].in);
   connect<> neti079(plat.src[ 79], fft_cols[39].in);
   connect<> neti080(plat.src[ 80], fft_rows[40].in);
   connect<> neti081(plat.src[ 81], fft_cols[40].in);
   connect<> neti082(plat.src[ 82], fft_rows[41].in);
   connect<> neti083(plat.src[ 83], fft_cols[41].in);
   connect<> neti084(plat.src[ 84], fft_rows[42].in);
   connect<> neti085(plat.src[ 85], fft_cols[42].in);
   connect<> neti086(plat.src[ 86], fft_rows[43].in);
   connect<> neti087(plat.src[ 87], fft_cols[43].in);
   connect<> neti088(plat.src[ 88], fft_rows[44].in);
   connect<> neti089(plat.src[ 89], fft_cols[44].in);
   connect<> neti090(plat.src[ 90], fft_rows[45].in);
   connect<> neti091(plat.src[ 91], fft_cols[45].in);
   connect<> neti092(plat.src[ 92], fft_rows[46].in);
   connect<> neti093(plat.src[ 93], fft_cols[46].in);
   connect<> neti094(plat.src[ 94], fft_rows[47].in);
   connect<> neti095(plat.src[ 95], fft_cols[47].in);
   connect<> neti096(plat.src[ 96], fft_rows[48].in);
   connect<> neti097(plat.src[ 97], fft_cols[48].in);
   connect<> neti098(plat.src[ 98], fft_rows[49].in);
   connect<> neti099(plat.src[ 99], fft_cols[49].in);
   connect<> neti100(plat.src[100], fft_rows[50].in);
   connect<> neti101(plat.src[101], fft_cols[50].in);
   connect<> neti102(plat.src[102], fft_rows[51].in);
   connect<> neti103(plat.src[103], fft_cols[51].in);
   connect<> neti104(plat.src[104], fft_rows[52].in);
   connect<> neti105(plat.src[105], fft_cols[52].in);
   connect<> neti106(plat.src[106], fft_rows[53].in);
   connect<> neti107(plat.src[107], fft_cols[53].in);
   connect<> neti108(plat.src[108], fft_rows[54].in);
   connect<> neti109(plat.src[109], fft_cols[54].in);
   connect<> neti110(plat.src[110], fft_rows[55].in);
   connect<> neti111(plat.src[111], fft_cols[55].in);
   connect<> neti112(plat.src[112], fft_rows[56].in);
   connect<> neti113(plat.src[113], fft_cols[56].in);
   connect<> neti114(plat.src[114], fft_rows[57].in);
   connect<> neti115(plat.src[115], fft_cols[57].in);
   connect<> neti116(plat.src[116], fft_rows[58].in);
   connect<> neti117(plat.src[117], fft_cols[58].in);
   connect<> neti118(plat.src[118], fft_rows[59].in);
   connect<> neti119(plat.src[119], fft_cols[59].in);
   
   connect<> neto000(fft_rows[ 0].out, plat.sink[  0]);
   connect<> neto001(fft_cols[ 0].out, plat.sink[  1]);
   connect<> neto002(fft_rows[ 1].out, plat.sink[  2]);
   connect<> neto003(fft_cols[ 1].out, plat.sink[  3]);
   connect<> neto004(fft_rows[ 2].out, plat.sink[  4]);
   connect<> neto005(fft_cols[ 2].out, plat.sink[  5]);
   connect<> neto006(fft_rows[ 3].out, plat.sink[  6]);
   connect<> neto007(fft_cols[ 3].out, plat.sink[  7]);
   connect<> neto008(fft_rows[ 4].out, plat.sink[  8]);
   connect<> neto009(fft_cols[ 4].out, plat.sink[  9]);
   connect<> neto010(fft_rows[ 5].out, plat.sink[ 10]);
   connect<> neto011(fft_cols[ 5].out, plat.sink[ 11]);
   connect<> neto012(fft_rows[ 6].out, plat.sink[ 12]);
   connect<> neto013(fft_cols[ 6].out, plat.sink[ 13]);
   connect<> neto014(fft_rows[ 7].out, plat.sink[ 14]);
   connect<> neto015(fft_cols[ 7].out, plat.sink[ 15]);
   connect<> neto016(fft_rows[ 8].out, plat.sink[ 16]);
   connect<> neto017(fft_cols[ 8].out, plat.sink[ 17]);
   connect<> neto018(fft_rows[ 9].out, plat.sink[ 18]);
   connect<> neto019(fft_cols[ 9].out, plat.sink[ 19]);
   connect<> neto020(fft_rows[10].out, plat.sink[ 20]);
   connect<> neto021(fft_cols[10].out, plat.sink[ 21]);
   connect<> neto022(fft_rows[11].out, plat.sink[ 22]);
   connect<> neto023(fft_cols[11].out, plat.sink[ 23]);
   connect<> neto024(fft_rows[12].out, plat.sink[ 24]);
   connect<> neto025(fft_cols[12].out, plat.sink[ 25]);
   connect<> neto026(fft_rows[13].out, plat.sink[ 26]);
   connect<> neto027(fft_cols[13].out, plat.sink[ 27]);
   connect<> neto028(fft_rows[14].out, plat.sink[ 28]);
   connect<> neto029(fft_cols[14].out, plat.sink[ 29]);
   connect<> neto030(fft_rows[15].out, plat.sink[ 30]);
   connect<> neto031(fft_cols[15].out, plat.sink[ 31]);
   connect<> neto032(fft_rows[16].out, plat.sink[ 32]);
   connect<> neto033(fft_cols[16].out, plat.sink[ 33]);
   connect<> neto034(fft_rows[17].out, plat.sink[ 34]);
   connect<> neto035(fft_cols[17].out, plat.sink[ 35]);
   connect<> neto036(fft_rows[18].out, plat.sink[ 36]);
   connect<> neto037(fft_cols[18].out, plat.sink[ 37]);
   connect<> neto038(fft_rows[19].out, plat.sink[ 38]);
   connect<> neto039(fft_cols[19].out, plat.sink[ 39]);
   connect<> neto040(fft_rows[20].out, plat.sink[ 40]);
   connect<> neto041(fft_cols[20].out, plat.sink[ 41]);
   connect<> neto042(fft_rows[21].out, plat.sink[ 42]);
   connect<> neto043(fft_cols[21].out, plat.sink[ 43]);
   connect<> neto044(fft_rows[22].out, plat.sink[ 44]);
   connect<> neto045(fft_cols[22].out, plat.sink[ 45]);
   connect<> neto046(fft_rows[23].out, plat.sink[ 46]);
   connect<> neto047(fft_cols[23].out, plat.sink[ 47]);
   connect<> neto048(fft_rows[24].out, plat.sink[ 48]);
   connect<> neto049(fft_cols[24].out, plat.sink[ 49]);
   connect<> neto050(fft_rows[25].out, plat.sink[ 50]);
   connect<> neto051(fft_cols[25].out, plat.sink[ 51]);
   connect<> neto052(fft_rows[26].out, plat.sink[ 52]);
   connect<> neto053(fft_cols[26].out, plat.sink[ 53]);
   connect<> neto054(fft_rows[27].out, plat.sink[ 54]);
   connect<> neto055(fft_cols[27].out, plat.sink[ 55]);
   connect<> neto056(fft_rows[28].out, plat.sink[ 56]);
   connect<> neto057(fft_cols[28].out, plat.sink[ 57]);
   connect<> neto058(fft_rows[29].out, plat.sink[ 58]);
   connect<> neto059(fft_cols[29].out, plat.sink[ 59]);
   connect<> neto060(fft_rows[30].out, plat.sink[ 60]);
   connect<> neto061(fft_cols[30].out, plat.sink[ 61]);
   connect<> neto062(fft_rows[31].out, plat.sink[ 62]);
   connect<> neto063(fft_cols[31].out, plat.sink[ 63]);
   connect<> neto064(fft_rows[32].out, plat.sink[ 64]);
   connect<> neto065(fft_cols[32].out, plat.sink[ 65]);
   connect<> neto066(fft_rows[33].out, plat.sink[ 66]);
   connect<> neto067(fft_cols[33].out, plat.sink[ 67]);
   connect<> neto068(fft_rows[34].out, plat.sink[ 68]);
   connect<> neto069(fft_cols[34].out, plat.sink[ 69]);
   connect<> neto070(fft_rows[35].out, plat.sink[ 70]);
   connect<> neto071(fft_cols[35].out, plat.sink[ 71]);
   connect<> neto072(fft_rows[36].out, plat.sink[ 72]);
   connect<> neto073(fft_cols[36].out, plat.sink[ 73]);
   connect<> neto074(fft_rows[37].out, plat.sink[ 74]);
   connect<> neto075(fft_cols[37].out, plat.sink[ 75]);
   connect<> neto076(fft_rows[38].out, plat.sink[ 76]);
   connect<> neto077(fft_cols[38].out, plat.sink[ 77]);
   connect<> neto078(fft_rows[39].out, plat.sink[ 78]);
   connect<> neto079(fft_cols[39].out, plat.sink[ 79]);
   connect<> neto080(fft_rows[40].out, plat.sink[ 80]);
   connect<> neto081(fft_cols[40].out, plat.sink[ 81]);
   connect<> neto082(fft_rows[41].out, plat.sink[ 82]);
   connect<> neto083(fft_cols[41].out, plat.sink[ 83]);
   connect<> neto084(fft_rows[42].out, plat.sink[ 84]);
   connect<> neto085(fft_cols[42].out, plat.sink[ 85]);
   connect<> neto086(fft_rows[43].out, plat.sink[ 86]);
   connect<> neto087(fft_cols[43].out, plat.sink[ 87]);
   connect<> neto088(fft_rows[44].out, plat.sink[ 88]);
   connect<> neto089(fft_cols[44].out, plat.sink[ 89]);
   connect<> neto090(fft_rows[45].out, plat.sink[ 90]);
   connect<> neto091(fft_cols[45].out, plat.sink[ 91]);
   connect<> neto092(fft_rows[46].out, plat.sink[ 92]);
   connect<> neto093(fft_cols[46].out, plat.sink[ 93]);
   connect<> neto094(fft_rows[47].out, plat.sink[ 94]);
   connect<> neto095(fft_cols[47].out, plat.sink[ 95]);
   connect<> neto096(fft_rows[48].out, plat.sink[ 96]);
   connect<> neto097(fft_cols[48].out, plat.sink[ 97]);
   connect<> neto098(fft_rows[49].out, plat.sink[ 98]);
   connect<> neto099(fft_cols[49].out, plat.sink[ 99]);
   connect<> neto100(fft_rows[50].out, plat.sink[100]);
   connect<> neto101(fft_cols[50].out, plat.sink[101]);
   connect<> neto102(fft_rows[51].out, plat.sink[102]);
   connect<> neto103(fft_cols[51].out, plat.sink[103]);
   connect<> neto104(fft_rows[52].out, plat.sink[104]);
   connect<> neto105(fft_cols[52].out, plat.sink[105]);
   connect<> neto106(fft_rows[53].out, plat.sink[106]);
   connect<> neto107(fft_cols[53].out, plat.sink[107]);
   connect<> neto108(fft_rows[54].out, plat.sink[108]);
   connect<> neto109(fft_cols[54].out, plat.sink[109]);
   connect<> neto110(fft_rows[55].out, plat.sink[110]);
   connect<> neto111(fft_cols[55].out, plat.sink[111]);
   connect<> neto112(fft_rows[56].out, plat.sink[112]);
   connect<> neto113(fft_cols[56].out, plat.sink[113]);
   connect<> neto114(fft_rows[57].out, plat.sink[114]);
   connect<> neto115(fft_cols[57].out, plat.sink[115]);
   connect<> neto116(fft_rows[58].out, plat.sink[116]);
   connect<> neto117(fft_cols[58].out, plat.sink[117]);
   connect<> neto118(fft_rows[59].out, plat.sink[118]);
   connect<> neto119(fft_cols[59].out, plat.sink[119]);

#endif

#ifdef __AIESIM__

   int main(int argc, char ** argv)
   {
      for(int i = 0; i< FFT2D_INSTS; i++)
      {
         fft_rows[i].init();
         fft_cols[i].init();
      }

      for(int i = 0; i< FFT2D_INSTS; i++)
      {
         fft_rows[i].run(MAT_ROWS);
         fft_cols[i].run(MAT_ROWS);
      }

      for(int i = 0; i< FFT2D_INSTS; i++)
      {
         fft_rows[i].end();
         fft_cols[i].end();
      }
      
      return 0;
   }

#endif
