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

DEVICE_NAME := xcvc1902-vsva2197-1LP-e-S-es1

platform: xsa xpfm

xsa: 
	$(MAKE) xsa -C hw 

xpfm: 
	$(MAKE) xpfm -C sw 

clean:
	$(MAKE) clean -C hw
	$(MAKE) clean -C sw
