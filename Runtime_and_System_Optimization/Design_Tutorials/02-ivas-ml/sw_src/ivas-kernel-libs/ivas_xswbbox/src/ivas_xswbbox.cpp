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

#include "font8x8_basic.h"
#include <gst/ivas/gstivasmeta.h>
#include <iostream>
#include <ivas/ivas_kernel.h>

#define MAX_BOXES 20
#define DEFAULT_THICKNESS 1

using namespace std;

typedef struct _ivas_bbox
{
    uint32_t y_start;
    uint32_t y_end;
    uint32_t x_start;
    uint32_t x_end;
    uint8_t y_val;
    uint16_t uv_val;
    uint8_t obj_id;
} ivas_bbox;

typedef struct _kern_priv
{
    uint32_t thickness;
} sw_bbox_kernel_priv;

static void ivas_parse_roi(ivas_bbox *roi_data, GstIvasMeta *ivas_meta, int nobj)
{
    IvasObjectMetadata *xva_obj = NULL;
    int i, nobjects = 0;

    for (i = 0; i < nobj; i++) {
        if (ivas_meta)
            xva_obj = (IvasObjectMetadata *)g_list_nth_data(ivas_meta->xmeta.objects, nobjects++);
        roi_data[i].x_start = (uint32_t)xva_obj->bbox_meta.xmin >> 1;
        roi_data[i].y_start = (uint32_t)xva_obj->bbox_meta.ymin >> 1;
        roi_data[i].x_end   = (uint32_t)xva_obj->bbox_meta.xmax >> 1;
        roi_data[i].y_end   = (uint32_t)xva_obj->bbox_meta.ymax >> 1;
        roi_data[i].x_start = roi_data[i].x_start << 1;
        roi_data[i].y_start = roi_data[i].y_start << 1;
        roi_data[i].x_end   = roi_data[i].x_end << 1;
        roi_data[i].y_end   = roi_data[i].y_end << 1;
        roi_data[i].y_val   = 0x0;
        roi_data[i].uv_val  = 0xFFFF;
        roi_data[i].obj_id  = (uint32_t)xva_obj->obj_id;
        //printf ("ROI %d: %d %d %d %d\n", i, roi_data[i].x_start, roi_data[i].y_start, roi_data[i].x_end, roi_data[i].y_end);
    }
}

static void draw_reid(IVASFrame *in_ivas_frame, ivas_bbox ivas_roi, int pos)
{
    uint8_t *buffer_y   = NULL;
    uint16_t *buffer_uv = NULL;
    uint32_t stride     = in_ivas_frame->props.stride;
    uint32_t uv_stride  = stride / 2, index, index_uv;
    uint32_t y_uv_start, x_uv_start;
    y_uv_start = ivas_roi.y_start >> 1;
    x_uv_start = ivas_roi.x_start >> 1;

    buffer_y  = (uint8_t *)in_ivas_frame->vaddr[0];  /* input y buffer */
    buffer_uv = (uint16_t *)in_ivas_frame->vaddr[1]; /* input uv buffer */

    std::string id_str = std::to_string(ivas_roi.obj_id);
    for (long unsigned int k = 0; k < id_str.length(); k++) {
        for (int i = 0; i < 16; i++) {
            uint8_t line = font8x8_basic[(int)id_str.at(k)][(i / 2)];
            for (int j = 0; j < 16; j++) {
                if (line & 0x01) {
                    index               = (ivas_roi.y_start + 8 + i) * stride + ivas_roi.x_start + 8 + (k * 16) + j;
                    index_uv            = (y_uv_start + 4 + (i >> 1)) * uv_stride + x_uv_start + 4 + ((k * 16) >> 1) + (j >> 1);
                    buffer_y[index]     = 0xFF;
                    buffer_uv[index_uv] = 0xFFFF;
                }
                if (j % 2 == 0) {
                    line >>= 1;
                }
            }
        }
    }
}

