# Copyright 2021–2022 Xilinx Inc.
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

setws ./build
set platform_name [lindex $argv 0]
puts "The platform name is \"$platform_name\""

set xsa_path [lindex $argv 1]
puts "The xsa path is \"$xsa_path\""

set versal_sw_path [lindex $argv 2]
puts "The Versal SW path is \"$versal_sw_path\""

set platform_sw_path [lindex $argv 3]
puts "The Platform SW path is \"$platform_sw_path\""


# Global Versal platforms
set rootfs_path $versal_sw_path/rootfs.ext4
puts "The rootfs path is \"$rootfs_path\""

set sysroot_path $versal_sw_path/sysroots
puts "The sysroot path is \"$sysroot_path\""



#Specific to VCK190
set qemu_path $platform_sw_path/qemu
puts "The qemu path is \"$qemu_path\""

set boot_path $platform_sw_path/boot
puts "The boot path is \"$boot_path\""

set bif_path $platform_sw_path/boot/linux.bif
puts "The bif path is \"$bif_path\""

set image_path $platform_sw_path/xrt/image
puts "The image path is \"$image_path\""

# Output platform
set OUTPUT platform_repo
set SW_COMP $OUTPUT/tmp/sw_components/

platform create -name $platform_name -hw $xsa_path/$platform_name.xsa -out ./$OUTPUT -no-boot-bsp

platform config -desc "A custom platform VCK190 platform for PostLink AI Engine Application re-compile"


# AIE domain
domain create -name aiengine -os aie_runtime -proc ai_engine
domain config -pmcqemu-args $qemu_path/pmc_args.txt
domain config -qemu-args $qemu_path/qemu_args.txt
domain config -qemu-data $boot_path



# PS (A72) domain
domain create -name xrt -os linux -proc psv_cortexa72

domain config -sd-dir $image_path
domain config -boot $boot_path
domain config -bif $bif_path
domain config -pmcqemu-args $qemu_path/pmc_args.txt
domain config -qemu-args $qemu_path/qemu_args.txt
domain config -qemu-data $boot_path
domain config -runtime {ocl}
domain config -bootmode {sd}
domain config -rootfs $rootfs_path
domain config -sysroot $sysroot_path/aarch64-xilinx-linux

domain -report -json


platform generate
