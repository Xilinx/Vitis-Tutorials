#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

import vitis
import os
import shutil
import argparse

parser = argparse.ArgumentParser(description="Vitis Workspace Setup")
parser.add_argument("-b", "--board", type=str, required=True, choices=["zcu102", "vck190", "vek280", "vek385"], help="The board in use (zcu102, vck190, vek280, vek385)")
args = parser.parse_args()

WORKSPACE_DIR = "unified_workspace"

workspace = os.path.join(os.getcwd(), f"./{WORKSPACE_DIR}")
workspace = os.path.normpath(workspace)
            
print("Creating workspace at " + workspace)
if os.path.exists(workspace):
    print("workspace exists. Deleting ...")
    shutil.rmtree(workspace)

client = vitis.create_client()
client.set_workspace(workspace)

platform_dict = {"board_name": args.board, "hw_design": "null", "os": "null", "cpu": "null", "domain_name": "null"}

if args.board == "zcu102":
    platform_dict["hw_design"] = "zcu102/design_1_wrapper.xsa";
    platform_dict["os"] = "standalone";
    platform_dict["cpu"] = "psu_cortexa53_0";
    platform_dict["domain_name"] = "standalone_psu_cortexa53_0";
elif args.board == "vck190":
    platform_dict["hw_design"] = "vck190/design_1_wrapper.xsa";
    platform_dict["os"] = "standalone";
    platform_dict["cpu"] = "psv_cortexa72_0";
    platform_dict["domain_name2"] = "standalone_psv_cortexa72_0"
elif args.board == "vek280":
    platform_dict["hw_design"] = "vek280/design_1_wrapper.xsa";
    platform_dict["os"] = "standalone";
    platform_dict["cpu"] = "psv_cortexa72_0";
    platform_dict["domain_name"] = "standalone_psv_cortexa72_0"
elif args.board == "vek385":
    platform_dict["hw_design"] = "vek385/design_1_wrapper.xsa";
    platform_dict["os"] = "standalone";
    platform_dict["cpu"] = "cortexa78_0";
    platform_dict["domain_name"] = "standalone_cortexa78_0"


platform = client.create_platform_component(name=platform_dict["board_name"],hw_design=platform_dict["hw_design"], os=platform_dict["os"], cpu=platform_dict["cpu"], domain_name=platform_dict["domain_name"])
xpfm = os.path.join(workspace, platform_dict["board_name"], "export", platform_dict["board_name"], f"{platform_dict['board_name']}.xpfm")
xpfm = os.path.normpath(xpfm)
comp = client.create_app_component(name="hello_world", platform=xpfm, domain=platform_dict["domain_name"], template="hello_world")

platform = client.get_component(name=platform_dict["board_name"])
status = platform.build()

status = platform.build()

comp = client.get_component(name="hello_world")
comp.build()
