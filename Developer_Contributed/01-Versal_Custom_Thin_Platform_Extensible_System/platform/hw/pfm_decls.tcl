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

puts "Platform Category :\"${PLATFORM_TYPE}\""
puts "Platform Name : \"${PLATFORM_NAME}\""
puts "Version : \"${VER}\""

set_property PFM_NAME \
    "xilinx.com:${PLATFORM_TYPE}:${PLATFORM_NAME}:${VER}" \
    [get_files build/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/${PLATFORM_NAME}.bd]

set pl_clks [get_bd_cells /clk_wizard_0]

# tag the dynamic region clock ports
set clk_props [dict create]
for {set x 1} {$x <= 3} {incr x} {
  set pl_clk_pin [get_bd_pins $pl_clks/clk_out1_o${x}]
  set id [expr $x - 1]
  set pl_rst [get_bd_cells /sys_reset_${id}]
  set d false
  if {$x == 2} {
    set d true
  }
  set clk_settings [concat id \"$id\" is_default \"$d\" proc_sys_reset \"$pl_rst\" status \"fixed\"]
  dict set clk_props clk_out1_o$x $clk_settings
}
puts "PFM.CLOCK String : "
puts $clk_props
puts "********"
set_property PFM.CLOCK $clk_props $pl_clks

# tag the PL control ports
set gp_masters []
for {set x 1} {$x < 16} {incr x} {
  lappend gp_masters M[format %02d $x]_AXI {memport "M_AXI_GP" sptag "GP"}
}
for {set x 0} {$x < 1} {incr x} {
  set_property PFM.AXI_PORT $gp_masters [get_bd_cells /axi_smc_$x]
}

set noc_intf_tags []
# tag the AIE configuration NSU
lappend noc_intf_tags M01_AXI {memport "M_AXI_NOC" sptag "GP"}

# tag the DDR slave ports
for {set x [get_property CONFIG.NUM_SI [get_bd_cells /axi_noc_0]]} \
    {$x < [expr [get_property CONFIG.NUM_SI [get_bd_cells /axi_noc_0]] + 28]} \
    {incr x} {
  set sport S[format %02d $x]_AXI
  lappend noc_intf_tags S[format %02d $x]_AXI {memport "S_AXI_NOC" sptag "DDR"}
}

set_property PFM.AXI_PORT $noc_intf_tags [get_bd_cells /axi_noc_0]
set_property PFM.IRQ {intr {id 0 range 32}} [get_bd_cells /axi_intc_0]


## Hardcoding PFM properties for debug only ##
## Hardcoding PFM properties for debug only ##  # Create PFM attributes
## Hardcoding PFM properties for debug only ##  set_property PFM.IRQ {intr { id 0 range 32 }} [get_bd_cells /axi_intc_0]
## Hardcoding PFM properties for debug only ##
## Hardcoding PFM properties for debug only ##  set_property PFM.AXI_PORT { \
## Hardcoding PFM properties for debug only ##    S08_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S09_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S10_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S11_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S12_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S13_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S14_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S15_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S16_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S17_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S18_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S19_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S20_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S21_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    S22_AXI {memport "S_AXI_NOC" sptag "DDR" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M01_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M02_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M03_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M04_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M05_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M06_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M07_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M08_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M09_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M10_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M11_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M12_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M13_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M14_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##    M15_AXI {memport "M_AXI_NOC" sptag "GP" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  } [get_bd_cells /axi_noc_0]
## Hardcoding PFM properties for debug only ##
## Hardcoding PFM properties for debug only ##set_property PFM.AXI_PORT { \
## Hardcoding PFM properties for debug only ##  S01_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S02_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S03_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S04_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S05_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S06_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S07_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S08_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S09_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S10_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S11_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S12_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S13_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S14_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  S15_AXI {memport "S_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M01_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M02_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M03_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M04_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M05_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M06_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M07_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M08_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M09_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M10_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M11_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M12_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M13_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M14_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##  M15_AXI {memport "M_AXI_GP" sptag "" memory "" is_range "true"} \
## Hardcoding PFM properties for debug only ##} [get_bd_cells /axi_smc_0]
## Hardcoding PFM properties for debug only ##
## Hardcoding PFM properties for debug only ##set_property PFM.CLOCK { \
## Hardcoding PFM properties for debug only ##  clk_out1 {id "0" is_default "true"  proc_sys_reset "/sys_reset_0" status "fixed" freq_hz "500000000"} \
## Hardcoding PFM properties for debug only ##  clk_out2 {id "1" is_default "false" proc_sys_reset "/sys_reset_1" status "fixed" freq_hz "250000000"} \
## Hardcoding PFM properties for debug only ##  clk_out3 {id "2" is_default "false" proc_sys_reset "/sys_reset_2" status "fixed" freq_hz "100000000"} \
## Hardcoding PFM properties for debug only ##} [get_bd_cells /clk_wizard_0]
## Hardcoding PFM properties for debug only ##
## Hardcoding PFM properties for debug only ###puts "Setting PFM_NAME for : \"$design_name\" "
## Hardcoding PFM properties for debug only #### passing the name as variable breaks in Vitis ##set_property PFM_NAME {xilinx.com:vck190_es:${design_name}:1.0} [get_files [current_bd_design].bd]
## Hardcoding PFM properties for debug only ##set_property PFM_NAME {xilinx.com:vck190_es:vck190_thin:1.0} [get_files [current_bd_design].bd]
