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

export XILINX_XRT=/usr
export DPU_XCLBIN_PATH=/mnt/sd-mmcblk0p1/dpu.xclbin

#HP0
/sbin/devmem 0xFD380008 w 0x7
/sbin/devmem 0xFD38001C w 0x7
/sbin/devmem 0xFD380004 w 0xF
/sbin/devmem 0xFD380018 w 0xF

#HP1
/sbin/devmem 0xFD390008 w 0x3
/sbin/devmem 0xFD39001C w 0x3
/sbin/devmem 0xFD390004 w 0xF
/sbin/devmem 0xFD390018 w 0xF

#HP2
/sbin/devmem 0xFD3A0000 w 0x3b0
/sbin/devmem 0xFD3A0014 w 0x3b0
/sbin/devmem 0xFD3A0008 w 0x3
/sbin/devmem 0xFD3A001C w 0x3
/sbin/devmem 0xFD3A0004 w 0xF
/sbin/devmem 0xFD3A0018 w 0xF

#HP3
/sbin/devmem 0xFD3B0000 w 0x3b0
/sbin/devmem 0xFD3B0014 w 0x3b0
/sbin/devmem 0xFD3B0008 w 0x3
/sbin/devmem 0xFD3B001C w 0x3
/sbin/devmem 0xFD3B0004 w 0xF
/sbin/devmem 0xFD3B0018 w 0xF

#HPC0
/sbin/devmem 0xFD360000 w 0x3b0
/sbin/devmem 0xFD360014 w 0x3b0
/sbin/devmem 0xFD360008 w 0x3
/sbin/devmem 0xFD36001C w 0x3
/sbin/devmem 0xFD360004 w 0xF
/sbin/devmem 0xFD360018 w 0xF

#HPC1
/sbin/devmem 0xFD370000 w 0x3b0
/sbin/devmem 0xFD370014 w 0x3b0
/sbin/devmem 0xFD370008 w 0x3
/sbin/devmem 0xFD37001C w 0x3
/sbin/devmem 0xFD370004 w 0xF
/sbin/devmem 0xFD370018 w 0xF

#LPD
/sbin/devmem 0xFF9B0008 w 0x3
/sbin/devmem 0xFF9B001C w 0x3
/sbin/devmem 0xFF9B0004 w 0xF
/sbin/devmem 0xFF9B0018 w 0xF

echo "Init script done"
