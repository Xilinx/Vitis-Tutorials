# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */


##################################### Step 1: create vivado project and add design sources

# create ip project with part name in command line argvs
create_project krnl_aurora ./krnl_aurora -part [lindex $argv 0]

# add design sources into project
add_files -norecurse \
       {                                         \
              ../rtl/krnl_aurora_control_s_axi.v \
              ../rtl/krnl_aurora.v               \
              ../ip_generation/aurora_64b66b_0/aurora_64b66b_0.xci \
              ../ip_generation/axis_data_fifo_0/axis_data_fifo_0.xci \
       }

add_files -norecurse -fileset constrs_1 \
       {                                         \
              ../xdc/aurora_64b66b_0.xdc         \
       }

update_compile_order -fileset sources_1

# create IP packaging project
ipx::package_project -root_dir ./krnl_aurora_ip -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current true



# inference clock and reset signals
ipx::infer_bus_interface ap_clk xilinx.com:signal:clock_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface ap_rst_n xilinx.com:signal:reset_rtl:1.0 [ipx::current_core]
# remove auto inferred clock interface init_clk, just treat it as a normal port
ipx::remove_bus_interface init_clk [ipx::current_core]


# associate AXI/AXIS interface with clock
ipx::associate_bus_interfaces -busif s_axi_control  -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif rx_axis        -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx_axis        -clock ap_clk [ipx::current_core]

# associate axis and reset signal with clock
ipx::associate_bus_interfaces -busif rx_axis -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx_axis -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -clock ap_clk -reset ap_rst_n [ipx::current_core]

# create GT data port
ipx::add_bus_interface gt_port [ipx::current_core]
set_property interface_mode master                                    [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property abstraction_type_vlnv xilinx.com:interface:gt_rtl:1.0    [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:gt:1.0                [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]

ipx::add_port_map GRX_P                                               [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_rxp_in [ipx::get_port_maps GRX_P -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]

ipx::add_port_map GTX_N                                               [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_txn_out [ipx::get_port_maps GTX_N -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]

ipx::add_port_map GRX_N                                               [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_rxn_in [ipx::get_port_maps GRX_N -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]

ipx::add_port_map GTX_P                                               [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_txp_out [ipx::get_port_maps GTX_P -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]

# create GT clock port
ipx::add_bus_interface gt_refclk [ipx::current_core]
set_property interface_mode slave                                            [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property abstraction_type_vlnv xilinx.com:interface:diff_clock_rtl:1.0   [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:diff_clock:1.0               [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]

ipx::add_port_map CLK_P                                                      [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property physical_name gt_refclk_p [ipx::get_port_maps CLK_P -of_objects [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]]

ipx::add_port_map CLK_N                                                      [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property physical_name gt_refclk_n [ipx::get_port_maps CLK_N -of_objects [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]]


# Set required property for Vitis kernel
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]

# Packaging Vivado IP
ipx::update_source_project_archive -component [ipx::current_core]
ipx::save_core [ipx::current_core]

# Generate Vitis Kernel from Vivado IP
package_xo -force \
           -xo_path ../krnl_aurora.xo \
           -kernel_name krnl_aurora \
           -ctrl_protocol ap_ctrl_none \
           -ip_directory ./krnl_aurora_ip \
           -output_kernel_xml ../krnl_aurora.xml
