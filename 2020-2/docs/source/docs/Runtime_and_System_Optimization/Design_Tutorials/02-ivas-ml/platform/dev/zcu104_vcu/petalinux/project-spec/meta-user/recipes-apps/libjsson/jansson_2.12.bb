DESCRIPTION = "Jansson library"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=6946b728e700de875e60ebb453cc3a20"

SRC_URI = "http://www.digip.org/jansson/releases/${BPN}-${PV}.tar.gz"

SRC_URI[md5sum] = "c4629b89bf0432f3158c461e88fe0113"

inherit autotools pkgconfig

