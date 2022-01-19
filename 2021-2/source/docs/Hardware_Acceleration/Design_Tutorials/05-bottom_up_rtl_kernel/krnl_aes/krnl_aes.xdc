set_false_path -to [get_pins rst_sync1_reg/CLR]
set_false_path -to [get_pins rst_sync2_reg/CLR]
set_false_path -to [get_pins rst_sync3_reg/CLR]

set_false_path -through krnl_aes_axi_ctrl_slave_inst/mode
set_false_path -through krnl_aes_axi_ctrl_slave_inst/key_len*
set_false_path -through krnl_aes_axi_ctrl_slave_inst/status
set_false_path -through krnl_aes_axi_ctrl_slave_inst/key_w*

