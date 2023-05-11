#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/


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




