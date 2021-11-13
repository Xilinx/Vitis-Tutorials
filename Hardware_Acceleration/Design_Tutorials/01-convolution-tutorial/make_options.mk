############## Default Vitis Build Target : Select only ONE  #
TARGET ?=hw
PLATFORM ?= xilinx_u200_gen3x16_xdma_1_202110_1
ENABLE_STALL_TRACE ?= yes
TRACE_DDR=DDR[3]

############## Host Application Options
FILTER_TYPE :=3
PARALLEL_ENQ_REQS :=6
NUM_IMAGES :=1000
INPUT_TYPE :=random
INPUT_IMAGE :=../test_images/xilinx_versal_1080p.bmp
ENABLE_PROF?=yes
PROFILE_ALL_IMAGES?=no
IMAGE_WIDTH :=1920
IMAGE_HEIGHT :=1080
############## Extended Options  host options
NUM_IMAGES_SW_EMU := 1
NUM_IMAGES_HW_EMU := 1

############# pre-built xclbin path and options
USE_PRE_BUILT_XCLBIN := 0
PRE_BUILT_XCLBIN_PATH :=../xclbin/fpgabinary.hw.xclbin

############## OpenCV Installation Paths
#OPENCV_INCLUDE :=/wrk/xsjhdnobkup3/shahzadb/d_apps/softwareInstall/anacondaInstall5aug20/envs/opencv2.4/include
#OPENCV_LIB :=/wrk/xsjhdnobkup3/shahzadb/d_apps/softwareInstall/anacondaInstall5aug20/envs/opencv2.4/lib
OPENCV_INCLUDE :=/usr/include/opencv2
OPENCV_LIB :=/usr/lib64

############## Kernel Configuration File
KERNEL_CONFIG_FILE :=krnl_build_options.cfg

############## Different log  dirs
VPP_TEMP_DIRS :=vpp_temp_dir
VPP_LOG_DIRS :=vpp_log_dir

