# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

set_directive_top -name fir_filter "fir_filter"
set_directive_pipeline -II 1 "fir_filter/Shift_Accum_Loop"
