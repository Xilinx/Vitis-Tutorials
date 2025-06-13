export XILINX_VITIS=/proj/xbuilds/2024.1_daily_latest/installs/lin64/Vitis/2024.1

export PYTHON_VER="python-3.8.3"
export CMAKE_VER="cmake-3.24.2"

export XBUILDS_CMAKE_PATH=${XILINX_VITIS}/tps/lnx64/${CMAKE_VER}
export XBUILDS_PYTHON_PATH=${XILINX_VITIS}/tps/lnx64/${PYTHON_VER}

# LD_LIBRARY_PATH is needed to add the linker libraries (e.g. libffi) to the path
export LD_LIBRARY_PATH=${XBUILDS_PYTHON_PATH}/lib:${XBUILDS_CMAKE_PATH}/libs/Ubuntu:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${XILINX_VITIS}/lib/lnx64.o/Ubuntu/20:$LD_LIBRARY_PATH

# Source Paths of different toolchains, python, cmake and lopper binary
export PATH=${XILINX_VITIS}/bin:${XILINX_VITIS}/gnu/microblaze/lin/bin:${XILINX_VITIS}/gnu/arm/lin/bin:${XILINX_VITIS}/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin:${XILINX_VITIS}/gnu/aarch32/lin/gcc-arm-none-eabi/bin:${XILINX_VITIS}/gnu/aarch64/lin/aarch64-none/bin:${XILINX_VITIS}/gnu/armr5/lin/gcc-arm-none-eabi/bin:${XBUILDS_PYTHON_PATH}/bin:${XBUILDS_CMAKE_PATH}/bin:$PATH

# VALIDATE_ARGS flag is needed to enable the validation of inputs. Set this flag to "" when
# using the GUI flow.
export VALIDATE_ARGS="True"
# OSF Flag is needed to be enabled for Open Source Flow.
export OSF="False"

# Below is a fallback mechanism to set an esw repo when no user repo is set
# export ESW_REPO="${XILINX_VITIS}/data/embeddedsw-sdt"

# LOPPER_DTC_FLAGS is needed to generate device tree with symbols in it.
export LOPPER_DTC_FLAGS="-b 0 -@"

source ${XILINX_VITIS}/tps/lnx64/lopper-1.1.0/env/bin/activate
