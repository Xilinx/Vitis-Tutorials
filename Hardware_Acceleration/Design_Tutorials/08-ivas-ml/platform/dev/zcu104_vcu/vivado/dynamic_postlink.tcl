#
# Copyright 2019 Xilinx Inc.
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

#
# Generate an empty _post_sys_link_gen_constrs.xdc file
# -------------------------------------------------------------------------
set fd [open "./_post_sys_link_gen_constrs.xdc" w]
puts $fd "# No content"
close $fd

# Set SLR_ASSIGNMENTS to SLR1 for the host path to MemSS S00_AXI
# -------------------------------------------------------------------------
####set_property CONFIG.SLR_ASSIGNMENTS SLR1 [get_bd_cells axi_vip_data] //no SLR

# Connect available interrupt pins on compute units to the interrupt vector
# -------------------------------------------------------------------------

# The wiring proc takes in the CU's interrupt BD pin and the overall interrupt index
proc wire_cu_to_xlconcat_intr {__cu_inst_intr_pin __intr_pin_num} {
  # Set number of xlconcat blocks and number of interrupts per block
  set __num_xlconcat 4
  set __num_pin_per_xlconcat 32

  # Get the xlconcat instance and pin number to work on now
  set __xlconcat_inst_num [expr {$__intr_pin_num / $__num_pin_per_xlconcat}]
  set __xlconcat_pin_num [expr {$__intr_pin_num - ($__xlconcat_inst_num * $__num_pin_per_xlconcat)}]

  # Ensure that the xlconcat instance and its pin exist, then get those objects
  if {($__xlconcat_pin_num < $__num_pin_per_xlconcat) && ($__xlconcat_inst_num < $__num_xlconcat)} {
    set __xlconcat_inst [get_bd_cells -hierarchical -quiet -filter NAME=~xlconcat_interrupt_${__xlconcat_inst_num}]
    set __xlconcat_pin [get_bd_pins -of_objects $__xlconcat_inst -quiet -filter NAME=~In${__xlconcat_pin_num}]

    # If the xlconcat pin object exists, disconnect it from ground and connect the CU's interrupt BD pin to it
    if {[llength $__xlconcat_pin] == 1} {
      disconnect_bd_net /interrupt_concat/xlconstant_gnd_dout $__xlconcat_pin -quiet
      connect_bd_net $__cu_inst_intr_pin $__xlconcat_pin -quiet
    } else {
      puts "(Post-linking XSA Tcl hook) No available xlconcat pins found"
    }
  } else {
    puts "(Post-linking XSA Tcl hook) No remaining xlconcat pins to connect to"
  }
}

# Make sure the kernel key in the config_info dict exists
if {[dict exists $config_info kernels]} {
  # Make sure that list of CUs is populated
  set __cu_list [dict get $config_info kernels]
  if {[llength $__cu_list] > 0} {
    # Translate the list of CUs to a list of BD cells
    set __cu_inst_list {}
    foreach __cu_inst $__cu_list {
	set str [get_bd_cells -quiet -filter "VLNV=~*:*:${__cu_inst}:*"]
	foreach name [split $str " "] {
	    lappend __cu_inst_list $name
	}
    }

    set __cu_inst_addr_list {}
    # Sort the list of CUs by offset address
    foreach __cu_bd_cell $__cu_inst_list {
      set __cu_bd_cell_sub [string range $__cu_bd_cell 1 [string length $__cu_bd_cell]]
      set __cu_bd_cell_segs [get_bd_addr_segs -of_objects [get_bd_addr_spaces ps_*] -filter "NAME =~ *${__cu_bd_cell_sub}_*"]
      if {[llength ${__cu_bd_cell_segs}] > 0} {
        set __cu_offset [get_property OFFSET [get_bd_addr_segs -of_objects [get_bd_addr_spaces ps_*] -filter "NAME =~ *${__cu_bd_cell_sub}_*"]]
        lappend __cu_inst_addr_list "$__cu_bd_cell $__cu_offset"
      }
    }

    if {[llength $__cu_inst_addr_list] > 0} {
      # Order the list by increasing AXI-Lite address offsets, then extract just ordered BD cells
      set __cu_inst_list {}
      unset __cu_inst_list
      set __cu_inst_addr_list_ordered [lsort -index 1 $__cu_inst_addr_list]
      foreach __cu_pair $__cu_inst_addr_list_ordered {
        lappend __cu_inst_list [lindex $__cu_pair 0]
      }
    }

    # Make sure the list of BD cells is populated
    if {[llength $__cu_inst_list] > 0} {
      # Of the BD cells, iterate through those with an interrupt BD pin
      set __intr_pin_num 0
      foreach __cu_inst_intr $__cu_inst_list {
        set __cu_inst_intr_pin [get_bd_pins -of_objects [get_bd_cells $__cu_inst_intr] -quiet -filter "TYPE=~intr"]
        if {[llength $__cu_inst_intr_pin] == 1} {
          # When a BD cell has an interrupt BD pin, wire it to the next available xlconcat pin
          wire_cu_to_xlconcat_intr $__cu_inst_intr_pin $__intr_pin_num
          incr __intr_pin_num
        }
      }
    } else {
      puts "(Post-linking XSA Tcl hook) No BD cells found for interrupt wiring"
    }
  } else {
    puts "(Post-linking XSA Tcl hook) No CUs found for interrupt wiring"
  }
} else {
  puts "(Post-linking XSA Tcl hook) No kernels key in config_info dict for interrupt wiring"
}
