#******************************************************************************
# Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#******
reset_run synth_1
reset_run impl_1
launch_runs synth_1 -jobs 48
wait_on_run synth_1
launch_runs impl_1 -to_step write_device_image -jobs 48
wait_on_run impl_1