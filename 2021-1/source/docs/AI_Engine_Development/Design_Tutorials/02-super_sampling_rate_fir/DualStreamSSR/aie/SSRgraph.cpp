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
#include "system_settings.h"

#include "SSRgraph.h"

adf::PLIO *pl64_in000  = new adf::PLIO("Phase 0 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_0_0.txt", 500);
adf::PLIO *pl64_in001  = new adf::PLIO("Phase 0 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_0_0.txt", 500);
adf::PLIO *pl64_in010  = new adf::PLIO("Phase 0 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_0_1.txt", 500);
adf::PLIO *pl64_in011  = new adf::PLIO("Phase 0 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_0_1.txt", 500);
adf::PLIO *pl64_in100  = new adf::PLIO("Phase 1 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_1_0.txt", 500);
adf::PLIO *pl64_in101  = new adf::PLIO("Phase 1 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_1_0.txt", 500);
adf::PLIO *pl64_in110  = new adf::PLIO("Phase 1 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_1_1.txt", 500);
adf::PLIO *pl64_in111  = new adf::PLIO("Phase 1 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_1_1.txt", 500);
adf::PLIO *pl64_in200  = new adf::PLIO("Phase 2 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_2_0.txt", 500);
adf::PLIO *pl64_in201  = new adf::PLIO("Phase 2 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_2_0.txt", 500);
adf::PLIO *pl64_in210  = new adf::PLIO("Phase 2 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_2_1.txt", 500);
adf::PLIO *pl64_in211  = new adf::PLIO("Phase 2 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_2_1.txt", 500);
adf::PLIO *pl64_in300  = new adf::PLIO("Phase 3 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_3_0.txt", 500);
adf::PLIO *pl64_in301  = new adf::PLIO("Phase 3 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_3_0.txt", 500);
adf::PLIO *pl64_in310  = new adf::PLIO("Phase 3 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_3_1.txt", 500);
adf::PLIO *pl64_in311  = new adf::PLIO("Phase 3 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_3_1.txt", 500);
adf::PLIO *pl64_in400  = new adf::PLIO("Phase 4 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_4_0.txt", 500);
adf::PLIO *pl64_in401  = new adf::PLIO("Phase 4 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_4_0.txt", 500);
adf::PLIO *pl64_in410  = new adf::PLIO("Phase 4 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_4_1.txt", 500);
adf::PLIO *pl64_in411  = new adf::PLIO("Phase 4 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_4_1.txt", 500);
adf::PLIO *pl64_in500  = new adf::PLIO("Phase 5 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_5_0.txt", 500);
adf::PLIO *pl64_in501  = new adf::PLIO("Phase 5 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_5_0.txt", 500);
adf::PLIO *pl64_in510  = new adf::PLIO("Phase 5 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_5_1.txt", 500);
adf::PLIO *pl64_in511  = new adf::PLIO("Phase 5 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_5_1.txt", 500);
adf::PLIO *pl64_in600  = new adf::PLIO("Phase 6 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_6_0.txt", 500);
adf::PLIO *pl64_in601  = new adf::PLIO("Phase 6 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_6_0.txt", 500);
adf::PLIO *pl64_in610  = new adf::PLIO("Phase 6 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_6_1.txt", 500);
adf::PLIO *pl64_in611  = new adf::PLIO("Phase 6 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_6_1.txt", 500);
adf::PLIO *pl64_in700  = new adf::PLIO("Phase 7 Stream 0 Instance 0", plio_64_bits,"data/PhaseIn_7_0.txt", 500);
adf::PLIO *pl64_in701  = new adf::PLIO("Phase 7 Stream 0 Instance 1", plio_64_bits,"data/PhaseIn_7_0.txt", 500);
adf::PLIO *pl64_in710  = new adf::PLIO("Phase 7 Stream 1 Instance 0", plio_64_bits,"data/PhaseIn_7_1.txt", 500);
adf::PLIO *pl64_in711  = new adf::PLIO("Phase 7 Stream 1 Instance 1", plio_64_bits,"data/PhaseIn_7_1.txt", 500);





