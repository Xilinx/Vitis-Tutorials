# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


set_directive_top -name fir_filter "fir_filter"
set_directive_pipeline -off "fir_filter/Shift_Accum_Loop"
