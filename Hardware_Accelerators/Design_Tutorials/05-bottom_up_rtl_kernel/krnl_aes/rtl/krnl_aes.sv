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

`timescale 1 ns / 1 ps

module krnl_aes (
  // System Signals
  input             ap_clk,
  input             ap_rst_n,

  // AXI4-Stream (master) interface axis_mst0
  output            axis_mst0_tvalid    ,
  input             axis_mst0_tready    ,
  output  [127:0]   axis_mst0_tdata     ,
  // AXI4-Stream (slave) interface axis_slv0
  input             axis_slv0_tvalid     ,
  output            axis_slv0_tready     ,
  input   [127:0]   axis_slv0_tdata      ,
  // AXI4-Stream (master) interface axis_mst1
  output            axis_mst1_tvalid    ,
  input             axis_mst1_tready    ,
  output  [127:0]   axis_mst1_tdata     ,
  // AXI4-Stream (slave) interface axis_slv1
  input             axis_slv1_tvalid     ,
  output            axis_slv1_tready     ,
  input   [127:0]   axis_slv1_tdata      ,
  // AXI4-Stream (master) interface axis_mst2
  output            axis_mst2_tvalid    ,
  input             axis_mst2_tready    ,
  output  [127:0]   axis_mst2_tdata     ,
  // AXI4-Stream (slave) interface axis_slv2
  input             axis_slv2_tvalid     ,
  output            axis_slv2_tready     ,
  input   [127:0]   axis_slv2_tdata      ,
  // AXI4-Stream (master) interface axis_mst3
  output            axis_mst3_tvalid    ,
  input             axis_mst3_tready    ,
  output  [127:0]   axis_mst3_tdata     ,
  // AXI4-Stream (slave) interface axis_slv3
  input             axis_slv3_tvalid     ,
  output            axis_slv3_tready     ,
  input   [127:0]   axis_slv3_tdata      ,
  
  // AXI4-Lite slave interface
  input             s_axi_control_awvalid,
  output            s_axi_control_awready,
  input   [11:0]    s_axi_control_awaddr ,
  input             s_axi_control_wvalid ,
  output            s_axi_control_wready ,
  input   [31:0]    s_axi_control_wdata  ,
  input   [3:0]     s_axi_control_wstrb  ,
  input             s_axi_control_arvalid,
  output            s_axi_control_arready,
  input   [11:0]    s_axi_control_araddr ,
  output            s_axi_control_rvalid ,
  input             s_axi_control_rready ,
  output  [31:0]    s_axi_control_rdata  ,
  output  [1:0]     s_axi_control_rresp  ,
  output            s_axi_control_bvalid ,
  input             s_axi_control_bready ,
  output  [1:0]     s_axi_control_bresp  
);


// internel connection signals
    wire            int_s_axi_control_awvalid;
    wire            int_s_axi_control_awready;
    wire  [11:0]    int_s_axi_control_awaddr ;
    wire            int_s_axi_control_wvalid ;
    wire            int_s_axi_control_wready ;
    wire  [31:0]    int_s_axi_control_wdata  ;
    wire  [3:0]     int_s_axi_control_wstrb  ;
    wire            int_s_axi_control_arvalid;
    wire            int_s_axi_control_arready;
    wire  [11:0]    int_s_axi_control_araddr ;
    wire            int_s_axi_control_rvalid ;
    wire            int_s_axi_control_rready ;
    wire  [31:0]    int_s_axi_control_rdata  ;
    wire  [1:0]     int_s_axi_control_rresp  ;
    wire            int_s_axi_control_bvalid ;
    wire            int_s_axi_control_bready ;
    wire  [1:0]     int_s_axi_control_bresp  ;

    wire            int_axis_mst0_tvalid    ;
    wire            int_axis_mst0_tready    ;
    wire  [127:0]   int_axis_mst0_tdata     ;
    wire            int_axis_slv0_tvalid    ;
    wire            int_axis_slv0_tready    ;
    wire  [127:0]   int_axis_slv0_tdata     ;
    wire            int_axis_mst1_tvalid    ;
    wire            int_axis_mst1_tready    ;
    wire  [127:0]   int_axis_mst1_tdata     ;
    wire            int_axis_slv1_tvalid    ;
    wire            int_axis_slv1_tready    ;
    wire  [127:0]   int_axis_slv1_tdata     ;
    wire            int_axis_mst2_tvalid    ;
    wire            int_axis_mst2_tready    ;
    wire  [127:0]   int_axis_mst2_tdata     ;
    wire            int_axis_slv2_tvalid    ;
    wire            int_axis_slv2_tready    ;
    wire  [127:0]   int_axis_slv2_tdata     ;
    wire            int_axis_mst3_tvalid    ;
    wire            int_axis_mst3_tready    ;
    wire  [127:0]   int_axis_mst3_tdata     ;
    wire            int_axis_slv3_tvalid    ;
    wire            int_axis_slv3_tready    ;
    wire  [127:0]   int_axis_slv3_tdata     ;


    wire          ap_start;
    wire          ap_done;
    wire          mode;
    wire  [1:0]   key_len;
    wire  [3:0]   status;
    wire  [31:0]  key_w7;
    wire  [31:0]  key_w6;
    wire  [31:0]  key_w5;
    wire  [31:0]  key_w4;
    wire  [31:0]  key_w3;
    wire  [31:0]  key_w2;
    wire  [31:0]  key_w1;
    wire  [31:0]  key_w0;
    
    wire          ap_clk_2;
    wire          ap_rst_n_2;
    

// SystemVerilog interface
    axis_if axis_if_master[4]();
    axis_if axis_if_slave[4]();

// clock sync for axi control slave
  axi_clock_converter axi_clock_sync (
    .s_axi_aclk         (ap_clk),                     
    .s_axi_aresetn      (ap_rst_n),                   
    .s_axi_awaddr       (s_axi_control_awaddr),
    .s_axi_awprot       (3'b0),                       
    .s_axi_awvalid      (s_axi_control_awvalid),       
    .s_axi_awready      (s_axi_control_awready),       
    .s_axi_wdata        (s_axi_control_wdata),         
    .s_axi_wstrb        (s_axi_control_wstrb),         
    .s_axi_wvalid       (s_axi_control_wvalid),        
    .s_axi_wready       (s_axi_control_wready),        
    .s_axi_bresp        (s_axi_control_bresp),         
    .s_axi_bvalid       (s_axi_control_bvalid),        
    .s_axi_bready       (s_axi_control_bready),        
    .s_axi_araddr       (s_axi_control_araddr),
    .s_axi_arprot       (3'b0),                           
    .s_axi_arvalid      (s_axi_control_arvalid),       
    .s_axi_arready      (s_axi_control_arready),       
    .s_axi_rdata        (s_axi_control_rdata),         
    .s_axi_rresp        (s_axi_control_rresp),         
    .s_axi_rvalid       (s_axi_control_rvalid),        
    .s_axi_rready       (s_axi_control_rready),        
                    
    .m_axi_aclk         (ap_clk_2),                  
    .m_axi_aresetn      (ap_rst_n_2),                
    .m_axi_awaddr       (int_s_axi_control_awaddr),    
    .m_axi_awprot       (),                           
    .m_axi_awvalid      (int_s_axi_control_awvalid),   
    .m_axi_awready      (int_s_axi_control_awready),   
    .m_axi_wdata        (int_s_axi_control_wdata),     
    .m_axi_wstrb        (int_s_axi_control_wstrb),     
    .m_axi_wvalid       (int_s_axi_control_wvalid),    
    .m_axi_wready       (int_s_axi_control_wready),    
    .m_axi_bresp        (int_s_axi_control_bresp),     
    .m_axi_bvalid       (int_s_axi_control_bvalid),    
    .m_axi_bready       (int_s_axi_control_bready),    
    .m_axi_araddr       (int_s_axi_control_araddr),    
    .m_axi_arprot       (),                           
    .m_axi_arvalid      (int_s_axi_control_arvalid),   
    .m_axi_arready      (int_s_axi_control_arready),   
    .m_axi_rdata        (int_s_axi_control_rdata),     
    .m_axi_rresp        (int_s_axi_control_rresp),     
    .m_axi_rvalid       (int_s_axi_control_rvalid),    
    .m_axi_rready       (int_s_axi_control_rready)     
);

// clock sync for axi stream ports
  axis_clock_converter axis_clock_sync_s0 (
    .s_axis_aresetn     (ap_rst_n),             
    .m_axis_aresetn     (ap_rst_n_2),          
    .s_axis_aclk        (ap_clk),               
    .s_axis_tvalid      (axis_slv0_tvalid),     
    .s_axis_tready      (axis_slv0_tready),     
    .s_axis_tdata       (axis_slv0_tdata),      
    .m_axis_aclk        (ap_clk_2),            
    .m_axis_tvalid      (int_axis_slv0_tvalid), 
    .m_axis_tready      (int_axis_slv0_tready), 
    .m_axis_tdata       (int_axis_slv0_tdata )
);

  axis_clock_converter axis_clock_sync_s1 (
    .s_axis_aresetn     (ap_rst_n),             
    .m_axis_aresetn     (ap_rst_n_2),          
    .s_axis_aclk        (ap_clk),               
    .s_axis_tvalid      (axis_slv1_tvalid),     
    .s_axis_tready      (axis_slv1_tready),     
    .s_axis_tdata       (axis_slv1_tdata),      
    .m_axis_aclk        (ap_clk_2),            
    .m_axis_tvalid      (int_axis_slv1_tvalid), 
    .m_axis_tready      (int_axis_slv1_tready), 
    .m_axis_tdata       (int_axis_slv1_tdata )
);

  axis_clock_converter axis_clock_sync_s2 (
    .s_axis_aresetn     (ap_rst_n),             
    .m_axis_aresetn     (ap_rst_n_2),          
    .s_axis_aclk        (ap_clk),               
    .s_axis_tvalid      (axis_slv2_tvalid),     
    .s_axis_tready      (axis_slv2_tready),     
    .s_axis_tdata       (axis_slv2_tdata),      
    .m_axis_aclk        (ap_clk_2),            
    .m_axis_tvalid      (int_axis_slv2_tvalid), 
    .m_axis_tready      (int_axis_slv2_tready), 
    .m_axis_tdata       (int_axis_slv2_tdata ) 
);


  axis_clock_converter axis_clock_sync_s3 (
    .s_axis_aresetn     (ap_rst_n),             
    .m_axis_aresetn     (ap_rst_n_2),          
    .s_axis_aclk        (ap_clk),               
    .s_axis_tvalid      (axis_slv3_tvalid),     
    .s_axis_tready      (axis_slv3_tready),     
    .s_axis_tdata       (axis_slv3_tdata),      
    .m_axis_aclk        (ap_clk_2),            
    .m_axis_tvalid      (int_axis_slv3_tvalid), 
    .m_axis_tready      (int_axis_slv3_tready), 
    .m_axis_tdata       (int_axis_slv3_tdata ) 
);

  axis_clock_converter axis_clock_sync_m0 (
    .s_axis_aresetn     (ap_rst_n_2),          
    .m_axis_aresetn     (ap_rst_n),             
    .s_axis_aclk        (ap_clk_2),            
    .s_axis_tvalid      (int_axis_mst0_tvalid), 
    .s_axis_tready      (int_axis_mst0_tready), 
    .s_axis_tdata       (int_axis_mst0_tdata ), 
    .m_axis_aclk        (ap_clk),             
    .m_axis_tvalid      (axis_mst0_tvalid),     
    .m_axis_tready      (axis_mst0_tready),     
    .m_axis_tdata       (axis_mst0_tdata)     
);

  axis_clock_converter axis_clock_sync_m1 (
    .s_axis_aresetn     (ap_rst_n_2),          
    .m_axis_aresetn     (ap_rst_n),             
    .s_axis_aclk        (ap_clk_2),            
    .s_axis_tvalid      (int_axis_mst1_tvalid), 
    .s_axis_tready      (int_axis_mst1_tready), 
    .s_axis_tdata       (int_axis_mst1_tdata ), 
    .m_axis_aclk        (ap_clk),             
    .m_axis_tvalid      (axis_mst1_tvalid),     
    .m_axis_tready      (axis_mst1_tready),     
    .m_axis_tdata       (axis_mst1_tdata)     
);

  axis_clock_converter axis_clock_sync_m2 (
    .s_axis_aresetn     (ap_rst_n_2),          
    .m_axis_aresetn     (ap_rst_n),             
    .s_axis_aclk        (ap_clk_2),            
    .s_axis_tvalid      (int_axis_mst2_tvalid), 
    .s_axis_tready      (int_axis_mst2_tready), 
    .s_axis_tdata       (int_axis_mst2_tdata ), 
    .m_axis_aclk        (ap_clk),             
    .m_axis_tvalid      (axis_mst2_tvalid),     
    .m_axis_tready      (axis_mst2_tready),     
    .m_axis_tdata       (axis_mst2_tdata)     
);

  axis_clock_converter axis_clock_sync_m3 (
    .s_axis_aresetn     (ap_rst_n_2),          
    .m_axis_aresetn     (ap_rst_n),             
    .s_axis_aclk        (ap_clk_2),            
    .s_axis_tvalid      (int_axis_mst3_tvalid), 
    .s_axis_tready      (int_axis_mst3_tready), 
    .s_axis_tdata       (int_axis_mst3_tdata ), 
    .m_axis_aclk        (ap_clk),             
    .m_axis_tvalid      (axis_mst3_tvalid),     
    .m_axis_tready      (axis_mst3_tready),     
    .m_axis_tdata       (axis_mst3_tdata)     
);


// AXI4-Lite slave interface
  krnl_aes_axi_ctrl_slave #(.AES_ENINE_NUM (4)) krnl_aes_axi_ctrl_slave_inst (
    .ACLK     ( ap_clk_2               ),
    .ARESETn  ( ap_rst_n_2             ),
    
    .AWVALID  ( int_s_axi_control_awvalid ),
    .AWREADY  ( int_s_axi_control_awready ),
    .AWADDR   ( int_s_axi_control_awaddr  ),
    .WVALID   ( int_s_axi_control_wvalid  ),
    .WREADY   ( int_s_axi_control_wready  ),
    .WDATA    ( int_s_axi_control_wdata   ),
    .WSTRB    ( int_s_axi_control_wstrb   ),
    .ARVALID  ( int_s_axi_control_arvalid ),
    .ARREADY  ( int_s_axi_control_arready ),
    .ARADDR   ( int_s_axi_control_araddr  ),
    .RVALID   ( int_s_axi_control_rvalid  ),
    .RREADY   ( int_s_axi_control_rready  ),
    .RDATA    ( int_s_axi_control_rdata   ),
    .RRESP    ( int_s_axi_control_rresp   ),
    .BVALID   ( int_s_axi_control_bvalid  ),
    .BREADY   ( int_s_axi_control_bready  ),
    .BRESP    ( int_s_axi_control_bresp   ),
    
    .ap_start (ap_start               ),
    .ap_done  (ap_done                ),
    
    .mode     (mode                   ),
    .key_len  (key_len                ),
    .status   (status                 ),
    .key_w7   (key_w7                 ),
    .key_w6   (key_w6                 ),
    .key_w5   (key_w5                 ),
    .key_w4   (key_w4                 ),
    .key_w3   (key_w3                 ),
    .key_w2   (key_w2                 ),
    .key_w1   (key_w1                 ),
    .key_w0   (key_w0                 )
);


// aes wrapper
  aes_wrapper #(.AES_ENINE_NUM (4)) aes_wrapper_inst (
    .CLK                (ap_clk_2              ),       
    .RESETn             (ap_rst_n_2            ),

    .mode               (mode                   ),      
    .key_length         (key_len                ),
    .key                ({key_w7, key_w6, key_w5, key_w4, key_w3, key_w2, key_w1, key_w0}),       
    .status             (status                 ),    
    .keyexp_start       (ap_start               ),
    .keyexp_done        (ap_done                ),

    .axis_if_slave      (axis_if_slave          ),
    .axis_if_master     (axis_if_master         )

);

    wire          pll_locked;

// clock doubler
  clk_gen clk_gen_inst (
    .clk_out1       (ap_clk_2  ), 
    .locked         (pll_locked ),    
    .clk_in1        (ap_clk     )
);     

// generate reset for ap_clk_2
  reg rst_sync1, rst_sync2, rst_sync3;
  wire reset_n_trigger;
  
  assign reset_n_trigger = ap_rst_n & pll_locked;
  
  always @ (posedge ap_clk_2 or negedge reset_n_trigger) begin
    if (!reset_n_trigger) begin
      rst_sync1 <= 1'b0;
      rst_sync2 <= 1'b0;
      rst_sync3 <= 1'b0;
    end else begin  
      rst_sync1 <= 1'b1;
      rst_sync2 <= rst_sync1;
      rst_sync3 <= rst_sync2;
    end
  end
  
  assign ap_rst_n_2 = rst_sync3;

// SystemVerilog interface signals assignment

  assign int_axis_mst0_tvalid  = axis_if_master[0].tvalid;
  assign int_axis_mst0_tdata   = axis_if_master[0].tdata;
  assign axis_if_master[0].tready = int_axis_mst0_tready;

  assign int_axis_mst1_tvalid  = axis_if_master[1].tvalid;
  assign int_axis_mst1_tdata   = axis_if_master[1].tdata;
  assign axis_if_master[1].tready = int_axis_mst1_tready;

  assign int_axis_mst2_tvalid  = axis_if_master[2].tvalid;
  assign int_axis_mst2_tdata   = axis_if_master[2].tdata;
  assign axis_if_master[2].tready = int_axis_mst2_tready;

  assign int_axis_mst3_tvalid  = axis_if_master[3].tvalid;
  assign int_axis_mst3_tdata   = axis_if_master[3].tdata;
  assign axis_if_master[3].tready = int_axis_mst3_tready;

  assign axis_if_slave[0].tvalid  = int_axis_slv0_tvalid;
  assign axis_if_slave[0].tdata   = int_axis_slv0_tdata;
  assign int_axis_slv0_tready = axis_if_slave[0].tready;

  assign axis_if_slave[1].tvalid  = int_axis_slv1_tvalid;
  assign axis_if_slave[1].tdata   = int_axis_slv1_tdata;
  assign int_axis_slv1_tready = axis_if_slave[1].tready;

  assign axis_if_slave[2].tvalid  = int_axis_slv2_tvalid;
  assign axis_if_slave[2].tdata   = int_axis_slv2_tdata;
  assign int_axis_slv2_tready = axis_if_slave[2].tready;

  assign axis_if_slave[3].tvalid  = int_axis_slv3_tvalid;
  assign axis_if_slave[3].tdata   = int_axis_slv3_tdata;
  assign int_axis_slv3_tready = axis_if_slave[3].tready;

endmodule
