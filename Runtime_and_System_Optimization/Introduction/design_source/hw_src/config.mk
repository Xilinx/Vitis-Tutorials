# Options for TARGET: sw_emu, hw_emu and hw
TARGET ?= hw
# Options for DEVICE: u200, u250. Default platform is XDMA, defined by PLATFORM
DEVICE ?= u250
# If other some specific platforms needs to be used, provide platform path directly
PLATFORM ?= xilinx_$(DEVICE)_xdma_201830_2
# If your platform is not in the standard install area edit this line
PLATFORM_REPO_PATHS ?= /opt/xilinx/platforms/
