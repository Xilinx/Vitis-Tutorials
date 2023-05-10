

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

catch {unset ys}
foreach cr [get_clock_regions {X*Y1 X*Y2 X*Y3}] {
  set bram [get_sites -quiet -filter NAME=~RAMB36_X*Y* -of $cr]
  if {$bram == {}} { continue }
  lappend ys [lindex [lsort -integer -increasing [regsub -all {RAMB36_X\d+Y(\d+)} $bram {\1}]] end]
}
foreach y $ys {
  set_property PROHIBIT TRUE [get_sites RAMB36_X*Y$y]
  set_property PROHIBIT TRUE [get_sites RAMB18_X*Y[expr 2 * $y]]
  set_property PROHIBIT TRUE [get_sites RAMB18_X*Y[expr 2 * $y + 1]]
}
