# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11

# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  ipgui::add_page $IPINST -name "Page 0"


}


proc update_MODELPARAM_VALUE.C_S_AXI_CONTROL_ADDR_WIDTH { MODELPARAM_VALUE.C_S_AXI_CONTROL_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	# WARNING: There is no corresponding user parameter named "C_S_AXI_CONTROL_ADDR_WIDTH". Setting updated value from the model parameter.
set_property value 12 ${MODELPARAM_VALUE.C_S_AXI_CONTROL_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S_AXI_CONTROL_DATA_WIDTH { MODELPARAM_VALUE.C_S_AXI_CONTROL_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	# WARNING: There is no corresponding user parameter named "C_S_AXI_CONTROL_DATA_WIDTH". Setting updated value from the model parameter.
set_property value 32 ${MODELPARAM_VALUE.C_S_AXI_CONTROL_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_FONTREAD_AXI_M_ADDR_WIDTH { MODELPARAM_VALUE.C_FONTREAD_AXI_M_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	# WARNING: There is no corresponding user parameter named "C_FONTREAD_AXI_M_ADDR_WIDTH". Setting updated value from the model parameter.
set_property value 64 ${MODELPARAM_VALUE.C_FONTREAD_AXI_M_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.C_FONTREAD_AXI_M_DATA_WIDTH { MODELPARAM_VALUE.C_FONTREAD_AXI_M_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	# WARNING: There is no corresponding user parameter named "C_FONTREAD_AXI_M_DATA_WIDTH". Setting updated value from the model parameter.
set_property value 32 ${MODELPARAM_VALUE.C_FONTREAD_AXI_M_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_DATAOUT_AXIS_M_TDATA_WIDTH { MODELPARAM_VALUE.C_DATAOUT_AXIS_M_TDATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	# WARNING: There is no corresponding user parameter named "C_DATAOUT_AXIS_M_TDATA_WIDTH". Setting updated value from the model parameter.
set_property value 64 ${MODELPARAM_VALUE.C_DATAOUT_AXIS_M_TDATA_WIDTH}
}

