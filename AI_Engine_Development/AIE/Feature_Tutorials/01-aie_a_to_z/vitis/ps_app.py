#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis
import sys
import os

app_path= os.getcwd()
client = vitis.create_client()
client.set_workspace(path="./workspace")

platform_name="AIE_A-to-Z_pfm_vck190"
platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

platform = client.create_platform_component(name = platform_name,hw_design = "./workspace/simple_aie_application_system_project/build/hw/hw_link/binary_container_1.xsa",os = "standalone",cpu = "psv_cortexa72_0",domain_name = "standalone_psv_cortexa72_0")

platform = client.get_component(name=platform_name)
status = platform.build()

comp = client.create_app_component(name="A-to-Z_app",platform = platform_path ,domain = "standalone_psv_cortexa72_0")
comp = client.get_component(name="A-to-Z_app")
status = comp.import_files(from_loc="../src", files=["main.cpp"], dest_dir_in_cmp = "src")
status = comp.import_files(from_loc="./workspace/simple_aie_application/build/hw/Work/ps/c_rts", files=["aie_control.cpp"], dest_dir_in_cmp = "src")

comp.set_app_config(key = 'USER_INCLUDE_DIRECTORIES', values = ['../../../workspace/simple_aie_application/src', '$ENV{XILINX_VITIS}/aietools/include'])
comp.set_app_config(key = 'USER_LINK_LIBRARIES', values = 'adf_api')
comp.set_app_config(key = 'USER_LINK_DIRECTORIES', values = '$ENV{XILINX_VITIS}/aietools/lib/aarchnone64.o')

vitis.dispose()