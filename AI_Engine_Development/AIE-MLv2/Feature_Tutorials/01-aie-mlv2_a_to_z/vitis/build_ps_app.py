#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vitis

client = vitis.create_client()
client.update_workspace(path="./workspace")
    

comp = client.get_component(name="A-to-Z_app")
comp.build()

vitis.dispose()

