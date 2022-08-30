#© Copyright 2022 Xilinx, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#


.PHONY: clean

BASE_PLATFORM ?= "Unknown Platform"
BASE_DIR := $(PWD)

all:$(subst .cpp,.xo,$(wildcard *.cpp))
%.xo: %.cpp
	v++ -c -k $(basename $<) --platform $(BASE_PLATFORM) --target=$(TARGET) -DDATA_WIDTH=32 $< -o $@ --verbose --save-temps -g -I $(BASE_DIR)/pl_kernels

clean:
	rm -rf _x *.xo* *.log .Xil
