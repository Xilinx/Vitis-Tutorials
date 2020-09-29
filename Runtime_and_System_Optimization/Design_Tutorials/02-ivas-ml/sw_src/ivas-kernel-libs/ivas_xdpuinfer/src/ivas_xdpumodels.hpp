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
#ifndef DPUMODELS_H
#define DPUMODELS_H

enum
{
  IVAS_XCLASS_YOLOV3,
  IVAS_XCLASS_FACEDETECT,
  IVAS_XCLASS_CLASSIFICATION,
  IVAS_XCLASS_SSD,
  IVAS_XCLASS_REID,
  IVAS_XCLASS_REFINEDET,
  IVAS_XCLASS_TFSSD,
  IVAS_XCLASS_YOLOV2,

  IVAS_XCLASS_NOTFOUND
};

static const char *ivas_xmodelclass[IVAS_XCLASS_NOTFOUND + 1] = {
  [IVAS_XCLASS_YOLOV3] = "YOLOV3",
  [IVAS_XCLASS_FACEDETECT] = "FACEDETECT",
  [IVAS_XCLASS_CLASSIFICATION] = "CLASSIFICATION",
  [IVAS_XCLASS_SSD] = "SSD",
  [IVAS_XCLASS_REID] = "REID",
  [IVAS_XCLASS_REFINEDET] = "REFINEDET",
  [IVAS_XCLASS_TFSSD] = "TFSSD",
  [IVAS_XCLASS_YOLOV2] = "YOLOV2",

  /* Add model above this */
  [IVAS_XCLASS_NOTFOUND] = ""
};

#endif
