

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

catch {unset ys}
foreach cr [get_clock_regions {X*Y1 X*Y2 X*Y3}] {
  set dsp [get_sites -quiet -filter NAME=~DSP_X*Y* -of $cr]
  if {$dsp == {}} { continue }
  lappend ys [lindex [lsort -integer -increasing [regsub -all {DSP_X\d+Y(\d+)} $dsp {\1}]] end]
}
foreach y $ys {
  set_property PROHIBIT TRUE [get_sites DSP*_X*Y$y]
}
