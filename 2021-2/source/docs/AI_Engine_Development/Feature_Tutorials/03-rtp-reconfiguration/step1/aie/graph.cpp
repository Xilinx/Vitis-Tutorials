/**********
© Copyright 2020-2021 Xilinx, Inc.
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
#include <adf.h>
#include "graph.h"
#include <unistd.h>

using namespace adf;

ddsgraph gr;

#ifdef __AIESIM__
int main(int argc, char ** argv) { 

  gr.init();
  gr.run(4);

  //synchronous RTP trigger calls
  gr.update(gr.trigger,10);
  gr.update(gr.trigger,10);
  gr.update(gr.trigger,100);
  gr.update(gr.trigger,100);

  gr.end();
};
#endif
