#/*
#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

# 2023-04-02T21:02:13.258137
import vitis
import argparse
import os

print("Platform generation")
parser = argparse.ArgumentParser()
parser.add_argument("--platform_name", type=str, dest="platform_name")
parser.add_argument("--xsa_path", type=str, dest="xsa_path")
parser.add_argument("--xsa-emu_path", type=str, dest="emuxsa_path")
parser.add_argument("--boot", type=str, dest="boot")
args = parser.parse_args()
platform_name=args.platform_name
xsa_path=args.xsa_path
emuxsa_path=args.emuxsa_path
TOP_DIR = os.getcwd()
USER_DTSI = TOP_DIR + "/system-user.dtsi"
boot=args.boot
print('args',args)
client = vitis.create_client()
workspace_path = os.getcwd() + "/ws" 
client.set_workspace(path=workspace_path)
print(workspace_path)
#advanced_options = client.create_advanced_options_dict(board_dtsi="versal-vck190-reva-x-ebm-02-reva",dt_zocl="1",dt_overlay="0", user_dtsi=USER_DTSI)
advanced_options = client.create_advanced_options_dict(dt_zocl="1",dt_overlay="0", user_dtsi=USER_DTSI)
platform = client.create_platform_component(name =platform_name ,hw_design = xsa_path,os = "linux",cpu = "psv_cortexa72",domain_name = "linux_psv_cortexa72",generate_dtb = True,advanced_options = advanced_options)

platform = client.get_component(name=platform_name)
domain = platform.add_domain(cpu = "ai_engine",os = "aie_runtime",name = "ai_eingine",display_name = "ai_eingine")

domain = platform.get_domain(name="linux_psv_cortexa72")

status = domain.update_name(new_name="xrt")

status = domain.generate_bif()
status = domain.set_boot_dir(path=boot)
status = platform.build()
