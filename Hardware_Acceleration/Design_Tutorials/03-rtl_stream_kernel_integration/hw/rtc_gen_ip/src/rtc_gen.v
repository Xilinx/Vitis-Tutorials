//
//# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
//# SPDX-License-Identifier: X11
//

// default_nettype of none prevents implicit wire declaration.

`default_nettype none
`timescale 1 ns / 1 ps

// Top level of the kernel. Do not modify module name, parameters or ports.
module rtc_gen #(
  parameter integer C_S_AXI_CONTROL_ADDR_WIDTH   = 12,
  parameter integer C_S_AXI_CONTROL_DATA_WIDTH   = 32,
  parameter integer C_FONTREAD_AXI_M_ADDR_WIDTH  = 64,
  parameter integer C_FONTREAD_AXI_M_DATA_WIDTH  = 32,
  parameter integer C_DATAOUT_AXIS_M_TDATA_WIDTH = 64
)
(
  // System Signals
  input  wire                                      ap_clk                ,
  input  wire                                      ap_rst_n              ,
  //  Note: A minimum subset of AXI4 memory mapped signals are declared.  AXI
  // signals omitted from these interfaces are automatically inferred with the
  // optimal values for Xilinx accleration platforms.  This allows Xilinx AXI4 Interconnects
  // within the system to be optimized by removing logic for AXI4 protocol
  // features that are not necessary. When adapting AXI4 masters within the RTL
  // kernel that have signals not declared below, it is suitable to add the
  // signals to the declarations below to connect them to the AXI4 Master.
  // 
  // List of ommited signals - effect
  // -------------------------------
  // ID - Transaction ID are used for multithreading and out of order
  // transactions.  This increases complexity. This saves logic and increases Fmax
  // in the system when ommited.
  // SIZE - Default value is log2(data width in bytes). Needed for subsize bursts.
  // This saves logic and increases Fmax in the system when ommited.
  // BURST - Default value (0b01) is incremental.  Wrap and fixed bursts are not
  // recommended. This saves logic and increases Fmax in the system when ommited.
  // LOCK - Not supported in AXI4
  // CACHE - Default value (0b0011) allows modifiable transactions. No benefit to
  // changing this.
  // PROT - Has no effect in current acceleration platforms.
  // QOS - Has no effect in current acceleration platforms.
  // REGION - Has no effect in current acceleration platforms.
  // USER - Has no effect in current acceleration platforms.
  // RESP - Not useful in most acceleration platforms.
  // 
  // AXI4 master interface fontread_axi_m
  output wire                                      fontread_axi_m_awvalid,
  input  wire                                      fontread_axi_m_awready,
  output wire [C_FONTREAD_AXI_M_ADDR_WIDTH-1:0]    fontread_axi_m_awaddr ,
  output wire [8-1:0]                              fontread_axi_m_awlen  ,
  output wire                                      fontread_axi_m_wvalid ,
  input  wire                                      fontread_axi_m_wready ,
  output wire [C_FONTREAD_AXI_M_DATA_WIDTH-1:0]    fontread_axi_m_wdata  ,
  output wire [C_FONTREAD_AXI_M_DATA_WIDTH/8-1:0]  fontread_axi_m_wstrb  ,
  output wire                                      fontread_axi_m_wlast  ,
  input  wire                                      fontread_axi_m_bvalid ,
  output wire                                      fontread_axi_m_bready ,
  output wire                                      fontread_axi_m_arvalid,
  input  wire                                      fontread_axi_m_arready,
  output wire [C_FONTREAD_AXI_M_ADDR_WIDTH-1:0]    fontread_axi_m_araddr ,
  output wire [8-1:0]                              fontread_axi_m_arlen  ,
  input  wire                                      fontread_axi_m_rvalid ,
  output wire                                      fontread_axi_m_rready ,
  input  wire [C_FONTREAD_AXI_M_DATA_WIDTH-1:0]    fontread_axi_m_rdata  ,
  input  wire                                      fontread_axi_m_rlast  ,
  // AXI4-Stream (master) interface dataout_axis_m
  output wire                                      dataout_axis_m_tvalid ,
  input  wire                                      dataout_axis_m_tready ,
  output wire [C_DATAOUT_AXIS_M_TDATA_WIDTH-1:0]   dataout_axis_m_tdata  ,
  output wire [C_DATAOUT_AXIS_M_TDATA_WIDTH/8-1:0] dataout_axis_m_tkeep  ,
  output wire                                      dataout_axis_m_tlast  ,
  // AXI4-Lite slave interface
  input  wire                                      s_axi_control_awvalid ,
  output wire                                      s_axi_control_awready ,
  input  wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]     s_axi_control_awaddr  ,
  input  wire                                      s_axi_control_wvalid  ,
  output wire                                      s_axi_control_wready  ,
  input  wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0]     s_axi_control_wdata   ,
  input  wire [C_S_AXI_CONTROL_DATA_WIDTH/8-1:0]   s_axi_control_wstrb   ,
  input  wire                                      s_axi_control_arvalid ,
  output wire                                      s_axi_control_arready ,
  input  wire [C_S_AXI_CONTROL_ADDR_WIDTH-1:0]     s_axi_control_araddr  ,
  output wire                                      s_axi_control_rvalid  ,
  input  wire                                      s_axi_control_rready  ,
  output wire [C_S_AXI_CONTROL_DATA_WIDTH-1:0]     s_axi_control_rdata   ,
  output wire [2-1:0]                              s_axi_control_rresp   ,
  output wire                                      s_axi_control_bvalid  ,
  input  wire                                      s_axi_control_bready  ,
  output wire [2-1:0]                              s_axi_control_bresp   ,
  output wire                                      interrupt             
);

