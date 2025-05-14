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
import sys

def generate_workspace(*args):
    xsa = ""
    lab = ""
    for i in range(0, len(args)):
        if args[i] == "-xsa":
            xsa = args[i+1]
        if args[i] == "-lab":
            lab = args[i+1]
    
    if xsa == "":
        print("Error: No -xsa passed")
        return ""
    
    if lab == "":
        print("Error: No -lab passed")
        return ""
       
    if lab == "1":
        print("Info: Lab1: Processor to Processor Cross Triggering")
        
        workspace = os.path.join(os.getcwd(), "./lab" + lab)
        workspace = os.path.normpath(workspace)
            
        print("Creating workspace at " + workspace)
        if os.path.exists(workspace):
            print("workspace exists. Deleting ...")
            shutil.rmtree(workspace)

        client = vitis.create_client()
        client.set_workspace(workspace)
        
        platform = client.create_platform_component(name = "zcu102_platform",hw_design = xsa,os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")
        xpfm = os.path.join(os.getcwd(), "./lab" + lab +"/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm")
        xpfm = os.path.normpath(xpfm)
        
        platform = client.get_component(name="zcu102_platform")
        a53_1_domain = platform.add_domain(cpu = "psu_cortexa53_1",os = "standalone",name = "standalone_psu_cortexa53_1",display_name = "standalone_psu_cortexa53_1",support_app = "hello_world")
        platform.build()
        
        hello_world_0 = client.create_app_component(name="hello_world_a53_0",platform = xpfm,domain = "standalone_psu_cortexa53_0",template = "hello_world")
        hello_world_0.import_files(from_loc="./src_files/a53_0", files=["helloworld.c"], dest_dir_in_cmp = "src")
        hello_world_1 = client.create_app_component(name="hello_world_a53_1",platform = xpfm,domain = "standalone_psu_cortexa53_1",template = "hello_world")
        hello_world_1.import_files(from_loc="./src_files/a53_1", files=["helloworld.c"], dest_dir_in_cmp = "src")
        hello_world_1.import_files(from_loc="./src_files/a53_1", files=["lscript.ld"], dest_dir_in_cmp = "src")
        hello_world_0.build()
        hello_world_1.build()
        
        platform.build()
        sys_proj = client.create_sys_project(name="system_project", platform=xpfm, template="empty_accelerated_application")
        sys_proj.add_component(name="hello_world_a53_0")
        sys_proj.add_component(name="hello_world_a53_1")
    elif lab == "2":
        print("Info: Lab2: PL to Processor Cross Triggering")
        
        workspace = os.path.join(os.getcwd(), "./lab" + lab)
        workspace = os.path.normpath(workspace)
            
        print("Creating workspace at " + workspace)
        if os.path.exists(workspace):
            print("workspace exists. Deleting ...")
            shutil.rmtree(workspace)

        client = vitis.create_client()
        client.set_workspace(workspace)
        
        platform = client.create_platform_component(name = "zcu102_platform",hw_design = xsa,os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")
        xpfm = os.path.join(os.getcwd(), "./lab" + lab +"/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm")
        xpfm = os.path.normpath(xpfm)
        
        platform = client.get_component(name="zcu102_platform")
        platform.build()
        
        gpio = client.create_app_component(name="gpio_test",platform = xpfm,domain = "standalone_psu_cortexa53_0",template = "empty_application")
        gpio.import_files(from_loc="./src_files/a53_0", files=["gpio_rx.c"], dest_dir_in_cmp = "src")
        gpio.build()
    elif lab == "3":
        print("Info: Lab3: Processor to PL Cross Triggering")
        
        workspace = os.path.join(os.getcwd(), "./lab" + lab)
        workspace = os.path.normpath(workspace)
            
        print("Creating workspace at " + workspace)
        if os.path.exists(workspace):
            print("workspace exists. Deleting ...")
            shutil.rmtree(workspace)

        client = vitis.create_client()
        client.set_workspace(workspace)
        
        platform = client.create_platform_component(name = "zcu102_platform",hw_design = xsa,os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")
        xpfm = os.path.join(os.getcwd(), "./lab" + lab +"/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm")
        xpfm = os.path.normpath(xpfm)
        
        platform = client.get_component(name="zcu102_platform")
        platform.build()
        
        gpio = client.create_app_component(name="gpio_test",platform = xpfm,domain = "standalone_psu_cortexa53_0",template = "empty_application")
        gpio.import_files(from_loc="./src_files/a53_0", files=["gpio_tx.c"], dest_dir_in_cmp = "src")
        gpio.build()
    else:
        print("Error: Unsupported lab " + lab + ". Supported labs are 1, 2, and 3")
        return ""
        
# This is used to allow users to call different functions with correct args
if sys.argv[1] == 'generate_workspace':
    print("Info: Generate Vitis Unified Workspace")
    xsa = ""
    lab = 1
    for i in range(1, len(sys.argv)):
        if sys.argv[i] == "--config":
            xsa = sys.argv[i+1]
            file_name, file_extension = os.path.splitext(xsa)
            if file_extension != ".xsa":
                print("Error: Invalid --config value. Expected XSA file")
                exit
        if sys.argv[i] == "--lab":
            lab = sys.argv[i+1]
                
    generate_workspace("-xsa", xsa, "-lab", lab)
    
