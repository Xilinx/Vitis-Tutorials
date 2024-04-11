# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Create Workspace
setws ../workspace

# Create HW platform with A53 and R5 domains
platform create -name zcu102_platform -hw zcu102

domain create -name a53_domain -os standalone -proc psu_cortexa53_0
domain active a53_domain

platform write
platform generate

# Create test applications
app create -name testapp_a53 -platform zcu102_platform -domain a53_domain -template "Empty Application"
file copy -force ../src/main.c ../workspace/testapp_a53/src/main.c
file copy -force ../src/lscript.ld ../workspace/testapp_a53/src/lscript.ld

# Build applications
app build all
