do_install:append() {
  sed -i  '/mmcblk0p1/s/^#//g'                   ${D}${sysconfdir}/fstab
  sed -i  's+/media/card+/run/media/mmcblk0p1+g' ${D}${sysconfdir}/fstab
  sed -i  '/mmcblk0p1/s/,noauto//g'              ${D}${sysconfdir}/fstab
}

