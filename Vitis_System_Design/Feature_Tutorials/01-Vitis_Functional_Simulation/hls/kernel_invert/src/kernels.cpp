//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <hls_stream.h>
#include <ap_axi_sdata.h>


void invert(hls::stream< ap_ufixed<128,128> > &in,
            hls::stream< ap_ufixed<128,128> > &out)
{
   ap_ufixed<128,128> oval;
   auto ival = in.read();
   out.write(~ival);
}

void invert87(hls::stream< ap_ufixed<87,87> > &in,
            hls::stream< ap_ufixed<87,87> > &out)
{
   ap_ufixed<87,87> oval;
   auto ival = in.read();
   out.write(~ival);
}

void invert43(hls::stream< ap_ufixed<43,43> > &in,
            hls::stream< ap_ufixed<43,43> > &out)
{
   ap_ufixed<43,43> oval;
   auto ival = in.read();
   out.write(~ival);
}

void invert7_13(hls::stream< ap_ufixed<7,7> > &in7, hls::stream< ap_ufixed<13,13> > &in13,
            hls::stream< ap_ufixed<7,7> > &out7, hls::stream< ap_ufixed<13,13> > &out13 )
{
   auto ival7 = in7.read();
   out7.write(~ival7);
    
   auto ival13 = in13.read();
   out13.write(~ival13);
}