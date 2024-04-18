/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
#include "graph.h"
using namespace adf;

simpleGraph addergraph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char** argv) {
    addergraph.init();
    addergraph.run(4);
    addergraph.end();
    return 0;
}
#endif
