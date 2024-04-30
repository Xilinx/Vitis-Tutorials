#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

set currpath [pwd]

create_project custom_pfm_vck190 $currpath/custom_pfm_vck190 
set_property board_part xilinx.com:vck190:part0:3.3 [current_project]
create_bd_design "ext_platform" -mode batch
instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design ext_platform -options { Clock_Options.VALUE {clk_out1 104.16667 0 false clk_out2 156.25 1 false clk_out3 312.5 2 true clk_out4 520.83333 3 false clk_out5 625 4 false} Include_AIE.VALUE true Include_BDC.VALUE false Include_DDR.VALUE true}
update_compile_order -fileset sources_1
open_bd_design {$currpath/custom_pfm_vck190/custom_pfm_vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd}
generate_target all [get_files  $currpath/custom_pfm_vck190/custom_pfm_vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd]
catch { config_ip_cache -export [get_ips -all ext_platform_axi_intc_0_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_clk_wizard_0_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_cips_noc_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_noc_ddr4_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_proc_sys_reset_0_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_proc_sys_reset_1_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_proc_sys_reset_2_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_proc_sys_reset_3_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_proc_sys_reset_4_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_icn_ctrl_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_noc_lpddr4_0] }
export_ip_user_files -of_objects [get_files $currpath/custom_pfm_vck190/custom_pfm_vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd] -no_script -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] $currpath/custom_pfm_vck190/custom_pfm_vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd]
launch_runs ext_platform_axi_intc_0_0_synth_1 ext_platform_cips_noc_0_synth_1 ext_platform_clk_wizard_0_0_synth_1 ext_platform_icn_ctrl_0_synth_1 ext_platform_noc_ddr4_0_synth_1 ext_platform_noc_lpddr4_0_synth_1 ext_platform_proc_sys_reset_0_0_synth_1 ext_platform_proc_sys_reset_1_0_synth_1 ext_platform_proc_sys_reset_2_0_synth_1 ext_platform_proc_sys_reset_3_0_synth_1 ext_platform_proc_sys_reset_4_0_synth_1 -jobs 8
export_simulation -of_objects [get_files $currpath/custom_pfm_vck190/custom_pfm_vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd] -directory $currpath/custom_pfm_vck190/custom_pfm_vck190.ip_user_files/sim_scripts -ip_user_files_dir $currpath/custom_pfm_vck190/custom_pfm_vck190.ip_user_files -ipstatic_source_dir $currpath/custom_pfm_vck190/custom_pfm_vck190.ip_user_files/ipstatic -lib_map_path [list {modelsim=$currpath/custom_pfm_vck190/custom_pfm_vck190.cache/compile_simlib/modelsim} {questa=$currpath/custom_pfm_vck190/custom_pfm_vck190.cache/compile_simlib/questa} {xcelium=$currpath/custom_pfm_vck190/custom_pfm_vck190.cache/compile_simlib/xcelium} {vcs=$currpath/custom_pfm_vck190/custom_pfm_vck190.cache/compile_simlib/vcs} {riviera=$currpath/custom_pfm_vck190/custom_pfm_vck190.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet
set_property platform.name {vck190} [current_project]
set_property platform.version {1.0} [current_project]
set_property pfm_name {xilinx:vck190:vck190:1.0} [get_files -all $currpath/custom_pfm_vck190/custom_pfm_vck190.srcs/sources_1/bd/ext_platform/ext_platform.bd]
set_property platform.uses_pr {false} [current_project]
write_hw_platform -force -file $currpath/custom_pfm_vck190/custom_platform_vck190.xsa
