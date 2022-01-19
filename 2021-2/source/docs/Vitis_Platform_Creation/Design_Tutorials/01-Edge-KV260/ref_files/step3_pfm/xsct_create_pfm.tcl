# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# setws ./build
set platform_name [lindex $argv 0]
puts "The platform name is \"$platform_name\"" 

set xsa_path [lindex $argv 1]
puts "The xsa path is \"$xsa_path\"" 

set OUTPUT platform_repo
# set SW_COMP platform_repo/tmp/sw_components/

platform create -name $platform_name -desc "A custom platform KV260 platform" -hw $xsa_path/$platform_name.xsa -out ./$OUTPUT -no-boot-bsp 

# Linux domain
domain create -name xrt -proc psu_cortexa53 -os linux -arch {64-bit} -runtime {ocl} -sd-dir {./sd_dir}  -bootmode {sd}
# domain active xrt
domain config -boot {./boot}
domain config -generate-bif
domain config -qemu-data ./boot
# Standalone Domain
#domain create -name {standalone} -display-name {standalone_domain} -os {standalone} -proc {psv_cortexa72_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}

platform write
platform generate






