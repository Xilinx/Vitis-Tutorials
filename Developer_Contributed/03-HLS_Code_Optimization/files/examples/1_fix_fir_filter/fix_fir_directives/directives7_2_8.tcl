# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

set_directive_top -name fir_filter "fir_filter"
set_directive_pipeline -II 2 "fir_filter"
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_allocation -limit 8 -type operation "fir_filter" mul
