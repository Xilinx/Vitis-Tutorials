#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis

client = vitis.create_client()
client.set_workspace(path="./workspace")
    

comp = client.get_component(name="A-to-Z_app")
comp.build()

vitis.dispose()

