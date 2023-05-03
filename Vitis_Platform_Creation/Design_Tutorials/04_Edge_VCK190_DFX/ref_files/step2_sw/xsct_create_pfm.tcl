#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

# parsing options
set options [dict create {*}$argv]

set static_xsa_path [dict get $options static_xsa_path]
set platform_name [dict get $options platform_name]
set rp_xsa_path [dict get $options rp_xsa_path]
set emu_xsa_path [dict get $options emu_xsa_path]
set boot_dir_path [dict get $options boot_dir_path]
set boot_bin [dict get $options boot_bin]
set sd_dir_path [dict get $options sd_dir_path]
set platform_out [dict get $options platform_out]
# Hard coded variable


puts "INFO: Start to create platform with following inputs"
puts "static_xsa_path=${static_xsa_path}"
puts "platform_name=${platform_name}"
puts "rp_xsa_path=${rp_xsa_path}"
puts "emu_xsa_path=${emu_xsa_path}"
puts "boot_dir_path=${boot_dir_path}"
puts "sd_dir_path=${sd_dir_path}"


# Option validation
if {$static_xsa_path eq ""} {
  error "***STATIC_XSA can't be empty, please rerun after setting it"
}

# Formatting arg list for RP
set rp_info_arg [list]
if { ($rp_xsa_path ne "") && ($emu_xsa_path ne "") && [file exists $rp_xsa_path] && [file exists $emu_xsa_path] } {
  lappend rp_info_arg id
  lappend rp_info_arg 0
  lappend rp_info_arg hw
  lappend rp_info_arg $rp_xsa_path
 lappend rp_info_arg hw_emu
 lappend rp_info_arg $emu_xsa_path
} elseif { $rp_xsa_path ne "" && [file exists $rp_xsa_path] } {
  lappend rp_info_arg id
  lappend rp_info_arg 0
  lappend rp_info_arg hw
  lappend rp_info_arg $rp_xsa_path
} else {
  error "***RP_XSA can't be empty, please rerun after setting it"
}



# XSCT commands
platform create -name $platform_name -desc "A custom VCK190 DFX platform" \
    -hw $static_xsa_path \
    -rp $rp_info_arg \
    -out $platform_out \
    -no-boot-bsp
# AI Engine Domain
domain create -name aiengine -os aie_runtime -proc {ai_engine} 
domain config -qemu-data $boot_dir_path
# Linux Domain
domain create -name xrt -proc psv_cortexa72 -os linux -sd-dir $sd_dir_path 
# DFX static boot image
domain config -hw-boot-bin $boot_bin
domain config -boot $boot_dir_path
domain config -generate-bif
domain config -qemu-data $boot_dir_path

platform generate
