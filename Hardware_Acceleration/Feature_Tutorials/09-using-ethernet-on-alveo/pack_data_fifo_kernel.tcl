# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */



##################################### Step 1: create vivado project and add design sources

# create ip project with part name in command line argvs
create_project data_fifo_kernel ./data_fifo_kernel -part [lindex $argv 0]

# add design sources into project
add_files -norecurse \
       {                                    \
        ../rtl/data_fifo_krnl.sv            \
        ../ip_generation/axis_data_fifo_0/axis_data_fifo_0.xci \
       }

update_compile_order -fileset sources_1

# create IP packaging project
ipx::package_project -root_dir ./data_fifo_kernel_pack -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current true


ipx::infer_bus_interface ap_clk xilinx.com:signal:clock_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface ap_rst_n xilinx.com:signal:reset_rtl:1.0 [ipx::current_core]

ipx::associate_bus_interfaces -busif rx_axis        -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx_axis        -clock ap_clk [ipx::current_core]

# associate reset signal with clock
ipx::associate_bus_interfaces -clock ap_clk -reset ap_rst_n [ipx::current_core]

# Set required property for Vitis kernel
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]

# Packaging Vivado IP
ipx::update_source_project_archive -component [ipx::current_core]
ipx::save_core [ipx::current_core]

# Generate Vitis Kernel from Vivado IP
package_xo -force -xo_path ../data_fifo_krnl.xo \
           -kernel_name data_fifo_krnl \
           -ctrl_protocol ap_ctrl_none \
           -ip_directory ./data_fifo_kernel_pack \
           -output_kernel_xml ../data_fifo_krnl.xml

