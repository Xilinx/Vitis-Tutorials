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

    workspace = os.path.join(os.getcwd(), "../unified_workspace")
    workspace = os.path.normpath(workspace)
    
    print("Creating workspace at " + workspace)
    if os.path.exists(workspace):
        print("workspace exists. Deleting ...")
        shutil.rmtree(workspace)

    client = vitis.create_client()
    client.set_workspace(workspace)

    platform = client.create_platform_component(name = "zcu102_platform",hw = xsa,os = "standalone",cpu = "psu_cortexa53_0")

    platform = client.get_platform_component(name="zcu102_platform")
    status = platform.build()

    xpfm = os.path.join(os.getcwd(), "../unified_workspace/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm")
    xpfm = os.path.normpath(xpfm)
    comp = client.create_app_component(name="testapp_a53",platform = xpfm,domain = "standalone_psu_cortexa53_0",template = "empty_application")

    comp = client.get_component(name="testapp_a53")
    comp.import_files(from_loc="../src", files=["main.c"], dest_dir_in_cmp = "src")
    comp.import_files(from_loc="../src", files=["lscript.ld"], dest_dir_in_cmp = "src")
    comp.build()
else:
    print("Error: No XSA passed")
