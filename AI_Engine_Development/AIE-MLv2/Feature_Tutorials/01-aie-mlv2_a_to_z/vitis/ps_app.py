#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vitis
import sys
import os
from hsi import *
import glob

def get_metadata(*args):
    xsa = "binary_container_1.xsa"

    ret_metadata = {'arch' : '', 'target_proc' : '', 'part' : ''}
    xsa = glob.glob("./workspace/simple_aie_application_system_project/build/hw/hw_link/"+xsa, recursive=True)
    if len(xsa) == 0:
        print("Error: "+xsa+" not found at ../vivado/build/xsa_platform")
        return ""

    xsa = xsa[0]
    print("Info: Using XSA file: " + xsa + " to extract HW metadata using HSI Python API")

    HwDesign = HwManager.open_hw_design(xsa)
    arch = HwDesign.FAMILY
    part = HwDesign.PART
    board = HwDesign.BOARD
    for proc in HwDesign.get_cells(hierarchical='true',filter='IP_TYPE==PROCESSOR'):
        if proc.IP_NAME == "psu_cortexa53" or proc.IP_NAME == "psv_cortexa72" or proc.IP_NAME == "cortexa78":
            target_proc = proc.IP_NAME+"_0"
            break

    if board == "" and part == "xc2ve3858-ssva2112-2MP-e-S":
        board = "vek385"
    elif board == "":
        board = "custom"
    else:
        board = board.split(":")[1]

    print("Info: ARCH: "+arch)
    print("Info: BOARD: "+board)
    print("Info: PART: "+part)
    print("Info: TARGET PROC: "+target_proc)

    ret_metadata['arch'] = arch
    ret_metadata['part'] = part
    ret_metadata['board'] = board
    ret_metadata['target_proc'] = target_proc

    return ret_metadata

app_path= os.getcwd()
Vitis_dir = os.environ.get('XILINX_VITIS')

client = vitis.create_client()
client.update_workspace(path="./workspace")

ret_metadata = get_metadata()

platform_name="AIE_A-to-Z_pfm_"+ret_metadata['board']

platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

if (not os.path.isdir("./workspace/"+platform_name)):
    platform = client.create_platform_component(name = platform_name,hw_design = "./workspace/simple_aie_application_system_project/build/hw/hw_link/binary_container_1.xsa",os = "standalone",cpu = ret_metadata['target_proc'],domain_name = "standalone_"+ret_metadata['target_proc'])
    platform = client.get_component(name=platform_name)
    domain = platform.get_domain(name="standalone_"+ret_metadata['target_proc'])
    status = domain.set_lib(lib_name="aiebaremetal", path=Vitis_dir+"/data/embeddedsw/XilinxProcessorIPLib/drivers/aiebaremetal_v1_0")
    status = platform.build()

if (not os.path.isdir("./workspace/A-to-Z_app")):
    comp = client.create_app_component(name="A-to-Z_app",platform = platform_path ,domain = "standalone_"+ret_metadata['target_proc'])
    comp = client.get_component(name="A-to-Z_app")
    status = comp.import_files(from_loc="../src", files=["main.cpp"], dest_dir_in_cmp = "src")
    status = comp.import_files(from_loc="./workspace/simple_aie_application/build/hw/Work", files=["baremetal_metadata_compile.cpp"], dest_dir_in_cmp = "src")
    comp.set_app_config(key = 'USER_COMPILE_DEFINITIONS', values = "__PS_BARE_METAL__")
    

vitis.dispose()
