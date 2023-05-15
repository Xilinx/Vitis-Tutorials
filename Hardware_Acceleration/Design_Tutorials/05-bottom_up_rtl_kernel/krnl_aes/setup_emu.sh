#! /bin/bash

# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11

print_usage () {
  echo "Usage: "
  echo "  setup_emu.sh -s EMU_MODE -p PLATFORM        set emulation mode"
  echo ""
  echo "  EMU_MODE: on  - turn on hw_emu mode"
  echo "            off - turn off emulation mode"
  echo "  PLATFORM: the installed development platform name, which can be ..."
  echo "              xilinx_u200_gen3x16_xdma_2_202110_1 (default)"
  echo "              xilinx_u250_gen3x16_xdma_4_1_202210_1"
  echo "              xilinx_u280_gen3x16_xdma_1_202211_1"
  echo "              xilinx_u50_gen3x16_xdma_5_202210_1"
  echo "              xilinx_u55c_gen3x16_xdma_3_202210_1"
  echo "" 
}

if [ "$1" = "" ]
then
  print_usage
fi

switch=""
platform="xilinx_u200_gen3x16_xdma_2_202110_1"

while true;
do
    case "$1" in
        -s)
            case "$2" in
                on) switch="on"; shift 2;;
                off) switch="off"; break;;
                *) print_usage;;
            esac ;;
        -p)
            platform="$2"; shift 2;;
        "") break;;
        *) print_usage; break ;;
    esac
done

if [ "$switch" = "off" ]
then
  echo "Exit Emulation Mode"
  unset XCL_EMULATION_MODE
else 
  if [ "$switch" = "on" ]
  then
    echo "Generating emulation config file for platform $platform.."
    export XCL_EMULATION_MODE=hw_emu
    emconfigutil --platform $platform
    echo "Enter Hardware Emulation Mode"
  fi
fi
