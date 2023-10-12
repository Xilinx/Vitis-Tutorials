// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"

template<int L1=0, int L2=0, int L3=0>
class G1_Buffer : public adf::graph
{

private:

public:
    adf::kernel k1,k2;
    adf::port<input> din;
    adf::port<output> dout;

    G1_Buffer()
    {
        k1 = adf::kernel::create(UpConv_7_5_iobuf);
        adf::source(k1) = "UpConv_7_5_iobuf.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::dimensions(k1.in[0]) = {InputSize};
        adf::dimensions(k1.out[0]) = {InputSize*7/5};

        k2 = adf::kernel::create(DownConv_5_7_iobuf);
        adf::source(k2) = "DownConv_5_7_iobuf.cpp";
        adf::runtime<ratio>(k2) = 0.9;
        adf::dimensions(k2.in[0]) = {InputSize};
        adf::dimensions(k2.out[0]) = {InputSize*5/7};

        adf::connect G1_1(din,k1.in[0]);
        adf::connect G1_2(k1.out[0],k2.in[0]);
        adf::connect G1_3(k2.out[0],dout);

        if(L1>0) fifo_depth(G1_1) = L1;
        if(L2>0) fifo_depth(G1_2) = L2;
        if(L3>0) fifo_depth(G1_3) = L3;

    };
};

template<int L1=0, int L2=0, int L3=0>
class G2_Buffer : public adf::graph
{

private:

public:
    adf::kernel k1,k2;
    adf::port<input> din;
    adf::port<output> dout;

    G2_Buffer()
    {
        k1 = adf::kernel::create(DownConv_5_7_iobuf);
        adf::source(k1) = "DownConv_5_7_iobuf.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::dimensions(k1.in[0]) = {InputSize};
        adf::dimensions(k1.out[0]) = {InputSize*5/7};


        k2 = adf::kernel::create(UpConv_7_5_iobuf);
        adf::source(k2) = "UpConv_7_5_iobuf.cpp";
        adf::runtime<ratio>(k2) = 0.9;
        adf::dimensions(k2.in[0]) = {InputSize};
        adf::dimensions(k2.out[0]) = {InputSize*7/5};

        adf::connect G2_1(din,k1.in[0]);
        adf::connect G2_2(k1.out[0],k2.in[0]);
        adf::connect G2_3(k2.out[0],dout);

        if(L1>0) fifo_depth(G2_1) = L1;
        if(L2>0) fifo_depth(G2_2) = L2;
        if(L3>0) fifo_depth(G2_3) = L3;

    };
};

template<int L1=0, int L2=0,int G11=0,int G12=0,int G13=0,int G21=0,int G22=0,int G23=0>
class G3_Buffer : public adf::graph
{

private:
    G1_Buffer<G11,G12,G13> G1;
    G2_Buffer<G21,G22,G23> G2;
    adf::kernel k1;

public:
    adf::port<input> din;
    adf::port<output> dout;

    G3_Buffer()
    {
        k1 = adf::kernel::create(Subtract_iobuf);
        adf::source(k1) = "Subtract_iobuf.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::dimensions(k1.in[0]) = {InputSize};
        adf::dimensions(k1.in[1]) = {InputSize};
        adf::dimensions(k1.out[0]) = {InputSize};


        adf::connect Branch1(din,G1.din);
        adf::connect Branch2(din,G2.din);
        adf::connect(G1.dout,k1.in[0]);
        adf::connect(G2.dout,k1.in[1]);
        adf::connect(k1.out[0],dout);

        if(L1>0) fifo_depth(Branch1) = L1;
        if(L2>0) fifo_depth(Branch2) = L2;


        adf::location<adf::kernel>(G1.k1) = adf::tile(20,0);
        adf::location<adf::kernel>(G1.k2) = adf::tile(21,0);
        adf::location<adf::kernel>(G2.k1) = adf::tile(20,2);
        adf::location<adf::kernel>(G2.k2) = adf::tile(21,2);
        adf::location<adf::kernel>(k1) = adf::tile(23,1);

    };
};


template<int VERSION >
class MultiRateGraph_Buffer : public adf::graph
{
public:

    constexpr static bool V1() {return VERSION==1;}
    constexpr static bool V2() {return VERSION==2;}
    constexpr static bool V3() {return VERSION==3;}
    constexpr static bool V4() {return VERSION==4;}
    constexpr static bool V5() {return VERSION==5;}

    adf::input_plio plin64;
    adf::output_plio plout64;



    typename std::conditional<V1(),G1_Buffer<>,int>::type G1;
    typename std::conditional<V2(),G2_Buffer<>,int>::type G2;
    typename std::conditional<V3(),G3_Buffer<>,int>::type G3_nofifo;
    typename std::conditional<V4(),G3_Buffer<0,6800,3620,2640,0,7612,80,0>,int>::type G3_fifo1;
    typename std::conditional<V5(),G3_Buffer<16300,16300>,int>::type G3_fifo2;

    MultiRateGraph_Buffer()
    {

        plin64 = adf::input_plio::create("input64_1", adf::plio_64_bits, "data/Input_64.txt", 625);
        plout64 = adf::output_plio::create("output64_1", adf::plio_64_bits, "data/OutputIObuf_64.txt", 625);

        if constexpr (V1())
        {
            adf::connect(plin64.out[0], G1.din);
            adf::connect(G1.dout, plout64.in[0]);
        }
        if constexpr (V2())
        {
            adf::connect(plin64.out[0], G2.din);
            adf::connect(G2.dout, plout64.in[0]);
        }
        if constexpr (V3())
        {
            adf::connect(plin64.out[0], G3_nofifo.din);
            adf::connect(G3_nofifo.dout, plout64.in[0]);
        }

        if constexpr (V4())
        {
            adf::connect(plin64.out[0], G3_fifo1.din);
            adf::connect(G3_fifo1.dout, plout64.in[0]);
        }

        if constexpr (V5())
        {
            adf::connect(plin64.out[0], G3_fifo2.din);
            adf::connect(G3_fifo2.dout, plout64.in[0]);
        }

    };
};
