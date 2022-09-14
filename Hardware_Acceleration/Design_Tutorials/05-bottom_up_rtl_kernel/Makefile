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

ECHO=@echo

.PHONY: help

PART := xcu200-fsgd2104-2-e
#PART := xcu250-figd2104-2L-e
#PART := xcu50-fsvh2104-2-e
#PART := xcu55c-fsvh2892-2L-e
#PART := xcu280-fsvh2892-2L-e

PLATFORM := xilinx_u200_gen3x16_xdma_2_202110_1
#PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
#PLATFORM := xilinx_u50_gen3x16_xdma_5_202210_1
#PLATFORM := xilinx_u55c_gen3x16_xdma_3_202210_1
#PLATFORM := xilinx_u280_gen3x16_xdma_1_202211_1

TARGET := hw


# Build hareware (xclbin) objects
build_krnl_aes:
	make -C ./krnl_aes all PART=$(PART) PLATFORM=$(PLATFORM) TARGET=$(TARGET)

build_krnl_cbc:
	make -C ./krnl_cbc all PART=$(PART) PLATFORM=$(PLATFORM) TARGET=$(TARGET)

all: build_krnl_aes build_krnl_cbc
