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

# set the device part from command line argvs
set_part [lindex $argv 0]

# ----------------------------------------------------------------------------
# create clock gen IP
# ----------------------------------------------------------------------------
create_ip -name clk_wiz \
          -vendor xilinx.com \
          -library ip \
          -version 6.0 \
          -module_name clk_gen \
          -dir ./ip_generation

set_property -dict [list CONFIG.USE_PHASE_ALIGNMENT {false} \
                         CONFIG.PRIM_SOURCE {No_buffer} \
                         CONFIG.PRIM_IN_FREQ {300.000} \
                         CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {400.000} \
                         CONFIG.USE_RESET {false} \
                         CONFIG.CLKOUT1_DRIVES {Buffer} \
                         CONFIG.RESET_PORT {resetn}] \
             [get_ips clk_gen]

generate_target all [get_files  ./ip_generation/clk_gen/clk_gen.xci]

# ----------------------------------------------------------------------------
# create axi stream clock converter IP
# ----------------------------------------------------------------------------
create_ip -name axis_clock_converter \
          -vendor xilinx.com \
          -library ip \
          -version 1.1 \
          -module_name axis_clock_converter \
          -dir ./ip_generation
          
set_property -dict [list CONFIG.TDATA_NUM_BYTES {16} \
                         CONFIG.IS_ACLK_ASYNC {1} \
                         CONFIG.SYNCHRONIZATION_STAGES {3}] \
             [get_ips axis_clock_converter]

generate_target all [get_files  ./ip_generation/axis_clock_converter/axis_clock_converter.xci]


# ----------------------------------------------------------------------------
# create axi clock converter IP
# ----------------------------------------------------------------------------
create_ip -name axi_clock_converter \
          -vendor xilinx.com \
          -library ip \
          -version 2.1 \
          -module_name axi_clock_converter \
          -dir ./ip_generation
    
set_property -dict [list CONFIG.PROTOCOL {AXI4LITE} \
                         CONFIG.ADDR_WIDTH {12} \
                         CONFIG.SYNCHRONIZATION_STAGES {3} \
                         CONFIG.DATA_WIDTH {32} \
                         CONFIG.ID_WIDTH {0} \
                         CONFIG.AWUSER_WIDTH {0} \
                         CONFIG.ARUSER_WIDTH {0} \
                         CONFIG.RUSER_WIDTH {0} \
                         CONFIG.WUSER_WIDTH {0} \
                         CONFIG.BUSER_WIDTH {0}] \
             [get_ips axi_clock_converter]

generate_target all [get_files ./ip_generation/axi_clock_converter/axi_clock_converter.xci]


# ----------------------------------------------------------------------------
# generate axi stream master vip
# ----------------------------------------------------------------------------
create_ip -name axi4stream_vip \
          -vendor xilinx.com \
          -library ip \
          -version 1.1 \
          -module_name axis_vip_mst \
          -dir ./ip_generation

set_property -dict [list CONFIG.INTERFACE_MODE {MASTER} \
                         CONFIG.TDATA_NUM_BYTES {16}] \
             [get_ips axis_vip_mst]
             
generate_target all [get_files  ./ip_generation/axis_vip_mst/axis_vip_mst.xci]

# ----------------------------------------------------------------------------
# generate axi stream slave vip
# ----------------------------------------------------------------------------
create_ip -name axi4stream_vip \
          -vendor xilinx.com \
          -library ip \
          -version 1.1 \
          -module_name axis_vip_slv \
          -dir ./ip_generation
          
set_property -dict [list CONFIG.INTERFACE_MODE {SLAVE} \
                         CONFIG.TDATA_NUM_BYTES {16}] \
             [get_ips axis_vip_slv]
             
generate_target all [get_files  ./ip_generation/axis_vip_slv/axis_vip_slv.xci]

# ----------------------------------------------------------------------------
# generate axi master vip
# ----------------------------------------------------------------------------
create_ip -name axi_vip \
          -vendor xilinx.com \
          -library ip \
          -version 1.1 \
          -module_name axi_vip_mst \
          -dir ./ip_generation
          
set_property -dict [list CONFIG.INTERFACE_MODE {MASTER} \
                         CONFIG.PROTOCOL {AXI4LITE} \
                         CONFIG.SUPPORTS_NARROW {0} \
                         CONFIG.HAS_BURST {0} \
                         CONFIG.HAS_LOCK {0} \
                         CONFIG.HAS_CACHE {0} \
                         CONFIG.HAS_REGION {0} \
                         CONFIG.HAS_QOS {0} \
                         CONFIG.HAS_PROT {0} \
                         CONFIG.HAS_WSTRB {1}] \
             [get_ips axi_vip_mst]
             
generate_target all [get_files  ./ip_generation/axi_vip_mst/axi_vip_mst.xci]