///////////////////////////////////////////////////////////////////////////////
// Local Parameters
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Wires and Variables
///////////////////////////////////////////////////////////////////////////////
  (* DONT_TOUCH = "yes" *)
  reg                                 areset                         = 1'b0;
  wire                                ap_start                      ;
  wire                                ap_idle                       ;
  wire                                ap_done                       ;
  wire                                ap_ready                      ;
  
  wire [31:0]                         work_mode                     ;
  wire [31:0]                         cs_count                      ;
  wire [31:0]                         time_format                   ;
  wire [31:0]                         time_set_val                  ;
  wire [31:0]                         time_set_en                   ;

  wire        ctrl_start;
  wire        ctrl_done;
  wire [63:0] ctrl_addr_offset;
  wire [31:0] ctrl_xfer_size_in_bytes; 

  wire        rd_tready;
  wire        rd_tvalid;
  wire [31:0] rd_tdata;
  
  wire [31:0] time_val;

// Register and invert reset signal.
always @(posedge ap_clk) begin
  areset <= ~ap_rst_n;
end






///////////////////////////////////////////////////////////////////////////////
// Begin control interface RTL.
///////////////////////////////////////////////////////////////////////////////

// unused signals tie-off
  assign fontread_axi_m_awvalid = 1'b0;
  assign fontread_axi_m_awaddr = 'b0  ;
  assign fontread_axi_m_awlen = 'b0   ;
  assign fontread_axi_m_wvalid = 'b0  ;
  assign fontread_axi_m_wdata = 'b0   ;
  assign fontread_axi_m_wstrb = 'b0   ;
  assign fontread_axi_m_wlast = 'b0   ;
  assign fontread_axi_m_bready = 'b1  ;  

// AXI read master
rtc_gen_example_axi_read_master #(
  .C_M_AXI_ADDR_WIDTH  ( C_FONTREAD_AXI_M_ADDR_WIDTH ) ,
  .C_M_AXI_DATA_WIDTH  ( C_FONTREAD_AXI_M_DATA_WIDTH ) ,
  .C_XFER_SIZE_WIDTH   ( 32                    ) ,
  .C_MAX_OUTSTANDING   ( 2                     ) ,
  .C_INCLUDE_DATA_FIFO ( 1                     )
)
inst_axi_read_master (
  .aclk                    ( ap_clk                  ) ,
  .areset                  ( areset                  ) ,
  .ctrl_start              ( ctrl_start              ) ,
  .ctrl_done               ( ctrl_done               ) ,
  .ctrl_addr_offset        ( ctrl_addr_offset        ) ,
  .ctrl_xfer_size_in_bytes ( ctrl_xfer_size_in_bytes ) ,
  .m_axi_arvalid           ( fontread_axi_m_arvalid  ) ,
  .m_axi_arready           ( fontread_axi_m_arready  ) ,
  .m_axi_araddr            ( fontread_axi_m_araddr   ) ,
  .m_axi_arlen             ( fontread_axi_m_arlen    ) ,
  .m_axi_rvalid            ( fontread_axi_m_rvalid   ) ,
  .m_axi_rready            ( fontread_axi_m_rready   ) ,
  .m_axi_rdata             ( fontread_axi_m_rdata    ) ,
  .m_axi_rlast             ( fontread_axi_m_rlast    ) ,
  .m_axis_aclk             ( ap_clk                  ) ,
  .m_axis_areset           ( areset                  ) ,
  .m_axis_tvalid           ( rd_tvalid               ) ,
  .m_axis_tready           ( rd_tready               ) ,
  .m_axis_tlast            (                         ) ,        // don't use
  .m_axis_tdata            ( rd_tdata                )
);


