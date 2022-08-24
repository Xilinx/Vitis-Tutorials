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

#include "fir_graph.h"

adf::PLIO *pl128_in  = new adf::PLIO("128 bits read in",  adf::plio_128_bits,"data/input_128b.txt",  250);
adf::PLIO *pl128_out = new adf::PLIO("128 bits read out", adf::plio_128_bits,"data/output_128b.txt", 250);

simulation::platform<1,1> platform(pl128_in, pl128_out);
FirGraph filter ;

connect<> net0(platform.src[0], filter.in);
connect<> net1(filter.out, platform.sink[0]);

int main(void) {
    filter.init() ;
    filter.run(NUM_ITER) ;
    filter.end() ;
    return 0 ;
}
