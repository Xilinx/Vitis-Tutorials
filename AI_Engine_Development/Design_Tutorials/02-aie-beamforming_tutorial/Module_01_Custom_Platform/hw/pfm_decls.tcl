# © Copyright 2020 Xilinx, Inc.
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

set_property PFM_NAME \
    "xilinx.com:vck190_v1_0:vck190_v1_0:1.0" \
    [get_files build/vck190_v1_0_vivado/vck190_v1_0.srcs/sources_1/bd/vck190_v1_0/vck190_v1_0.bd]

set pl_clks [get_bd_cells /pl_clkwiz]
set pl_clk_ctrl [get_bd_pins $pl_clks/clk_out1]

# tag the dynamic region clock ports
set clk_props [dict create]
for {set x 1} {$x < 6} {incr x} {
  set pl_clk_pin [get_bd_pins $pl_clks/clk_out${x}]
  set pl_rst [get_bd_cells /pl_clk_out${x}_rst]
  set id [expr $x - 1]
  set d false
  if {$x == 1} {
    set d true
  }
  set clk_settings [concat id \"$id\" is_default \"$d\" proc_sys_reset \"$pl_rst\" status \"fixed\"]
  dict set clk_props clk_out$x $clk_settings
}
set_property PFM.CLOCK $clk_props $pl_clks

# tag the PL control ports
set gp_masters []
for {set x 1} {$x < 16} {incr x} {
  lappend gp_masters M[format %02d $x]_AXI {memport "M_AXI_GP" sptag "GP"}
}
for {set x 0} {$x < 16} {incr x} {
  set_property PFM.AXI_PORT $gp_masters [get_bd_cells /ctrl_sm_$x]
}

set noc_intf_tags []
# tag the AIE configuration NSU
lappend noc_intf_tags M01_AXI {memport "M_AXI_NOC" sptag "GP"}

# tag the DDR slave ports
for {set x [get_property CONFIG.NUM_SI [get_bd_cells /ps_noc]]} \
    {$x < [expr [get_property CONFIG.NUM_SI [get_bd_cells /ps_noc]] + 28]} \
    {incr x} {
  set sport S[format %02d $x]_AXI
  lappend noc_intf_tags S[format %02d $x]_AXI {memport "S_AXI_NOC" sptag "DDR"}
}

set_property PFM.AXI_PORT $noc_intf_tags [get_bd_cells /ps_noc]
