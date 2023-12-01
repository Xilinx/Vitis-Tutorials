#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/



#unset ::env(HTTP_PROXY) 
file mkdir local_user_repo

# The environment variable XILINX_XHUB_USERAREA_ROOT_DIR is responsible for redirecting downloaded board files to local_user_repo
set ::env(XILINX_XHUB_USERAREA_ROOT_DIR) local_user_repo

xhub::get_xstores


xhub::refresh_catalog [xhub::get_xstores Vivado_example_project]
get_property LOCAL_ROOT_DIR [xhub::get_xstores Vivado_example_project]
set_param ced.repoPaths [get_property LOCAL_ROOT_DIR [xhub::get_xstores Vivado_example_project]]
xhub::install [xhub::get_xitems *ext_platform*]


# ------ Create Vivado Project ------
create_project project_1 ./project_1 -part  xcve2802-vsvh1760-2MP-e-S -force 
set_property board_part xilinx.com:vek280:part0:1.0 [current_project]
create_bd_design "ext_platform" -mode batch
instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design ext_platform -options { Include_AIE.VALUE true Include_DDR.VALUE true}
# Other options are default
# - Three clocks
# - 32 interrupt

generate_target all [get_files ext_platform.bd]

update_compile_order -fileset sources_1

write_hw_platform -force ./vek280_custom.xsa



