#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

#config_dataflow -default_channel fifo -fifo_depth 1024 -start_fifo_depth 8
#set_param hls.enable_reflow_partition_reshape 0
config_dataflow -start_fifo_depth 16
