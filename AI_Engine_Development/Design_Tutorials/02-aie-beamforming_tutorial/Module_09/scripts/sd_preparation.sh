#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

################################################################################
#Description      : This script is used to Prepare SD card to copy BOOT.BIN and
#					image.ub of prebuilt/build images.
#Author           : Ramesh Kumar Behara <rkumarbe@xilinx.com>
#Args             : COMPONENT (prebuilt/build)
#
#Date             : 10/15/2019
#Path in git repo : https://gitenterprise.xilinx.com/regressions/regressions-xlnx
#                   /common/petalinux/sd_preparation.sh
#Usage            : n/a [not for independent execution]
################################################################################

COMPONENT=$1
ip_address=$2
sdboot_ext4=$3
build_images_path="/mnt_nfsroot/"
prebuilt_images_path="/mnt_nfsroot/pre-built/linux/images/"

echo "START: sd_preparation.sh"

if [ ! -f "/tmp/dmesg.txt" ]; then
	dmesg >> /tmp/dmesg.txt
fi

status_check() {
	retCode="$1"
	msg="$2"
	if [ "$retCode" -eq 0 ]; then
		echo "$msg successful"
	else
		echo "$msg failed"
		exit 1
	fi
}

is_sd_present() {
	sdstring=$(cat /tmp/dmesg.txt | grep "sdhci" | wc -l)
	if [ $sdstring -ge 1 ]; then
		echo "sd node found $sdstring"
		retval="true"
	else
		echo "no sd node found"
		retval="false"
		exit 1
	fi
}
is_sd_present

ls -l /dev | grep mmcblk
retCode=$?
status_check "$retCode" "finding /dev/mmcblk mount point"

for mmc_host in /sys/class/mmc_host/* ; do
	sd_num=$(echo "$mmc_host" | cut -d "/" -f 5 | sed 's/mmc//g')
	grep -irn -w -q 'MMC_TYPE=SD' "$mmc_host"
	if [ $? -eq 0 ]; then
		sd_device="/dev/mmcblk${sd_num}"
		echo "sd_device=$sd_device, sd_num=$sd_num, mmc_host=$mmc_host "
	fi
	grep -irn -w -q 'MMC_TYPE=MMC' "$mmc_host"
	if [ $? -eq 0 ]; then
		emmc_device="/dev/mmcblk${sd_num}"
		echo "emmc_device=$emmc_device, sd_num=$sd_num, mmc_host=$mmc_host "
	fi
done

nfs_mount() {
	echo "Loading Images by nfsmount....!!!"
	echo "NFS mount is started"
	mkdir -p /mnt_nfsroot; umount /mnt_nfsroot;mount -o port=2049,nolock,proto=tcp,vers=2 $ip_address:/exports/root /mnt_nfsroot
	retCode=$?
	status_check "$retCode" "nfs mount"
	df -h
	chmod 777 *
	rm -rf ./*
	if [ "$COMPONENT" == "build" ] ; then
		echo "Copying Build Images..."
		cp $build_images_path/boot.scr ./
		cp $build_images_path/BOOT.BIN ./
		cp $build_images_path/Image ./
		echo "Images copying completed...."
	elif [ "$COMPONENT" == "prebuilt" ] && [ -f "$prebuilt_images_path/BOOT.BIN" ] && [ -f "$prebuilt_images_path/image.ub" ]; then
		echo "Copying prebuilt Images..."
		cp $prebuilt_images_path/boot.scr ./
		cp $prebuilt_images_path/BOOT.BIN ./
		echo "Images copying completed...."
	else
		echo ".BIN or .ub file is not found."
		exit 1;
	fi
}


fat32 () {
	df -h
	dd if=/dev/zero of=/dev/mmcblk0 bs=1024 count=4
	umount /mnt
	echo "fdisk started"
	echo -e "d\n1\nd\n2\nn\np\n1\n\n+500M\nY\n\nw\n" | fdisk $sd_device
	umount ${sd_device}p1
	echo -e "y\ny" | mkfs.vfat -F 32 -n boot ${sd_device}p1
	retCode=$?
	status_check "$retCode" "p1 set for vfat"
	umount /mnt
	echo -e "y\ny" | mount ${sd_device}p1 /mnt
	retCode=$?
	status_check "$retCode" "mounting p1"
	cd /mnt
	rm -rf ./*
	nfs_mount
	cd /
	ls -alt /mnt/
	umount /mnt
	retCode=$?
	status_check "$retCode" "un-mounting p1"
	sync
	df -h
	umount /mnt_nfsroot
	retCode=$?
	status_check "$retCode" "un-mounting mnt_nfsroot"
	umount ${sd_device}p1
	retCode=$?
	status_check "$retCode" "un-mounting of ${sd_device}p1"
	df -h
}

sd_ext4 () {
	df -h
	dd if=/dev/zero of=/dev/mmcblk0 bs=1024 count=4
	umount /mnt
	echo "fdisk started"
	echo -e "d\n1\nd\n2\nn\np\n1\n\n+500M\nY\nn\np\n2\n\n+2500M\np\nw\ny" | fdisk $sd_device
	umount ${sd_device}p1
	umount ${sd_device}p2
	echo -e "y\ny" | mkfs.vfat -F 32 -n boot ${sd_device}p1
	retCode=$?
	status_check "$retCode" "p1 set for vfat"
	echo -e "y\ny" | mkfs.ext4 -L root ${sd_device}p2
	retCode=$?
	status_check "$retCode" "p2 set for ext4"
	umount /mnt
	echo -e "y\ny" | mount ${sd_device}p1 /mnt
	retCode=$?
	status_check "$retCode" "mounting p1"
	cd /mnt
	rm -rf ./*
	nfs_mount
	cd /
	ls -alt /mnt/
	umount /mnt
	retCode=$?
	status_check "$retCode" "un-mounting p1"
	sync
	echo -e "y\ny" | mount ${sd_device}p2 /mnt
	retCode=$?
	status_check "$retCode" "mounting p2"
	cd /mnt
	rm -rf ./*
	if [ "$COMPONENT" == "build" ] && [ -f "$build_images_path/rootfs.ext4.gz" ]; then
		echo "Copying rootfs.ext4.gz"
		cp $build_images_path/rootfs.ext4.gz ./
	else
		echo "rootfs.ext4.gz file not found."
		exit 1;
	fi
	tar -xf rootfs.ext4.gz
	retCode=$?
	status_check "$retCode" "un-tar rootfs.ext4.gz"
	cd /
	ls -alt /mnt/
	sync
	umount /mnt
	retCode=$?
	status_check "$retCode" "un-mounting p2"
	sync
	umount /mnt_nfsroot
	retCode=$?
	status_check "$retCode" "un-mounting mnt_nfsroot"
	df -h
	umount ${sd_device}p1
	retCode=$?
	status_check "$retCode" "un-mounting of ${sd_device}p1"
	umount ${sd_device}p2
	retCode=$?
	status_check "$retCode" "un-mounting of ${sd_device}p2"
	df -h
}

df -h
if mount | grep $sd_device > /dev/null; then
	echo "$sd_device is already mounted..... unmounting it";
	umount $sd_device
else
	echo "$sd_device not mounted earlier";
fi

if [ ! -z "$sdboot_ext4" ] && [ "$sdboot_ext4" = "sd_ext4" ]; then
	sd_ext4 "$sd_device"
else
	fat32 "$sd_device"
fi

echo "END: sd_preparation.sh"
