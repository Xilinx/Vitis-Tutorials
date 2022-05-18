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

VERSION				?=2022.1
COMMON_IMAGE_PATH 	?=${PLATFORM_REPO_PATHS}/sw/zynqmp/xilinx-zynqmp-common-v$(VERSION)/

.PHONY: help

help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_PATH=<path/to/common_image/> 	## Generate platform with pre-built software components and compile the vadd application to test this platform "


all: check-common-image step1 step2 step3 

#target : check  whether the common image is ready 
check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_PATH)/Image),)
	@echo "'COMMON_IMAGE_PATH' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif
	
all: step1 step2 step3 

step1:
	$(MAKE) -C step1_vivado all

step2:
	$(MAKE) -C step2_pfm all

step3:
	$(MAKE) -C step3_validate all


clean:
	$(MAKE) -C step1_vivado clean
	$(MAKE) -C step2_pfm clean
	$(MAKE) -C step3_validate clean
ultraclean:clean
	$(MAKE) -C step2_pfm ultraclean
	$(MAKE) -C step3_validate ultraclean