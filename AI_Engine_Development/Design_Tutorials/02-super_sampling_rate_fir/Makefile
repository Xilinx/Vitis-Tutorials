#  (c) Copyright 2021–2022 Xilinx, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


.PHONY: run


PFM_NAME := xilinx_vck190_base_202220_1

run:
	make -C SingleKernel PFM_NAME=$(PFM_NAME) run
	make -C MultiKernel PFM_NAME=$(PFM_NAME) run
	make -C SingleStreamSSR PFM_NAME=$(PFM_NAME) run
	make -C DualStreamSSR PFM_NAME=$(PFM_NAME) run

clean:
	make -C SingleKernel clean
	make -C MultiKernel  clean
	make -C SingleStreamSSR  clean
	make -C DualStreamSSR  clean
