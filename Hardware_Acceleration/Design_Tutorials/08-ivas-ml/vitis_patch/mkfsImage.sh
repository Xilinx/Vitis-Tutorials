#!/bin/sh

SDCARD=sdcard.img
FATSDCARD=sdcardfat.img
EXT4SDCARD=

error()
{
        echo "ERROR: $1" 1>&2
        usage_and_exit 1
}

usage()
{
        echo "Usage: $PROGRAM [options] -s <sdcard folder> -o <sdcard image name> -e <ext4 imange file>"
        echo "  options:"
        echo "          --help, -v                              Print this usage"
        echo "NOTE: Generate sdcard with 1GB fat and 1GB ext4 partition"
        echo ""
}

usage_and_exit()
{
        usage
        exit $1
}

[ ! -x "$(command -v parted)" ] && error "parted command not exist"
[ ! -x "$(command -v dd)" ] && error "dd command not exist"
[ ! -x "$(command -v /sbin/fdisk)" ] && error "/sbin/fdisk command not exist"
[ ! -x "$(command -v truncate)" ] && error "truncate command not exist"

while [ $# -gt 0 ]; do
        case $1 in
                --help | -h )
                        usage_and_exit 0
                        ;;
                -s )
                        shift
                        SDCARD_FOLDER=$1
                        ;;
                -o )
                        shift
                        SDCARD=$1
                        ;;
                -e )
                        shift
                        EXT4SDCARD=$1
                        ;;
                --* | -* )
                        error "Unregognized option: $1"
                        ;;
                * )
                        break
                        ;;
        esac
        shift
done

PROGRAM=`basename $0`

[ ! -d $SDCARD_FOLDER ] && error "$SDCARD_FOLDER not exist"
[ -f $SDCARD ] && error "$SDCARD already exist"
[ ! -f $EXT4SDCARD ] && error "$EXT4SDCARD not exist"

mkfatimg $SDCARD_FOLDER ${FATSDCARD} 1048576
FATSIZE=1124 #1024MB #${/sbin/fdisk -l ${FATSDCARD} | grep "${FATSDCARD}1" |  awk -F" "  '{ print $4 }'}*512
fat_start=$(/sbin/fdisk -l ${FATSDCARD} | grep "${FATSDCARD}1" |  awk -F" "  '{ print $2 }')
fat_end=$(/sbin/fdisk -l ${FATSDCARD} | grep "${FATSDCARD}1" |  awk -F" "  '{ print $3 }')
fat_sector=`expr ${fat_end} - ${fat_start} + 1`
ext4_start=0
#ext4_sector=4194304
rootfs_img_size=`expr "$(ls -l ${EXT4SDCARD} | awk -F" " '{ print $5 }')" / 1`
ext4_size=`expr $(((((${rootfs_img_size}/(1024*1024))+511)/512)*512))`
ext4_sector=`expr $((${rootfs_img_size} / 512))`
dummy_ext4_sector=`expr $(((${ext4_size} * 1024 * 1024) / 512 - $ext4_sector))`;
TOTALSIZE=`expr $ext4_size + $FATSIZE` #MB

echo "FATSIZE:${FATSIZE}"
echo "fat_start:${fat_start}"
echo "fat_end:${fat_end}"
echo "fat_sector:${fat_sector}"
echo "ext4_start:${ext4_start}"
echo "ext4_sector:${ext4_sector}"
echo "EXT4SIZE:${ext4_size}"
echo "TOTALSIZE:${TOTALSIZE}"
echo "dummy_ext4_sector:${dummy_ext4_sector}"

truncate -s ${TOTALSIZE}M ${SDCARD}
parted -s ${SDCARD} mklabel msdos
parted -a optimal -s ${SDCARD} mkpart primary fat32 1 "${FATSIZE}"
parted -a optimal -s ${SDCARD} mkpart primary ext4 "${FATSIZE}" 100%
parted -s ${SDCARD} set 1 boot on
parted -s ${SDCARD} set 1 lba off

sd_card_fat_start=$(/sbin/fdisk -l ${SDCARD} | grep "${SDCARD}1" |  awk -F" "  '{ print $3 }')
sd_card_ext4_start=$(/sbin/fdisk -l ${SDCARD} | grep "${SDCARD}2" |  awk -F" "  '{ print $2 }')

dummy_ext4_start=`expr ${sd_card_ext4_start} + $ext4_sector`
echo "sd_card_fat_start:${sd_card_fat_start}"
echo "sd_card_ext4_start:${sd_card_ext4_start}"
echo "dummy_ext4_start:${dummy_ext4_start}"

dd bs=512 skip=${fat_start} seek=${sd_card_fat_start} if=${FATSDCARD} of=${SDCARD} count=${fat_sector}
dd bs=512 seek=${sd_card_ext4_start} if=${EXT4SDCARD} of=${SDCARD} count=${ext4_sector}
dd bs=512 seek=${dummy_ext4_start} if=/dev/zero of=${SDCARD} count=${dummy_ext4_sector}

rm -f $FATSDCARD

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
