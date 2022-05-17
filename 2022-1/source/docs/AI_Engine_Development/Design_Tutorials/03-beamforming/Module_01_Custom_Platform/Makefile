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

#Check environment setup
ifndef PLATFORM_REPO_PATHS
  PLATFORM_REPO_PATHS=/proj/xbuilds/$(TOOLVERSION)/internal_platforms/
  export PLATFORM_REPO_PATHS
endif
ifndef XILINX_VITIS
  XILINX_VITIS = /opt/xilinx/Vitis/$(TOOL_VERSION)
  export XILINX_VITIS
endif
ifndef XILINX_XRT
  XILINX_XRT = /opt/xilinx/xrt
  export XILINX_XRT
endif

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')
PERL ?= /tools/xgs/perl/5.8.5/bin/perl
PLATFORM := xilinx_vck190_base_202120_1

DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S

platform: xsa xpfm

xsa: 
	$(MAKE) xsa -C hw 

xpfm: 
	$(MAKE) xpfm -C sw 

clean:
	$(MAKE) clean -C hw
	$(MAKE) clean -C sw
