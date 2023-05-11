#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

puts "Platform ID : \"xilinx.com:${PLATFORM_TYPE}:${PLATFORM_NAME}:${VER}\""
puts "Platform BD : \"build/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/${PLATFORM_NAME}.bd\""

# Configure PFM with PFM_NAME
set_property PFM_NAME "xilinx.com:${PLATFORM_TYPE}:${PLATFORM_NAME}:${VER}" [get_files build/${PLATFORM_NAME}_vivado/${PLATFORM_NAME}.srcs/sources_1/bd/${PLATFORM_NAME}/${PLATFORM_NAME}.bd]

# Configure PFM for /clk_wizard_0
set NUM_CLOCKS_TOTAL   2
set NUM_CLOCKS_MBUFGCE 1

set clk_props [dict create]
for {set x 1} {$x <= $NUM_CLOCKS_TOTAL} {incr x} {
  for {set y 1} {$y <= 4} {incr y} {
    if {$x <= $NUM_CLOCKS_MBUFGCE || $y == 1} {
      if {$x <= $NUM_CLOCKS_MBUFGCE} {
        set clk_id [expr (($x - 1) * 4) + $y - 1]
      } else {
        set clk_id [expr ($NUM_CLOCKS_MBUFGCE * 4) + $x - $NUM_CLOCKS_MBUFGCE - 1]
      }
      if {$x == 1 && $y == 1} {
        set clk_default true
      } else {
        set clk_default false
      }
      set pl_rst [get_bd_cells /proc_sys_reset_0]
      set clk_settings [concat id \"$clk_id\" is_default \"$clk_default\" proc_sys_reset \"$pl_rst\" status \"fixed\"]
      if {$x <= $NUM_CLOCKS_MBUFGCE} {
        dict set clk_props clk_out${x}_o${y} $clk_settings
      } else {
        dict set clk_props clk_out${x} $clk_settings
      }
    }
  }
}

set_property PFM.CLOCK $clk_props [get_bd_cells /clk_wizard_0]

# Configure PFM for /icn_ctrl
set gp_masters []
for {set x 1} {$x < 16} {incr x} {
  lappend gp_masters M[format %02d $x]_AXI {memport "M_AXI_GP" sptag "GP"}
}

set_property PFM.AXI_PORT $gp_masters [get_bd_cells /icn_ctrl]

# Configure PFM for /axi_intc_0
set_property PFM.IRQ {intr {id 0 range 31}} [get_bd_cells /axi_intc_0]

# Configure PFM for /cips_noc
set_property PFM.AXI_PORT {M00_AXI {memport "NOC_MASTER"}} [get_bd_cells /cips_noc]

# Configure PFM for /noc_ddr4
set_property PFM.AXI_PORT {S00_AXI {memport "S_AXI_NOC" sptag "DDR"} S01_AXI {memport "S_AXI_NOC" sptag "DDR"} S02_AXI {memport "S_AXI_NOC" sptag "DDR"} S03_AXI {memport "S_AXI_NOC" sptag "DDR"} S04_AXI {memport "S_AXI_NOC" sptag "DDR"} S05_AXI {memport "S_AXI_NOC" sptag "DDR"} S06_AXI {memport "S_AXI_NOC" sptag "DDR"} S07_AXI {memport "S_AXI_NOC" sptag "DDR"} S08_AXI {memport "S_AXI_NOC" sptag "DDR"} S09_AXI {memport "S_AXI_NOC" sptag "DDR"} S10_AXI {memport "S_AXI_NOC" sptag "DDR"} S11_AXI {memport "S_AXI_NOC" sptag "DDR"} S12_AXI {memport "S_AXI_NOC" sptag "DDR"} S13_AXI {memport "S_AXI_NOC" sptag "DDR"} S14_AXI {memport "S_AXI_NOC" sptag "DDR"} S15_AXI {memport "S_AXI_NOC" sptag "DDR"} S16_AXI {memport "S_AXI_NOC" sptag "DDR"} S17_AXI {memport "S_AXI_NOC" sptag "DDR"} S18_AXI {memport "S_AXI_NOC" sptag "DDR"} S19_AXI {memport "S_AXI_NOC" sptag "DDR"} S20_AXI {memport "S_AXI_NOC" sptag "DDR"} S21_AXI {memport "S_AXI_NOC" sptag "DDR"} S22_AXI {memport "S_AXI_NOC" sptag "DDR"} S23_AXI {memport "S_AXI_NOC" sptag "DDR"} S24_AXI {memport "S_AXI_NOC" sptag "DDR"} S25_AXI {memport "S_AXI_NOC" sptag "DDR"} S26_AXI {memport "S_AXI_NOC" sptag "DDR"} S27_AXI {memport "S_AXI_NOC" sptag "DDR"}} [get_bd_cells /noc_ddr4]

# Configure PFM for /noc_lpddr4
set_property PFM.AXI_PORT {S00_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S01_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S02_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S03_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S04_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S05_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S06_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S07_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S08_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S09_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S10_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S11_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S12_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S13_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S14_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S15_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S16_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S17_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S18_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S19_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S20_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S21_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S22_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S23_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S24_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S25_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S26_AXI {memport "S_AXI_NOC" sptag "LPDDR"} S27_AXI {memport "S_AXI_NOC" sptag "LPDDR"}} [get_bd_cells /noc_lpddr4]


