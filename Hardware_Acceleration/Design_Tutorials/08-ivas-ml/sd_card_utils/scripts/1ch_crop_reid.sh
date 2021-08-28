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

modetest -M xlnx -w 39:g_alpha_en:0

gst-launch-1.0 -v \
	filesrc location=/home/root/videos/dahua2.mp4 ! \
	qtdemux ! \
	h264parse ! \
	omxh264dec internal-entropy-buffers=3 !  \
	tee name=t0 t0.src_0 ! \
		queue ! \
		ivas_xm2m kconfig="/home/root/jsons/kernel_resize_bgr.json" ! \
		ivas_xfilter kernels-config="/home/root/jsons/kernel_densebox_640_360.json" ! \
		scalem0.sink_master ivas_xmetaaffixer name=scalem0 scalem0.src_master ! \
		fakesink \
	t0.src_1 ! \
		scalem0.sink_slave_0 scalem0.src_slave_0 ! \
		queue ! \
		ivas_xfilter kernels-config="/home/root/jsons/kernel_crop.json" ! \
		ivas_xfilter kernels-config="/home/root/jsons/kernel_reid.json" ! \
		ivas_xfilter kernels-config="/home/root/jsons/kernel_swbbox.json" ! \
		fpsdisplaysink video-sink="kmssink driver-name=xlnx sync=false" text-overlay=false sync=false

modetest -M xlnx -w 39:g_alpha_en:1
