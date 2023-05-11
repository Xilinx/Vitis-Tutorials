#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

do_install:append() {
  sed -i  '/mmcblk0p1/s/^#//g'                   ${D}${sysconfdir}/fstab
  sed -i  's+/media/card+/run/media/mmcblk0p1+g' ${D}${sysconfdir}/fstab
  sed -i  '/mmcblk0p1/s/,noauto//g'              ${D}${sysconfdir}/fstab
}

