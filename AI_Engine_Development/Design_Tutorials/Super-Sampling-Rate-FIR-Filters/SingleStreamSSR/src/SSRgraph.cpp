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

adf::PLIO *pl64_in00  = new adf::PLIO("Phase 0 Instance 0", plio_64_bits,"data/PhaseIn_0.txt", 500);
adf::PLIO *pl64_in01  = new adf::PLIO("Phase 0 Instance 1", plio_64_bits,"data/PhaseIn_0.txt", 500);
adf::PLIO *pl64_in10  = new adf::PLIO("Phase 1 Instance 0", plio_64_bits,"data/PhaseIn_1.txt", 500);
adf::PLIO *pl64_in11  = new adf::PLIO("Phase 1 Instance 1", plio_64_bits,"data/PhaseIn_1.txt", 500);
adf::PLIO *pl64_in20  = new adf::PLIO("Phase 2 Instance 0", plio_64_bits,"data/PhaseIn_2.txt", 500);
adf::PLIO *pl64_in21  = new adf::PLIO("Phase 2 Instance 1", plio_64_bits,"data/PhaseIn_2.txt", 500);
adf::PLIO *pl64_in30  = new adf::PLIO("Phase 3 Instance 0", plio_64_bits,"data/PhaseIn_3.txt", 500);
adf::PLIO *pl64_in31  = new adf::PLIO("Phase 3 Instance 1", plio_64_bits,"data/PhaseIn_3.txt", 500);

adf::PLIO *pl64_out0 = new adf::PLIO("64 bits out 0",plio_64_bits,"data/output_0.txt",500);
adf::PLIO *pl64_out1 = new adf::PLIO("64 bits out 1",plio_64_bits,"data/output_1.txt",500);
adf::PLIO *pl64_out2 = new adf::PLIO("64 bits out 2",plio_64_bits,"data/output_2.txt",500);
adf::PLIO *pl64_out3 = new adf::PLIO("64 bits out 3",plio_64_bits,"data/output_3.txt",500);

adf::simulation::platform<8,4> Platform(
	pl64_in00,pl64_in01,pl64_in10,pl64_in11,pl64_in20,pl64_in21,pl64_in30,pl64_in31
	,pl64_out0,pl64_out1,pl64_out2,pl64_out3);

TopGraph G;

connect<> net000(Platform.src[0],G.in[0]);
connect<> net001(Platform.src[1],G.in[1]);
connect<> net002(Platform.src[2],G.in[2]);
connect<> net003(Platform.src[3],G.in[3]);
connect<> net004(Platform.src[4],G.in[4]);
connect<> net005(Platform.src[5],G.in[5]);
connect<> net006(Platform.src[6],G.in[6]);
connect<> net007(Platform.src[7],G.in[7]);



connect<> net100(G.out[0],Platform.sink[0]);
connect<> net101(G.out[1],Platform.sink[1]);
connect<> net102(G.out[2],Platform.sink[2]);
connect<> net103(G.out[3],Platform.sink[3]);

int main()
{
	G.init();
	G.run(NFRAMES);
	G.end();

	return(0);
}
