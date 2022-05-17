# Copyright 2022 Xilinx Inc.
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


set platform_name [lindex $argv 0]
puts "The platform name is \"$platform_name\"" 
set xsa_name [lindex $argv 1]
puts "The xsa name is \"$xsa_name\"" 
set xsa_path [lindex $argv 2]
puts "The xsa path is \"$xsa_path\"" 
#set OUTPUT platform_repo
#we use setws  command to specify the current directory as output directory. If you do not use ,you can set -out option.
setws .

if { [string equal -nocase fsbl [lindex $argv 3]] } {
    
    puts "create a simple Platform to get FSBL  and PMU"
    platform create -name $platform_name \
    -desc "A custom platform ZCU104 platform" \
    -hw ${xsa_path}/${xsa_name}_hw.xsa \
    -hw_emu ${xsa_path}/${xsa_name}_hwemu.xsa \
    -fsbl-target psu_cortexa53_0
#-out ./${OUTPUT}  

#   Standalone Domain
    domain create -name standalone -proc psu_cortexa53_0 -os standalone -arch {64-bit} 
    platform write
    platform generate
} else {
    
    puts "create final Vitis platform"
    platform create -name $platform_name \
        -desc "A custom platform ZCU104 platform" \
        -hw ${xsa_path}/${xsa_name}_hw.xsa \
        -hw_emu ${xsa_path}/${xsa_name}_hwemu.xsa \
        -no-boot-bsp    
#-out ./${OUTPUT}  

# Linux domain
    domain create -name xrt -proc psu_cortexa53 -os linux -arch {64-bit} -runtime {ocl} -sd-dir {./sd_dir}  -bootmode {sd}
    domain config -boot {./boot}
    domain config -generate-bif
    domain config -qemu-data ./boot
    platform write
    platform generate
  
}

    exit




