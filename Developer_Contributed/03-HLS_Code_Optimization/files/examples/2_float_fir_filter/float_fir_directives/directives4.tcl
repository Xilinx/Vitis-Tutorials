# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

set_directive_top -name fir_filter "fir_filter"
set_directive_bind_op -op fmul -impl maxdsp "fir_filter" mult
set_directive_bind_op -op fadd -impl fulldsp "fir_filter" acc
set_directive_pipeline "fir_filter"
set_directive_array_partition -dim 1 -type complete "fir_filter" c
