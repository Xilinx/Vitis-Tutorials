#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************
set_property -name questa.compile.sccom.more_options -value {-suppress sccom-6168} -objects [current_fileset -simset]

