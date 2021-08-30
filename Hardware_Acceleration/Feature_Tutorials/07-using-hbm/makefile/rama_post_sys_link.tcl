puts "########################"
puts "########################"
puts "######################## have used ./rama_post_sys_link.tcl #####################"
puts "########################"
puts "########################"

#set ::env(HMSS_RA_PORT_LIST) [get_bd_intf_pins -of [get_bd_cells hbt_kernel*] -filter {mode=="Master"}]
# the above would get both :
# m_axi_gmem 
# __and__ 
# m_axi_plr
# like so : 
# /hbt_kernel_0/m_axi_gmem /hbt_kernel_0/m_axi_plr /hbt_kernel_1/m_axi_gmem /hbt_kernel_1/m_axi_plr ......
#

# 2018.2/.3 option also compatible with 2019.1
set ::env(HMSS_RA_PORT_LIST) [get_bd_intf_pins vadd*/m_axi_gmem*]

# 2019.1 option
# hbm_memory_subsystem::ra_master_interface [get_bd_intf_pins vadd*/m_axi_gmem*] [get_bd_cells hbm_memory_subsystem_0]


validate_bd_design -force
