#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

# Create Workspace
if {[file exists ./classic_workspace]} {
	puts "workspace exists. Deleting ..."
	file delete -force ./classic_workspace
}
setws ./classic_workspace

# Create HW platform with A53 and R5 domains
platform create -name zcu102_platform -hw ./zcu102/design_1_wrapper.xsa

domain create -name a53_domain -os standalone -proc psu_cortexa53_0
domain active a53_domain

platform write
platform generate

# Create test applications
app create -name testapp_a53 -platform zcu102_platform -domain a53_domain -template "Empty Application"
file copy -force ./src/main.c ./classic_workspace/testapp_a53/src/main.c
file copy -force ./src/lscript.ld ./classic_workspace/testapp_a53/src/lscript.ld

# Build applications
app build all