static void draw_nv12_box(IVASFrame *in_ivas_frame, ivas_bbox ivas_roi, uint32_t thickness)
{
    uint8_t *buffer_y   = NULL;
    uint16_t *buffer_uv = NULL;
    uint32_t loc = 0, locend = 0, i = 0, j = 0;
    uint32_t y_uv_start, y_uv_end, x_uv_start, x_uv_end, uv_stride;
    uint32_t stride = in_ivas_frame->props.stride;

    /* Top and Bottom finishing line */
    buffer_y  = (uint8_t *)in_ivas_frame->vaddr[0];  /* input y buffer */
    buffer_uv = (uint16_t *)in_ivas_frame->vaddr[1]; /* input uv buffer */
    uv_stride = stride / 2;

    for (i = 0; i < thickness; i++) {

        y_uv_start = ivas_roi.y_start / 2;
        y_uv_end   = ivas_roi.y_end / 2;
        x_uv_start = ivas_roi.x_start / 2;
        x_uv_end   = ivas_roi.x_end / 2;

        loc    = (ivas_roi.y_start * stride);
        locend = (ivas_roi.y_end * stride);
        for (j = ivas_roi.x_start; j < ivas_roi.x_end; j++) {
            buffer_y[loc + j]    = ivas_roi.y_val;
            buffer_y[locend + j] = ivas_roi.y_val;
        }
        /* Top and Bottom finishing line */
        loc    = (y_uv_start * uv_stride);
        locend = (y_uv_end * uv_stride);
        for (j = x_uv_start; j < x_uv_end; j++) {
            buffer_uv[loc + j]    = ivas_roi.uv_val;
            buffer_uv[locend + j] = ivas_roi.uv_val;
        }
        /* Standing lines */
        loc    = ivas_roi.x_start;
        locend = ivas_roi.x_end;
        for (j = ivas_roi.y_start; j < ivas_roi.y_end; j++) {
            buffer_y[loc + (j * stride)]    = ivas_roi.y_val;
            buffer_y[locend + (j * stride)] = ivas_roi.y_val;
        }

        loc    = x_uv_start;
        locend = x_uv_end;
        for (j = y_uv_start; j < y_uv_end; j++) {
            buffer_uv[loc + (j * uv_stride)]    = ivas_roi.uv_val;
            buffer_uv[locend + (j * uv_stride)] = ivas_roi.uv_val;
        }

        ivas_roi.y_start++;
        ivas_roi.y_end--;
        ivas_roi.x_start++;
        ivas_roi.x_end--;
    }
}

extern "C"
{
    int32_t xlnx_kernel_start(IVASKernel *handle, int start /*unused */, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
    {
        IVASFrame *in_ivas_frame;
        GstIvasMeta *ivas_meta           = NULL;
        ivas_bbox ivas_roi[MAX_BOXES]    = {0};
        int nobj                         = 0;
        sw_bbox_kernel_priv *kernel_priv = (sw_bbox_kernel_priv *)handle->kernel_priv;
        uint32_t thickness               = kernel_priv->thickness;

        in_ivas_frame = input[0];
        /* get metadata from input */
        ivas_meta = gst_buffer_get_ivas_meta((GstBuffer *)in_ivas_frame->app_priv);
        if (ivas_meta == NULL) {
            goto done;
        }
        else if (g_list_length(ivas_meta->xmeta.objects) > MAX_BOXES) {
            printf("ERROR: IVAS: SW BBOX: can't process more than %d objects\n", MAX_BOXES);
            return FALSE;
        }

        nobj = ivas_meta ? g_list_length(ivas_meta->xmeta.objects) : 0; /* Number of outputs */
        if (nobj < 1)
            goto done;

        ivas_parse_roi(ivas_roi, ivas_meta, nobj);
        for (int i = 0; i < nobj; i++) {
            draw_nv12_box(in_ivas_frame, ivas_roi[i], thickness);
            draw_reid(in_ivas_frame, ivas_roi[i], i);
        }
    done:
        return TRUE;
    }

    int32_t xlnx_kernel_init(IVASKernel *handle)
    {
        sw_bbox_kernel_priv *kernel_priv = (sw_bbox_kernel_priv *)calloc(1, sizeof(sw_bbox_kernel_priv));
        json_t *jconfig                  = handle->kernel_config, *val;

        /* parse config */
        val = json_object_get(jconfig, "thickness");
        if (!val || !json_is_integer(val))
            kernel_priv->thickness = DEFAULT_THICKNESS;
        else
            kernel_priv->thickness = json_integer_value(val);

        if (kernel_priv->thickness > 10 || kernel_priv->thickness < 1) {
            printf("WARNING :IVAS SW BBOX: Invalid Thickness %d, setting to %d\n",
                   kernel_priv->thickness, DEFAULT_THICKNESS);
            kernel_priv->thickness = DEFAULT_THICKNESS;
        }
        printf("IVAS SW BBOX: Thickness value %d\n", kernel_priv->thickness);
        handle->is_multiprocess = 1;
        handle->kernel_priv     = (void *)kernel_priv;

        return 0;
    }

    uint32_t xlnx_kernel_deinit(IVASKernel *handle)
    {
        sw_bbox_kernel_priv *kernel_priv = (sw_bbox_kernel_priv *)handle->kernel_priv;
        free(kernel_priv);
        return 0;
    }

    int32_t xlnx_kernel_done(IVASKernel *handle)
    {
        /* dummy */
        return 0;
    }
}
