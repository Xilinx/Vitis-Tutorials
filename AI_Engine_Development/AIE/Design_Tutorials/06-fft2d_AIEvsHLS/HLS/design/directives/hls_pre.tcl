#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

#config_dataflow -default_channel fifo -fifo_depth 1024 -start_fifo_depth 8
#set_param hls.enable_reflow_partition_reshape 0
config_compile -pipeline_style flp
config_dataflow -start_fifo_depth 16
