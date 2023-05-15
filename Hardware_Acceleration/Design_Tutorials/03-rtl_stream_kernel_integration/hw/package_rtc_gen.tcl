# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11

# package RTL kernel IP to XO file
package_xo  -xo_path ./rtc_gen.xo \
            -kernel_name rtc_gen \
            -ip_directory ./rtc_gen_ip \
            -kernel_xml ./rtc_gen_kernel.xml

