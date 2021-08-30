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

#include "ivas_xyolov3.hpp"

ivas_xyolov3::ivas_xyolov3 (ivas_xkpriv * kpriv, const std::string & model_name,
    bool need_preprocess)
{
  log_level = kpriv->log_level;
  kpriv->labelflags = IVAS_XLABEL_REQUIRED;
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

  if (kpriv->labelptr == NULL) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "label not found");
    kpriv->labelflags |= IVAS_XLABEL_NOT_FOUND;
  } else
    kpriv->labelflags |= IVAS_XLABEL_FOUND;


  model = vitis::ai::YOLOv3::create (model_name, need_preprocess);
}

int
ivas_xyolov3::run (ivas_xkpriv * kpriv, const cv::Mat & image,
    GstIvasMeta * ivas_meta)
{

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  auto result = model->run (image);

  labels *lptr;
  int cols = image.cols;
  int rows = image.rows;

  if (kpriv->labelptr == NULL) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "label not found");
    return false;
  }

for (auto & box:result.bboxes) {
    int label = box.label;
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
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "failed to allocate memory");
      return false;
    }

    if (label > kpriv->max_labels) {
      LOG_MESSAGE (LOG_LEVEL_WARNING, kpriv->log_level,
          "%d label not found\n", label);
      strcpy ((char *) xva_obj->obj_class, "WRONG_LABEL");
    } else {
      lptr = kpriv->labelptr + label;
      strcpy ((char *) xva_obj->obj_class, lptr->display_name.c_str ());
    }

    xva_obj->bbox_meta.xmin = xmin;
    xva_obj->bbox_meta.ymin = ymin;
    xva_obj->bbox_meta.xmax = xmax;
    xva_obj->bbox_meta.ymax = ymax;
    xva_obj->obj_prob = confidence;
    ivas_meta->xmeta.objects =
        g_list_append (ivas_meta->xmeta.objects, xva_obj);

    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level,
        "RESULT: %s(%d) %f %f %f %f %f", lptr->display_name.c_str (), label,
        xmin, ymin, xmax, ymax, confidence);
  }
  LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, " ");

  return true;
}


int
ivas_xyolov3::requiredwidth (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputWidth ();
}

int
ivas_xyolov3::requiredheight (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputHeight ();
}

int
ivas_xyolov3::close (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return true;
}

ivas_xyolov3::~ivas_xyolov3 ()
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
}
