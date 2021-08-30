/*
 * Copyright (C) 2020 � 2021 Xilinx, Inc.  All rights reserved.
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

#include <gst/ivas/gstivasmeta.h>
#include <ivas/ivas_kernel.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <vitis/ai/nnpp/reid.hpp>
#include <vitis/ai/reid.hpp>

#define MAX_REID 20
#define DEFAULT_REID_THRESHOLD 0.2
//#define DEBUG
//#define PROFILING 1

using namespace std;

struct _Face
{
    int last_frame_seen;
    int xctr;
    int yctr;
    int id;
    cv::Mat features;
};

typedef struct _kern_priv
{
    uint32_t top;
    bool first_frame_ref;
    bool print_reid;
    bool dump_frames;
    double threshold;
    int max_distance_per_frame;
    int max_face_age;
    std::unique_ptr<vitis::ai::Reid> det;
    std::vector<_Face> faces;
} ReidKernelPriv;


#if 0
static double ivas_cosine_distance(Mat feat1, Mat feat2){
    return 1 - feat1.dot(feat2);
}
#endif
float calc_spatial(int x1, int y1, int x2, int y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

static int ivas_reid_run(const cv::Mat &image, IVASKernel *handle, int frame_num, int buf_num, int xctr, int yctr)
{
    cv::Mat featprev;
    ReidKernelPriv *kernel_priv = (ReidKernelPriv *)handle->kernel_priv;
    float spatial_distance;
    float distance_metric;
    thread_local static int id_ctr = 0;

    //vitis::ai::ReidResult result = kernel_priv->det->run(image);
#if PROFILING
    uint64_t delta_us;
    struct timespec tstart, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tstart);
#endif
    cv::Mat featnew = kernel_priv->det->run(image).feat;
#if PROFILING
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    delta_us = (end.tv_sec - tstart.tv_sec) * 1000000 + (end.tv_nsec - tstart.tv_nsec) / 1000;
    printf("reid> %ld\n", delta_us);
#endif

    double dismat;
    bool found           = false;
    int n                = 0;
    double best_combined = 999999.0;
    double best_distance = 99.0;
    int best_match;

    for (auto face = kernel_priv->faces.begin(); face != kernel_priv->faces.end(); face++) {
        if (face->last_frame_seen == frame_num) {
            // We already matched this one, let's move on
            n++;
            continue;
        }

        spatial_distance = calc_spatial(xctr, yctr, face->xctr, face->yctr);
        if (spatial_distance > (kernel_priv->max_distance_per_frame * (frame_num - face->last_frame_seen))) {
            if (kernel_priv->print_reid)
                printf("buffer [%d] face [%d] skipping for distance (%f > %d)\n", buf_num, n, spatial_distance, kernel_priv->max_distance_per_frame * (frame_num - face->last_frame_seen));
            n++;
            continue;
        }

        dismat          = cv::norm(face->features, featnew);
        distance_metric = dismat * spatial_distance;

        if ((distance_metric < best_combined) && (dismat < kernel_priv->threshold)) {
            if (kernel_priv->print_reid)
                std::cout << "Buffer [" << buf_num << "] found best match [" << n << "] with distance " << distance_metric << ", spatial " << spatial_distance << std::endl;
            found         = true;
            best_combined = distance_metric;
            best_distance = spatial_distance;
            best_match    = n;
        }
        n++;

#if 0

        if (reid_db[i].data) {
            featprev = reid_db[i];
            // dismat = ivas_cosine_distance(featprev, featnew);
            dismat           = cv::norm(featprev, featnew);
            spatial_distance = calc_spatial(xctr, yctr, kernel_priv->xctr[i], kernel_priv->yctr[i]);
            distance_metric  = dismat * spatial_distance;
#ifdef DEBUG
            printf("distance_metric : %.3lf \n", distance_metric);
#endif
            if (distance_metric < min_dismat) {
                found      = true;
                min_dismat = distance_metric;
                min_i      = i;
            }
        }
        else {
            break;
        }
#endif
    }

    if (found) {
        featnew.copyTo(kernel_priv->faces[best_match].features);
        kernel_priv->faces[best_match].last_frame_seen = frame_num;
        kernel_priv->faces[best_match].xctr            = xctr;
        kernel_priv->faces[best_match].yctr            = yctr;
    }
    else {
        _Face f;
        f.features        = featnew.clone();
        f.id              = id_ctr;
        f.last_frame_seen = frame_num;
        f.xctr            = xctr;
        f.yctr            = yctr;
        kernel_priv->faces.push_back(f);
        id_ctr++;
        best_match = id_ctr - 1;
    }

    return best_match;

#if 0
    if (found)
        i = min_i;
    if (!found && (i == MAX_REID)) /* all filled and none matched, need to circle back */
        i = (kernel_priv->top++ % MAX_REID);
    if (kernel_priv->first_frame_ref) {
        if (!found && kernel_priv->top <= MAX_REID)
            reid_db[i] = featnew.clone();
    }
    else {
        reid_db[i] = featnew.clone();
    }
