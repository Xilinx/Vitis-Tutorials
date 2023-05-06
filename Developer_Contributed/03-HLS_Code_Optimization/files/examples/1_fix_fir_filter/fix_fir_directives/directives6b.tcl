# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


set_directive_top -name fir_filter "fir_filter"
set_directive_pipeline -II 1 "fir_filter"
set_directive_array_partition -dim 1 -type complete "fir_filter" c
