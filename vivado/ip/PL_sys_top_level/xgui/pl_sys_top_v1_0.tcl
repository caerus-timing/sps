# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "DATA_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "DEFAULT_HALF_PERIOD" -parent ${Page_0}
  ipgui::add_param $IPINST -name "MEM_DEPTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "MEM_INIT_FILE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "N_ADDR_BITS" -parent ${Page_0}


}

proc update_PARAM_VALUE.DATA_WIDTH { PARAM_VALUE.DATA_WIDTH } {
	# Procedure called to update DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DATA_WIDTH { PARAM_VALUE.DATA_WIDTH } {
	# Procedure called to validate DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.DEFAULT_HALF_PERIOD { PARAM_VALUE.DEFAULT_HALF_PERIOD } {
	# Procedure called to update DEFAULT_HALF_PERIOD when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEFAULT_HALF_PERIOD { PARAM_VALUE.DEFAULT_HALF_PERIOD } {
	# Procedure called to validate DEFAULT_HALF_PERIOD
	return true
}

proc update_PARAM_VALUE.MEM_DEPTH { PARAM_VALUE.MEM_DEPTH } {
	# Procedure called to update MEM_DEPTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MEM_DEPTH { PARAM_VALUE.MEM_DEPTH } {
	# Procedure called to validate MEM_DEPTH
	return true
}

proc update_PARAM_VALUE.MEM_INIT_FILE { PARAM_VALUE.MEM_INIT_FILE } {
	# Procedure called to update MEM_INIT_FILE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MEM_INIT_FILE { PARAM_VALUE.MEM_INIT_FILE } {
	# Procedure called to validate MEM_INIT_FILE
	return true
}

proc update_PARAM_VALUE.N_ADDR_BITS { PARAM_VALUE.N_ADDR_BITS } {
	# Procedure called to update N_ADDR_BITS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.N_ADDR_BITS { PARAM_VALUE.N_ADDR_BITS } {
	# Procedure called to validate N_ADDR_BITS
	return true
}


proc update_MODELPARAM_VALUE.N_ADDR_BITS { MODELPARAM_VALUE.N_ADDR_BITS PARAM_VALUE.N_ADDR_BITS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.N_ADDR_BITS}] ${MODELPARAM_VALUE.N_ADDR_BITS}
}

proc update_MODELPARAM_VALUE.DATA_WIDTH { MODELPARAM_VALUE.DATA_WIDTH PARAM_VALUE.DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DATA_WIDTH}] ${MODELPARAM_VALUE.DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.MEM_DEPTH { MODELPARAM_VALUE.MEM_DEPTH PARAM_VALUE.MEM_DEPTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MEM_DEPTH}] ${MODELPARAM_VALUE.MEM_DEPTH}
}

proc update_MODELPARAM_VALUE.MEM_INIT_FILE { MODELPARAM_VALUE.MEM_INIT_FILE PARAM_VALUE.MEM_INIT_FILE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MEM_INIT_FILE}] ${MODELPARAM_VALUE.MEM_INIT_FILE}
}

proc update_MODELPARAM_VALUE.DEFAULT_HALF_PERIOD { MODELPARAM_VALUE.DEFAULT_HALF_PERIOD PARAM_VALUE.DEFAULT_HALF_PERIOD } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEFAULT_HALF_PERIOD}] ${MODELPARAM_VALUE.DEFAULT_HALF_PERIOD}
}

