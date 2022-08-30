/*
 * Copyright 2021 Xilinx, Inc.
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

TEST_BF<COL_OFFSET> dut;

#if defined  (__AIESIM__) || defined(__X86SIM__)
int main(void) {
    dut.init();
    dut.run(100) ;
    dut.end() ;
    return 0 ;
}
#endif
