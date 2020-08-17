# This is a generated file. Use and modify at your own risk.
################################################################################

# Usage: Custom kernel level constraints can be added to this file.


# Example multicycle constraint on reset to inst_example:
# Multicycle paths can be used on the reset register to help with timing closure on designs with
# high reset fanouts.  To ensure the reset register is not optimized out, add a DONT_TOUCH = "yes"
# or KEEP = "yes" attribute to the register inside the RTL code.
# set_multicycle_path -setup 3 -hold 2 [get_cells inst_example/areset_reg]
