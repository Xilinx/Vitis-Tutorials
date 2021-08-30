# IVAS utility for IVAS Gstreamer plugins and IVAS Kernel libs

***We dont't suggest to modify ivas-util code.***

# Steps to install meson:
```
sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build
pip3 install meson 
Above steps installs meson in  /home/<username>/.local/bin/meson, so change PATH env to point using
export PATH= /home/<username>/.local/bin/meson:$PATH
```
# Native compilation steps:

```
meson --libdir=/usr/local/lib --includedir=/usr/local/include build;
cd build;
ninja;
sudo ninja install;
```

# Steps for Cross Compilation:
1. Copy sdk.sh file to <sdk.sh_folder> on build machine
```
/proj/petalinux/2020.1/petalinux-v2020.1_daily_latest/bsp/internal/zynqmp/xilinx-zynqmp-common-v2020.1/sdk.sh 
```
2. Prepare SYSROOT and set environment variables
```
cd <sdk.sh_folder>
./sdk.sh -d `pwd` -y
```
3. Edit gst-plugins-ivas/meson.cross to point to SYSROOT path

4. Build & Compile IVAS GStreamer plugins
```
meson build --cross-file meson.cross
cd build;
ninja;
sudo ninja install;
```
