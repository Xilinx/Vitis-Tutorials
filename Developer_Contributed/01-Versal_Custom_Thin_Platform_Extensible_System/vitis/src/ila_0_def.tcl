#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

# System ILA core
set_property -dict [list \
  CONFIG.C_DATA_DEPTH 8192 \
  CONFIG.C_INPUT_PIPE_STAGES 6 \
  CONFIG.C_MEMORY_TYPE  1 \
  CONFIG.C_EN_STRG_QUAL 1 \
  CONFIG.C_PROBE0_MU_CNT 2 \
  CONFIG.ALL_PROBE_SAME_MU_CNT 2 \
] [get_bd_cells axis_ila_0]
