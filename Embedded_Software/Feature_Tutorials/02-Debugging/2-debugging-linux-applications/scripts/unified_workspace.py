#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

import argparse
import vitis
import glob
import shutil
import os

parser = argparse.ArgumentParser()
parser.add_argument("echo")
args = parser.parse_args()
xsa = args.echo

if os.path.isfile(xsa):

    workspace = os.path.join(os.getcwd(), "./unified_workspace")
    workspace = os.path.normpath(workspace)

    print("Creating workspace at " + workspace)
    if os.path.exists(workspace):
        print("workspace exists. Deleting ...")
        shutil.rmtree(workspace)

    client = vitis.create_client()
    client.set_workspace(workspace)

    platform = client.create_platform_component(name = "zcu102_platform",hw_design = xsa,os = "linux",cpu = "psu_cortexa53",no_boot_bsp = True)
    platform = client.get_component(name="zcu102_platform")
    status = platform.build()

    sysroots = os.path.join(os.getcwd(), "./sysroot/sysroots/cortexa72-cortexa53-xilinx-linux")
    sysroots = os.path.normpath(sysroots)

    status = client.set_preference(level = "WORKSPACE", device = "ZynqMP", key = "Sysroot", value = sysroots)

    xpfm = os.path.join(os.getcwd(), "./unified_workspace/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm")
    xpfm = os.path.normpath(xpfm)
    comp = client.create_app_component(name="linux_test_app",platform = xpfm,domain = "linux_psu_cortexa53",template = "linux_empty_application")

    status = comp.set_sysroot(sysroot=sysroots)

    comp = client.get_component(name="linux_test_app")
    status = comp.import_files(from_loc="../src", files=["linux_test_application.c"], dest_dir_in_cmp = "src")
else:
    print("Error: No XSA passed")
