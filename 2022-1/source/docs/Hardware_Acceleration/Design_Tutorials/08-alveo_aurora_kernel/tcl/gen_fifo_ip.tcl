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

