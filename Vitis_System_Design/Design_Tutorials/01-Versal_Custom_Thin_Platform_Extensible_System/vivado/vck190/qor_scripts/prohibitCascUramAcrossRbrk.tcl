#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

catch {unset ys}
foreach cr [get_clock_regions {X*Y1 X*Y2 X*Y3}] {
  set uram [get_sites -quiet -filter NAME=~URAM288_X*Y* -of $cr]
  if {$uram == {}} { continue }
  lappend ys [lindex [lsort -integer -increasing [regsub -all {URAM288_X\d+Y(\d+)} $uram {\1}]] end]
}
foreach y $ys {
  set_property PROHIBIT TRUE [get_sites URAM288_X*Y$y]
}
