set host_name [lindex $argv 0]
connect -host $host_name -port 3121
ta 1
mwr 0xF1260200 0xE100
mrd 0xF1260200
rst -system
