# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set pl_intf [get_objects -r $scope_path/CIPS_0/*M_AXI*FPD*tlm -filter {type==proto_inst}]
add_wave $pl_intf

set pl_intf [get_objects -r $scope_path/random_noise_1/* -filter {type==proto_inst}]
add_wave $pl_intf

set cips_intf [get_objects -r $scope_path/ai_engine_0/*S00*tlm -filter {type==proto_inst}]
add_wave $cips_intf
