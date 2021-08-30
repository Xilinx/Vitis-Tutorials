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

#!/usr/bin/env bash
# Short-Description: Resize the root filesystem to fill partition

ROOT_PART=$(mount | sed -n 's|^\(.*\) on / .*|\1|p')
ROOT_DEV=$(echo $ROOT_PART | cut -c 1-12)
PART_NUM=$(echo -n $ROOT_PART | tail -c 1)
MOUNTED=$(mount | sed -n '/^\(.*\)'$PART_NUM' on \/[a-zA-Z0-9].*ext/p')

do_expand_rootfs() 
{
	local target=$1
	local part_num=$2

	if [ -n "${MOUNTED}" ]; then
		parted ${target} resizepart ${part_num} yes 100% > /dev/null 2>&1
	else
		parted ${target} resizepart ${part_num} 100% > /dev/null 2>&1
	fi
}

ext4_resize() 
{
	if [ "$ROOT_DEV" != "/dev/mmcblk1" -a "$ROOT_DEV" != "/dev/mmcblk0" ]; then
		echo "$ROOT_DEV is not an emmc or sd card. Don't know how to expand"
		exit
	fi

	do_expand_rootfs $ROOT_DEV $PART_NUM && resize2fs $ROOT_PART > /dev/null 2>&1
}

ext4_resize

