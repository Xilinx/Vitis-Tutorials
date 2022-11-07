
# Current Working Directory...
puts ""
puts "Current Working Directory..."
set currDir [pwd]
puts $currDir
puts ""

## Disabling OOC...
#puts "Disabling OOC..."
#set_property synth_checkpoint_mode None [get_files ./prj/prj.srcs/sources_1/bd/vitis_design/vitis_design.bd]

# Setting CDC Properties...
set cdc_list [get_bd_cells -hierarchical "*cdc*"]
puts ""
puts "CDC List..."
puts $cdc_list
foreach i $cdc_list {
   puts "Setting CONFIG.IS_ACLK_ASYNC and CONFIG.ACLK_RATIO property for CDC $i..."
   set_property -dict [list CONFIG.IS_ACLK_ASYNC {1} CONFIG.ACLK_RATIO {1:2}] [get_bd_cells $i]
   set cdc_async_clk_prop [get_property CONFIG.IS_ACLK_ASYNC [get_bd_cells $i]]
   set cdc_async_clk_ratio [get_property CONFIG.ACLK_RATIO [get_bd_cells $i]]
   puts "cdc_async_clk_prop: $cdc_async_clk_prop"
   puts "cdc_async_clk_ratio: $cdc_async_clk_ratio"
   puts ""
}

puts "Set required properties for all CDCs..."
puts ""
