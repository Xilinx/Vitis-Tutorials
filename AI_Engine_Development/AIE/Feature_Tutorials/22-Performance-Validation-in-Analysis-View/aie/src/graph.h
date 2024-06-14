// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "kernels.h"

using namespace adf;

template<int RATIO_PT,int RATIO>
class GraphWithRTP : public graph {

public:
    kernel ptin,filt,gaink,ptout;

    port<input> in;
    port<output> out;
    port<input> Gval,Fsel;

    GraphWithRTP() {
        ptin = kernel::create(passthrough_strbuf);
        source(ptin) = "kernels/Passthrough.cpp";
        runtime<ratio>(ptin) = (double)RATIO_PT/100.0;

        filt = kernel::create(filter_buf);
        source(filt) = "kernels/Filter.cpp";
        runtime<ratio>(filt) = (double)RATIO/100.0;

        gaink = kernel::create(gain);
        source(gaink) = "kernels/Gain.cpp";
        runtime<ratio>(gaink) = (double)RATIO/100.0;

        ptout = kernel::create(passthrough_bufstr);
        source(ptout) = "kernels/Passthrough.cpp";
        runtime<ratio>(ptout) = (double)RATIO_PT/100.0;

        connect(in, ptin.in[0]);
        connect(ptin.out[0],filt.in[0]);
        connect(filt.out[0],gaink.in[0]);
        connect(gaink.out[0],ptout.in[0]);
        connect(ptout.out[0],out);

        connect<parameter>(Gval,async(gaink.in[1]));
        connect<parameter>(Fsel,async(filt.in[1]));
    };
};

template<int RATIO_PT,int RATIO,int UPRATIO>
class GraphUpsampleWithRTP : public graph {

public:
    kernel ptin,filt,gaink,ptout;

    port<input> in;
    port<output> out;
    port<input> Gval,Fsel;

    GraphUpsampleWithRTP() {
        ptin = kernel::create(passthrough_strbuf);
        source(ptin) = "kernels/Passthrough.cpp";
        runtime<ratio>(ptin) = (double)RATIO_PT/100.0;

        filt = kernel::create(filter_buf);
        source(filt) = "kernels/Filter.cpp";
        runtime<ratio>(filt) = (double)RATIO/100.0;

        gaink = kernel::create(gain);
        source(gaink) = "kernels/Gain.cpp";
        runtime<ratio>(gaink) = (double)RATIO/100.0;

        ptout = kernel::create(upsample_buffer<UPRATIO>);
        source(ptout) = "kernels/Upsample.cpp";
        runtime<ratio>(ptout) = (double)RATIO_PT/100.0;

        connect(in, ptin.in[0]);
        connect(ptin.out[0],filt.in[0]);
        connect(filt.out[0],gaink.in[0]);
        connect(gaink.out[0],ptout.in[0]);
        connect(ptout.out[0],out);

        connect<parameter>(Gval,async(gaink.in[1]));
        connect<parameter>(Fsel,async(filt.in[1]));
    };
};


template<int N,int RATIO_FILTER_GAIN>
class AllGraphs : public graph {
public:
    GraphWithRTP<80,RATIO_FILTER_GAIN> G;
    AllGraphs<N - 1, RATIO_FILTER_GAIN> RG;

    port<input> in[N];
    port<output> out[N];
    port<input> gin[N],fin[N];


    AllGraphs()
    {
        connect<> (in[N-1],G.in);
        connect<> (G.out,out[N-1]);
        connect<> (gin[N-1],G.Gval);
        connect<> (fin[N-1],G.Fsel);

        for(int i=0;i<N-1;i++)
        {
            connect<> (in[i],RG.in[i]);
            connect<> (RG.out[i],out[i]);
            connect<> (gin[i],RG.gin[i]);
            connect<> (fin[i],RG.fin[i]);
        }

    };
};

template <int RATIO_FILTER_GAIN>
class AllGraphs<1,RATIO_FILTER_GAIN> : public graph
{
public:
    GraphWithRTP<80, RATIO_FILTER_GAIN> G;

    port<input> in[1];
    port<output> out[1];
    port<input> gin[1],fin[1];


    AllGraphs()
    {
        connect<> (in[0],G.in);
        connect<> (G.out,out[0]);
        connect<> (gin[0],G.Gval);
        connect<> (fin[0],G.Fsel);
    };
};

