#! /bin/bash

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

print_usage () {
  echo "Usage: "
  echo "  setup_emu.sh -m EMU_MODE -p PLATFORM        set emulation mode"
  echo "  setup_emu.sh off                            turn off emulation mode"
  echo ""
  echo "  EMU_MODE: hw - hw_emu"
  echo "            sw - sw_emu"
  echo "  PLATFORM: u200/u250/u50/u280"
  echo "" 
}

if [ "$1" = "" ]
then
  print_usage
fi

while true;
do
    case "$1" in
        off)
            echo "Exit Emulation Mode"
            unset XCL_EMULATION_MODE
            break;;
        -m)
            case "$2" in
                hw) echo "Enter Hardware Emulation Mode"; export XCL_EMULATION_MODE=hw_emu; shift 2;;
                sw) echo "Enter Software Emulation Mode"; export XCL_EMULATION_MODE=sw_emu; shift 2;;
                *) print_usage; break;;
            esac ;;
        -p)
            case "$2" in
                u200) echo "Preparing for U200 Platform Emulation"
                      emconfigutil --platform xilinx_u200_xdma_201830_2
                      shift 2;;
                u250) echo "Preparing for U200 Platform Emulation"
                      emconfigutil --platform xilinx_u250_xdma_201830_2
                      shift 2;;
                u50)  echo "Preparing for U50 Platform Emulation"
                      emconfigutil --platform xilinx_u50_gen3x16_xdma_201920_3
                      shift 2;;
                u280) echo "Preparing for U280 Platform Emulation"
                      emconfigutil --platform xilinx_u280_xdma_201920_3
                      shift 2;;
                *) print_usage ; break ;;
            esac ;;
        "") break;;
        *) print_usage; break;;
    esac
done

