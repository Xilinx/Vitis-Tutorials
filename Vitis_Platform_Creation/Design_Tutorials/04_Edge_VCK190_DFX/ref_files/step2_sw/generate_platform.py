#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******************************************************************************
import vitis
import argparse
import os

print("Platform generation")
parser = argparse.ArgumentParser()
parser.add_argument("--platform_name", type=str, dest="platform_name")
parser.add_argument("--static_xsa_path", type=str, dest="static_xsa_path")
parser.add_argument("--emu_xsa_path", type=str, dest="emu_xsa_path")
parser.add_argument("--platform_out", type=str, dest="platform_out")
parser.add_argument("--boot_dir_path", type=str, dest="boot_dir_path")
parser.add_argument("--sd_dir_path", type=str, dest="sd_dir_path")
parser.add_argument("--rp_xsa_path", type=str, dest="rp_xsa_path")
parser.add_argument("--user_dtsi", type=str, dest="user_dtsi")

args = parser.parse_args()

# Extract variables
platform_name=args.platform_name
static_xsa_path=args.static_xsa_path
emu_xsa_path=args.emu_xsa_path
platform_out=args.platform_out
boot_dir_path=args.boot_dir_path
sd_dir_path=args.sd_dir_path
rp_xsa_path=args.rp_xsa_path
user_dtsi=args.user_dtsi

print('args',args)
client = vitis.create_client()

current_path = os.getcwd()
print(current_path)
client.update_workspace(path=current_path)
client.set_workspace(path=current_path)

advanced_options = client.create_advanced_options_dict(board_dtsi="versal-vck190-reva-x-ebm-01-reva",user_dtsi=user_dtsi,dt_overlay="0",dt_zocl="1")
rp_info_args = client.add_rp_info_args(rp_xsa_path=rp_xsa_path)

platform = client.create_platform_component(name = platform_name,hw_design = static_xsa_path,os = "linux",cpu = "psv_cortexa72",domain_name = "xrt",emu_design = emu_xsa_path,generate_dtb = True, rp_info_args = rp_info_args, advanced_options = advanced_options )
platform = client.get_component(name = platform_name)
domain = platform.add_domain(cpu = "ai_engine",os = "aie_runtime",name = "aiengine",display_name = "aiengine")  
domain = platform.get_domain(name = "xrt")
domain.set_sd_dir(path = sd_dir_path)
status = domain.set_boot_dir(path=boot_dir_path)
status = domain.generate_bif()
