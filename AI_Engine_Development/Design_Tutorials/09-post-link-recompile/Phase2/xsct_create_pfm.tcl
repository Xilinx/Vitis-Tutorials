# Copyright 2021 Xilinx Inc.
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

set rootfs_path [lindex $argv 2]
puts "The rootfs path is \"$rootfs_path\""

set sysroot_path [lindex $argv 3]
puts "The sysroot path is \"$sysroot_path\""

set qemu_path [lindex $argv 4]
puts "The qemu path is \"$qemu_path\""

set image_path [lindex $argv 5]
puts "The image path is \"$image_path\""

set boot_path [lindex $argv 6]
puts "The boot path is \"$boot_path\""

set bif_path [lindex $argv 7]
puts "The bif path is \"$bif_path\""

set OUTPUT platform_repo
set SW_COMP platform_repo/tmp/sw_components/

platform create -name $platform_name -desc "A custom platform VCK190 platform for PostLink AI Engine Application compile" \
     -hw $xsa_path/$platform_name.xsa -out ./$OUTPUT -fsbl-target {psu_cortexa53_0}

platform write
platform generate domains

# AIE domain
domain create -name aiengine -os aie_runtime -proc ai_engine -runtime {cpp} -arch {32-bit}

platform active $platform_name
domain active {aiengine}
platform generate -quick
platform generate

# PS domain
domain create -name {xrt} -os {linux} -proc {psv_cortexa72} -arch {64-bit} -display-name {xrt} -desc {} -runtime {ocl}
platform write
domain config -boot $boot_path
platform write
domain config -bif $bif_path
platform write
domain config -sd-dir $image_path
domain -report -json
platform write
domain config -runtime {ocl}
platform write
domain config -bootmode {sd}
platform write
domain config -rootfs $rootfs_path
platform write
domain config -sysroot $sysroot_path/aarch64-xilinx-linux
platform write
domain config -qemu-data $qemu_path
platform write
domain config -qemu-args $qemu_path/qemu_args.txt
platform write
domain config -pmcqemu-args $qemu_path/pmc_args.txt
platform write
platform generate -domains
platform clean
platform generate
platform active $platform_name
