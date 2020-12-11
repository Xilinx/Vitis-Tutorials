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

#include "graph.h"

adf::PLIO *pl64_in  = new adf::PLIO("64 bits in G1", plio_64_bits,"data/PhaseIn_0.txt", 500);
adf::PLIO *pl64_out = new adf::PLIO("64 bits out G1",plio_64_bits,"data/Output_0.txt",500);

adf::simulation::platform<1,1> Platform(pl64_in,pl64_out);

TopGraph G;

connect<> net000(Platform.src[0],G.in);


connect<> net100(G.out,Platform.sink[0]);

int main()
{
	G.init();
	G.run(NFRAMES);
	G.end();

	return(0);
}
