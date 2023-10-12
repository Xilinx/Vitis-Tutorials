// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"

template<int R1=1, int R2=1>
class G1_Stream : public adf::graph
{

private:

public:
    adf::kernel k1,k2;
    adf::port<input> din;
    adf::port<output> dout;

    G1_Stream()
    {
        k1 = adf::kernel::create(UpConv_7_5_stream<InputSize>);
        adf::source(k1) = "UpConv_7_5_stream.cpp";
        adf::runtime<ratio>(k1) = 0.9;

        k2 = adf::kernel::create(DownConv_5_7_stream<InputSize>);
        adf::source(k2) = "DownConv_5_7_stream.cpp";
        adf::runtime<ratio>(k2) = 0.9;

        adf::connect G1_1(din,k1.in[0]);
        adf::connect G1_2(k1.out[0],k2.in[0]);
        adf::connect G1_3(k2.out[0],dout);

        if(R1>1) repetition_count(k1) = R1;
        if(R2>1) repetition_count(k2) = R2;
    };
};

template<int R1=1, int R2=1>
class G2_Stream : public adf::graph
{

private:

public:
    adf::kernel k1,k2;
    adf::port<input> din;
    adf::port<output> dout;

    G2_Stream()
    {
        k1 = adf::kernel::create(DownConv_5_7_stream<InputSize>);
        adf::source(k1) = "DownConv_5_7_stream.cpp";
        adf::runtime<ratio>(k1) = 0.9;


        k2 = adf::kernel::create(UpConv_7_5_stream<InputSize>);
        adf::source(k2) = "UpConv_7_5_stream.cpp";
        adf::runtime<ratio>(k2) = 0.9;

        adf::connect G2_1(din,k1.in[0]);
        adf::connect G2_2(k1.out[0],k2.in[0]);
        adf::connect G2_3(k2.out[0],dout);

        if(R1>1) repetition_count(k1) = R1;
        if(R2>1) repetition_count(k2) = R2;

    };
};

template<int R11=1, int R12=1,int R21=1,int R22=1,int R1=1,int L1=0,int L2=0>
class G3_Stream : public adf::graph
{

private:
    G1_Stream<R11,R12> G1;
    G2_Stream<R21,R22> G2;
    adf::kernel k1;

public:
    adf::port<input> din;
    adf::port<output> dout;

    G3_Stream()
    {
        k1 = adf::kernel::create(Subtract_stream<InputSize>);
        adf::source(k1) = "Subtract_stream.cpp";
        adf::runtime<ratio>(k1) = 0.9;


        adf::connect Branch1In(din,G1.din);
        adf::connect Branch2In(din,G2.din);
        adf::connect Branch1Out(G1.dout,k1.in[0]);
        adf::connect Branch2Out(G2.dout,k1.in[1]);
        adf::connect(k1.out[0],dout);

        if(R1>1) repetition_count(k1) = R1;

        if(L1>0) fifo_depth(Branch1In) = L1;
        if(L1>0) fifo_depth(Branch1Out) = L1;
        if(L2>0) fifo_depth(Branch2In) = L2;
        if(L2>0) fifo_depth(Branch2Out) = L2;

        adf::location<adf::kernel>(G1.k1) = adf::tile(20,0);
        adf::location<adf::kernel>(G1.k2) = adf::tile(21,0);
        adf::location<adf::kernel>(G2.k1) = adf::tile(20,2);
        adf::location<adf::kernel>(G2.k2) = adf::tile(21,2);
        adf::location<adf::kernel>(k1) = adf::tile(23,1);


    };
};


template<int VERSION >
class MultiRateGraph_Stream : public adf::graph
{
public:

    constexpr static bool V0() {return VERSION==0;}
    constexpr static bool V1() {return VERSION==1;}
    constexpr static bool V2() {return VERSION==2;}
    constexpr static bool V3() {return VERSION==3;}
    constexpr static bool V4() {return VERSION==4;}

    adf::input_plio plin64;
    adf::output_plio plout64;



    typename std::conditional<V0(),G2_Stream<>,int>::type G2_norep;
    typename std::conditional<V1(),G1_Stream<5,7>,int>::type G1_rep;
    typename std::conditional<V2(),G2_Stream<7,5>,int>::type G2_rep;
    typename std::conditional<V3(),G3_Stream<35,49,35,25,35,0,0>,int>::type G3;
    typename std::conditional<V4(),G3_Stream<35,49,35,25,35,4000,4000>,int>::type G4;

    MultiRateGraph_Stream()
    {

        plin64 = adf::input_plio::create("input64_1", adf::plio_64_bits, "data/Input_64.txt", 625);
        plout64 = adf::output_plio::create("output64_1", adf::plio_64_bits, "data/OutputStream_64.txt", 625);

        if constexpr (V0())
        {
            adf::connect(plin64.out[0], G2_norep.din);
            adf::connect(G2_norep.dout, plout64.in[0]);
        }
        if constexpr (V1())
        {
            adf::connect(plin64.out[0], G1_rep.din);
            adf::connect(G1_rep.dout, plout64.in[0]);
        }
        if constexpr (V2())
        {
            adf::connect(plin64.out[0], G2_rep.din);
            adf::connect(G2_rep.dout, plout64.in[0]);
        }

        if constexpr (V3())
        {
            adf::connect(plin64.out[0], G3.din);
            adf::connect(G3.dout, plout64.in[0]);
        }

        if constexpr (V4())
        {
            adf::connect(plin64.out[0], G4.din);
            adf::connect(G4.dout, plout64.in[0]);
        }


    };
};
