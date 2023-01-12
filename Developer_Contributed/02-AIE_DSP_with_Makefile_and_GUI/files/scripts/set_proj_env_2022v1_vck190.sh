#!/bin/bash
#
# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Authors: Peter Schillinger, Daniele Bagni  Xilinx Inc


# for VCK190P common sw package
export EDGE_COMMON_SW=/opt/xilinx/common/xilinx-versal-common-v2022.1


# COMMON VERSAL "SDK" settings
# ******************************************************************************************************
unset LD_LIBRARY_PATH
#source ${EDGE_COMMON_SW}/environment-setup-cortexa72-cortexa53-xilinx-linux
# ******************************************************************************************************

# Check for LD_LIBRARY_PATH being set, which can break SDK and generally is a bad practice
# http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html#AEN80
# http://xahlee.info/UnixResource_dir/_/ldpath.html
# Only disable this check if you are absolutely know what you are doing!
if [ ! -z "$LD_LIBRARY_PATH" ]; then
    echo "Your environment is misconfigured, you probably need to 'unset LD_LIBRARY_PATH'"
    echo "but please check why this was set in the first place and that it's safe to unset."
    echo "The SDK will not operate correctly in most cases when LD_LIBRARY_PATH is set."
    echo "For more references see:"
    echo "  http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html#AEN80"
    echo "  http://xahlee.info/UnixResource_dir/_/ldpath.html"
    return 1
