#!/bin/sh

set -e

if [ -z "$CC" ]
then
    echo "Cross compilation not set - source environment setup first"
    exit
fi

if [ ! -e "$SDKTARGETSYSROOT/usr/lib/libvart-util.so" ]
then
    echo "Vitis AI is not installed in the target sysroot"
    echo "Please see instructions here: "
    echo "    https://github.com/Xilinx/Vitis-AI/tree/v1.2/Vitis-AI-Library#quick-start-for-edge"
    while true; do
    read -p "Do you wish to install this program? (y/n) " yn
    case $yn in
        [Yy]* ) wget -O vitis_ai_2020.1.tar.gz https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_2020.1-r1.2.0.tar.gz; tar -xzvf vitis_ai_2020.1.tar.gz -C $SDKTARGETSYSROOT; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
fi

# This script automates the build and deployment of images to the target. If your
# environment is different you may need to make minor changes.

rm -rf install

if [ ! -e "$SDKTARGETSYSROOT/usr/lib/pkgconfig/xrt.pc" ]
then
    echo "Installing XRT PackageConfig files"
    cp xrt.pc $SDKTARGETSYSROOT/usr/lib/pkgconfig/xrt.pc
fi

cd ivas-utils
sed -E 's@<SYSROOT>@'"$SDKTARGETSYSROOT"'@g; s@<NATIVESYSROOT>@'"$OECORE_NATIVE_SYSROOT"'@g' meson.cross.template > meson.cross
cp meson.cross $OECORE_NATIVE_SYSROOT/usr/share/meson/aarch64-xilinx-linux-meson.cross
meson --prefix /usr build --cross-file meson.cross
cd build
ninja
DESTDIR=../../install ninja install

cd ../../ivas-gst-plugins
sed -E 's@<SYSROOT>@'"$SDKTARGETSYSROOT"'@g; s@<NATIVESYSROOT>@'"$OECORE_NATIVE_SYSROOT"'@g' meson.cross.template > meson.cross
cp meson.cross $OECORE_NATIVE_SYSROOT/usr/share/meson/aarch64-xilinx-linux-meson.cross
meson --prefix /usr build --cross-file meson.cross
cd build
ninja
DESTDIR=../../install ninja install

cd ../../ivas-kernel-libs
sed -E 's@<SYSROOT>@'"$SDKTARGETSYSROOT"'@g; s@<NATIVESYSROOT>@'"$OECORE_NATIVE_SYSROOT"'@g' meson.cross.template > meson.cross
cp meson.cross $OECORE_NATIVE_SYSROOT/usr/share/meson/aarch64-xilinx-linux-meson.cross
meson build --cross-file meson.cross --prefix /usr 
cd build
ninja
DESTDIR=../../install ninja install

cd ../../test_apps
if [ ! -d build ]
then
    mkdir build
fi
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ..
make
make DESTDIR=../../install install

cd ../../install

# Install ML models 
mkdir -p ./usr/share
cp -r ../../vitis_ai_library ./usr/share
#user to create ml_models folder and keep genrated densebox and reid models in it. ie dpu_densebox_640_360.elf dpu_reid.elf
cp ../../ml_models/dpu_densebox_640_360.elf ./usr/share/vitis_ai_library/models/densebox_640_360/densebox_640_360.elf
cp ../../ml_models/dpu_reid.elf ./usr/share/vitis_ai_library/models/reid/reid.elf
tar -pczvf update.tar.gz usr
cd ..

#
# Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
# EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
# not be used in advertising or otherwise to promote the sale, use or other
# dealings in this Software without prior written authorization from Xilinx.
#
