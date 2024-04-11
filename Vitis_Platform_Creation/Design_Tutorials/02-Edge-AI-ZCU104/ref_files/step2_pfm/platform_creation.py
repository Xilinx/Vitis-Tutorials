#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
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
parser.add_argument("--dtb", type=str, dest="dtb")
args = parser.parse_args()
platform_name=args.platform_name
xsa_path=args.xsa_path
emuxsa_path=args.emuxsa_path
dtb=args.dtb
boot=args.boot
print('args',args)
client = vitis.create_client()
client.set_workspace(path=os.getcwd())
platform = client.create_platform_component(name = platform_name, hw =xsa_path, os = "linux",cpu = "psu_cortexa53",emulation_xsa_path = emuxsa_path )
platform = client.get_platform_component(name=platform_name)
domain = platform.get_domain(name="linux_psu_cortexa53")
status = domain.update_name(new_name="xrt")
status = domain.generate_bif() 
status = domain.add_boot_dir(path=boot)
status = domain.set_dtb(path=dtb)

status = platform.build()
