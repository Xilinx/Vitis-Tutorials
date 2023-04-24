#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*

# Variables
set platform_name [lindex $argv 0]
puts "The platform name is \"$platform_name\"" 

set xsa_name [lindex $argv 1]
puts "The xsa_name is \"$xsa_name\"" 

set xsa_path [lindex $argv 2]
puts "The xsa path is \"$xsa_path\"" 

set boot_dir [lindex $argv 3]
puts "The boot_dir is \"$boot_dir\"" 

set sd_dir [lindex $argv 4]
puts "The sd_dir is \"$sd_dir\"" 

#set OUTPUT platform_repo

# XSCT commands
setws .


platform create -name $platform_name \
    -desc "A custom board platform" \
    -hw ${xsa_path}/${xsa_name}_hw.xsa \
    -hw_emu ${xsa_path}/${xsa_name}_hwemu.xsa \
    -out . \
    -no-boot-bsp 

# AIE domain
domain create -name {aiengine} -os {aie_runtime} -proc {ai_engine} -arch {32-bit} -desc {} -runtime {cpp}

# Linux domain
domain create -name xrt -proc psv_cortexa72 -os linux -arch {64-bit} -runtime {ocl} -sd-dir ${sd_dir}  -bootmode {sd}
domain active xrt
domain config -boot ${boot_dir}
domain config -generate-bif
domain config -qemu-data  ${boot_dir}
# Generate Platform
platform write
platform generate
