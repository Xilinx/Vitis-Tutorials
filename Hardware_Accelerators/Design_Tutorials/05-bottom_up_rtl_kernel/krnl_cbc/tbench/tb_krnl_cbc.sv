//
// Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

`timescale 1ns/1ps

import axi_vip_pkg::*;
import axi_vip_mst_pkg::*;
import axi_vip_slv_pkg::*;

module tb_krnl_cbc ();

// Kernel register address map
// Common Control Register 
parameter KRNL_CTRL_REG_ADDR     = 32'h00000000;
parameter CTRL_START_MASK        = 32'h00000001;
parameter CTRL_DONE_MASK         = 32'h00000002;
parameter CTRL_IDLE_MASK         = 32'h00000004;
parameter CTRL_READY_MASK        = 32'h00000008;
parameter CTRL_CONTINUE_MASK     = 32'h00000010; 
// krnl_cbc argument
parameter CBC_MODE        = 32'h0000_0010; 
parameter CBC_IV_W3       = 32'h0000_0018; 
parameter CBC_IV_W2       = 32'h0000_0020; 
parameter CBC_IV_W1       = 32'h0000_0028; 
parameter CBC_IV_W0       = 32'h0000_0030; 
parameter CBC_WORDS_NUM   = 32'h0000_0038; 
parameter CBC_SRC_ADDR_0  = 32'h0000_0040; 
parameter CBC_SRC_ADDR_1  = 32'h0000_0044; 
parameter CBC_DEST_ADDR_0 = 32'h0000_0048; 
parameter CBC_DEST_ADDR_1 = 32'h0000_004C; 
parameter CBC_CBC_MODE    = 32'h0000_0050; 
// krnl_aes argument
parameter AES_MODE        = 32'h0000_0010; 
parameter AES_KEY_LEN     = 32'h0000_0018; 
parameter AES_STATUS      = 32'h0000_0020; 
parameter AES_KEY_W7      = 32'h0000_0028; 
parameter AES_KEY_W6      = 32'h0000_0030; 
parameter AES_KEY_W5      = 32'h0000_0038; 
parameter AES_KEY_W4      = 32'h0000_0040; 
parameter AES_KEY_W3      = 32'h0000_0048; 
parameter AES_KEY_W2      = 32'h0000_0050; 
parameter AES_KEY_W1      = 32'h0000_0058; 
parameter AES_KEY_W0      = 32'h0000_0060; 

// input/output buffer base address definition
parameter IN_BUFFER_BASE  = 64'h0000_0040_0000_0000;
parameter OUT_BUFFER_BASE = 64'h0000_0040_1000_0000;


// clock frequency definition
parameter real CLK_PERIOD = 3.333; // 300MHz

//System Signals
logic ap_clk = 0;

initial begin: AP_CLK
  forever begin
    ap_clk = #(CLK_PERIOD/2) ~ap_clk;
  end
end
 
//System Signals
logic ap_rst_n = 0;

task automatic ap_rst_n_sequence(input integer unsigned width = 20);
  @(posedge ap_clk);
  #1ns;
  ap_rst_n = 0;
  repeat (width) @(posedge ap_clk);
  #1ns;
  ap_rst_n = 1;
endtask

initial begin: AP_RST
  ap_rst_n_sequence(50);
end


// connnection signal declare
  wire [31:0]   m_axi_aes_awaddr   ;   
  wire          m_axi_aes_awvalid  ;
  wire          m_axi_aes_awready  ;
  wire [31:0]   m_axi_aes_wdata    ;
  wire [3:0]    m_axi_aes_wstrb    ;
  wire          m_axi_aes_wvalid   ;
  wire          m_axi_aes_wready   ;
  wire [1:0]    m_axi_aes_bresp    ;
  wire          m_axi_aes_bvalid   ;
  wire          m_axi_aes_bready   ;
  wire [31:0]   m_axi_aes_araddr   ;
  wire          m_axi_aes_arvalid  ;
  wire          m_axi_aes_arready  ;
  wire [31:0]   m_axi_aes_rdata    ;
  wire [1:0]    m_axi_aes_rresp    ;
  wire          m_axi_aes_rvalid   ;
  wire          m_axi_aes_rready   ;

  wire [31:0]   m_axi_cbc_awaddr   ;   
  wire          m_axi_cbc_awvalid  ;
  wire          m_axi_cbc_awready  ;
  wire [31:0]   m_axi_cbc_wdata    ;
  wire [3:0]    m_axi_cbc_wstrb    ;
  wire          m_axi_cbc_wvalid   ;
  wire          m_axi_cbc_wready   ;
  wire [1:0]    m_axi_cbc_bresp    ;
  wire          m_axi_cbc_bvalid   ;
  wire          m_axi_cbc_bready   ;
  wire [31:0]   m_axi_cbc_araddr   ;
  wire          m_axi_cbc_arvalid  ;
  wire          m_axi_cbc_arready  ;
  wire [31:0]   m_axi_cbc_rdata    ;
  wire [1:0]    m_axi_cbc_rresp    ;
  wire          m_axi_cbc_rvalid   ;
  wire          m_axi_cbc_rready   ;

  wire [63:0]   s_axi_in_awaddr    ; 
  wire [7:0]    s_axi_in_awlen     ; 
  wire [1:0]    s_axi_in_awburst   ; 
  wire          s_axi_in_awvalid   ; 
  wire          s_axi_in_awready   ; 
  wire [127:0]  s_axi_in_wdata     ; 
  wire [15:0]   s_axi_in_wstrb     ; 
  wire          s_axi_in_wlast     ; 
  wire          s_axi_in_wvalid    ; 
  wire          s_axi_in_wready    ; 
  wire [1:0]    s_axi_in_bresp     ; 
  wire          s_axi_in_bvalid    ; 
  wire          s_axi_in_bready    ; 
  wire [63:0]   s_axi_in_araddr    ; 
  wire [7:0]    s_axi_in_arlen     ; 
  wire [1:0]    s_axi_in_arburst   ; 
  wire          s_axi_in_arvalid   ; 
  wire          s_axi_in_arready   ; 
  wire [127:0]  s_axi_in_rdata     ; 
  wire [1:0]    s_axi_in_rresp     ; 
  wire          s_axi_in_rlast     ; 
  wire          s_axi_in_rvalid    ; 
  wire          s_axi_in_rready    ;  

  wire [63:0]   s_axi_out_awaddr    ; 
  wire [7:0]    s_axi_out_awlen     ; 
  wire [1:0]    s_axi_out_awburst   ; 
  wire          s_axi_out_awvalid   ; 
  wire          s_axi_out_awready   ; 
  wire [127:0]  s_axi_out_wdata     ; 
  wire [15:0]   s_axi_out_wstrb     ; 
  wire          s_axi_out_wlast     ; 
  wire          s_axi_out_wvalid    ; 
  wire          s_axi_out_wready    ; 
  wire [1:0]    s_axi_out_bresp     ; 
  wire          s_axi_out_bvalid    ; 
  wire          s_axi_out_bready    ; 
  wire [63:0]   s_axi_out_araddr    ; 
  wire [7:0]    s_axi_out_arlen     ; 
  wire [1:0]    s_axi_out_arburst   ; 
  wire          s_axi_out_arvalid   ; 
  wire          s_axi_out_arready   ; 
  wire [127:0]  s_axi_out_rdata     ; 
  wire [1:0]    s_axi_out_rresp     ; 
  wire          s_axi_out_rlast     ; 
  wire          s_axi_out_rvalid    ; 
  wire          s_axi_out_rready    ;  

  wire          m_axis_0_tvalid;
  wire          m_axis_0_tready;
  wire [127:0]  m_axis_0_tdata;

  wire          m_axis_1_tvalid;
  wire          m_axis_1_tready;
  wire [127:0]  m_axis_1_tdata;

  wire          m_axis_2_tvalid;
  wire          m_axis_2_tready;
  wire [127:0]  m_axis_2_tdata;

  wire          m_axis_3_tvalid;
  wire          m_axis_3_tready;
  wire [127:0]  m_axis_3_tdata;

  wire          s_axis_0_tvalid;
  wire          s_axis_0_tready;
  wire [127:0]  s_axis_0_tdata;

  wire          s_axis_1_tvalid;
  wire          s_axis_1_tready;
  wire [127:0]  s_axis_1_tdata;

  wire          s_axis_2_tvalid;
  wire          s_axis_2_tready;
  wire [127:0]  s_axis_2_tdata;

  wire          s_axis_3_tvalid;
  wire          s_axis_3_tready;
  wire [127:0]  s_axis_3_tdata;


// instantiation of axi master vip for krnl_aes
  axi_vip_mst axi_vip_mst_krnl_aes (
    .aclk           (ap_clk),           // input wire aclk
    .aresetn        (ap_rst_n),         // input wire aresetn
    .m_axi_awaddr   (m_axi_aes_awaddr),     // output wire [31 : 0] m_axi_awaddr
    .m_axi_awvalid  (m_axi_aes_awvalid),    // output wire m_axi_awvalid
    .m_axi_awready  (m_axi_aes_awready),    // input wire m_axi_awready
    .m_axi_wdata    (m_axi_aes_wdata),      // output wire [31 : 0] m_axi_wdata
    .m_axi_wstrb    (m_axi_aes_wstrb),      // output wire [3 : 0] m_axi_wstrb
    .m_axi_wvalid   (m_axi_aes_wvalid),     // output wire m_axi_wvalid
    .m_axi_wready   (m_axi_aes_wready),     // input wire m_axi_wready
    .m_axi_bresp    (m_axi_aes_bresp),      // input wire [1 : 0] m_axi_bresp
    .m_axi_bvalid   (m_axi_aes_bvalid),     // input wire m_axi_bvalid
    .m_axi_bready   (m_axi_aes_bready),     // output wire m_axi_bready
    .m_axi_araddr   (m_axi_aes_araddr),     // output wire [31 : 0] m_axi_araddr
    .m_axi_arvalid  (m_axi_aes_arvalid),    // output wire m_axi_arvalid
    .m_axi_arready  (m_axi_aes_arready),    // input wire m_axi_arready
    .m_axi_rdata    (m_axi_aes_rdata),      // input wire [31 : 0] m_axi_rdata
    .m_axi_rresp    (m_axi_aes_rresp),      // input wire [1 : 0] m_axi_rresp
    .m_axi_rvalid   (m_axi_aes_rvalid),     // input wire m_axi_rvalid
    .m_axi_rready   (m_axi_aes_rready)      // output wire m_axi_rready
);

// instantiation of axi master vip for krnl_cbc
  axi_vip_mst axi_vip_mst_krnl_cbc (
    .aclk           (ap_clk),           // input wire aclk
    .aresetn        (ap_rst_n),         // input wire aresetn
    .m_axi_awaddr   (m_axi_cbc_awaddr),     // output wire [31 : 0] m_axi_awaddr
    .m_axi_awvalid  (m_axi_cbc_awvalid),    // output wire m_axi_awvalid
    .m_axi_awready  (m_axi_cbc_awready),    // input wire m_axi_awready
    .m_axi_wdata    (m_axi_cbc_wdata),      // output wire [31 : 0] m_axi_wdata
    .m_axi_wstrb    (m_axi_cbc_wstrb),      // output wire [3 : 0] m_axi_wstrb
    .m_axi_wvalid   (m_axi_cbc_wvalid),     // output wire m_axi_wvalid
    .m_axi_wready   (m_axi_cbc_wready),     // input wire m_axi_wready
    .m_axi_bresp    (m_axi_cbc_bresp),      // input wire [1 : 0] m_axi_bresp
    .m_axi_bvalid   (m_axi_cbc_bvalid),     // input wire m_axi_bvalid
    .m_axi_bready   (m_axi_cbc_bready),     // output wire m_axi_bready
    .m_axi_araddr   (m_axi_cbc_araddr),     // output wire [31 : 0] m_axi_araddr
    .m_axi_arvalid  (m_axi_cbc_arvalid),    // output wire m_axi_arvalid
    .m_axi_arready  (m_axi_cbc_arready),    // input wire m_axi_arready
    .m_axi_rdata    (m_axi_cbc_rdata),      // input wire [31 : 0] m_axi_rdata
    .m_axi_rresp    (m_axi_cbc_rresp),      // input wire [1 : 0] m_axi_rresp
    .m_axi_rvalid   (m_axi_cbc_rvalid),     // input wire m_axi_rvalid
    .m_axi_rready   (m_axi_cbc_rready)      // output wire m_axi_rready
);


// instantiation of axi slave vip for input buffer
  axi_vip_slv axi_vip_slv_in_buf (
    .aclk           (ap_clk),              // input wire aclk
    .aresetn        (ap_rst_n),            // input wire aresetn
    .s_axi_awaddr   (s_axi_in_awaddr),     // input wire [63 : 0] s_axi_awaddr
    .s_axi_awlen    (s_axi_in_awlen),      // input wire [7 : 0] s_axi_awlen
    .s_axi_awburst  (s_axi_in_awburst),    // input wire [1 : 0] s_axi_awburst
    .s_axi_awvalid  (s_axi_in_awvalid),    // input wire s_axi_awvalid
    .s_axi_awready  (s_axi_in_awready),    // output wire s_axi_awready
    .s_axi_wdata    (s_axi_in_wdata),      // input wire [127 : 0] s_axi_wdata
    .s_axi_wstrb    (s_axi_in_wstrb),      // input wire [15 : 0] s_axi_wstrb
    .s_axi_wlast    (s_axi_in_wlast),      // input wire s_axi_wlast
    .s_axi_wvalid   (s_axi_in_wvalid),     // input wire s_axi_wvalid
    .s_axi_wready   (s_axi_in_wready),     // output wire s_axi_wready
    .s_axi_bresp    (s_axi_in_bresp),      // output wire [1 : 0] s_axi_bresp
    .s_axi_bvalid   (s_axi_in_bvalid),     // output wire s_axi_bvalid
    .s_axi_bready   (s_axi_in_bready),     // input wire s_axi_bready
    .s_axi_araddr   (s_axi_in_araddr),     // input wire [63 : 0] s_axi_araddr
    .s_axi_arlen    (s_axi_in_arlen),      // input wire [7 : 0] s_axi_arlen
    .s_axi_arburst  (s_axi_in_arburst),    // input wire [1 : 0] s_axi_arburst
    .s_axi_arvalid  (s_axi_in_arvalid),    // input wire s_axi_arvalid
    .s_axi_arready  (s_axi_in_arready),    // output wire s_axi_arready
    .s_axi_rdata    (s_axi_in_rdata),      // output wire [127 : 0] s_axi_rdata
    .s_axi_rresp    (s_axi_in_rresp),      // output wire [1 : 0] s_axi_rresp
    .s_axi_rlast    (s_axi_in_rlast),      // output wire s_axi_rlast
    .s_axi_rvalid   (s_axi_in_rvalid),     // output wire s_axi_rvalid
    .s_axi_rready   (s_axi_in_rready)      // input wire s_axi_rready
);

  assign s_axi_in_awburst = 2'b01;
  assign s_axi_in_arburst = 2'b01;

// instantiation of axi slave vip for output buffer
  axi_vip_slv axi_vip_slv_out_buf (
    .aclk           (ap_clk),              // input wire aclk
    .aresetn        (ap_rst_n),            // input wire aresetn
    .s_axi_awaddr   (s_axi_out_awaddr),    // input wire [63 : 0] s_axi_awaddr
    .s_axi_awlen    (s_axi_out_awlen),     // input wire [7 : 0] s_axi_awlen
    .s_axi_awburst  (s_axi_out_awburst),   // input wire [1 : 0] s_axi_awburst
    .s_axi_awvalid  (s_axi_out_awvalid),   // input wire s_axi_awvalid
    .s_axi_awready  (s_axi_out_awready),   // output wire s_axi_awready
    .s_axi_wdata    (s_axi_out_wdata),     // input wire [127 : 0] s_axi_wdata
    .s_axi_wstrb    (s_axi_out_wstrb),     // input wire [15 : 0] s_axi_wstrb
    .s_axi_wlast    (s_axi_out_wlast),     // input wire s_axi_wlast
    .s_axi_wvalid   (s_axi_out_wvalid),    // input wire s_axi_wvalid
    .s_axi_wready   (s_axi_out_wready),    // output wire s_axi_wready
    .s_axi_bresp    (s_axi_out_bresp),     // output wire [1 : 0] s_axi_bresp
    .s_axi_bvalid   (s_axi_out_bvalid),    // output wire s_axi_bvalid
    .s_axi_bready   (s_axi_out_bready),    // input wire s_axi_bready
    .s_axi_araddr   (s_axi_out_araddr),    // input wire [63 : 0] s_axi_araddr
    .s_axi_arlen    (s_axi_out_arlen),     // input wire [7 : 0] s_axi_arlen
    .s_axi_arburst  (s_axi_out_arburst),   // input wire [1 : 0] s_axi_arburst
    .s_axi_arvalid  (s_axi_out_arvalid),   // input wire s_axi_arvalid
    .s_axi_arready  (s_axi_out_arready),   // output wire s_axi_arready
    .s_axi_rdata    (s_axi_out_rdata),     // output wire [127 : 0] s_axi_rdata
    .s_axi_rresp    (s_axi_out_rresp),     // output wire [1 : 0] s_axi_rresp
    .s_axi_rlast    (s_axi_out_rlast),     // output wire s_axi_rlast
    .s_axi_rvalid   (s_axi_out_rvalid),    // output wire s_axi_rvalid
    .s_axi_rready   (s_axi_out_rready)     // input wire s_axi_rready
);

  assign s_axi_out_awburst = 2'b01;
  assign s_axi_out_arburst = 2'b01;


// instantiation of krnl_aes
  krnl_aes krnl_aes_inst (
  // System Signals
    .ap_clk                 (ap_clk),
    .ap_rst_n               (ap_rst_n),

  // AXI4-Stream (master) interface axis_mst0
    .axis_mst0_tvalid       (s_axis_0_tvalid),
    .axis_mst0_tready       (s_axis_0_tready),
    .axis_mst0_tdata        (s_axis_0_tdata),
  // AXI4-Stream (slave) interface axis_slv0
    .axis_slv0_tvalid       (m_axis_0_tvalid),
    .axis_slv0_tready       (m_axis_0_tready),
    .axis_slv0_tdata        (m_axis_0_tdata),
  // AXI4-Stream (master) interface axis_mst1
    .axis_mst1_tvalid       (s_axis_1_tvalid),
    .axis_mst1_tready       (s_axis_1_tready),
    .axis_mst1_tdata        (s_axis_1_tdata),
  // AXI4-Stream (slave) interface axis_slv1
    .axis_slv1_tvalid       (m_axis_1_tvalid),
    .axis_slv1_tready       (m_axis_1_tready),
    .axis_slv1_tdata        (m_axis_1_tdata),
  // AXI4-Stream (master) interface axis_mst2
    .axis_mst2_tvalid       (s_axis_2_tvalid),
    .axis_mst2_tready       (s_axis_2_tready),
    .axis_mst2_tdata        (s_axis_2_tdata),
  // AXI4-Stream (slave) interface axis_slv2
    .axis_slv2_tvalid       (m_axis_2_tvalid),
    .axis_slv2_tready       (m_axis_2_tready),
    .axis_slv2_tdata        (m_axis_2_tdata),
  // AXI4-Stream (master) interface axis_mst3
    .axis_mst3_tvalid       (s_axis_3_tvalid),
    .axis_mst3_tready       (s_axis_3_tready),
    .axis_mst3_tdata        (s_axis_3_tdata),
  // AXI4-Stream (slave) interface axis_slv3
    .axis_slv3_tvalid       (m_axis_3_tvalid),
    .axis_slv3_tready       (m_axis_3_tready),
    .axis_slv3_tdata        (m_axis_3_tdata),
  
  // AXI4-Lite slave interface
    .s_axi_control_awvalid   (m_axi_aes_awvalid),
    .s_axi_control_awready   (m_axi_aes_awready),
    .s_axi_control_awaddr    (m_axi_aes_awaddr[11:0]),
    .s_axi_control_wvalid    (m_axi_aes_wvalid),
    .s_axi_control_wready    (m_axi_aes_wready),
    .s_axi_control_wdata     (m_axi_aes_wdata),
    .s_axi_control_wstrb     (m_axi_aes_wstrb),
    .s_axi_control_arvalid   (m_axi_aes_arvalid),
    .s_axi_control_arready   (m_axi_aes_arready),
    .s_axi_control_araddr    (m_axi_aes_araddr[11:0]),
    .s_axi_control_rvalid    (m_axi_aes_rvalid),
    .s_axi_control_rready    (m_axi_aes_rready),
    .s_axi_control_rdata     (m_axi_aes_rdata),
    .s_axi_control_rresp     (m_axi_aes_rresp),
    .s_axi_control_bvalid    (m_axi_aes_bvalid),
    .s_axi_control_bready    (m_axi_aes_bready),
    .s_axi_control_bresp     (m_axi_aes_bresp)
);


// instantiation of krnl_cbc
 krnl_cbc krnl_cbc_inst (
// System Signals
    .ap_clk                 (ap_clk),
    .ap_rst_n               (ap_rst_n),

// AXI4-Stream (master) interface axis_mst0
    .axis_mst0_tvalid       (m_axis_0_tvalid  ),
    .axis_mst0_tready       (m_axis_0_tready  ),
    .axis_mst0_tdata        (m_axis_0_tdata   ),
// AXI4-Stream (slave) interface axis_slv0
    .axis_slv0_tvalid       (s_axis_0_tvalid  ),
    .axis_slv0_tready       (s_axis_0_tready  ),
    .axis_slv0_tdata        (s_axis_0_tdata   ),
// AXI4-Stream (master) interface axis_mst1
    .axis_mst1_tvalid       (m_axis_1_tvalid  ),
    .axis_mst1_tready       (m_axis_1_tready  ),
    .axis_mst1_tdata        (m_axis_1_tdata   ),
// AXI4-Stream (slave) interface axis_slv1
    .axis_slv1_tvalid       (s_axis_1_tvalid  ),
    .axis_slv1_tready       (s_axis_1_tready  ),
    .axis_slv1_tdata        (s_axis_1_tdata   ),
// AXI4-Stream (master) interface axis_mst2
    .axis_mst2_tvalid       (m_axis_2_tvalid  ),
    .axis_mst2_tready       (m_axis_2_tready  ),
    .axis_mst2_tdata        (m_axis_2_tdata   ),
// AXI4-Stream (slave) interface axis_slv2
    .axis_slv2_tvalid       (s_axis_2_tvalid  ),
    .axis_slv2_tready       (s_axis_2_tready  ),
    .axis_slv2_tdata        (s_axis_2_tdata   ),
// AXI4-Stream (master) interface axis_mst3
    .axis_mst3_tvalid       (m_axis_3_tvalid  ),
    .axis_mst3_tready       (m_axis_3_tready  ),
    .axis_mst3_tdata        (m_axis_3_tdata   ),
// AXI4-Stream (slave) interface axis_slv3
    .axis_slv3_tvalid       (s_axis_3_tvalid  ),
    .axis_slv3_tready       (s_axis_3_tready  ),
    .axis_slv3_tdata        (s_axis_3_tdata   ),

// AXI4-Lite slave interface
    .s_axi_control_awvalid   (m_axi_cbc_awvalid),
    .s_axi_control_awready   (m_axi_cbc_awready),
    .s_axi_control_awaddr    (m_axi_cbc_awaddr[11:0]),
    .s_axi_control_wvalid    (m_axi_cbc_wvalid),
    .s_axi_control_wready    (m_axi_cbc_wready),
    .s_axi_control_wdata     (m_axi_cbc_wdata),
    .s_axi_control_wstrb     (m_axi_cbc_wstrb),
    .s_axi_control_arvalid   (m_axi_cbc_arvalid),
    .s_axi_control_arready   (m_axi_cbc_arready),
    .s_axi_control_araddr    (m_axi_cbc_araddr[11:0]),
    .s_axi_control_rvalid    (m_axi_cbc_rvalid),
    .s_axi_control_rready    (m_axi_cbc_rready),
    .s_axi_control_rdata     (m_axi_cbc_rdata),
    .s_axi_control_rresp     (m_axi_cbc_rresp),
    .s_axi_control_bvalid    (m_axi_cbc_bvalid),
    .s_axi_control_bready    (m_axi_cbc_bready),
    .s_axi_control_bresp     (m_axi_cbc_bresp),

// AXI read master interface
    .axi_rmst_awvalid       (s_axi_in_awvalid),  
    .axi_rmst_awready       (s_axi_in_awready),  
    .axi_rmst_awaddr        (s_axi_in_awaddr), 
    .axi_rmst_awlen         (s_axi_in_awlen),
    .axi_rmst_wvalid        (s_axi_in_wvalid),
    .axi_rmst_wready        (s_axi_in_wready),
    .axi_rmst_wdata         (s_axi_in_wdata),
    .axi_rmst_wstrb         (s_axi_in_wstrb),
    .axi_rmst_wlast         (s_axi_in_wlast),
    .axi_rmst_bvalid        (s_axi_in_bvalid),
    .axi_rmst_bready        (s_axi_in_bready), 
    .axi_rmst_arvalid       (s_axi_in_arvalid),
    .axi_rmst_arready       (s_axi_in_arready),
    .axi_rmst_araddr        (s_axi_in_araddr),
    .axi_rmst_arlen         (s_axi_in_arlen),
    .axi_rmst_rvalid        (s_axi_in_rvalid),
    .axi_rmst_rready        (s_axi_in_rready),
    .axi_rmst_rdata         (s_axi_in_rdata),
    .axi_rmst_rlast         (s_axi_in_rlast),

// AXI write master interface
    .axi_wmst_awvalid       (s_axi_out_awvalid),  
    .axi_wmst_awready       (s_axi_out_awready),  
    .axi_wmst_awaddr        (s_axi_out_awaddr), 
    .axi_wmst_awlen         (s_axi_out_awlen),
    .axi_wmst_wvalid        (s_axi_out_wvalid),
    .axi_wmst_wready        (s_axi_out_wready),
    .axi_wmst_wdata         (s_axi_out_wdata),
    .axi_wmst_wstrb         (s_axi_out_wstrb),
    .axi_wmst_wlast         (s_axi_out_wlast),
    .axi_wmst_bvalid        (s_axi_out_bvalid),
    .axi_wmst_bready        (s_axi_out_bready), 
    .axi_wmst_arvalid       (s_axi_out_arvalid),
    .axi_wmst_arready       (s_axi_out_arready),
    .axi_wmst_araddr        (s_axi_out_araddr),
    .axi_wmst_arlen         (s_axi_out_arlen),
    .axi_wmst_rvalid        (s_axi_out_rvalid),
    .axi_wmst_rready        (s_axi_out_rready),
    .axi_wmst_rdata         (s_axi_out_rdata),
    .axi_wmst_rlast         (s_axi_out_rlast)
);
  
  axi_vip_mst_mst_t krnl_aes_ctrl;
  axi_vip_mst_mst_t krnl_cbc_ctrl;
  axi_vip_slv_slv_mem_t input_buffer;
  axi_vip_slv_slv_mem_t output_buffer;


`include "tb_krnl_cbc.vh"


  // handle the data processing for all groups
  task automatic cipher_processing (input axi_vip_mst_mst_t ctrl, input bit [63:0] read_buffer, input bit [63:0] write_buffer);
    int input_groups = 0;
    int output_groups = 0;
    bit [31:0] rd_value;
    bit [63:0] temp_addr;
    bit [31:0] addr_lsb, addr_msb;

    forever begin
      read_register(ctrl, KRNL_CTRL_REG_ADDR, rd_value);

      //$display($time, "rd_value = %x", rd_value);

      // input sync
      if (input_groups < `GROUP_NUM) begin
        if (!(rd_value & CTRL_START_MASK)) begin
          temp_addr = read_buffer + input_groups * `WORD_NUM * 16;
          addr_msb = temp_addr[63:32];
          addr_lsb = temp_addr[31:0];
          blocking_write_register (ctrl, CBC_SRC_ADDR_0, addr_lsb);// input buffer base address in axi slave vip
          blocking_write_register (ctrl, CBC_SRC_ADDR_1, addr_msb);  
          temp_addr = write_buffer + input_groups * `WORD_NUM * 16;
          addr_msb = temp_addr[63:32];
          addr_lsb = temp_addr[31:0];
          blocking_write_register (ctrl, CBC_DEST_ADDR_0, addr_lsb);// output buffer base address in axi slave vip
          blocking_write_register (ctrl, CBC_DEST_ADDR_1, addr_msb);   
          blocking_write_register (ctrl, KRNL_CTRL_REG_ADDR, CTRL_START_MASK);
          $display($time,, " [INPUT SYNC] ap_start low detected, assert ap_start for group %2d", input_groups);
          input_groups++;
        end
      end


      read_register(ctrl, KRNL_CTRL_REG_ADDR, rd_value);
      // output sync
      if (output_groups < `GROUP_NUM) begin
        if (rd_value & CTRL_DONE_MASK) begin
          blocking_write_register(ctrl, KRNL_CTRL_REG_ADDR, CTRL_CONTINUE_MASK);
          $write("%c[1;34m",27);
          $display($time,, " [OUTPUT SYNC] ap_done for group %2d detected, assert ap_continue", output_groups);
          $write("%c[0m",27); 
          output_groups++;
        end
      end
      
      if (output_groups == `GROUP_NUM) begin
        break;
      end
    end
  endtask


initial  begin : main_test_routine
    
    int     i, j;
    int     temp;
    int     mismatch;
    int     start_time;
    int     finish_time;
    int     enc_fail;
    int     dec_fail;

    bit [127:0] plain_data[`WORD_NUM * `GROUP_NUM];
    bit [127:0] cipher_ecb_data[`WORD_NUM * `GROUP_NUM];
    bit [127:0] cipher_cbc_data[`WORD_NUM * `GROUP_NUM];
    bit [127:0] output_data[`WORD_NUM * `GROUP_NUM];
    
    bit [31:0]  reg_read_data;
    bit [31:0]  reg_rw_addr; 
    
    bit [255:0] aes_key;
    bit [127:0] cbc_iv;

    int file_ptr;

    aes_key = 256'h`KEY;
    cbc_iv  = 128'h`IV;

    file_ptr = $fopen("plain.dat", "rb");
    temp = $fread(plain_data, file_ptr);
    $fclose(file_ptr);    

    file_ptr = $fopen("cipher_ecb.dat", "rb");
    temp = $fread(cipher_ecb_data, file_ptr);
    $fclose(file_ptr); 

    file_ptr = $fopen("cipher_cbc.dat", "rb");
    temp = $fread(cipher_cbc_data, file_ptr);
    $fclose(file_ptr); 

    #2000
        init_vips();

    #1000
    $display("---------------------------------------------------------------------------");
    $display("  AES KEY expansion");
    $display("---------------------------------------------------------------------------");    
    
    $display(" Setting AES key length (256 bit)");
    blocking_write_register (krnl_aes_ctrl, AES_KEY_LEN, 32'h2);       // key_len = 256-bit
    
    $display(" Setting AES key");
    for (i = 0; i < 8; i = i + 1) begin
        blocking_write_register(krnl_aes_ctrl, AES_KEY_W7 + i * 32'h8, aes_key[255-i*32-:32]);
    end
    
    $display(" Setting IV");
    for (i = 0; i < 4; i = i + 1) begin
        blocking_write_register(krnl_cbc_ctrl, CBC_IV_W3 + i * 32'h8, cbc_iv[127-i*32-:32]);
    end
    
    $display(" Start key expansion");
    set_start_register(krnl_aes_ctrl);
    poll_done_register(krnl_aes_ctrl);
    $display(" Key expansion finished");    


    #1000
    $display("-------------------------------------------------------------------------------------");
    $display("  AES ECB mode Encryption test with 4 engines, total %1d groups of %1d WORD (128-bit)", `GROUP_NUM, `WORD_NUM);
    $display("-------------------------------------------------------------------------------------");    
    // set kernel registers                      
    blocking_write_register (krnl_aes_ctrl, AES_MODE, 32'h1);                 // mode = encryption
    blocking_write_register (krnl_cbc_ctrl, CBC_MODE, 32'h1);                 // mode = encryption
    blocking_write_register (krnl_cbc_ctrl, CBC_CBC_MODE, 32'h0);             // set ECB mode
    blocking_write_register (krnl_cbc_ctrl, CBC_WORDS_NUM, `WORD_NUM);        // 128-bit word number to be processed

    // fill input buffer memory with plain data
    in_buffer_fill_memory(input_buffer, IN_BUFFER_BASE, plain_data, 0, `WORD_NUM*`GROUP_NUM);   

    // run ap_ctrl_chain mode processing     
    cipher_processing (krnl_cbc_ctrl, IN_BUFFER_BASE, OUT_BUFFER_BASE);

    // dump output buffer memory data
    out_buffer_dump_memory(output_buffer, OUT_BUFFER_BASE, output_data, 0, `WORD_NUM*`GROUP_NUM);

    // Data compare        
    if (words_compare(cipher_ecb_data, output_data, `WORD_NUM*`GROUP_NUM)) begin
      $write("%c[1;32m",27);
      $display($time,, "      [CHECK] Data check SUCCEED!");
      $write("%c[0m",27); 
    end else begin
      $write("%c[1;31m",27);
      $display($time,, "      [CHECK] Data check FAIL!");
      $write("%c[0m",27); 
    end    

    #1000
    $display("-------------------------------------------------------------------------------------");
    $display("  AES ECB mode Decryption test with 4 engines, total %1d groups of %1d WORD (128-bit)", `GROUP_NUM, `WORD_NUM);
    $display("-------------------------------------------------------------------------------------");    
    // set kernel registers                      
    blocking_write_register (krnl_aes_ctrl, AES_MODE, 32'h0);                 // mode = decryption
    blocking_write_register (krnl_cbc_ctrl, CBC_MODE, 32'h0);                 // mode = decryption
    blocking_write_register (krnl_cbc_ctrl, CBC_CBC_MODE, 32'h0);             // set ECB mode
    blocking_write_register (krnl_cbc_ctrl, CBC_WORDS_NUM, `WORD_NUM);        // 128-bit word number to be processed
   
    // fill input buffer memory with plain data
    in_buffer_fill_memory(input_buffer, IN_BUFFER_BASE, cipher_ecb_data, 0, `WORD_NUM*`GROUP_NUM);   

    // run ap_ctrl_chain mode processing     
    cipher_processing (krnl_cbc_ctrl, IN_BUFFER_BASE, OUT_BUFFER_BASE);

    // dump output buffer memory data
    out_buffer_dump_memory(output_buffer, OUT_BUFFER_BASE, output_data, 0, `WORD_NUM*`GROUP_NUM);

    // Data compare        
    if (words_compare(plain_data, output_data, `WORD_NUM*`GROUP_NUM)) begin
      $write("%c[1;32m",27);
      $display($time,, "      [CHECK] Data check SUCCEED!");
      $write("%c[0m",27); 
    end else begin
      $write("%c[1;31m",27);
      $display($time,, "      [CHECK] Data check FAIL!");
      $write("%c[0m",27); 
    end   


    #1000
    $display("-------------------------------------------------------------------------------------");
    $display("  AES CBC mode Encryption test with 4 engines, total %1d groups of %1d WORD (128-bit)", `GROUP_NUM, `WORD_NUM);
    $display("-------------------------------------------------------------------------------------");    
    // set kernel registers                      
    blocking_write_register (krnl_aes_ctrl, AES_MODE, 32'h1);                 // mode = encryption
    blocking_write_register (krnl_cbc_ctrl, CBC_MODE, 32'h1);                 // mode = encryption
    blocking_write_register (krnl_cbc_ctrl, CBC_CBC_MODE, 32'h1);             // set CBC mode
    blocking_write_register (krnl_cbc_ctrl, CBC_WORDS_NUM, `WORD_NUM);        // 128-bit word number to be processed
    
    // fill input buffer memory with plain data
    in_buffer_fill_memory(input_buffer, IN_BUFFER_BASE, plain_data, 0, `WORD_NUM*`GROUP_NUM);   

    // run ap_ctrl_chain mode processing     
    cipher_processing (krnl_cbc_ctrl, IN_BUFFER_BASE, OUT_BUFFER_BASE);

    // dump output buffer memory data
    out_buffer_dump_memory(output_buffer, OUT_BUFFER_BASE, output_data, 0, `WORD_NUM*`GROUP_NUM);

    // Data compare        
    if (words_compare(cipher_cbc_data, output_data, `WORD_NUM*`GROUP_NUM)) begin
      $write("%c[1;32m",27);
      $display($time,, "      [CHECK] Data check SUCCEED!");
      $write("%c[0m",27); 
    end else begin
      $write("%c[1;31m",27);
      $display($time,, "      [CHECK] Data check FAIL!");
      $write("%c[0m",27); 
    end   

    #1000
    $display("-------------------------------------------------------------------------------------");
    $display("  AES CBC mode Decryption test with 4 engines, total %1d groups of %1d WORD (128-bit)", `GROUP_NUM, `WORD_NUM);
    $display("-------------------------------------------------------------------------------------");    
    // set kernel registers                      
    blocking_write_register (krnl_aes_ctrl, AES_MODE, 32'h0);                 // mode = decryption
    blocking_write_register (krnl_cbc_ctrl, CBC_MODE, 32'h0);                 // mode = decryption
    blocking_write_register (krnl_cbc_ctrl, CBC_CBC_MODE, 32'h1);             // set CBC mode
    blocking_write_register (krnl_cbc_ctrl, CBC_WORDS_NUM, `WORD_NUM);        // 128-bit word number to be processed
   
    // fill input buffer memory with plain data
    in_buffer_fill_memory(input_buffer, IN_BUFFER_BASE, cipher_cbc_data, 0, `WORD_NUM*`GROUP_NUM);   

    // run ap_ctrl_chain mode processing     
    cipher_processing (krnl_cbc_ctrl, IN_BUFFER_BASE, OUT_BUFFER_BASE);

    // dump output buffer memory data
    out_buffer_dump_memory(output_buffer, OUT_BUFFER_BASE, output_data, 0, `WORD_NUM*`GROUP_NUM);

    // Data compare        
    if (words_compare(plain_data, output_data, `WORD_NUM*`GROUP_NUM)) begin
      $write("%c[1;32m",27);
      $display($time,, "      [CHECK] Data check SUCCEED!");
      $write("%c[0m",27); 
    end else begin
      $write("%c[1;31m",27);
      $display($time,, "      [CHECK] Data check FAIL!");
      $write("%c[0m",27); 
    end   

    #1000 $finish;

end


// Waveform dump
`ifdef DUMP_WAVEFORM
  initial begin
    $dumpfile("tb_krnl_cbc.vcd");
    $dumpvars(0,tb_krnl_cbc);
  end
`endif


endmodule
