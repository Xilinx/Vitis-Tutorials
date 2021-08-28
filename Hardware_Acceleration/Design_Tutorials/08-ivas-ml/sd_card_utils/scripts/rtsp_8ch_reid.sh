#
# Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
# EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
# not be used in advertising or otherwise to promote the sale, use or other
# dealings in this Software without prior written authorization from Xilinx.
#

#!/bin/bash
LATENCY=300

modetest -M xlnx -w 39:g_alpha_en:0

STREAM_SOURCE_0=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_1=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_2=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_3=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_4=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_5=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_6=rtsp://10.0.0.1:8554/face
STREAM_SOURCE_7=rtsp://10.0.0.1:8554/face

VCU_QUEUE_SETTINGS="max-size-buffers=30 leaky=1 min-threshold-buffers=10"

gst-launch-1.0 -v \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_0 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_0 ! \
		tee name=t0 t0.src_0 ! \
			queue name=facedetect_0 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem0.sink_master ivas_xmetaaffixer name=scalem0 scalem0.src_master ! \
			fakesink \
		t0.src_1 ! \
			queue ! \
			scalem0.sink_slave_0 scalem0.src_slave_0 ! \
			queue name = crop_reid_0 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fpsdisplaysink video-sink="kmssink driver-name=xlnx sync=false" text-overlay=false sync=false \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_1 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_1 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		tee name=t1 t1.src_0 ! \
			queue name=facedetect_1 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem1.sink_master ivas_xmetaaffixer name=scalem1 scalem1.src_master ! \
			fakesink \
		t1.src_1 ! \
			queue ! \
			scalem1.sink_slave_0 scalem1.src_slave_0 ! \
			queue name = crop_reid_1 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_2 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_2 ! \
		tee name=t_2 t_2.src_0 ! \
			queue name=facedetect_2 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem_2.sink_master ivas_xmetaaffixer name=scalem_2 scalem_2.src_master ! \
			fakesink \
		t_2.src_1 ! \
			queue ! \
			scalem_2.sink_slave_0 scalem_2.src_slave_0 ! \
			queue name = crop_reid_2 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_3 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_3 ! \
		tee name=t_3 t_3.src_0 ! \
			queue name=facedetect_3 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem_3.sink_master ivas_xmetaaffixer name=scalem_3 scalem_3.src_master ! \
			fakesink \
		t_3.src_1 ! \
			queue ! \
			scalem_3.sink_slave_0 scalem_3.src_slave_0 ! \
			queue name = crop_reid_3 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_4 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_4 ! \
		tee name=t_4 t_4.src_0 ! \
			queue name=facedetect_4 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem_4.sink_master ivas_xmetaaffixer name=scalem_4 scalem_4.src_master ! \
			fakesink \
		t_4.src_1 ! \
			queue ! \
			scalem_4.sink_slave_0 scalem_4.src_slave_0 ! \
			queue name = crop_reid_4 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_5 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_5 ! \
		tee name=t_5 t_5.src_0 ! \
			queue name=facedetect_5 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem_5.sink_master ivas_xmetaaffixer name=scalem_5 scalem_5.src_master ! \
			fakesink \
		t_5.src_1 ! \
			queue ! \
			scalem_5.sink_slave_0 scalem_5.src_slave_0 ! \
			queue name = crop_reid_5 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_6 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_6 ! \
		tee name=t_6 t_6.src_0 ! \
			queue name=facedetect_6 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem_6.sink_master ivas_xmetaaffixer name=scalem_6 scalem_6.src_master ! \
			fakesink \
		t_6.src_1 ! \
			queue ! \
			scalem_6.sink_slave_0 scalem_6.src_slave_0 ! \
			queue name = crop_reid_6 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink \
	rtspsrc latency=$LATENCY location=$STREAM_SOURCE_7 ! \
		rtph264depay  ! \
		h264parse ! \
		omxh264dec internal-entropy-buffers=3 ! \
		videorate ! "video/x-raw, framerate=20/1" ! \
		queue $VCU_QUEUE_SETTINGS name=vcu_buffer_7 ! \
		tee name=t_7 t_7.src_0 ! \
			queue name=facedetect_7 ! \
			ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
			scalem_7.sink_master ivas_xmetaaffixer name=scalem_7 scalem_7.src_master ! \
			fakesink \
		t_7.src_1 ! \
			queue ! \
			scalem_7.sink_slave_0 scalem_7.src_slave_0 ! \
			queue name = crop_reid_7 ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
			ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
			fakesink

modetest -M xlnx -w 39:g_alpha_en:1