adf::PLIO *pl64_out00 = new adf::PLIO("PhaseOut 0 Stream 0",plio_64_bits,"data/output_0_0.txt",500);
adf::PLIO *pl64_out01 = new adf::PLIO("PhaseOut 0 Stream 1",plio_64_bits,"data/output_0_1.txt",500);
adf::PLIO *pl64_out10 = new adf::PLIO("PhaseOut 1 Stream 0",plio_64_bits,"data/output_1_0.txt",500);
adf::PLIO *pl64_out11 = new adf::PLIO("PhaseOut 1 Stream 1",plio_64_bits,"data/output_1_1.txt",500);
adf::PLIO *pl64_out20 = new adf::PLIO("PhaseOut 2 Stream 0",plio_64_bits,"data/output_2_0.txt",500);
adf::PLIO *pl64_out21 = new adf::PLIO("PhaseOut 2 Stream 1",plio_64_bits,"data/output_2_1.txt",500);
adf::PLIO *pl64_out30 = new adf::PLIO("PhaseOut 3 Stream 0",plio_64_bits,"data/output_3_0.txt",500);
adf::PLIO *pl64_out31 = new adf::PLIO("PhaseOut 3 Stream 1",plio_64_bits,"data/output_3_1.txt",500);
adf::PLIO *pl64_out40 = new adf::PLIO("PhaseOut 4 Stream 0",plio_64_bits,"data/output_4_0.txt",500);
adf::PLIO *pl64_out41 = new adf::PLIO("PhaseOut 4 Stream 1",plio_64_bits,"data/output_4_1.txt",500);
adf::PLIO *pl64_out50 = new adf::PLIO("PhaseOut 5 Stream 0",plio_64_bits,"data/output_5_0.txt",500);
adf::PLIO *pl64_out51 = new adf::PLIO("PhaseOut 5 Stream 1",plio_64_bits,"data/output_5_1.txt",500);
adf::PLIO *pl64_out60 = new adf::PLIO("PhaseOut 6 Stream 0",plio_64_bits,"data/output_6_0.txt",500);
adf::PLIO *pl64_out61 = new adf::PLIO("PhaseOut 6 Stream 1",plio_64_bits,"data/output_6_1.txt",500);
adf::PLIO *pl64_out70 = new adf::PLIO("PhaseOut 7 Stream 0",plio_64_bits,"data/output_7_0.txt",500);
adf::PLIO *pl64_out71 = new adf::PLIO("PhaseOut 7 Stream 1",plio_64_bits,"data/output_7_1.txt",500);

adf::simulation::platform<32,16> Platform(
	// 32 input PLIOS
	pl64_in000,	pl64_in001,	pl64_in010,	pl64_in011,
	pl64_in100,	pl64_in101,	pl64_in110,	pl64_in111,
	pl64_in200,	pl64_in201,	pl64_in210,	pl64_in211,
	pl64_in300,	pl64_in301,	pl64_in310,	pl64_in311,
	pl64_in400,	pl64_in401,	pl64_in410,	pl64_in411,
	pl64_in500,	pl64_in501,	pl64_in510,	pl64_in511,
	pl64_in600,	pl64_in601,	pl64_in610,	pl64_in611,
	pl64_in700,	pl64_in701,	pl64_in710,	pl64_in711,
	// 16 output PLIOS
	pl64_out00,	pl64_out01,
	pl64_out10,	pl64_out11,
	pl64_out20,	pl64_out21,
	pl64_out30,	pl64_out31,
	pl64_out40,	pl64_out41,
	pl64_out50,	pl64_out51,
	pl64_out60,	pl64_out61,
	pl64_out70,	pl64_out71);


TopGraph G;

connect<> net000(Platform.src[0],G.in[0]);
connect<> net001(Platform.src[1],G.in[1]);
connect<> net002(Platform.src[2],G.in[2]);
connect<> net003(Platform.src[3],G.in[3]);
connect<> net004(Platform.src[4],G.in[4]);
connect<> net005(Platform.src[5],G.in[5]);
connect<> net006(Platform.src[6],G.in[6]);
connect<> net007(Platform.src[7],G.in[7]);
connect<> net008(Platform.src[8],G.in[8]);
connect<> net009(Platform.src[9],G.in[9]);
connect<> net010(Platform.src[10],G.in[10]);
connect<> net011(Platform.src[11],G.in[11]);
connect<> net012(Platform.src[12],G.in[12]);
connect<> net013(Platform.src[13],G.in[13]);
connect<> net014(Platform.src[14],G.in[14]);
connect<> net015(Platform.src[15],G.in[15]);
connect<> net016(Platform.src[16],G.in[16]);
connect<> net017(Platform.src[17],G.in[17]);
connect<> net018(Platform.src[18],G.in[18]);
connect<> net019(Platform.src[19],G.in[19]);
connect<> net020(Platform.src[20],G.in[20]);
connect<> net021(Platform.src[21],G.in[21]);
connect<> net022(Platform.src[22],G.in[22]);
connect<> net023(Platform.src[23],G.in[23]);
connect<> net024(Platform.src[24],G.in[24]);
connect<> net025(Platform.src[25],G.in[25]);
connect<> net026(Platform.src[26],G.in[26]);
connect<> net027(Platform.src[27],G.in[27]);
connect<> net028(Platform.src[28],G.in[28]);
connect<> net029(Platform.src[29],G.in[29]);
connect<> net030(Platform.src[30],G.in[30]);
connect<> net031(Platform.src[31],G.in[31]);



connect<> net100(G.out[0],Platform.sink[0]);
connect<> net101(G.out[1],Platform.sink[1]);
connect<> net102(G.out[2],Platform.sink[2]);
connect<> net103(G.out[3],Platform.sink[3]);
connect<> net104(G.out[4],Platform.sink[4]);
connect<> net105(G.out[5],Platform.sink[5]);
connect<> net106(G.out[6],Platform.sink[6]);
connect<> net107(G.out[7],Platform.sink[7]);
connect<> net108(G.out[8],Platform.sink[8]);
connect<> net109(G.out[9],Platform.sink[9]);
connect<> net110(G.out[10],Platform.sink[10]);
connect<> net111(G.out[11],Platform.sink[11]);
connect<> net112(G.out[12],Platform.sink[12]);
connect<> net113(G.out[13],Platform.sink[13]);
connect<> net114(G.out[14],Platform.sink[14]);
connect<> net115(G.out[15],Platform.sink[15]);

int main()
{
	G.init();
	G.run(NFRAMES);
	G.end();

	return(0);
}
