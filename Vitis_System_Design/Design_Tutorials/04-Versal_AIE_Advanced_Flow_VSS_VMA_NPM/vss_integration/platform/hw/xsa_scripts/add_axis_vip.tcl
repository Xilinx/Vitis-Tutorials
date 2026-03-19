#******************************************************************************
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************

########################################################################
# Vivado Tcl utility – AXI4-Stream VIP instantiation
# Author : Ricky Su       Date : 2025-06-10
########################################################################

# ----------------------------------------------------------
# create_axi_inputs  <num_inputs>
# ----------------------------------------------------------
proc create_axi_inputs {num_in} {
  puts "===== DEBUG: create_axi_inputs $num_in ====="
  for {set n 0} {$n < $num_in} {incr n} {

    puts "---- Input[$n] : instantiate VIP"
    create_bd_cell -type ip -vlnv xilinx.com:ip:axi4stream_vip:1.1 \
                   ai_engine_0_xin_${n}

    puts "DEBUG: [get_bd_cells ai_engine_0_xin_${n}] = [get_bd_cells ai_engine_0_xin_${n}]"
    puts "---- Input[$n] : export interface/pins"
    # Set TDATA width to 32 (TDATA_NUM_BYTES to 4)
    set_property -dict [list CONFIG.TDATA_NUM_BYTES.VALUE_SRC USER] [get_bd_cells ai_engine_0_xin_${n}]
    set_property CONFIG.TDATA_NUM_BYTES {4} [get_bd_cells ai_engine_0_xin_${n}]
    
    make_bd_intf_pins_external [get_bd_intf_pins ai_engine_0_xin_${n}/S_AXIS]
    make_bd_pins_external  [get_bd_pins ai_engine_0_xin_${n}/aclk]
    make_bd_pins_external  [get_bd_pins ai_engine_0_xin_${n}/aresetn]

    puts "---- Input[$n] : set PFM.AXIS_PORT tag"
    set_property PFM.AXIS_PORT \
        [subst {M_AXIS {type "M_AXIS" sptag "xin${n}" is_range "false"}}] \
        [get_bd_cells /ai_engine_0_xin_${n}]

    puts "---- Input[$n] : enable TLAST on external port"
    set_property -dict [list CONFIG.HAS_TLAST 1] [get_bd_intf_ports S_AXIS_${n}]

    puts "---- Input[$n] : completed OK"
  }
  puts "===== DEBUG: create_axi_inputs done ====="
}

# ----------------------------------------------------------
# create_axi_outputs  <num_outputs>
# ----------------------------------------------------------
proc create_axi_outputs {num_out} {
  puts "===== DEBUG: create_axi_outputs $num_out ====="
  for {set n 0} {$n < $num_out} {incr n} {

    puts "---- Output[$n] : instantiate VIP"
    create_bd_cell -type ip -vlnv xilinx.com:ip:axi4stream_vip:1.1 \
                   ai_engine_0_yout_${n}

    puts "---- Output[$n] : export interface/pins"
    set ext_of [make_bd_intf_pins_external \
                  [get_bd_intf_pins ai_engine_0_yout_${n}/M_AXIS]]
    # Set TDATA width to 32 (TDATA_NUM_BYTES to 4)
    set_property -dict [list CONFIG.TDATA_NUM_BYTES.VALUE_SRC USER] [get_bd_cells ai_engine_0_yout_${n}]
    set_property CONFIG.TDATA_NUM_BYTES {4} [get_bd_cells ai_engine_0_yout_${n}]

    make_bd_pins_external [get_bd_pins ai_engine_0_yout_${n}/aclk]
    make_bd_pins_external [get_bd_pins ai_engine_0_yout_${n}/aresetn]

    puts "---- Output[$n] : set PFM.AXIS_PORT tag"
    set_property PFM.AXIS_PORT \
        [subst {S_AXIS {type "S_AXIS" sptag "yout${n}" is_range "false"}}] \
        [get_bd_cells /ai_engine_0_yout_${n}]

    puts "---- Output[$n] : set stream attributes"
    set_property -dict [list \
        CONFIG.HAS_TLAST.VALUE_SRC {USER}  CONFIG.HAS_TLAST {1} \
        CONFIG.HAS_TKEEP.VALUE_SRC {USER}  CONFIG.HAS_TKEEP {1} \
        CONFIG.TDEST_WIDTH {0}             CONFIG.TID_WIDTH {0}] \
        [get_bd_cells ai_engine_0_yout_${n}]

    puts "---- Output[$n] : completed OK"
  }
  puts "===== DEBUG: create_axi_outputs done ====="
}

########################################################################
# Example usage (uncomment and edit numbers as needed)
########################################################################
# set NUM_IN  6
# set NUM_OUT 6
# create_axi_inputs  $NUM_IN
# create_axi_outputs $NUM_OUT
