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

#!/bin/bash

petalinux-create -t project --template zynqMP -n ../xilinx-zcu102-v2023.2
cd ../xilinx-zcu102-v2023.2
petalinux-config --get-hw-description=../zcu102/mpsoc_preset_wrapper.xsa --silentconfig
sed -i 's/template/zcu102-rev1.0/' ../xilinx-zcu102-v2023.2/project-spec/configs/config
petalinux-build
petalinux-build --sdk
