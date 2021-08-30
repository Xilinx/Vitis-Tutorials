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

#include "ivas_xfacedetect.hpp"

ivas_xfacedetect::ivas_xfacedetect (ivas_xkpriv * kpriv,
    const std::string & model_name, bool need_preprocess)
{
  log_level = kpriv->log_level;
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  model = vitis::ai::FaceDetect::create (model_name, need_preprocess);
}

int
ivas_xfacedetect::run (ivas_xkpriv * kpriv, const cv::Mat & image,
    GstIvasMeta * ivas_meta)
{

  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  auto result = model->run (image);

  int cols = image.cols;
  int rows = image.rows;

for (auto & box:result.rects) {
    float xmin = box.x * cols + 1;
    float ymin = box.y * rows + 1;
    float xmax = xmin + box.width * cols;
    float ymax = ymin + box.height * rows;
    if (xmin < 0.)
      xmin = 1.;
    if (ymin < 0.)
      ymin = 1.;
    if (xmax > cols)
      xmax = cols;
    if (ymax > rows)
      ymax = rows;
    float confidence = box.score;

    IvasObjectMetadata *xva_obj =
        (IvasObjectMetadata *) calloc (1, sizeof (IvasObjectMetadata));
    if (xva_obj == NULL) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, log_level, "failed to allocate memory");
      return false;
    }

#if 0
    xva_obj->bbox_meta.xmin = xmin;
    xva_obj->bbox_meta.ymin = ymin;
    xva_obj->bbox_meta.xmax = xmax;
    xva_obj->bbox_meta.ymax = ymax;
#endif
    float bbox_expand       = (xmax - xmin) * 0.2;
    xva_obj->bbox_meta.xmin = fmax(xmin - bbox_expand, 0.0f);
    xva_obj->bbox_meta.xmax = fmin(xmax + bbox_expand, cols);
    bbox_expand             = (ymax - ymin) * 0.2;
    xva_obj->bbox_meta.ymin = fmax(ymin - bbox_expand, 0.0f);
    xva_obj->bbox_meta.ymax = fmin(ymax + bbox_expand, rows);
    xva_obj->obj_prob = confidence;
    //xva_obj->pts = GST_BUFFER_PTS ((GstBuffer*)inframe->app_priv);
    ivas_meta->xmeta.objects =
        g_list_append (ivas_meta->xmeta.objects, xva_obj);

    LOG_MESSAGE (LOG_LEVEL_INFO, log_level, "RESULT: %f %f %f %f %f", xmin,
        ymin, xmax, ymax, confidence);
  }
  LOG_MESSAGE (LOG_LEVEL_INFO, log_level, " ");

  return true;
}


int
ivas_xfacedetect::requiredwidth (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputWidth ();
}

int
ivas_xfacedetect::requiredheight (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputHeight ();
}

int
ivas_xfacedetect::close (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return true;
}

ivas_xfacedetect::~ivas_xfacedetect ()
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
}
