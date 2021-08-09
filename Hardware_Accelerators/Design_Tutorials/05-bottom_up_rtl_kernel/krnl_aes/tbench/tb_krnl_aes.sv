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
import axi4stream_vip_pkg::*;
import axi_vip_mst_pkg::*;
import axis_vip_mst_pkg::*;
import axis_vip_slv_pkg::*;

module tb_krnl_aes ();


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
  wire [31:0]   m_axi_awaddr   ;   
  wire          m_axi_awvalid  ;
  wire          m_axi_awready  ;
  wire [31:0]   m_axi_wdata    ;
  wire [3:0]    m_axi_wstrb    ;
  wire          m_axi_wvalid   ;
  wire          m_axi_wready   ;
  wire [1:0]    m_axi_bresp    ;
  wire          m_axi_bvalid   ;
  wire          m_axi_bready   ;
  wire [31:0]   m_axi_araddr   ;
  wire          m_axi_arvalid  ;
  wire          m_axi_arready  ;
  wire [31:0]   m_axi_rdata    ;
  wire [1:0]    m_axi_rresp    ;
  wire          m_axi_rvalid   ;
  wire          m_axi_rready   ;

  wire          m_axis_0_tvalid;
  wire          m_axis_0_tready;
  wire [127:0]  m_axis_0_tdata;

  wire          m_axis_1_tvalid;
  wire          m_axis_1_tready;
  wire [127:0]  m_axis_1_tdata;

  wire          s_axis_0_tvalid;
  wire          s_axis_0_tready;
  wire [127:0]  s_axis_0_tdata;

  wire          s_axis_1_tvalid;
  wire          s_axis_1_tready;
  wire [127:0]  s_axis_1_tdata;

// instantiation of axi master vip
  axi_vip_mst axi_vip_mst_inst (
    .aclk           (ap_clk),           // input wire aclk
    .aresetn        (ap_rst_n),         // input wire aresetn
    .m_axi_awaddr   (m_axi_awaddr),     // output wire [31 : 0] m_axi_awaddr
    .m_axi_awvalid  (m_axi_awvalid),    // output wire m_axi_awvalid
    .m_axi_awready  (m_axi_awready),    // input wire m_axi_awready
    .m_axi_wdata    (m_axi_wdata),      // output wire [31 : 0] m_axi_wdata
    .m_axi_wstrb    (m_axi_wstrb),      // output wire [3 : 0] m_axi_wstrb
    .m_axi_wvalid   (m_axi_wvalid),     // output wire m_axi_wvalid
    .m_axi_wready   (m_axi_wready),     // input wire m_axi_wready
    .m_axi_bresp    (m_axi_bresp),      // input wire [1 : 0] m_axi_bresp
    .m_axi_bvalid   (m_axi_bvalid),     // input wire m_axi_bvalid
    .m_axi_bready   (m_axi_bready),     // output wire m_axi_bready
    .m_axi_araddr   (m_axi_araddr),     // output wire [31 : 0] m_axi_araddr
    .m_axi_arvalid  (m_axi_arvalid),    // output wire m_axi_arvalid
    .m_axi_arready  (m_axi_arready),    // input wire m_axi_arready
    .m_axi_rdata    (m_axi_rdata),      // input wire [31 : 0] m_axi_rdata
    .m_axi_rresp    (m_axi_rresp),      // input wire [1 : 0] m_axi_rresp
    .m_axi_rvalid   (m_axi_rvalid),     // input wire m_axi_rvalid
    .m_axi_rready   (m_axi_rready)      // output wire m_axi_rready
);


