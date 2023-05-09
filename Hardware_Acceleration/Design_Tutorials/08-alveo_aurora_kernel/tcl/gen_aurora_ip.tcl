# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

# set the device part from command line argvs
set_part [lindex $argv 0]


# ----------------------------------------------------------------------------
# generate Aurora IP
# ----------------------------------------------------------------------------
create_ip -name aurora_64b66b \
          -vendor xilinx.com \
          -library ip \
          -version 12.0 \
          -module_name aurora_64b66b_0 \
          -dir ./ip_generation

set_property -dict [list CONFIG.C_AURORA_LANES {4} \
                         CONFIG.C_LINE_RATE {10.3125} \
                         CONFIG.C_REFCLK_FREQUENCY {161.1328125} \
                         CONFIG.C_INIT_CLK {100} \
                         CONFIG.interface_mode {Streaming} \
                         CONFIG.drp_mode {Native} \
                         CONFIG.SupportLevel {1}] \
             [get_ips aurora_64b66b_0]

generate_target all [get_files ./ip_generation/aurora_64b66b_0/aurora_64b66b_0.xci]
