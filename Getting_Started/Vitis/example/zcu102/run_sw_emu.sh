
#
# In QEMU, once Linux has booted, run this script by entering the following command:
#   source /mnt/sd-mmcblk0p1/run_app.sh 
#

mount /dev/mmcblk0p1 /mnt
cd /mnt

cp platform_desc.txt /etc/xocl.txt

export XILINX_XRT=/usr
export XILINX_VITIS=/mnt
export XCL_EMULATION_MODE=sw_emu

./app.exe 

echo "INFO: press Ctrl+a x to exit qemu"

