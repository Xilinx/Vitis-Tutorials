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
platform create -name zcu102_platform -hw ./zcu102/design_1_wrapper.xsa -proc psu_cortexa53 -os linux

platform write
platform generate

# Create test applications
app create -name linux_test_app -sysproj linux_test_app_system -platform zcu102_platform -domain linux_domain -template "Linux Empty Application"
importsources -name linux_test_app -path ../src

# Build applications
app build all
