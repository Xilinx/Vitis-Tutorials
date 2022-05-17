# Reference clock contraint for GTY
create_clock  -name gt_refclk1_in -period 6.206	 [get_ports gt_refclk_p]
set_clock_groups -asynchronous -group [get_clocks gt_refclk1_in -include_generated_clocks]
