#
#Copyright 2020 Xilinx, Inc.
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#    http://www.apache.org/licenses/LICENSE-2.0
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#
PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_es1_base_202020_1/xilinx_vck190_es1_base_202020_1.xpfm

.PHONY: clean

###
# Guarding Checks. Do not modify.
###
check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_es1_base_2020210_1)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2020.2/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2020.2/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2020.2/environment-setup-aarch64-xilinx-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2020.2/environment-setup-aarch64-xilinx-linux)

###

all: guard-PLATFORM_REPO_PATHS $(subst .cpp,.xo,$(wildcard *.cpp))
%.xo: %.cpp
	v++ -c --platform ${PLATFORM} -k $(basename $<) --kernel_frequency 250 $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log
