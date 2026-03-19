#/*
#Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
#Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#*/
# Script to create Vitis platform and application using common image sysroot

import vitis
import argparse
import os
import sys

parser = argparse.ArgumentParser(
    description="Script to create a Vitis platform from fixed XSA and build application using common image sysroot"
)
parser.add_argument("--platform_name", type=str, dest="platform_name", default="vck190_fixed_platform_hw",
                    help="Name for the generated Vitis platform.")
parser.add_argument("--xsa_path", type=str, dest="xsa_path",
                    help="Path to the hardware XSA file.")
parser.add_argument("--sysroot", type=str, dest="sysroot",
                    help="Path to the sysroot directory (from common image SDK).")
parser.add_argument("--platform", type=str, dest="platform_fixed",
                    help="Provide the generated .xpfm platform file for application creation (optional).")
parser.add_argument("--user_dtsi", type=str, dest="user_dtsi",
                    help="Specify the user custom DTSI file.")
parser.add_argument("--mode", type=str, dest="mode", choices=["hw", "app"], default="hw",
                    help="Mode: 'hw' to create hardware platform, 'app' to create application.")

args = parser.parse_args()
print(args)
# Resolve absolute paths
current_path = os.getcwd()
platform_name = args.platform_name
xsa_path = os.path.abspath(args.xsa_path) if args.xsa_path else None
sysroot = os.path.abspath(args.sysroot) if args.sysroot else None
platform_fixed = os.path.abspath(args.platform_fixed) if args.platform_fixed else None
user_dtsi = os.path.abspath(args.user_dtsi) if args.user_dtsi else None
mode = args.mode

# Validate inputs
if mode == "hw" and not xsa_path:
    print("ERROR: --xsa_path is required for platform creation mode")
    sys.exit(1)

if mode == "app" and not platform_fixed is None:
   raise RuntimeError(
        "Application creation requires an explicit fixed platform. "
        "Fallback to base platforms is not allowed."
    )

if mode == "app" and not sysroot:
    print("ERROR: --sysroot is required for application creation mode")
    sys.exit(1)

# Create Vitis client
print("INFO: Setting up Vitis workspace: {}".format(current_path))
client = vitis.create_client()
client.update_workspace(path=current_path)
client.set_workspace(path=current_path)

def generate_pfm():
    """Create Vitis platform from fixed XSA (hardware)"""
    print("INFO: Creating hardware platform: {}".format(platform_name))
    print("INFO: Using XSA: {}".format(xsa_path))
    
    # Advanced options for VCK190 with segmented configuration
    advanced_options = client.create_advanced_options_dict(
        board_dtsi="versal-vck190-reva-x-ebm-01-reva",
        dt_zocl="1",
        user_dtsi=user_dtsi,
        dt_overlay="1"
    )

    # Create platform component
    platform = client.create_platform_component(
        name=platform_name,
        hw_design=xsa_path,
        os="linux",
        cpu="psv_cortexa72",
        domain_name="linux_psv_cortexa72",
        generate_dtb=True,  # Using common image, DTB is pre-built
        advanced_options=advanced_options
    )
    
    # Build platform
    platform_component = client.get_component(name=platform_name)
    status = platform_component.build()
    
    if status == 0:
        print("INFO: Platform created successfully")
    else:
        print("ERROR: Platform build failed")
        sys.exit(1)


def generate_application():
    """Create and build application using existing platform"""
    print("INFO: Creating application component")
    print("INFO: Using platform: {}".format(platform_fixed))
    print("INFO: Using sysroot: {}".format(sysroot))
    
    # Create application component
    app = client.create_app_component(
        name="application",
        platform=platform_fixed,
        domain="linux_psv_cortexa72"
    )
    
    app_component = client.get_component(os.path.join(current_path, "application"))
    
    # Add source files (host_src is in step3_application directory)
    host_src_dir = os.path.join(current_path, "host_src")
    aie_control_src = os.path.join(current_path, "../step2_vitis_integration/_aie/ps/c_rts/aie_control_xrt.cpp")
    
    source_files = []
    if os.path.exists(host_src_dir):
        for f in ["host.cpp", "host.h", "input.h", "golden.h"]:
            file_path = os.path.join(host_src_dir, f)
            if os.path.exists(file_path):
                source_files.append(file_path)
    
    if os.path.exists(aie_control_src):
        source_files.append(aie_control_src)
    
    if source_files:
        print("INFO: Importing source files...")
        status = app_component.import_files(from_loc="", files=source_files, is_skip_copy_sources=False)
    
    # Set sysroot
    print("INFO: Setting sysroot...")
    app_component.set_sysroot(sysroot=sysroot)
    
    # Set compiler options
    app_component.set_app_config(key='USER_CMAKE_CXX_STANDARD', values=['17'])
    
    # Add include directories if needed (src is in step2_vitis_integration, same level as step3_application)
    src_dir = os.path.join(current_path, "../step2_vitis_integration/src")
    if os.path.exists(src_dir):
        app_component.set_app_config(key='USER_INCLUDE_DIRECTORIES', values=[src_dir])
    
    # Build application
    print("INFO: Building application...")
    status = app_component.build()
    
    if status == 0:
        print("INFO: Application built successfully")
    else:
        print("ERROR: Application build failed")
        sys.exit(1)

# Execute based on mode
if mode == "hw":
    generate_pfm()
elif mode == "app":
    generate_application()

print("INFO: Script completed successfully")
