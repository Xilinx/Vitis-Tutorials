// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include <unistd.h>

#include "graph.h"

#if MULTIRATE == false
MyGraph<NAntenna,40> G("");
#else
MyGraphUp<NAntenna,40> G("");
#endif

#if defined(__AIESIM__) || defined(__X86SIM__)

template<int N,int RATIO>
void UpdateParam(MyGraph<N,RATIO> &G, cint16 Gain, int32 FilterSel)
{
    for(int i=0;i<N;i++)
    {
      G.update(G.gin[i], Gain);
      G.update(G.fin[i], FilterSel);
    }
}
template<int N,int RATIO>
void UpdateParam(MyGraphUp<N,RATIO> &G, cint16 Gain, int32 FilterSel)
{
    for(int i=0;i<N;i++)
    {
      G.update(G.gin[i], Gain);
      G.update(G.fin[i], FilterSel);
    }
}

int main(int argc, char ** argv) {

cint16 coef1 = {1,0};
cint16 coef2 = {2,0};
cint16 coef3 = {4,0};

  G.init();
  UpdateParam(G, coef1,0);

#define MULTI_UPDATE false

#if MULTI_UPDATE == true
  G.run(NIterations/4);
  G.wait();
  UpdateParam(G, coef2,0);

  G.run(NIterations/4);
  G.wait();
  UpdateParam(G, coef3,0);

  G.run(NIterations/4);
  G.wait();
  UpdateParam(G, coef2,0);

  G.run(NIterations/4);
  G.wait();


#else
  G.run(NIterations);
#endif

  G.end();
  return 0;
}
#endif

/*
   int16 coef1[16] {
    -122,  674,  -2370,  10006,  16384,  0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0};
   int16 coef2[16] {
    0,  0,  0,  0,  16384,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,  0};
   mygraph.init();
   mygraph.run(1);
   mygraph.update(mygraph.In2, coef1, 16);
   mygraph.wait();
   mygraph.run(4);
   mygraph.update(mygraph.In2, coef2, 16);
   mygraph.wait();
   mygraph.run(6);
   mygraph.update(mygraph.In2, coef1, 16);
   mygraph.wait();
   mygraph.run(-1);
   mygraph.end();
   return 0;
}

*/