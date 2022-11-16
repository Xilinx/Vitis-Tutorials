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

ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
XSA_NAME=vck190_custom_dfx
PLATFORM = $(ROOT_DIR)/../../step2_sw/build/pfm/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm

# XSA_NAME=vck190_custom_dfx
#PLATFOMR_PATH = ../../step2_pfm/platform_repo/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
# PLATFORM = ../step2_sw/build/pfm/$(XSA_NAME)/export/$(XSA_NAME)/$(XSA_NAME).xpfm
.PHONY: all clean

all: platforminfo.txt

platforminfo.txt: $(PLATFORM)
	platforminfo $(PLATFORM) > platforminfo.txt

clean:
	rm platforminfo.txt