template<int N,int RATIO_FILTER_GAIN,int UPRATIO>
class AllGraphsUp : public graph {
public:
    GraphUpsampleWithRTP<80,RATIO_FILTER_GAIN,UPRATIO> G;
    AllGraphsUp<N - 1, RATIO_FILTER_GAIN,UPRATIO+1> RG;

    port<input> in[N];
    port<output> out[N];
    port<input> gin[N],fin[N];


    AllGraphsUp()
    {
        connect<> (in[N-1],G.in);
        connect<> (G.out,out[N-1]);
        connect<> (gin[N-1],G.Gval);
        connect<> (fin[N-1],G.Fsel);

        for(int i=0;i<N-1;i++)
        {
            connect<> (in[i],RG.in[i]);
            connect<> (RG.out[i],out[i]);
            connect<> (gin[i],RG.gin[i]);
            connect<> (fin[i],RG.fin[i]);
        }

    };
};

template <int RATIO_FILTER_GAIN,int UPRATIO>
class AllGraphsUp<1,RATIO_FILTER_GAIN,UPRATIO> : public graph
{
public:
    GraphUpsampleWithRTP<80, RATIO_FILTER_GAIN,UPRATIO> G;

    port<input> in[1];
    port<output> out[1];
    port<input> gin[1],fin[1];


    AllGraphsUp()
    {
        connect<> (in[0],G.in);
        connect<> (G.out,out[0]);
        connect<> (gin[0],G.Gval);
        connect<> (fin[0],G.Fsel);
    };
};

template <int N, int RATIO_FILTER_GAIN>
class MyGraph : public graph
{
public:
    AllGraphs<N,RATIO_FILTER_GAIN> G;
    input_plio in[N];
    output_plio out[N];
    input_port gin[N],fin[N];

    MyGraph(std::string Prefix)
    {
#if PLIOW == 32
#define PLIO_IF plio_32_bits      
#elif PLIOW == 64
#define PLIO_IF plio_64_bits        
#elif PLIOW == 128
#define PLIO_IF plio_128_bits        
#endif

#define FREQ_MHZ ((double)FREQ_HZ/1000000.0)

        for(int idx=0;idx<N;idx++)
        {
            in[idx] = input_plio::create(Prefix+"Input_"+std::to_string(idx),PLIO_IF,"data/input.txt",FREQ_MHZ);
            out[idx] = output_plio::create(Prefix+"Output_"+std::to_string(idx),PLIO_IF,"data/"+Prefix+"output"+std::to_string(idx)+".txt",FREQ_MHZ);
            connect<> (in[idx].out[0],G.in[idx]);
            connect<> (G.out[idx],out[idx].in[0]);
            connect<> (gin[idx],G.gin[idx]);
            connect<> (fin[idx],G.fin[idx]);
        }
#undef PLIO_IF
    };
};


template <int N, int RATIO_FILTER_GAIN>
class MyGraphUp : public graph
{
public:
    AllGraphsUp<N,RATIO_FILTER_GAIN,1> G;
    input_plio in[N];
    output_plio out[N];
    input_port gin[N],fin[N];

    MyGraphUp(std::string Prefix)
    {
#if PLIOW == 32
#define PLIO_IF plio_32_bits      
#elif PLIOW == 64
#define PLIO_IF plio_64_bits        
#elif PLIOW == 128
#define PLIO_IF plio_128_bits        
#endif

#define FREQ_MHZ ((double)FREQ_HZ/1000000.0)

        for(int idx=0;idx<N;idx++)
        {
            in[idx] = input_plio::create(Prefix+"Input_"+std::to_string(idx),PLIO_IF,"data/input.txt",FREQ_MHZ);
            out[idx] = output_plio::create(Prefix+"Output_"+std::to_string(idx),PLIO_IF,"data/"+Prefix+"output"+std::to_string(idx)+".txt",FREQ_MHZ);
            connect<> (in[idx].out[0],G.in[idx]);
            connect<> (G.out[idx],out[idx].in[0]);
            connect<> (gin[idx],G.gin[idx]);
            connect<> (fin[idx],G.fin[idx]);
        }
#undef PLIO_IF
    };
};
