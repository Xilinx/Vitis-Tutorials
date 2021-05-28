# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set pl_intf [get_objects -r $scope_path/CIPS_0/*FPD*CCI*NOC_0* -filter {type==proto_inst}]
add_wave $pl_intf

set pl_intf [get_objects -r $scope_path/cips_noc/*S00_AXI*tlm -filter {type==proto_inst}]
add_wave $pl_intf

set pl_intf [get_objects -r $scope_path/cips_noc/*M00_AXI*tlm -filter {type==proto_inst}]
add_wave $pl_intf

set aie_intf [get_objects -r $scope_path/ai_engine_0/*S00*tlm -filter {type==proto_inst}]
add_wave $aie_intf
