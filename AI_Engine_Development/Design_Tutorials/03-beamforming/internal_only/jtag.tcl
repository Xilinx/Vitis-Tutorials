set hostname [lindex $argv 0]
set images_path [lindex $argv 1]
connect -host $hostname -port 3121
targets -set -nocase -filter {name =~ "*Versal*"}
device program "$images_path/BOOT.BIN"
stop
after 2000
targets -set -nocase -filter {name =~ "*Versal*"}
dow -data -force "$images_path/Image" 0x00080000
after 2000
targets -set -nocase -filter {name =~ "*Versal*"}
dow -data -force "$images_path/rootfs.cpio.gz.u-boot" 0x4000000
after 2000
targets -set -nocase -filter {name =~ "*Versal*"}
dow -data -force "$images_path/boot.scr" 0x20000000
after 2000
con
