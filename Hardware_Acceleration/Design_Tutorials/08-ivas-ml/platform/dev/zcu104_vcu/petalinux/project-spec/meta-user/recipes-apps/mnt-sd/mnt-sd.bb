#
# This file is the mnt-sd recipe.
#

SUMMARY = "Simple mnt-sd application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://mnt-sd.sh"

INITSCRIPT_NAME = "mnt-sd"
INITSCRIPT_PARAMS = "start 99 5 ."

S = "${WORKDIR}"

do_install() {
	# Install to profile.d to set up environment for bash shell
	install -d ${D}${sysconfdir}/profile.d
	install -m 0755 mnt-sd.sh ${D}${sysconfdir}/profile.d/mnt-sd.sh
}
