# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set s2mm_intf [get_objects -r $scope_path/s2mm_1/m_axi_gmem -filter {type==proto_inst}]
add_wave $s2mm_intf

set s2mm_intf [get_objects -r $scope_path/s2mm_1/s -filter {type==proto_inst}]
add_wave $s2mm_intf

set ddr4_intf [get_objects -r $scope_path/noc_ddr4/S00_AXI -filter {type==proto_inst}]
add_wave $ddr4_intf

set aie_intf [get_objects -r $scope_path/ai_engine_0/M00_AXIS -filter {type==proto_inst}]
add_wave $aie_intf