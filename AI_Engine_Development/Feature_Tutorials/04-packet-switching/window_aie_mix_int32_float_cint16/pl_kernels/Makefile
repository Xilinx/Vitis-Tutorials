#
#© Copyright 2020-2022 Xilinx, Inc.
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
PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202210_1/xilinx_vck190_base_202210_1.xpfm

.PHONY: clean

all: $(subst .cpp,.xo,$(wildcard *.cpp)) ../Work/temp/packet_ids_c.h
%.xo: %.cpp
	cp ../Work/temp/packet_ids_c.h .
	v++ -c --platform ${PLATFORM} -k $(basename $<) --kernel_frequency 250 $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log