fi
export SDKTARGETSYSROOT=${EDGE_COMMON_SW}/sysroots/cortexa72-cortexa53-xilinx-linux
export PATH=${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/usr/bin:${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/usr/sbin:${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/bin:${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/sbin:${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/usr/bin/../x86_64-petalinux-linux/bin:${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/usr/bin/aarch64-xilinx-linux:${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/usr/bin/aarch64-xilinx-linux-musl:$PATH
export PKG_CONFIG_SYSROOT_DIR=$SDKTARGETSYSROOT
export PKG_CONFIG_PATH=$SDKTARGETSYSROOT/usr/lib/pkgconfig:$SDKTARGETSYSROOT/usr/share/pkgconfig
export CONFIG_SITE=${EDGE_COMMON_SW}/site-config-cortexa72-cortexa53-xilinx-linux
export OECORE_NATIVE_SYSROOT="${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux"
export OECORE_TARGET_SYSROOT="$SDKTARGETSYSROOT"
export OECORE_ACLOCAL_OPTS="-I ${EDGE_COMMON_SW}/sysroots/x86_64-petalinux-linux/usr/share/aclocal"
export OECORE_BASELIB="lib"
export OECORE_TARGET_ARCH="aarch64"
export OECORE_TARGET_OS="linux"
unset command_not_found_handle
export CC="aarch64-xilinx-linux-gcc  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$SDKTARGETSYSROOT"
export CXX="aarch64-xilinx-linux-g++  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$SDKTARGETSYSROOT"
export CPP="aarch64-xilinx-linux-gcc -E  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$SDKTARGETSYSROOT"
export AS="aarch64-xilinx-linux-as "
export LD="aarch64-xilinx-linux-ld  --sysroot=$SDKTARGETSYSROOT"
export GDB=aarch64-xilinx-linux-gdb
export STRIP=aarch64-xilinx-linux-strip
export RANLIB=aarch64-xilinx-linux-ranlib
export OBJCOPY=aarch64-xilinx-linux-objcopy
export OBJDUMP=aarch64-xilinx-linux-objdump
export READELF=aarch64-xilinx-linux-readelf
export AR=aarch64-xilinx-linux-ar
export NM=aarch64-xilinx-linux-nm
export M4=m4
export TARGET_PREFIX=aarch64-xilinx-linux-
export CONFIGURE_FLAGS="--target=aarch64-xilinx-linux --host=aarch64-xilinx-linux --build=x86_64-linux --with-libtool-sysroot=$SDKTARGETSYSROOT"
export CFLAGS=" -O2 -pipe -g -feliminate-unused-debug-types "
export CXXFLAGS=" -O2 -pipe -g -feliminate-unused-debug-types "
export LDFLAGS="-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed -Wl,-z,relro,-z,now"
export CPPFLAGS=""
export KCFLAGS="--sysroot=$SDKTARGETSYSROOT"
export OECORE_DISTRO_VERSION="2022.1"
export OECORE_SDK_VERSION="2022.1"
export ARCH=arm64
export CROSS_COMPILE=aarch64-xilinx-linux-

# Append environment subscripts
if [ -d "$OECORE_TARGET_SYSROOT/environment-setup.d" ]; then
    for envfile in $OECORE_TARGET_SYSROOT/environment-setup.d/*.sh; do
	    . $envfile
    done
fi
if [ -d "$OECORE_NATIVE_SYSROOT/environment-setup.d" ]; then
    for envfile in $OECORE_NATIVE_SYSROOT/environment-setup.d/*.sh; do
	    . $envfile
    done
fi


# ******************************************************************************************************
# VITIS ENVIRONMENT SETTINGS
# ******************************************************************************************************

# FOR VITIS AIE TUTORIALS
export XILINX_TOOLS_LOCATION=/tools/Xilinx/Vitis/2022.1
export XLNX_VERSAL=$EDGE_COMMON_SW

# set the Vitis/Vivado 2021.2 release
source ${XILINX_TOOLS_LOCATION}/settings64.sh

# set petalinux 2021.2 release
source /petalinux_2022.1/settings.sh


# working directory: change with your own
export WRK_DIR=/media/danieleb/DATA/2022.1/tutorials

# for Vitis AI 2.5: change with your own or leave commented if you do not have it
#export VITIS_AI_PATH=/media/danieleb/DATA/VAI2.5

# for the HLS kernels and host applications
#export MPSOCDPU_PRE_POST_PL_ACC=${WRK_DIR}/MPSOCDPU-PRE-POST-PL-ACC/files

# for the necessary included files and libraries of host_apps
#export TRD_HOME=/media/danieleb/DATA/MPSOCDPU-PRE-POST-PL-ACC/files/dpu_trd


# for VCK190P common sw package
export VITIS_SYSROOTS=${EDGE_COMMON_SW}/sysroots/cortexa72-cortexa53-xilinx-linux
export SYSROOT=${VITIS_SYSROOTS}


export XILINX_XRT=/opt/xilinx/xrt #DB
#cd ${XILINX_XRT}
source ${XILINX_XRT}/setup.sh
#cd ${WRK_DIR}

# for common zcu102 platform
export VITIS_PLATFORM=xilinx_vck190_base_202210_1
#export VITIS_PLATFORM_DIR=${XILNX_TOOLS_LOCATION}/base_platforms/${VITIS_PLATFORM}
#export VITIS_PLATFORM_PATH=${VITIS_PLATFORM_DIR}
#/${VITIS_PLATFORM}.xpfm
export VITIS_PLATFORM_PATH=${XILINX_TOOLS_LOCATION}/base_platforms
#/${VITIS_PLATFORM}
export PLATFORM_REPO_PATHS=${VITIS_PLATFORM_PATH}
#/${VITIS_PLATFORM}.xpfm
#export SDX_PLATFORM=${VITIS_PLATFORM}


# for Model Composer
export XILINX_LOC=${XILINX_TOOLS_LOCATION}
#$XILINX_LOC/../../Model_Composer/2022.1/bin/model_composer -matlab /usr/local/MATLAB/R2021a -vitis $XILINX_LOC

# FOR VITIS DSP LIBRARIES
export DSPLIB_ROOT=/media/danieleb/DATA/2022.1/Vitis_Libraries/dsp
plat=${VITIS_PLATFORM}
#xilinx_vck190_base_202210_1
export PLATFORM=${PLATFORM_REPO_PATHS}/${plat}/${plat}.xpfm


#export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/xilinx_vck190_base_202120_1
#export SYSROOT=${VITIS_SYSROOTS} #already done
export K_IMAGE=${EDGE_COMMON_SW}/Image
export ROOTFS=${EDGE_COMMON_SW}/rootfs.ext4
export IMAGE=${K_IMAGE}

#some basic checking
error=0

: '
if [ -d "$TRD_HOME" ]; then
  echo "TRD_HOME=$TRD_HOME exists"
else
  echo "TRD_HOME=$TRD_HOME does not exist"
  error=$((error+1))
fi
'

if [ -d "$VITIS_AI_PATH" ]; then
  echo "VITIS_AI_PATH=$VITIS_AI_PATH exists"
else
  echo "VITIS_AI_PATH=$VITIS_AI_PATH does not exist"
  error=$((error+1))
fi

: '
if [ -d "$MPSOCDPU_PRE_POST_PL_ACC" ]; then
echo "MPSOCDPU_PRE_POST_PL_ACC=$MPSOCDPU_PRE_POST_PL_ACC exists"
else
  echo "MPSOCDPU_PRE_POST_PL_ACC=$MPSOCDPU_PRE_POST_PL_ACC does not exist"
  error=$((error+1))
fi
'

if [ -d "$VITIS_SYSROOTS" ]; then
echo "VITIS_SYSROOTS=$VITIS_SYSROOTS exists"
else
  echo "VITIS_SYSROOTS=$VITIS_SYSROOTS does not exist"
  error=$((error+1))
fi

if [ -d "$VITIS_PLATFORM_PATH" ]; then
echo "VITIS_PLATFORM_PATH=$VITIS_PLATFORM_PATH exists"
else
  echo "VITIS_PLATFORM_PATH=$VITIS_PLATFORM_PATH does not exist"
  error=$((error+1))
fi
if [ $error > 0 ];
then
    echo "$error variables not set correctly";
else
    echo "All Variables checked OK";
fi;



rm -rf 0
