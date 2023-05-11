#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

proc build_dts {xsa dts} {
	hsi::open_hw_design $xsa
	hsi::set_repo_path ./
	set proc 0
	foreach procs [hsi::get_cells -hier -filter {IP_TYPE==PROCESSOR}] {
		if {[regexp {cortex} $procs]} {
			set proc $procs
			break
		}
	}
	if {$proc != 0} {
		puts "Targeting $proc"
		hsi::create_sw_design device-tree -os device_tree -proc $proc
    hsi::set_property CONFIG.dt_zocl true [hsi::get_os]
		hsi::generate_target -dir $dts
	} else {
		puts "Error: No processor found in XSA file"
	}
	hsi::close_hw_design [hsi::current_hw_design]
}

proc include_board {dts} {
  foreach lib [glob -nocomplain -directory device-tree-xlnx/device_tree/data/kernel_dtsi/2022.2/include/dt-bindings -type d *] {
    if {![file exists $dts/include/dt-bindings/[file tail $lib]]} {
      file copy -force $lib $dts/include/dt-bindings
    }
  }
  set dtsi_files [glob -nocomplain -directory device-tree-xlnx/device_tree/data/kernel_dtsi/2022.2/versal -type f *versal*]
  if {[llength $dtsi_files] != 0} {
    puts "Info: Board file: versal is found"
    file copy -force [lindex $dtsi_files end] $dts
    set fileId [open $dts/system-user.dtsi "w"]
    puts $fileId "/include/ \"[file tail [lindex $dtsi_files end]]\""
    puts $fileId "/ {"
    puts $fileId "};"
    close $fileId
  } else {
    puts "Info: Board file: versal is not found and will not be added to the system-top.dts"
  }
}

proc include_dtsi {dtsi_dir dtsi_file dts} {
	if {[file exists $dtsi_dir/$dtsi_file]} {
		file copy -force $dtsi_dir/$dtsi_file $dts
		set fp [open $dts/system-top.dts r]
		set file_data [read $fp]
		close $fp
		set fileId [open $dts/system-top.dts "w"]
		set data [split $file_data "\n"]
		foreach line $data {
		     puts $fileId $line
		}
		puts $fileId "#include \"$dtsi_file\""
		close $fileId
	}
}

