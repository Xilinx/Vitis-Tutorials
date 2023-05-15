#!/bin/sh

# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11

rm -rf ./rtc_gen.xo
vivado -mode batch -source package_rtc_gen.tcl

