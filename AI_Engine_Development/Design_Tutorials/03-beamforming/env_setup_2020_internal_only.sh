
#Setup environment variables 
export XILINX_XRT=/proj/xbuilds/2020.2_daily_latest/xbb/xrt/packages/xrt-2.1.0-centos/opt/xilinx/xrt
export PATH_TO_BSP=/proj/petalinux/2020.2/petalinux-v2020.2_daily_latest_bsps/release

#INTERNAL ONLY: WILL BE REMOVED 
export LM_LICENSE_FILE=2100@aiengine

#Source setup scripts
source /proj/xbuilds/2020.2_daily_latest/installs/lin64/Vitis/2020.2/settings64.sh
source /proj/petalinux/2020.2/petalinux-v2020.2_daily_latest/tool/petalinux-v2020.2-final/settings.sh
source $XILINX_XRT/setup.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HSL         : "$XILINX_HLS
echo "PETALINUX          : "$PETALINUX
echo "PATH_TO_BSP        : "$PATH_TO_BSP
echo "XILINX_XRT         : "$XILINX_XRT

#INTERNAL ONLY: WILL BE REMOVED 
#Create Vivado_int.tcl and HLS_init.tcl if they do not exist already
touch ~/.Xilinx/Vivado/Vivado_init.tcl
touch ~/.Xilinx/HLS_init.tcl 

#INTERNAL ONLY: WILL BE REMOVED 
#Append enable_beta_device xcvc* if it is not already there
if grep -q enable_beta_device ~/.Xilinx/Vivado/Vivado_init.tcl; then
    echo "Beta device found!"
else
    echo "enable_beta_device xcvc*"
    echo "enable_beta_device xcvc*" >> ~/.Xilinx/Vivado/Vivado_init.tcl
fi 

if grep -q enable_beta_device ~/.Xilinx/HLS_init.tcl; then
    echo "Beta device found!"
else
    echo "enable_beta_device xcvc*"
    echo "enable_beta_device xcvc*" >> ~/.Xilinx/HLS_init.tcl
fi 
