#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis
import sys
import os

app_path= os.getcwd()
cmd_args=len(sys.argv)
args=str(sys.argv)

client = vitis.create_client()
client.set_workspace(path="./workspace")

if cmd_args == 1:
    platform_name=base_pfm_vck190
else:
    platform_name=sys.argv[1]
    
platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

comp = client.create_aie_component(name="simple_aie_application", platform = platform_path, template = "installed_aie_examples/simple")

comp = client.get_component(name="simple_aie_application")
comp.build(target="hw")
