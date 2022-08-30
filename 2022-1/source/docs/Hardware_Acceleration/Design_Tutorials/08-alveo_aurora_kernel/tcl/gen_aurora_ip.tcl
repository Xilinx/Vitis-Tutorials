#
# Copyright 2022 Xilinx, Inc.
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
