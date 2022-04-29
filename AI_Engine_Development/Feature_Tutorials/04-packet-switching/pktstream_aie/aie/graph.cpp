/**********
© Copyright 2020-2022 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include "graph.h"

using namespace adf;

mygraph gr;

#if defined(__AIESIM__) || defined(__ADF_FRONTEND__)
int main(int argc, char ** argv) {
  gr.init();
  gr.run(2);
  gr.end();
  return 0;
}
#endif
