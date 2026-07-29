#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

do_install:append() {
  sed -i  '/mmcblk0p1/s/^#//g'                   ${D}${sysconfdir}/fstab
  sed -i  's+/media/card+/run/media/mmcblk0p1+g' ${D}${sysconfdir}/fstab
  sed -i  '/mmcblk0p1/s/,noauto//g'              ${D}${sysconfdir}/fstab
}