// instantiation of axis master/slave vips
  axis_vip_mst axis_vip_mst_0 (
    .aclk           (ap_clk),               // input wire aclk
    .aresetn        (ap_rst_n),             // input wire aresetn
    .m_axis_tvalid  (m_axis_0_tvalid),    // output wire [0 : 0] m_axis_tvalid
    .m_axis_tready  (m_axis_0_tready),    // input wire [0 : 0] m_axis_tready
    .m_axis_tdata   (m_axis_0_tdata)      // output wire [127 : 0] m_axis_tdata
);

  axis_vip_mst axis_vip_mst_1 (
    .aclk           (ap_clk),               // input wire aclk
    .aresetn        (ap_rst_n),             // input wire aresetn
    .m_axis_tvalid  (m_axis_1_tvalid),    // output wire [0 : 0] m_axis_tvalid
    .m_axis_tready  (m_axis_1_tready),    // input wire [0 : 0] m_axis_tready
    .m_axis_tdata   (m_axis_1_tdata)      // output wire [127 : 0] m_axis_tdata
);

  axis_vip_slv axis_vip_slv_0 (
    .aclk           (ap_clk),               // input wire aclk
    .aresetn        (ap_rst_n),             // input wire aresetn
    .s_axis_tvalid  (s_axis_0_tvalid),    // input wire [0 : 0] s_axis_tvalid
    .s_axis_tready  (s_axis_0_tready),    // output wire [0 : 0] s_axis_tready
    .s_axis_tdata   (s_axis_0_tdata)      // input wire [127 : 0] s_axis_tdata
);

  axis_vip_slv axis_vip_slv_1 (
    .aclk           (ap_clk),               // input wire aclk
    .aresetn        (ap_rst_n),             // input wire aresetn
    .s_axis_tvalid  (s_axis_1_tvalid),    // input wire [0 : 0] s_axis_tvalid
    .s_axis_tready  (s_axis_1_tready),    // output wire [0 : 0] s_axis_tready
    .s_axis_tdata   (s_axis_1_tdata)      // input wire [127 : 0] s_axis_tdata
);


