proc run_sdtgen {args} {
	set xsa "NULL"
	set sdt_out sdt_out
	set sdt_repo "NULL"
	set sdt_dt_overlay 0
	set sdt_dt_zocl 0
	set sdt_board_dtsi "NULL"
	set sdt_user_dtsi "NULL"
	for {set i 0} {$i < [llength $args]} {incr i} {
		if {[lindex $args $i] == "-xsa"} {
			set xsa [lindex $args [expr {$i + 1}]]
			if {[file extension $xsa] != ".xsa" && $xsa != "NULL"} {
				puts "Error: $xsa is not a valid XSA file"
				set xsa "NULL" 
			}
		}
		if {[lindex $args $i] == "-sdt_out"} {
			set sdt_out [lindex $args [expr {$i + 1}]]
		}
		if {[lindex $args $i] == "-sdt_repo"} {
			set sdt_repo [lindex $args [expr {$i + 1}]]
		}
		if {[lindex $args $i] == "-sdt_dt_overlay"} {
			set sdt_dt_overlay 1
		}
		if {[lindex $args $i] == "-sdt_dt_zocl"} {
			set sdt_dt_zocl [lindex $args [expr {$i + 1}]]
		}
		if {[lindex $args $i] == "-sdt_board_dtsi"} {
			set sdt_board_dtsi [lindex $args [expr {$i + 1}]]
		}
		if {[lindex $args $i] == "-sdt_user_dtsi"} {
			set sdt_user_dtsi [lindex $args [expr {$i + 1}]]
		}
	}
    
	if {$xsa == "NULL"} {
		puts "Warning: No XSA passed. Looking in [pwd]"
		set xsa [glob -nocomplain -directory [pwd] -type f *.xsa]
		if {![llength $xsa]} {
			puts "Error: No XSA passed. Use -xsa to pass a Valid XSA file"
			return ""
		} 
		if {[llength $xsa] > 1} {
			puts "Error: More than one XSA found in [pwd]. Use -xsa to pass a Valid XSA file"
			return ""
		}
	}
	
	puts "Info: Using XSA file: $xsa"
	puts "Info: SDT output directory is set to $sdt_out"
	sdtgen set_dt_param -dir $sdt_out -xsa $xsa
	
	set board [get_board -xsa $xsa]
	if {$board == "vck190"} {
		set sdt_board_dtsi "versal-vck190-rev1.1"
	}
	
	if {$sdt_repo != "NULL"} {
		puts "Info: Using Custom SDT repo: $sdt_repo"
		sdtgen set_dt_param -dir $sdt_out -repo $sdt_repo
	}

	if {$sdt_board_dtsi != "NULL"} {
		puts "Info: Using Board DTSI: $sdt_board_dtsi"
		sdtgen set_dt_param -dir $sdt_out -board_dts $sdt_board_dtsi
	}

	if {$sdt_user_dtsi != "NULL"} {
		puts "Info: Using Board DTSI: $sdt_user_dtsi"
		sdtgen set_dt_param -dir $sdt_out -include_dts $sdt_user_dtsi
	}

	if {$sdt_dt_overlay == 1} {
		puts "Info: Using SDT DT Overlay"
		sdtgen set_dt_param -dir $sdt_out -dt_overlay $sdt_dt_overlay
	}

	if {$sdt_dt_zocl == 1} {
		puts "Info: Using SDT zocl"
		sdtgen set_dt_param -dir $sdt_out -zocl "enable"
	}
	sdtgen generate_sdt
}

proc get_board {args} {
	set xsa "NULL"
	for {set i 0} {$i < [llength $args]} {incr i} {
		if {[lindex $args $i] == "-xsa"} {
			set xsa [lindex $args [expr {$i + 1}]]
			if {[file extension $xsa] != ".xsa" && $xsa != "NULL"} {
				puts "Error: $xsa is not a valid XSA file"
				set xsa "NULL" 
			}
		}
	}
	hsi::open_hw_design $xsa
	set xsa_board [common::get_property BOARD [hsi::current_hw_design]]
	set xsa_board [lindex [split $xsa_board ":"] 1]
	if {$xsa_board != ""} {
		return [lindex [split $xsa_board "_"] 0]
	} else {
		return 0
	}
	hsi::close_hw_design [hsi::current_hw_design]
}