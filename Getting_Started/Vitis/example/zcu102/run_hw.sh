
#
# To run on the ZCU102 board, copy the packagge/sd_card directory onto the SD card, plug it into the board and power it up.
# When the Linux prompt appears, run this script by entering the following command:
#   source /mnt/sd-mmcblk0p1/run_app.sh 
#

mount /dev/mmcblk0p1 /mnt
cd /mnt

cp platform_desc.txt /etc/xocl.txt

export XILINX_XRT=/usr
export XILINX_VITIS=/mnt

./app.exe 

echo "INFO: press Ctrl+a x to exit qemu"

