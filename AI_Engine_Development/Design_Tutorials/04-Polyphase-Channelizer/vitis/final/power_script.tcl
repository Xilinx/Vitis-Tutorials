#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

open_run impl_1

## Add parameters below for power estimation based on desire, otherwise use default. Example commands shown, use GUI for full list.

# set_operating_conditions -process typical
# set append_eval__ vcco_500
# set append_eval__ 3.135
# list vcco_500 3.135
# set_operating_conditions -voltage {vcco_500 3.135}
# set append_eval__ vcco_501
# set append_eval__ 3.135
# list vcco_501 3.135
# set_operating_conditions -voltage {vcco_501 3.135}
# set append_eval__ vcco_502
# set append_eval__ 3.135
# list vcco_502 3.135
# set_operating_conditions -voltage {vcco_502 3.135}

report_power -hierarchical_depth 3 -name GUI -xpe system_power.xpe
