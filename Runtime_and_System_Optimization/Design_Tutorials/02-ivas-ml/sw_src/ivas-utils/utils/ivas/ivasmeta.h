/*
* Copyright (C) 2020 – 2021 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
*
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

#ifndef __IVAS_META_H__
#define __IVAS_META_H__

/* Update of this file by the user is not encouraged */
#include <stdint.h>

#define MAX_NAME_LENGTH 256

typedef struct _IvasColorMetadata {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
} IvasColorMetadata;

typedef struct _IvasTextMetadata {

  int8_t disp_text[MAX_NAME_LENGTH];

  IvasColorMetadata text_color;
} IvasTextMetadata;


typedef struct _XVABBoxMeta {
  float xmin;
  float ymin;
  float xmax;
  float ymax;

  IvasColorMetadata box_color;
} IvasBBoxMetadata;

typedef struct _IvasObjectMetadata {
  int32_t obj_id;
  int8_t obj_class[MAX_NAME_LENGTH];
  double obj_prob;
  IvasBBoxMetadata bbox_meta;

  IvasTextMetadata text_meta;

  GList *obj_list;
} IvasObjectMetadata;

#endif