// AXI4-Lite slave interface
rtc_gen_control_s_axi #(
  .C_S_AXI_ADDR_WIDTH ( C_S_AXI_CONTROL_ADDR_WIDTH ),
  .C_S_AXI_DATA_WIDTH ( C_S_AXI_CONTROL_DATA_WIDTH )
)
inst_control_s_axi (
  .ACLK         ( ap_clk                ),
  .ARESET       ( areset                ),
  .ACLK_EN      ( 1'b1                  ),
  .AWVALID      ( s_axi_control_awvalid ),
  .AWREADY      ( s_axi_control_awready ),
  .AWADDR       ( s_axi_control_awaddr  ),
  .WVALID       ( s_axi_control_wvalid  ),
  .WREADY       ( s_axi_control_wready  ),
  .WDATA        ( s_axi_control_wdata   ),
  .WSTRB        ( s_axi_control_wstrb   ),
  .ARVALID      ( s_axi_control_arvalid ),
  .ARREADY      ( s_axi_control_arready ),
  .ARADDR       ( s_axi_control_araddr  ),
  .RVALID       ( s_axi_control_rvalid  ),
  .RREADY       ( s_axi_control_rready  ),
  .RDATA        ( s_axi_control_rdata   ),
  .RRESP        ( s_axi_control_rresp   ),
  .BVALID       ( s_axi_control_bvalid  ),
  .BREADY       ( s_axi_control_bready  ),
  .BRESP        ( s_axi_control_bresp   ),
  .interrupt    ( interrupt             ),
  .ap_start     ( ap_start              ),
  .ap_done      ( ap_done               ),
  .ap_ready     ( ap_ready              ),
  .ap_idle      ( ap_idle               ),
  .work_mode    ( work_mode             ),
  .cs_count     ( cs_count              ),
  .time_format  ( time_format           ),
  .time_set_val ( time_set_val          ),
  .time_set_en  ( time_set_en           ),
  .time_val     ( time_val              ),
  .read_addr    ( ctrl_addr_offset      )
);

assign ap_ready = ap_done;

///////////////////////////////////////////////////////////////////////////////
// Add kernel logic here.  Modify/remove example code as necessary.
///////////////////////////////////////////////////////////////////////////////

// RTC generation core logic
  rtc_gen_core inst_rtc_gen_core (
  .clk            (ap_clk),
  .reset_n        (ap_rst_n),
// control_s signals  
  .work_mode      (work_mode),
  .cs_count       (cs_count),
  .time_format    (time_format),  
  .time_set_val   (time_set_val),
  .time_set_en    (time_set_en),
  .ap_start       (ap_start),
  .ap_done        (ap_done),
  .ap_idle        (ap_idle),
// write master signals  
  .ctrl_start     (ctrl_start),
  .ctrl_done      (ctrl_done),
  .ctrl_xfer_size_in_bytes  (ctrl_xfer_size_in_bytes),
  .mdata_tvalid   (rd_tvalid),
  .mdata_tready   (rd_tready),
  .mdata_tdata    (rd_tdata),  
// AXI Stream signals
  .axis_m_tvalid  (dataout_axis_m_tvalid),
  .axis_m_tready  (dataout_axis_m_tready),
  .axis_m_tdata   (dataout_axis_m_tdata),
  .axis_m_tkeep   (dataout_axis_m_tkeep),
  .axis_m_tlast   (dataout_axis_m_tlast),
// Time value output
  .time_value     (time_val)
);




endmodule
`default_nettype wire
