#/*
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier:MIT
#*/
# 2025-05-14T20:06:09.507263
import vitis
import argparse
import os

parser = argparse.ArgumentParser(
    description="Script to create a fixed Vitis platform (.xpfm) using XSA for hardware run or hardware emulation or create and build applicationn component ."
)
parser.add_argument("--platform_name", type=str, dest="platform_name",  
                    help="Name for the generated Vitis platform.")
parser.add_argument("--xsa_path", type=str, dest="xsa_path",  
                    help="Path to the hardware XSA file.")
parser.add_argument("--xsa-emu_path", type=str, dest="emuxsa_path",  
                    help="Path to the hardware emulation XSA file.")
parser.add_argument("--sysroot", type=str, dest="sysroot",  
                    help="Path to the sysroot directory for compiling the application.")
parser.add_argument("--platform", type=str, dest="platform_fixed",  
                    help="Provide the generated .xpfm platform file for application creation.")
parser.add_argument("--mode", type=str, dest="mode", choices=["hw", "hw_emu", "app"],  
                    help="Sellect to create hardware platform or hardware emulation platform or application component.")

args = parser.parse_args()

# Extract variables
platform_name = args.platform_name
xsa_path = args.xsa_path
emuxsa_path = args.emuxsa_path
sysroot = args.sysroot
mode = args.mode
platform_fixed = args.platform_fixed
client = vitis.create_client()


current_path = os.getcwd()
print(current_path)
client.update_workspace(path=current_path)
client.set_workspace(path=current_path)


def generate_pfm():
    print("Platform generation")
    advanced_options = client.create_advanced_options_dict(board_dtsi="versal2-vek385-reva",dt_zocl="1",dt_overlay="1")
    platform = client.create_platform_component(name =platform_name,hw_design = xsa_path,os = "linux",cpu = "cortexa78",domain_name = "linux_cortexa78",generate_dtb = False,advanced_options = advanced_options)
    platform = client.get_component(name=platform_name)
    domain = platform.add_domain(cpu = "ai_engine",os = "aie_runtime",name = "aie",display_name = "aie",generate_dtb = False)
    status = platform.build()
def generate_application():    
    print("Application generation and building")
    comp = client.create_app_component(name="application",platform =platform_fixed)
    comp = client.get_component(os.path.join(current_path, "application"))
    status = comp.import_files(from_loc="", files=["../aie2ps/input.h", "../aie2ps/host.cpp", "../aie2ps/golden.h", "./_aie/ps/c_rts/aie_control_xrt.cpp"])
    comp = client.get_component(os.path.join(current_path, "application"))
    status = comp.set_sysroot(sysroot=sysroot)
    comp = client.get_component(os.path.join(current_path, "application"))
    comp.set_app_config(key = 'USER_CMAKE_CXX_STANDARD',values = '17')
    comp.set_app_config(key = 'USER_INCLUDE_DIRECTORIES',values ='../../aie2ps/')
    comp.build()
def generate_pfm_emu():
    print("Platform generation")
    advanced_options = client.create_advanced_options_dict(board_dtsi="versal2-vek385-reva",dt_zocl="1",dt_overlay="1")
    platform = client.create_platform_component(name =platform_name,os = "linux",cpu = "cortexa78",domain_name = "linux_cortexa78",emu_design = emuxsa_path,generate_dtb = False,advanced_options = advanced_options)
    platform = client.get_component(name=platform_name)
    domain = platform.add_domain(cpu = "ai_engine",os = "aie_runtime",name = "aie",display_name = "aie",generate_dtb = False)
    status = platform.build()    
if args.mode == 'hw':
    generate_pfm()
elif args.mode == 'app':
    generate_application()
elif args.mode == 'hw_emu':
    generate_pfm_emu()

