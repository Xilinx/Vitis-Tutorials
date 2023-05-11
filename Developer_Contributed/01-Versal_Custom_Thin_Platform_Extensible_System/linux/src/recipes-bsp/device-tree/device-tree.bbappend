#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI                 += "file://system-conf.dtsi"
EXTRA_OVERLAYS           = "system-user.dtsi"

