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

#include "ivas_xreid.hpp"


ivas_xreid::ivas_xreid (ivas_xkpriv * kpriv, const std::string & model_name,
    bool need_preprocess)
{
  log_level = kpriv->log_level;
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  model = vitis::ai::Reid::create (model_name, need_preprocess);
}

int
ivas_xreid::run (ivas_xkpriv * kpriv, const cv::Mat & image,
    GstIvasMeta * ivas_meta)
{

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

  Mat imgx = imread ("images/sample_reid_002.jpg");
  auto result = model->run (image);
  auto result2 = model->run (imgx);

  printf ("Imaage heightXweidth = %dX%d\n", result.height, result.width);
  Mat featnew = result.feat;

  printf ("reid img HXw = %dX%d\n", featnew.size ().height,
      featnew.size ().width);

  for (int i = 0; i < featnew.dims; ++i) {
    if (i)
      std::cout << " X ";
    std::cout << featnew.size[i];
  }
  std::cout << "] temp.channels = " << featnew.channels () << std::endl;

  double dismat = (1 - featnew.dot (result2.feat));
  printf ("dismat : %.3lf \n", dismat);


  return true;
}

int
ivas_xreid::requiredwidth (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputWidth ();
}

int
ivas_xreid::requiredheight (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputHeight ();
}

int
ivas_xreid::close (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return true;
}

ivas_xreid::~ivas_xreid ()
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
}
