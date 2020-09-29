/*
* Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
* KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
* EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
* OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
* not be used in advertising or otherwise to promote the sale, use or other
* dealings in this Software without prior written authorization from Xilinx.
*/

#pragma once

#ifndef DPU2_H
#define DPU2_H

#include <vector>
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>

#include <ivas/ivas_kernel.h>
#include <ivas/ivaslogs.h>
#include <gst/ivas/gstivasmeta.h>

using namespace cv;
using namespace std;

/*  #define INT_MAX 2147483647 */

struct ivas_xkpriv;

class ivas_xdpumodel
{
public:
  virtual int run (ivas_xkpriv * kpriv, const cv::Mat & image,
      GstIvasMeta * ivas_meta) = 0;
  virtual int requiredwidth (void) = 0;
  virtual int requiredheight (void) = 0;
  virtual int close (void) = 0;
    virtual ~ ivas_xdpumodel () = 0;
};

struct performance_test
{
  int test_started = 0;
  unsigned long frames = 0;
  unsigned long last_displayed_frame = 0;
  long long timer_start;
  long long last_displayed_time;
};
typedef struct performance_test ivas_perf;

struct labels
{
  std::string name;
  int label;
    std::string display_name;
};
typedef struct lables lables;

enum
{
  IVAS_XLABEL_NOT_REQUIRED = 0,
  IVAS_XLABEL_REQUIRED = 1,
  IVAS_XLABEL_NOT_FOUND = 2,
  IVAS_XLABEL_FOUND = 4
};

/**
 * struct ivas_xkpriv - Keep private data of ivas_xdpuinfer
 */
struct ivas_xkpriv
{
  ivas_xdpumodel *model;        /* dpu handler */
  IVASKernel *handle;           /* ivas kernel handler */
  int modelclass;               /* Class of model, from Json file */
  int modelnum;                 /* map class to number ivas_xmodelclass[] */
  int log_level;                /* LOG_LEVEL_ERROR=0, LOG_LEVEL_WARNING=1,
                                   LOG_LEVEL_INFO=2, LOG_LEVEL_DEBUG=3 */
  bool need_preprocess;         /* enable/disable pre-processing of DPU */
  bool performance_test;        /* enable/disable performance */
  bool run_time_model;          /* enable model load on every frame */
  labels *labelptr;             /* contain label array */
  int labelflags;               /* IVAS_XLABEL_NOT_REQUIRED, IVAS_XLABEL_REQUIRED,
                                   IVAS_XLABEL_NOT_FOUND, IVAS_XLABEL_FOUND */
  int max_labels;               /* number of labels in label.json */
  ivas_perf pf;                 /* required for performance */
    std::string modelpath;      /* contain model files path from json */
    std::string modelname;      /* contain name of model from json */
    std::string elfname;        /* contail model elf name */
};
typedef struct ivas_xkpriv ivas_xkpriv;

#endif
