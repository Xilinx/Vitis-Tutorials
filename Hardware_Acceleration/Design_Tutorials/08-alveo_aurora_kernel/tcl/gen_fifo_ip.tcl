# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

# set the device part from command line argvs
set_part [lindex $argv 0]

# ----------------------------------------------------------------------------
# generate AXIS data fifo IP
# ----------------------------------------------------------------------------
create_ip -name axis_data_fifo \
          -vendor xilinx.com \
          -library ip \
          -version 2.0 \
          -module_name axis_data_fifo_0 \
          -dir ./ip_generation

set_property -dict [list CONFIG.TDATA_NUM_BYTES {32} \
                         CONFIG.IS_ACLK_ASYNC {1}] \
             [get_ips axis_data_fifo_0]

generate_target all [get_files ./ip_generation/axis_data_fifo_0/axis_data_fifo_0.xci]

