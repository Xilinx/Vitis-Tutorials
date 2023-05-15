//
//# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
//# SPDX-License-Identifier: X11
//
//

// SystemVerilog AXI stream interface definition
interface axis_if();
    logic               tvalid;
    logic               tready;
    logic   [127:0]     tdata;
    modport MASTER  (output     tvalid,
                     input      tready,
                     output     tdata);

    modport SLAVE   (input      tvalid,
                     output     tready,
                     input      tdata);
endinterface

