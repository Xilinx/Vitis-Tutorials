
#
# In QEMU, once Linux has booted, run this script by entering the following command:
#   source /mnt/sd-mmcblk0p1/run_app.sh 
#

mount /dev/mmcblk0p1 /mnt
cd /mnt

export XCL_EMULATION_MODE=hw_emu

./app.exe 

echo "INFO: press Ctrl+a x to exit qemu"

