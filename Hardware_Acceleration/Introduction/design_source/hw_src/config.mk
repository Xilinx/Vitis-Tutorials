# Options for TARGET: sw_emu, hw_emu and hw
TARGET ?= hw
# If other some specific platforms needs to be used, provide platform path directly
PLATFORM ?= xilinx_50_gen3x16_xdma_3_1_201920_3
# If your platform is not in the standard install area edit this line
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms/
