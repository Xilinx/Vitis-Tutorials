#
# Copyright 2021-2022 Xilinx, Inc.
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
#

all:
	make data
	make -C Emulation-AIE all

run:
	make data
	make -C Emulation-AIE aie aiesim

aie:
	make -C Emulation-AIE aie

aiesim:
	make -C Emulation-AIE aiesim

aieviz:
	make -C Emulation-AIE aieviz

clean:
	rm data/*
	touch data/dummy.txt
	make -C Emulation-AIE clean
	rm -f vitis_analyzer*

data:
	GenerateStreams cint16 64 8 2 256 4 Dirac data/PhaseIn

.PHONY: all clean aie aiesim aieviz data