#endif
}

extern "C"
{
    int32_t xlnx_kernel_init(IVASKernel *handle)
    {
        json_t *jconfig = handle->kernel_config;
        json_t *val; /* kernel config from app */

        handle->is_multiprocess     = 1;
        ReidKernelPriv *kernel_priv = (ReidKernelPriv *)calloc(1, sizeof(ReidKernelPriv));
        if (!kernel_priv) {
            printf("Error: Unable to allocate reID kernel memory\n");
        }

        /* parse config */
        val = json_object_get(jconfig, "threshold");
        if (!val || !json_is_number(val))
            kernel_priv->threshold = DEFAULT_REID_THRESHOLD;
        else
            kernel_priv->threshold = json_number_value(val);
        printf("REID: Threshold value %lf\n", kernel_priv->threshold);

        val = json_object_get(jconfig, "first_frame_ref");
        if (!val || !json_is_boolean(val))
            kernel_priv->first_frame_ref = false;
        else
            kernel_priv->first_frame_ref = json_boolean_value(val);
        printf("REID: First frame option %d\n", kernel_priv->first_frame_ref);

        val = json_object_get(jconfig, "dump_frames");
        if (!val || !json_is_boolean(val))
            kernel_priv->dump_frames = false;
        else
            kernel_priv->dump_frames = json_boolean_value(val);
        printf("REID: Dump Frame option %d\n", kernel_priv->dump_frames);

        val = json_object_get(jconfig, "print_reid");
        if (!val || !json_is_boolean(val))
            kernel_priv->print_reid = false;
        else
            kernel_priv->print_reid = json_boolean_value(val);
        printf("REID: First frame option %d\n", kernel_priv->print_reid);

        val = json_object_get(jconfig, "max_distance_per_frame");
        if (!val || !json_is_number(val))
            kernel_priv->max_distance_per_frame = 100;
        else
            kernel_priv->max_distance_per_frame = json_number_value(val);
        printf("REID: Max face movement expected per frame %d\n", kernel_priv->max_distance_per_frame);

        val = json_object_get(jconfig, "max_face_retention");
        if (!val || !json_is_number(val))
            kernel_priv->max_face_age = 100;
        else
            kernel_priv->max_face_age = json_number_value(val);
        printf("REID: Max face retention (frames) %d\n", kernel_priv->max_face_age);

        kernel_priv->top = 0;
        kernel_priv->det = vitis::ai::Reid::create("reid");

        handle->kernel_priv = (void *)kernel_priv;
        return 0;
    }

    uint32_t xlnx_kernel_deinit(IVASKernel *handle)
    {
        ReidKernelPriv *kernel_priv = (ReidKernelPriv *)handle->kernel_priv;
        free(kernel_priv);
        return 0;
    }

    int32_t xlnx_kernel_start(IVASKernel *handle, int start /*unused */, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
    {
        GstIvasMeta *ivas_meta = NULL;
        uint32_t i             = 0, n_obj;
        IVASFrame *in_ivas_frame;
        in_ivas_frame               = input[0];
        IvasObjectMetadata *xva_obj = NULL;
        GstBuffer *buffer, *buffer_crop;
        GstMapInfo info, info_crop;
        GstVideoMeta *vmeta, *vmeta_crop;
        ReidKernelPriv *kernel_priv = (ReidKernelPriv *)handle->kernel_priv;
        static int frame_num        = 0;
        int xctr, yctr;
        frame_num++;

        /* get metadata from input */
        ivas_meta = gst_buffer_get_ivas_meta((GstBuffer *)in_ivas_frame->app_priv);
        if (ivas_meta == NULL) {
            return 0;
        }
        else if (g_list_length(ivas_meta->xmeta.objects) > MAX_NUM_OBJECT) {
            printf("Can't process more then %d objects", MAX_NUM_OBJECT);
            return -1;
        }
        n_obj = ivas_meta ? g_list_length(ivas_meta->xmeta.objects) : 0;
        for (i = 0; i < n_obj; i++) {
            xva_obj = (IvasObjectMetadata *)g_list_nth_data(ivas_meta->xmeta.objects, i);
            if (xva_obj) {
                buffer_crop = (GstBuffer *)g_list_nth_data(xva_obj->obj_list, 0); /* original crop */
                buffer      = (GstBuffer *)g_list_nth_data(xva_obj->obj_list, 1); /* 80x80 resized image*/
                xctr        = xva_obj->bbox_meta.xmax - ((xva_obj->bbox_meta.xmax - xva_obj->bbox_meta.xmin) / 2);
                yctr        = xva_obj->bbox_meta.ymax - ((xva_obj->bbox_meta.ymax - xva_obj->bbox_meta.ymin) / 2);
            }
            else {
                printf("ERROR: IVAS REID: Unable to get meta data pointer");
                return -1;
            }
            gst_buffer_map(buffer, &info, GST_MAP_READ);
            gst_buffer_map(buffer_crop, &info_crop, GST_MAP_READ);

            vmeta      = gst_buffer_get_video_meta(buffer);
            vmeta_crop = gst_buffer_get_video_meta(buffer_crop);
            if (!vmeta || !vmeta_crop) {
                printf("ERROR: IVAS REID: video meta not present in buffer");
            }
            else if (vmeta->width == 80 && vmeta->height == 80) { /* be 100% sure */
                char *indata = (char *)info.data;
                cv::Mat image(vmeta->height, vmeta->width, CV_8UC3, indata);
                xva_obj->obj_id = ivas_reid_run(image, handle, frame_num, i, xctr, yctr);
            }
            else {
                printf("ERROR: IVAS REID: Invalid resolution for reid (%u x %u)\n", vmeta->width, vmeta->height);
            }
            if (kernel_priv->dump_frames) {
                char name[100], name_crop[100];
                FILE *fp;
                uint32_t size;
                char *indata      = (char *)info.data;
                char *indata_crop = (char *)info_crop.data;

                sprintf(name, "frame_%d_resize_obj%d_%dx%d.rgb", frame_num, i, vmeta->width, vmeta->height);
                sprintf(name_crop, "frame_%d_crop_obj%d_%dx%d.rgb", frame_num, i, vmeta_crop->width, vmeta_crop->height);
                fp   = fopen(name, "w");
                size = vmeta->width * vmeta->height * 3;
                fwrite(indata, 1, size, fp);
                fclose(fp);
                fp   = fopen(name_crop, "w");
                size = vmeta_crop->width * vmeta_crop->height * 3;
                fwrite(indata_crop, 1, size, fp);
                fclose(fp);
            }
            gst_buffer_unmap(buffer, &info);
            gst_buffer_unmap(buffer, &info_crop);
        }

        /// Every MAX_FACE_AGE frames we'll take a pass through the list and clean it up
        if (frame_num % kernel_priv->max_face_age == 0) {
            for (auto it = kernel_priv->faces.begin(); it != kernel_priv->faces.end();) {
                if (it->last_frame_seen < (frame_num - kernel_priv->max_face_age)) {
                    if (kernel_priv->print_reid)
                        printf("Face ID %d aged out\n", it->id);
                    it = kernel_priv->faces.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        return 0;
    }

    int32_t xlnx_kernel_done(IVASKernel *handle)
    {
        /* dummy */
        return 0;
    }
}
