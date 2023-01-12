FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI                 += "file://system-conf.dtsi"
EXTRA_OVERLAYS           = "system-user.dtsi"

