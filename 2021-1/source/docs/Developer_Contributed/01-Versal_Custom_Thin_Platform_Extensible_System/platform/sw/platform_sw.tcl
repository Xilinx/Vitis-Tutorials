# © Copyright 2020 Xilinx, Inc.
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
puts "Creating platform: \"$platform_name\""

set xsa [lindex $argv 1]
puts "using xsa: \"$xsa\""

set output_path [lindex $argv 2]
puts "with output path: \"$output_path\""

set OUTPUT platform_repo

set SRC ./src/

platform create -name $platform_name -desc " A platform targetting VCK190 for demonstration purpose with a Linux, AI Engine and a Standalone domain" -hw $xsa -out $output_path -no-boot-bsp
domain create -name aiengine -os aie_runtime -proc {ai_engine}
domain config -pmcqemu-args $SRC/qemu/aie/pmc_args.txt
domain config -qemu-args $SRC/qemu/aie/qemu_args.txt
domain config -qemu-data $SRC/boot

## Create the Linux domain
domain create -name xrt -proc psv_cortexa72 -os linux -image $SRC/a72/xrt/image
domain config -boot $SRC/boot
domain config -bif $SRC/a72/xrt/linux.bif
domain config -pmcqemu-args $SRC/qemu/lnx/pmc_args.txt
domain config -qemu-args $SRC/qemu/lnx/qemu_args.txt
domain config -qemu-data $SRC/boot

## Create the Standalone domain 
domain create -name standalone_domain -os standalone -proc psv_cortexa72_0

platform generate
