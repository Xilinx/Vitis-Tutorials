#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis
import sys
import os

app_path= os.getcwd()

Vitis_dir = os.environ.get('XILINX_VITIS')

client = vitis.create_client()
client.update_workspace(path="./workspace")

platform_name="AIE_A-to-Z_pfm_vck190"
platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

if (not os.path.isdir("./workspace/"+platform_name)):
    platform = client.create_platform_component(name = platform_name,hw_design = "./workspace/simple_aie_application_system_project/build/hw/hw_link/binary_container_1.xsa",os = "standalone",cpu = "psv_cortexa72_0",domain_name = "standalone_psv_cortexa72_0")
    platform = client.get_component(name=platform_name)
    domain = platform.get_domain(name="standalone_psv_cortexa72_0")
    status = domain.set_lib(lib_name="aiebaremetal", path=Vitis_dir+"/data/embeddedsw/XilinxProcessorIPLib/drivers/aiebaremetal_v1_0")
    status = platform.build()

if (not os.path.isdir("./workspace/A-to-Z_app")):
    comp = client.create_app_component(name="A-to-Z_app",platform = platform_path ,domain = "standalone_psv_cortexa72_0")
    comp = client.get_component(name="A-to-Z_app")
    status = comp.import_files(from_loc="../src", files=["main.cpp"], dest_dir_in_cmp = "src")
    status = comp.import_files(from_loc="./workspace/simple_aie_application/build/hw/Work", files=["baremetal_metadata_compile.cpp"], dest_dir_in_cmp = "src")
    comp.set_app_config(key = 'USER_COMPILE_DEFINITIONS', values = "__PS_BARE_METAL__")
    

vitis.dispose()
