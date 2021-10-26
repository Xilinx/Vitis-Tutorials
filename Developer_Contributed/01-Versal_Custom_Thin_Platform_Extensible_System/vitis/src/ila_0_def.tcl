#
# Copyright 2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
