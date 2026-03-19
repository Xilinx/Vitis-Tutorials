#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

puts "\[import_vma.tcl\] Starting"

save_project_as project_1 ./project_1 -exclude_run_results -force
import_files





vitis::import_archive ../link_vma/post_link.vma
puts "\[import_vma.tcl\] Done"
puts "\[import_vma.tcl\] Run 'vivado project_1/project_1.xpr' to open the project."

# Remove the original vitis_design.bd and vitis_design_wrapper.v
# remove_files  ./project_1/project_1.srcs/sources_1/bd/vitis_design/vitis_design.bd
# remove_files  ./project_1/project_1.gen/sources_1/bd/vitis_design/hdl/vitis_design_wrapper.v

set_property synth_checkpoint_mode None [get_files ./project_1/project_1.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd]

generate_target all [get_files  ./project_1/project_1.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd]
make_wrapper -files [get_files ./project_1/project_1.srcs/sources_1/bd/vitis_design_vma/vitis_design_vma.bd] -top
add_files -norecurse ./project_1/project_1.gen/sources_1/bd/vitis_design_vma/hdl/vitis_design_vma_wrapper.v

# Add ILA
create_ip -name axis_ila -vendor xilinx.com -library ip -version 1.3 -module_name axis_ila_0
set_property -dict [list \
  CONFIG.C_MON_TYPE {Interface_Monitor} \
  CONFIG.C_SLOT_0_AXI_TRIG_SEL {0} \
  CONFIG.C_SLOT_0_HAS_TKEEP {1} \
  CONFIG.C_SLOT_0_INTF_TYPE {xilinx.com:interface:axis_rtl:1.0} \
  CONFIG.C_SLOT_0_AXIS_TDATA_WIDTH {32}  \
] [get_ips axis_ila_0]
generate_target {instantiation_template} [get_files ./project_1/project_1.srcs/sources_1/ip/axis_ila_0/axis_ila_0.xci]

add_files -norecurse ./axis_sink.v
add_files -norecurse ./top.v

set_property top top [current_fileset]

launch_runs impl_1 -to_step write_device_image -jobs 48
wait_on_run impl_1

# Export fixed XSA
set_property platform.extensible false [current_project]
write_hw_platform -fixed -include_bit -force -file ./top.xsa
