/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#pragma once

#include <adf.h>
#include "include.h"
#include "kernels.h"

using namespace adf;

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

class WithoutPolarClip : public graph {

private:
    kernel interpolator;
    kernel classify;

public:
    // Graph ports
    port<input> in;
    port<output> out;

    // polar_clip connection ports
    port<output> clip_in;
    port<input> clip_out;

    WithoutPolarClip() {
        interpolator = kernel::create(fir_27t_sym_hb_2i);
        classify     = kernel::create(classifier);
        location<kernel>(interpolator) = tile(5,5);

        connect(in, interpolator.in[0]);
        connect(interpolator.out[0], clip_in);
        connect(clip_out, classify.in[0]);
        connect(classify.out[0], out);

        std::vector<std::string> myheaders;
        myheaders.push_back("include.h");

        adf::headers(interpolator) = myheaders;
        adf::headers(classify) = myheaders;

        source(interpolator) = "kernels/hb27_2i.cpp";
        source(classify)    = "kernels/classify.cpp";

        //Dimensions - Specifying the size of the buffers
        dimensions(interpolator.in[0]) = {INTERPOLATOR27_INPUT_SAMPLES};
        dimensions(interpolator.out[0]) = {INTERPOLATOR27_OUTPUT_SAMPLES};
        dimensions(classify.out[0]) = {CLASSIFIER_OUTPUT_SAMPLES};

        runtime<ratio>(interpolator) = 0.8;
        runtime<ratio>(classify) = 0.8;
    };
};

class Lab8 : public graph {
public:
    // Connection to the testbench:
    //  - standard one with files
    //  - or External Traffic Generator
    input_plio plin;
    output_plio clip_in;
    input_plio clip_out;
    output_plio plout;

    WithoutPolarClip AIE;

    Lab8()
    {
        std::string datadir(STR(DATA_DIR));
        plin = input_plio::create("DataIn1",adf::plio_32_bits,datadir + "/mm2s.txt");
        clip_in = output_plio::create("clip_in",adf::plio_32_bits,datadir + "/polar_clip_in.txt");
        clip_out = input_plio::create("clip_out",adf::plio_32_bits,datadir + "/polar_clip_out.txt");
        plout = output_plio::create("DataOut1",adf::plio_32_bits,datadir + "/DataOut1.txt");

        connect<> (plin.out[0],AIE.in);
        connect<> (AIE.clip_in,clip_in.in[0]);
        connect<> (clip_out.out[0],AIE.clip_out);
        connect<> (AIE.out,plout.in[0]);

    };

};
