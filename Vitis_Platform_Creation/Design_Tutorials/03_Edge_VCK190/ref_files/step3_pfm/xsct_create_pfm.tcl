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

# Variables
set platform_name [lindex $argv 0]
puts "The platform name is \"$platform_name\"" 

set xsa_path [lindex $argv 1]
puts "The xsa path is \"$xsa_path\"" 

set OUTPUT platform_repo

# XSCT commands
setws ./build

platform create -name $platform_name \
    -desc "A custom platform VCK190 platform" \
    -hw ${xsa_path}/${platform_name}_hw.xsa \
    -hw_emu ${xsa_path}/${platform_name}_hwemu.xsa \
    -out ./$OUTPUT \
    -no-boot-bsp 

# AIE domain
domain create -name aiengine -os aie_runtime -proc ai_engine

# Linux domain
domain create -name xrt -proc psv_cortexa72 -os linux -arch {64-bit} -runtime {ocl} -sd-dir {./sd_dir}  -bootmode {sd}
domain active xrt
domain config -boot {../step2_petalinux/build/petalinux/images/linux}
domain config -generate-bif

# Generate Platform
platform write
platform generate
