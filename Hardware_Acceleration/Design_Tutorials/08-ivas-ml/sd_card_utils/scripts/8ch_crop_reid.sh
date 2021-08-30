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

#export GST_DEBUG=ivas_xm2m:4
gst-launch-1.0 filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t0 t0.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem0.sink_master ivas_xmetaaffixer name=scalem0 scalem0.src_master ! fakesink \
t0.src_1 ! scalem0.sink_slave_0 scalem0.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t1 t1.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem1.sink_master ivas_xmetaaffixer name=scalem1 scalem1.src_master ! fakesink \
t1.src_1 ! scalem1.sink_slave_0 scalem1.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t2 t2.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem2.sink_master ivas_xmetaaffixer name=scalem2 scalem2.src_master ! fakesink \
t2.src_1 ! scalem2.sink_slave_0 scalem2.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t3 t3.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem3.sink_master ivas_xmetaaffixer name=scalem3 scalem3.src_master ! fakesink \
t3.src_1 ! scalem3.sink_slave_0 scalem3.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t4 t4.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem4.sink_master ivas_xmetaaffixer name=scalem4 scalem4.src_master ! fakesink \
t4.src_1 ! scalem4.sink_slave_0 scalem4.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t5 t5.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem5.sink_master ivas_xmetaaffixer name=scalem5 scalem5.src_master ! fakesink \
t5.src_1 ! scalem5.sink_slave_0 scalem5.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t6 t6.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem6.sink_master ivas_xmetaaffixer name=scalem6 scalem6.src_master ! fakesink \
t6.src_1 ! scalem6.sink_slave_0 scalem6.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false \
filesrc location=/home/root/videos/dahua2.mp4 ! qtdemux ! h264parse ! omxh264dec internal-entropy-buffers=3 !  \
tee name=t7 t7.src_0 ! \
queue ! ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
scalem7.sink_master ivas_xmetaaffixer name=scalem7 scalem7.src_master ! fakesink \
t7.src_1 ! scalem7.sink_slave_0 scalem7.src_slave_0 ! queue ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
fpsdisplaysink video-sink="fakesink" text-overlay=false sync=false -v
