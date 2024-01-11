# Copyright (C) 2022-2023, Advanced Micro Devices, Inc. All rights reserved. 
# SPDX-License-Identifier: MIT

proc build_dts {args} {
    set version 2023.2
	set hw_file 0
	set dtg_options ""
	set board ""
    for {set i 0} {$i < [llength $args]} {incr i} {
        if {[lindex $args $i] == "-version"} {
            set version [string toupper [lindex $args [expr {$i + 1}]]]
        }
        if {[lindex $args $i] == "-hw_file"} {
            set hw_file [lindex $args [expr {$i + 1}]]
        }
        if {[lindex $args $i] == "-dtg_options"} {
            set dtg_options [lindex $args [expr {$i + 1}]]
        }
        if {[lindex $args $i] == "-board"} {
            set board [lindex $args [expr {$i + 1}]]
        }
    }
	if {$hw_file == 0} {
		set xsa [glob -nocomplain -directory [pwd] -type f *.xsa *.hdf]
		if {[llength $xsa] > 1} {
			puts "INFO: Multiple XSA files found at location [pwd]. Use -hw_file to pass the XSA"
			set xsa [lindex $xsa 0]
		}
	} else {
		set xsa $hw_file
	}
	puts "INFO: Using XSA file [file tail $xsa]"
	hsi::open_hw_design $xsa
	set vivado_version [common::get_property VIVADO_VERSION [hsi::current_hw_design]]
	if {$version != $vivado_version} {
		puts "Warning: Vivado Version : $vivado_version is out of sync with DTG version : $version"
	}
	set proc [get_processor]
	hsi::set_repo_path ./repo
	hsi::create_sw_design device-tree -os device_tree -proc $proc
	if {$dtg_options != ""} {
		foreach options_arr [split $dtg_options] {
			hsi set_property CONFIG.${options_arr} true [hsi get_os]
		}
	}

	hsi::generate_target -dir my_dts
	if {![file exists my_dts/include/dt-bindings/]} {
		file mkdir my_dts/include/dt-bindings
	}
	foreach lib [glob -nocomplain -directory repo/my_dtg/device-tree-xlnx/device_tree/data/kernel_dtsi/${version}/include/dt-bindings -type d *] {
		if {![file exists my_dts/include/dt-bindings/[file tail $lib]]} {
			file copy -force $lib my_dts/include/dt-bindings
		}
	}
	if {$board != ""} {
		set fp [open my_dts/system-top.dts "r"] 
		set file_data [read $fp]
		close $fp
		set data [split $file_data "\n"]
		set fp [open my_dts/system-top.dts "w"]
		foreach line $data {
			if {[regexp {zynqmp-clk-ccf.dtsi} $line]} {
				puts $fp "#include \"zynqmp-clk-ccf.dtsi\""
				puts $fp "#include \"${board}.dtsi\""
			} elseif {[regexp {versal-clk.dtsi} $line]} {
				puts $fp "#include \"versal-clk.dtsi\""
				puts $fp "#include \"${board}.dtsi\""
			} else {
				puts $fp $line
			}
		}
		puts $fp "#include \"system-user.dtsi\""
		close $fp
	}
	set fileId [open my_dts/system-conf.dtsi "w"]
	close $fileId
	set fileId [open my_dts/system-user.dtsi "w"]
	puts $fileId "/include/ \"system-conf.dtsi\""
	puts $fileId "/ {"
	puts $fileId "};"
	close $fileId
	hsi::close_hw_design [hsi::current_hw_design]
}

proc get_processor {} {
	set procs [hsi::get_cells -hier -filter {IP_TYPE==PROCESSOR}]
	puts "INFO: Targeting proc: [lindex $procs 0]"
	return [lindex $procs 0]
}
