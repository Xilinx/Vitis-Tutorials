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
#include "ivas_xdpupriv.hpp"

#include <vitis/ai/yolov2.hpp>
#include <vitis/ai/nnpp/yolov2.hpp>

using namespace std;
using namespace cv;

class ivas_xyolov2:public ivas_xdpumodel
{

  int log_level = 0;
    std::unique_ptr < vitis::ai::YOLOv2 > model;

public:

    ivas_xyolov2 (ivas_xkpriv * kpriv, const std::string & model_name,
      bool need_preprocess);

  virtual int run (ivas_xkpriv * kpriv, const cv::Mat & image,
      GstIvasMeta * ivas_meta);

  virtual int requiredwidth (void);
  virtual int requiredheight (void);
  virtual int close (void);

    virtual ~ ivas_xyolov2 ();
};
