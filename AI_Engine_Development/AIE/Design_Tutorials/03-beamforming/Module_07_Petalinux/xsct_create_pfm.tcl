

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

setws ./build
set platform_name [lindex $argv 0]
puts "The platform name is \"$platform_name\"" 

set xsa [lindex $argv 1]
puts "The xsa is \"$xsa\"" 

set OUTPUT build/platform_repo

platform create -name $platform_name -desc "A custom platform VCK190 platform for beamforming tutorial" -hw $xsa -out ./$OUTPUT -no-boot-bsp 

# AIE domain
domain create -name aiengine -os aie_runtime -proc ai_engine
# AIE domain emulation
domain config -pmcqemu-args ./src/qemu/aie/pmc_args.txt
domain config -qemu-args ./src/qemu/aie/qemu_args.txt
domain config -qemu-data ./src/boot

# Linux domain
domain create -name xrt -proc psv_cortexa72 -os linux -arch {64-bit} -runtime {ocl} -sd-dir {./build/image}  -bootmode {sd}
domain active xrt
domain config -boot {build/vck190_linux/images/linux}
domain config -bif ./build/vck190_linux/images/linux/linux.bif

# Linux domain emulation
# domain config -pmcqemu-args ./qemu/lnx/pmc_args.txt
# domain config -qemu-args ./qemu/lnx/qemu_args.txt
domain config -qemu-data ./src/boot


# Standalone Domain
#domain create -name {standalone} -display-name {standalone_domain} -os {standalone} -proc {psv_cortexa72_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}

platform write
platform generate

