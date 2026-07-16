#/*
#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/

import argparse
import os
import vitis

parser = argparse.ArgumentParser()
parser.add_argument("--name", type=str, dest="platform_name", default="fixed_platform")
parser.add_argument("--hw", type=str, dest="xsa_path", required=True)
parser.add_argument("--cpu", type=str, dest="cpu", default="psv_cortexa72_0")
args = parser.parse_args()

client = vitis.create_client()
workspace_path = os.path.join(os.getcwd(), "ws")
client.set_workspace(path=workspace_path)

platform = client.create_platform_component(name=args.platform_name, hw_design=args.xsa_path, os="standalone", cpu=args.cpu)
platform = client.get_component(name=args.platform_name)
status = platform.build()
