#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


##################################### Step 1: create vivado project and add design sources

# create ip project with part name in command line argvs
set design_name [lindex $argv 0]
set device_name [lindex $argv 1]
set xo_name [lindex $argv 2]
create_project -f ${design_name} ./build/${design_name}_vivado -part ${device_name}

# add design sources into project
add_files -norecurse \
       {                         \
        src/my_diff.v                  \
        src/subtractor_S00_AXI.v                  \
        src/subtractor.v                  \
        subtractor.xdc                   \
       }

update_compile_order -fileset sources_1
# Check design and report ultrafast methodology
synth_design -rtl -rtl_skip_mlo -name rtl_1
report_methodology -name ultrafast_methodology_1

# create IP packaging project
ipx::package_project -root_dir ./build/${design_name}_ip -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current true

##################################### Step 2: Inference clock, reset, AXI interfaces and associate them with clock

# inference clock and reset signals
ipx::infer_bus_interface aclk xilinx.com:signal:clock_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface aresetn xilinx.com:signal:reset_rtl:1.0 [ipx::current_core]

# associate AXI/AXIS interface with clock
ipx::associate_bus_interfaces -busif s00_axi        -clock aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif s00_axis       -clock aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif s01_axis       -clock aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif s02_axis       -clock aclk [ipx::current_core]
ipx::associate_bus_interfaces -busif s03_axis       -clock aclk [ipx::current_core]

# associate reset signal with clock
ipx::associate_bus_interfaces -clock aclk -reset aresetn [ipx::current_core]

##################################### Step 3: Set the definition of AXI control slave registers, including CTRL and user kernel arguments

# Add RTL kernel registers
ipx::add_register DELAY0    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]
ipx::add_register DELAY1    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]
ipx::add_register DELAY2    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]
ipx::add_register DELAY3    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]

# Set RTL kernel registers property
set_property description    {Delay Counter 0}   [ipx::get_registers DELAY0 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property address_offset {0x000}             [ipx::get_registers DELAY0 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers DELAY0 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]

set_property description    {Delay Counter 1}   [ipx::get_registers DELAY1 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property address_offset {0x004}             [ipx::get_registers DELAY1 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers DELAY1 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]

set_property description    {Delay Counter 2}   [ipx::get_registers DELAY2 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property address_offset {0x008}             [ipx::get_registers DELAY2 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers DELAY2 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]

set_property description    {Delay Counter 3}   [ipx::get_registers DELAY3 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property address_offset {0x00c}             [ipx::get_registers DELAY3 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers DELAY3 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s00_axi -of_objects [ipx::current_core]]]]

#### Step 4: Package Vivado IP and generate Vitis kernel file

# Set required property for Vitis kernel
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]

# Packaging Vivado IP
ipx::update_source_project_archive -component [ipx::current_core]
ipx::save_core [ipx::current_core]

# Generate Vitis Kernel from Vivado IP
package_xo -force -xo_path ${xo_name} -kernel_name ${design_name} -ctrl_protocol user_managed -ip_directory ./build/${design_name}_ip -output_kernel_xml ./${design_name}.xml
