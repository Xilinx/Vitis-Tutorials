#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
import vitis
import sys
import os

app_path= os.getcwd()
cmd_args=len(sys.argv)
args=str(sys.argv)



platform_name=sys.argv[1]
design_part=sys.argv[2]

platform_path=app_path+"/workspace_"+design_part+"/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

client = vitis.create_client()
client.set_workspace(path="./workspace_"+design_part)

comp = client.create_aie_component(name="simple_aie_app", platform = platform_path, template = "empty_aie_component")

comp = client.get_component(name="simple_aie_app")

status = comp.import_files(from_loc="./src/aie/", files=["graph.cpp","graph.h","k_1_buff_i_1_buff_o.cc", "k_1_buff_i_1_buff_o.h"])
status = comp.update_top_level_file(top_level_file="graph.cpp")

comp.build(target="hw")

vitis.dispose()
