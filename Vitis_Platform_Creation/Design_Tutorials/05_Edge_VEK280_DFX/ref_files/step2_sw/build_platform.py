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
parser.add_argument("--static_xsa_path", type=str, dest="static_xsa_path")
parser.add_argument("--platform_name", type=str, dest="platform_name")
parser.add_argument("--emu_xsa_path", type=str, dest="emu_xsa_path")
parser.add_argument("--platform_out", type=str, dest="platform_out")
parser.add_argument("--boot_dir_path", type=str, dest="boot_dir_path")
parser.add_argument("--img_dir_path", type=str, dest="img_dir_path")
parser.add_argument("--rp_xsa_path", type=str, dest="rp_xsa_path")
parser.add_argument("--user_dtsi", type=str, dest="user_dtsi")
parser.add_argument("--boot_image", type=str, dest="boot_image")


args = parser.parse_args()
static_xsa_path=args.static_xsa_path
platform_name=args.platform_name
emu_xsa_path=args.emu_xsa_path
platform_out=args.platform_out
boot_dir_path=args.boot_dir_path
img_dir_path=args.img_dir_path
rp_xsa_path=args.rp_xsa_path
user_dtsi=args.user_dtsi
boot_image=args.boot_image
print('args',args)
client = vitis.create_client()
client.update_workspace(path=platform_out)

platform = client.get_component(name = platform_name)

#update boot_dir_path with boot.bin path
domain = platform.get_domain(name="xrt")
status = domain.update_hw_boot_bin(boot_image)

status = platform.build()