// instantiation of krnl_aes, only use AES engine 0 and engine 1
  krnl_aes krnl_aes_inst (
  // System Signals
    .ap_clk             (ap_clk),
    .ap_rst_n           (ap_rst_n),

  // AXI4-Stream (master) interface axis_mst0
    .axis_mst0_tvalid   (s_axis_0_tvalid),
    .axis_mst0_tready   (s_axis_0_tready),
    .axis_mst0_tdata    (s_axis_0_tdata),
  // AXI4-Stream (slave) interface axis_slv0
    .axis_slv0_tvalid   (m_axis_0_tvalid),
    .axis_slv0_tready   (m_axis_0_tready),
    .axis_slv0_tdata    (m_axis_0_tdata),
  // AXI4-Stream (master) interface axis_mst1
    .axis_mst1_tvalid   (s_axis_1_tvalid),
    .axis_mst1_tready   (s_axis_1_tready),
    .axis_mst1_tdata    (s_axis_1_tdata),
  // AXI4-Stream (slave) interface axis_slv1
    .axis_slv1_tvalid   (m_axis_1_tvalid),
    .axis_slv1_tready   (m_axis_1_tready),
    .axis_slv1_tdata    (m_axis_1_tdata),

  // tie-off input ports of axis channel 2-3
    .axis_mst2_tvalid   (),
    .axis_mst2_tready   (1'b0),
    .axis_mst2_tdata    (),
    .axis_slv2_tvalid   (1'b0),
    .axis_slv2_tdata    (128'b0),
    .axis_slv2_tready   (),
    
    .axis_mst3_tvalid   (),
    .axis_mst3_tready   (1'b0),
    .axis_mst3_tdata    (),
    .axis_slv3_tvalid   (1'b0),
    .axis_slv3_tdata    (128'b0),
    .axis_slv3_tready   (),
  
  // AXI4-Lite slave interface
    .s_axi_control_awvalid   (m_axi_awvalid),
    .s_axi_control_awready   (m_axi_awready),
    .s_axi_control_awaddr    (m_axi_awaddr[11:0]),
    .s_axi_control_wvalid    (m_axi_wvalid),
    .s_axi_control_wready    (m_axi_wready),
    .s_axi_control_wdata     (m_axi_wdata),
    .s_axi_control_wstrb     (m_axi_wstrb),
    .s_axi_control_arvalid   (m_axi_arvalid),
    .s_axi_control_arready   (m_axi_arready),
    .s_axi_control_araddr    (m_axi_araddr[11:0]),
    .s_axi_control_rvalid    (m_axi_rvalid),
    .s_axi_control_rready    (m_axi_rready),
    .s_axi_control_rdata     (m_axi_rdata),
    .s_axi_control_rresp     (m_axi_rresp),
    .s_axi_control_bvalid    (m_axi_bvalid),
    .s_axi_control_bready    (m_axi_bready),
    .s_axi_control_bresp     (m_axi_bresp)
);


  axis_vip_mst_mst_t   axis_mst0;
  axis_vip_mst_mst_t   axis_mst1;

  axis_vip_slv_slv_t   axis_slv0;
  axis_vip_slv_slv_t   axis_slv1;
  
  axi_vip_mst_mst_t    ctrl;

`include "tb_krnl_aes.vh"

// Control register address mapping
// 0x000 : CTRL
// 0x010 : MODE
// 0x018 : KEY_LEN
// 0x020 : STATUS
// 0x028 : KEY_W7
// 0x030 : KEY_W6
// 0x038 : KEY_W5
// 0x040 : KEY_W4
// 0x048 : KEY_W3
// 0x050 : KEY_W2
// 0x058 : KEY_W1
// 0x060 : KEY_W0

initial  begin : main_test_routine
    int     temp;
    int     plain_file;
    int     cipher_file;      
    int     i;
    int     mismatch;
    int     start_time;
    int     finish_time;
    int     enc_fail;
    int     dec_fail;

    bit [127:0] plain_data[0:`WORD_NUM-1];
    bit [127:0] cipher_data[0:`WORD_NUM-1];
    bit [127:0] axis_rcv_data[0:`WORD_NUM-1];
    
    bit [31:0]  reg_read_data;
    bit [31:0]  reg_rw_addr; 
    
    bit [255:0] aes_key;

    aes_key = 256'h`KEY;
    enc_fail = 0;
    dec_fail = 0;

    plain_file = $fopen("plain.dat", "rb");
    cipher_file = $fopen("cipher.dat", "rb");
    
    temp = $fread(plain_data, plain_file);
    temp = $fread(cipher_data, cipher_file);
    
    $fclose(plain_file);
    $fclose(cipher_file);

    #2000
        init_vips();
          
    #1000 // read control register default value
    $display("---------------------------------------------------------------------------");
    $display("  Control register read test");
    $display("---------------------------------------------------------------------------");    
    for (reg_rw_addr = 'h000; reg_rw_addr <= 'h060; reg_rw_addr = reg_rw_addr + 'h8) begin
        read_register (reg_rw_addr, reg_read_data);
        $display("  Read data on %h: %h", reg_rw_addr, reg_read_data);
    end

    $display("---------------------------------------------------------------------------");
    $display("  Control register wrte/read test");
    $display("---------------------------------------------------------------------------");    
    for (reg_rw_addr = 'h010; reg_rw_addr <= 'h060; reg_rw_addr = reg_rw_addr + 'h8) begin
        blocking_write_register (reg_rw_addr, 32'hffff_ffff);
        $display("  Write data on %h: 0xffffffff", reg_rw_addr);
        read_register (reg_rw_addr, reg_read_data);
        $display("  Read data on %h: %h", reg_rw_addr, reg_read_data);
    end
    
    #1000
    $display("---------------------------------------------------------------------------");
    $display("  AES KEY expansion");
    $display("---------------------------------------------------------------------------");    
    $display(" Setting AES key length (256 bit)");
    blocking_write_register (32'h018, 32'h2);       // key_len = 256-bit
    $display(" Setting AES key");
    for (i = 0; i < 8; i = i + 1) begin
        blocking_write_register(32'h028 + i * 32'h8, aes_key[255-i*32-:32]);
    end
    $display(" Start key expansion");
    blocking_write_register(32'h000, 32'h1);
    forever begin
        read_register (32'h000, reg_read_data);
        if ((reg_read_data & 32'h2) == 32'h2)
            break;
    end
    read_register (32'h000, reg_read_data);
    $display(" Key expansion finished");    
    $display(" Read data on %h: %h", 32'h000, reg_read_data);
    
    #1000
    $display("---------------------------------------------------------------------------");
    $display("  AES Encryption test with %d WORD (128-bit)", `WORD_NUM);
    $display("---------------------------------------------------------------------------");    
    $display(" Set mode to encryption");
    blocking_write_register (32'h010, 32'h1);       // mode = encryption
    $display(" Start data encryption");
    fork
        start_stream_traffic(axis_mst0, plain_data);
        start_stream_traffic(axis_mst1, plain_data);
    join
    $display(" Result checking for AES channel 0......");
    mismatch = 0;
    stream_data_receive(axis_slv0, axis_rcv_data);
    for (i = 0; i < `WORD_NUM; i = i + 1) begin
        if (cipher_data[i] != axis_rcv_data[i]) begin
            $display("-- WORD %d MISMATCH, EXT = %h, ACT = %h", i, cipher_data[i], axis_rcv_data[i]);
            mismatch = mismatch + 1;
            enc_fail = 1;
        end
    end
    $display(" Result checking for AES channel 1......");
    stream_data_receive(axis_slv1, axis_rcv_data);
    for (i = 0; i < `WORD_NUM; i = i + 1) begin
        if (cipher_data[i] != axis_rcv_data[i]) begin
            $display("-- WORD %d MISMATCH, EXT = %h, ACT = %h", i, cipher_data[i], axis_rcv_data[i]);
            mismatch = mismatch + 1;
            enc_fail = 1;
        end
    end
    if (enc_fail == 1)  begin
        $display(" Encryption test FAIL");
        $display(" Total mismatch : %d", mismatch);
    end
    else begin
        $display(" Encryption test SUCCESS");
    end

    #1000
    $display("---------------------------------------------------------------------------");
    $display("  AES Decryption test with %d WORD (128-bit)", `WORD_NUM);
    $display("---------------------------------------------------------------------------");    
    $display(" Set mode to decryption");
    blocking_write_register (32'h010, 32'h0);       // mode = encryption
    $display(" Start data decryption");
    fork
        start_stream_traffic(axis_mst0, cipher_data);
        start_stream_traffic(axis_mst1, cipher_data);
    join
    $display(" Result checking for AES channel 0......");
    mismatch = 0;
    stream_data_receive(axis_slv0, axis_rcv_data);
    for (i = 0; i < `WORD_NUM; i = i + 1) begin
        if (plain_data[i] != axis_rcv_data[i]) begin
            $display("-- WORD %d MISMATCH, EXT = %h, ACT = %h", i, plain_data[i], axis_rcv_data[i]);
            mismatch = mismatch + 1;
            enc_fail = 1;
        end
    end
    $display(" Result checking for AES channel 1......");
    stream_data_receive(axis_slv1, axis_rcv_data);
    for (i = 0; i < `WORD_NUM; i = i + 1) begin
        if (plain_data[i] != axis_rcv_data[i]) begin
            $display("-- WORD %d MISMATCH, EXT = %h, ACT = %h", i, plain_data[i], axis_rcv_data[i]);
            mismatch = mismatch + 1;
            enc_fail = 1;
        end
    end
    if (enc_fail == 1) begin
        $display(" Decryption test FAIL");
        $display(" Total mismatch : %d", mismatch);
    end
    else begin
        $display(" Decryption test SUCCESS");            
    end

    #1000 $finish;

end


// Waveform dump
`ifdef DUMP_WAVEFORM
  initial begin
    $dumpfile("tb_krnl_aes.vcd");
    $dumpvars(0,tb_krnl_aes);
  end
`endif


endmodule
