/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.
//
module ps_slave #
  (
   // Width of S_AXI data bus
   parameter integer C_S_AXI_DATA_WIDTH	= 32,
   // Width of S_AXI address bus
   parameter integer C_S_AXI_ADDR_WIDTH	= 10
   )
   (
    input  wire                                S_AXI_ACLK,
    input  wire                                S_AXI_ARESETN,
    input  wire [C_S_AXI_ADDR_WIDTH-1 : 0]     S_AXI_AWADDR,
    input  wire [2 : 0]                        S_AXI_AWPROT,
    input  wire                                S_AXI_AWVALID,
    output wire                                S_AXI_AWREADY,
    input  wire [C_S_AXI_DATA_WIDTH-1 : 0]     S_AXI_WDATA,
    input  wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
    input  wire                                S_AXI_WVALID,
    output wire                                S_AXI_WREADY,
    output wire [1 : 0]                        S_AXI_BRESP,
    output wire                                S_AXI_BVALID,
    input  wire                                S_AXI_BREADY,
    input  wire [C_S_AXI_ADDR_WIDTH-1 : 0]     S_AXI_ARADDR,
    input  wire [2 : 0]                        S_AXI_ARPROT,
    input  wire                                S_AXI_ARVALID,
    output wire                                S_AXI_ARREADY,
    output reg  [C_S_AXI_DATA_WIDTH-1 : 0]     S_AXI_RDATA,
    output wire [1 : 0]                        S_AXI_RRESP,
    output reg                                 S_AXI_RVALID,
    input  wire                                S_AXI_RREADY,
   
    output reg				       soft_reset, 
    // Row URAM Write interface 
    output reg [63:0]                       row_uram_ena,
    output reg [63:0]                       row_uram_wea,
    output reg [11:0]                       row_uram_addra,
    output reg [63:0]                       row_uram_dina,

    // Col URAM Write Interface
    output reg [63:0]                       col_uram_ena,
    output reg [63:0]                       col_uram_wea,
    output reg [11:0]                       col_uram_addra,
    output reg [63:0]                       col_uram_dina,
    input                                   uram_ena_ack,

    //OP URAM Read Interface
    output reg [63:0]                       op_uram_enb,
    output reg [13:0]                       op_uram_addrb,
    input      [15:0]                       op_uram_doutb,
    input                                   op_uram_doutb_valid,
    output reg                              op_uram_doutb_ack,

    output reg                              ps_app_done,
    input                                   dut_done,
    input    [4:0]                          num_mat_done,

    input                                   mem_init_done,
    input [31:0]                            active_clk_count


    );
/*******************************************************************************************************************/
// Address Map
//
//  Address                Register
//
//    10                    Control reg 
//                         bit[0] = 1 => DUT Enable. This bit should be set after writing Matrices into URAMs
//			   bit[1] = 1 => Address Auto Increment. If this bit is set, then after every write or read address
//				         will be incremented by 4 (32-bit read/writes)
//    14                    Status reg (bit[0] = 1 => DUT Done. This bit should be set before PS app reads output matrix
//    18                    Indirect addressing control reg bit[0] = 1 => command valid, bit[1] = R/W#
//    1C                    Indirect address register
//   20                    Indirect write register (32-bit)
//   24                    Indirect read register (32-bit)
//   28                    Indirect Read Address register
//   2C                    no.of matrices done
//   30                    No. of clocks taken by DUT to complete all matrix multiplications
//   34                    soft_reset
/*******************************************************************************************************************/


reg second_data;
   // AXI4LITE signals
   reg [C_S_AXI_ADDR_WIDTH-1 : 0] axi_awaddr;
   reg 				  axi_awready;
   reg 				  axi_wready;
   reg [1 : 0] 			  axi_bresp;
   reg 				  axi_bvalid;
   reg [C_S_AXI_ADDR_WIDTH-1 : 0] axi_araddr;
   reg 				  axi_arready;
   reg [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata;
   reg [1 : 0] 			  axi_rresp;

   localparam integer 		  ADDR_LSB = (C_S_AXI_DATA_WIDTH/32) + 1;  // 2
   localparam integer 		  OPT_MEM_ADDR_BITS = 5; // total 2^(5+1)=64 x 32b registers
   localparam integer             CHAN_ADDR_LSB = 8;  // 
   
    
   //-- Number of Slave Registers 9
   reg [C_S_AXI_DATA_WIDTH-1:0]   slv_reg0 ;
   reg [C_S_AXI_DATA_WIDTH-1:0]   slv_reg1 ;
   reg [C_S_AXI_DATA_WIDTH-1:0]   slv_reg2 ;
   reg [C_S_AXI_DATA_WIDTH-1:0]   slv_reg3 ;


   reg 				  chan_awaddr_valid;
   reg 				  chan_araddr_valid;

   wire 			  slv_reg_rden;
   wire 			  slv_reg_wren;
   integer 			  byte_index;
   reg 				  aw_en;
   wire                           ap_done;
// Internal Signals
   wire [63:0] row_uram_ena_dec;
   wire [63:0] row_uram_wea_dec;
   wire [63:0] col_uram_ena_dec;
   wire [63:0] col_uram_wea_dec;
   wire [63:0] op_uram_enb_dec;
   wire any_op_uram_enb;
   reg DUT_en;
   reg Addr_AutoIncr;
   reg CmdValid;
   reg RWn;  // 0 - W, 1 - R
   reg [31:0] ind_addr;
   reg even_oddn; // 0 - Even, 1 - Odd
   reg even_oddn_d; // 0 - Even, 1 - Odd
   reg [63:0] Wr_Data;
   reg WrDataValid;
   reg WrDataValid_d;
   reg [31:0] ps_RDATA_int;
   reg ps_RVALID_int;
   reg [31:0] ps_RDATA_int2;
   reg ps_RVALID_int2;
   reg [31:0] ind_rdaddr;
   reg RdCmdValid;
   reg op_uram_read;
   reg op_uram_enb1;
   reg op_uram_enb2;
   reg op_uram_enb3;
   reg op_uram_enb4;
   reg op_uram_enb5;
   reg op_uram_read_d;

   assign S_AXI_AWREADY	= axi_awready;
   assign S_AXI_WREADY	= axi_wready;
   assign S_AXI_BRESP	= axi_bresp;
   assign S_AXI_BVALID	= axi_bvalid;
   assign S_AXI_ARREADY	= axi_arready;
   assign S_AXI_RRESP	= axi_rresp;
   
   assign ap_done = dut_done;
   
   always @( posedge S_AXI_ACLK)
     begin
	if ( S_AXI_ARESETN == 1'b0 )
	  begin
	     axi_awready <= 1'b0;
	     aw_en <= 1'b1;
	  end 
	else
	  begin    
	     if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	       begin
	          axi_awready <= 1'b1;
	          aw_en <= 1'b0;
	       end
	     else if (S_AXI_BREADY && axi_bvalid)
	       begin
	          aw_en <= 1'b1;
	          axi_awready <= 1'b0;
	       end
	     else           
	       begin
	          axi_awready <= 1'b0;
	       end
	  end 
     end       

   always @( posedge S_AXI_ACLK)
     begin
	if ( S_AXI_ARESETN == 1'b0 )
	  begin
	     chan_awaddr_valid  <= 0;
	  end 
	else
	  begin    
	     if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	       begin
		  chan_awaddr_valid <= 1;
	       end
	  end 
     end       

   always @( posedge S_AXI_ACLK )
     begin
	if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	  begin
	     // Write Address latching 
	     axi_awaddr <= S_AXI_AWADDR;
	  end
     end 
    
    always @(posedge S_AXI_ACLK) begin
       if (~S_AXI_ARESETN) begin
          WrDataValid_d <= 1'b0;
       end
       else begin
          WrDataValid_d <= WrDataValid;
       end
    end   

    // ps_app_done output is generated when complete Matrix A and B are written
    // into URAMs
    // Assuming Mat A is written first and then Matrix B is written, write to
    // the address 0x7_FFF8 will be considered as both matrices are done
    
always @(posedge S_AXI_ACLK) begin
       if (~S_AXI_ARESETN) begin
          ps_app_done <= 1'b0;
       end
       else if (mem_init_done) begin
          ps_app_done <= 1'b1;
       end   
       //else if ((WrDataValid & ~WrDataValid_d) & (ind_addr[19:0] == 20'hFFF8)) begin
       //else if ((WrDataValid & ~WrDataValid_d) & (ind_addr[21:0] == 22'h278078)) begin
       //else if ((WrDataValid & ~WrDataValid_d) & (ind_addr[21:0] == 22'h3FFFF8)) begin
       else if (DUT_en) begin
          ps_app_done <= 1'b1;
       end
    end

   // Implement axi_wready generation
   // axi_wready is asserted for one S_AXI_ACLK clock cycle when both
   // S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_wready is 
   // de-asserted when ~S_AXI_ARESETN is low. 

   always @( posedge S_AXI_ACLK )
     begin
	if ( S_AXI_ARESETN == 1'b0 )
	  begin
	     axi_wready <= 1'b0;
	     second_data <= 1'b0;
	  end 
	else
	  begin    
	     if (uram_ena_ack_toggle) begin second_data<= 'b0; end
	     else if (even_oddn == 1'b1 & (~axi_wready && S_AXI_WVALID && S_AXI_AWVALID && aw_en )) begin second_data <= 1'b1; end
	     if (~axi_wready && S_AXI_WVALID && S_AXI_AWVALID && aw_en )
	       begin
	          // slave is ready to accept write data when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_wready <= 1'b1;
	       end
	     else
	       begin
	          axi_wready <= 1'b0;
	       end
	  end 
     end       

   assign slv_reg_wren = axi_wready && S_AXI_WVALID && axi_awready && S_AXI_AWVALID;

   always @( posedge S_AXI_ACLK or negedge S_AXI_ARESETN) begin
      if ( S_AXI_ARESETN == 1'b0 )begin
         slv_reg0  <= 4;
         slv_reg1  <= 0;
         slv_reg2  <= 0;
         slv_reg3  <= 0;
         DUT_en        <= 1'b0; 
         Addr_AutoIncr <= 1'b0; 
         CmdValid      <= 1'b0; 
         RWn           <= 1'b0; 
         even_oddn     <= 1'b0; 
         Wr_Data       <= 32'b0;
         WrDataValid   <= 1'b0;
	 soft_reset    <= 1'b0;
      end 
      else begin
         if (slv_reg_wren & chan_awaddr_valid) begin
            case ( axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
              6'h00:  //AP_CONTROL register
                    for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                      if ( S_AXI_WSTRB[byte_index] == 1)begin 
                         if (byte_index == 0)begin
                             slv_reg0[0] <= S_AXI_WDATA[0];
                             //slv_reg0[1] <= ap_done;
                             //slv_reg0[2] <= ap_idle;
                             //slv_reg0[3] <= ap_ready;
                             slv_reg0[4] <= S_AXI_WDATA[4];
                             slv_reg0[7] <= S_AXI_WDATA[7];
                         end else
                             slv_reg0[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
                      end
              6'h01:  //GIE register
                    for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                      if ( S_AXI_WSTRB[byte_index] == 1 ) begin
                         slv_reg1[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
                      end  
              6'h02: //IE register
                    for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                      if ( S_AXI_WSTRB[byte_index] == 1 ) begin
                         slv_reg2[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
                      end  
              6'h03: // ISR register
                    for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 ) 
                      if ( S_AXI_WSTRB[byte_index] == 1 ) begin
                         slv_reg3[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
                      end
              6'h04: // control register 
                      begin
                              DUT_en        <= S_AXI_WDATA[0]; 
                              Addr_AutoIncr <= S_AXI_WDATA[1];
                      end  
              6'h06: // Indirect addressing control reg
                    begin
                           CmdValid <= S_AXI_WDATA[0]; 
                           RWn      <= S_AXI_WDATA[1]; 
                     end  
              6'h08: //Indirect addressing control reg  
             	      begin
                         if (CmdValid & axi_wready) begin
                            even_oddn <= ~even_oddn;
                            if (even_oddn == 1'b0) begin
                               Wr_Data <= {32'h0, S_AXI_WDATA[31:0]}; 
                               WrDataValid <= 1'b0;
                            end
                            else begin
                               Wr_Data <= {S_AXI_WDATA[31:0], Wr_Data[31:0]};
                               WrDataValid <= 1'b1;
                            end
                         end   
                      end 
		      
              6'h0D: //soft reset
             	      begin
                           soft_reset <= S_AXI_WDATA[0]; 
                      end 
	      default : begin
	       end
            endcase 
         end 
         else begin
            WrDataValid <= 1'b0;
        end
    end
 end
    
always @(posedge S_AXI_ACLK) begin
   if (~S_AXI_ARESETN | soft_reset) begin
      ind_addr <= 32'b0;
   end
   else if (S_AXI_WVALID & S_AXI_WREADY & (axi_awaddr[5:2] == 3'h7)) begin
      ind_addr <= S_AXI_WDATA[31:0]; 
   end
   //else if (WrDataValid) begin
   else if (uram_ena_ack_toggle) begin
      ind_addr <= ind_addr + 8;
   end
end   

// Writes to Row and Col URAMs

// ind_addr [20:13] are decoded to determine which URAM is written
// ind_addr [22:21] are decoded to decide whether to access Row URAM or Col
// URAM or OP URAM
// ind_addr [22:21]    URAM accessed
//   2'b00                Row
//   2'b01                Col
//   2'b10                 OP

assign row_uram_ena_dec[0]    = (ind_addr[20:15] == 6'd0);
assign row_uram_ena_dec[1]    = (ind_addr[20:15] == 6'd1);
assign row_uram_ena_dec[2]    = (ind_addr[20:15] == 6'd2);
assign row_uram_ena_dec[3]    = (ind_addr[20:15] == 6'd3);
assign row_uram_ena_dec[4]    = (ind_addr[20:15] == 6'd4);
assign row_uram_ena_dec[5]    = (ind_addr[20:15] == 6'd5);
assign row_uram_ena_dec[6]    = (ind_addr[20:15] == 6'd6);
assign row_uram_ena_dec[7]    = (ind_addr[20:15] == 6'd7);
assign row_uram_ena_dec[8]    = (ind_addr[20:15] == 6'd8);
assign row_uram_ena_dec[9]    = (ind_addr[20:15] == 6'd9);
assign row_uram_ena_dec[10]   = (ind_addr[20:15] == 6'd10);
assign row_uram_ena_dec[11]   = (ind_addr[20:15] == 6'd11);
assign row_uram_ena_dec[12]   = (ind_addr[20:15] == 6'd12);
assign row_uram_ena_dec[13]   = (ind_addr[20:15] == 6'd13);
assign row_uram_ena_dec[14]   = (ind_addr[20:15] == 6'd14);
assign row_uram_ena_dec[15]   = (ind_addr[20:15] == 6'd15);
assign row_uram_ena_dec[16]   = (ind_addr[20:15] == 6'd16);
assign row_uram_ena_dec[17]   = (ind_addr[20:15] == 6'd17);
assign row_uram_ena_dec[18]   = (ind_addr[20:15] == 6'd18);
assign row_uram_ena_dec[19]   = (ind_addr[20:15] == 6'd19);
assign row_uram_ena_dec[20]   = (ind_addr[20:15] == 6'd20);
assign row_uram_ena_dec[21]   = (ind_addr[20:15] == 6'd21);
assign row_uram_ena_dec[22]   = (ind_addr[20:15] == 6'd22);
assign row_uram_ena_dec[23]   = (ind_addr[20:15] == 6'd23);
assign row_uram_ena_dec[24]   = (ind_addr[20:15] == 6'd24);
assign row_uram_ena_dec[25]   = (ind_addr[20:15] == 6'd25);
assign row_uram_ena_dec[26]   = (ind_addr[20:15] == 6'd26);
assign row_uram_ena_dec[27]   = (ind_addr[20:15] == 6'd27);
assign row_uram_ena_dec[28]   = (ind_addr[20:15] == 6'd28);
assign row_uram_ena_dec[29]   = (ind_addr[20:15] == 6'd29);
assign row_uram_ena_dec[30]   = (ind_addr[20:15] == 6'd30);
assign row_uram_ena_dec[31]   = (ind_addr[20:15] == 6'd31);
assign row_uram_ena_dec[32]   = (ind_addr[20:15] == 6'd32);
assign row_uram_ena_dec[33]   = (ind_addr[20:15] == 6'd33);
assign row_uram_ena_dec[34]   = (ind_addr[20:15] == 6'd34);
assign row_uram_ena_dec[35]   = (ind_addr[20:15] == 6'd35);
assign row_uram_ena_dec[36]   = (ind_addr[20:15] == 6'd36);
assign row_uram_ena_dec[37]   = (ind_addr[20:15] == 6'd37);
assign row_uram_ena_dec[38]   = (ind_addr[20:15] == 6'd38);
assign row_uram_ena_dec[39]   = (ind_addr[20:15] == 6'd39);
assign row_uram_ena_dec[40]   = (ind_addr[20:15] == 6'd40);
assign row_uram_ena_dec[41]   = (ind_addr[20:15] == 6'd41);
assign row_uram_ena_dec[42]   = (ind_addr[20:15] == 6'd42);
assign row_uram_ena_dec[43]   = (ind_addr[20:15] == 6'd43);
assign row_uram_ena_dec[44]   = (ind_addr[20:15] == 6'd44);
assign row_uram_ena_dec[45]   = (ind_addr[20:15] == 6'd45);
assign row_uram_ena_dec[46]   = (ind_addr[20:15] == 6'd46);
assign row_uram_ena_dec[47]   = (ind_addr[20:15] == 6'd47);
assign row_uram_ena_dec[48]   = (ind_addr[20:15] == 6'd48);
assign row_uram_ena_dec[49]   = (ind_addr[20:15] == 6'd49);
assign row_uram_ena_dec[50]   = (ind_addr[20:15] == 6'd50);
assign row_uram_ena_dec[51]   = (ind_addr[20:15] == 6'd51);
assign row_uram_ena_dec[52]   = (ind_addr[20:15] == 6'd52);
assign row_uram_ena_dec[53]   = (ind_addr[20:15] == 6'd53);
assign row_uram_ena_dec[54]   = (ind_addr[20:15] == 6'd54);
assign row_uram_ena_dec[55]   = (ind_addr[20:15] == 6'd55);
assign row_uram_ena_dec[56]   = (ind_addr[20:15] == 6'd56);
assign row_uram_ena_dec[57]   = (ind_addr[20:15] == 6'd57);
assign row_uram_ena_dec[58]   = (ind_addr[20:15] == 6'd58);
assign row_uram_ena_dec[59]   = (ind_addr[20:15] == 6'd59);
assign row_uram_ena_dec[60]   = (ind_addr[20:15] == 6'd60);
assign row_uram_ena_dec[61]   = (ind_addr[20:15] == 6'd61);
assign row_uram_ena_dec[62]   = (ind_addr[20:15] == 6'd62);
assign row_uram_ena_dec[63]   = (ind_addr[20:15] == 6'd63);

assign row_uram_wea_dec[0]    = (ind_addr[20:15] == 6'd0);
assign row_uram_wea_dec[1]    = (ind_addr[20:15] == 6'd1);
assign row_uram_wea_dec[2]    = (ind_addr[20:15] == 6'd2);
assign row_uram_wea_dec[3]    = (ind_addr[20:15] == 6'd3);
assign row_uram_wea_dec[4]    = (ind_addr[20:15] == 6'd4);
assign row_uram_wea_dec[5]    = (ind_addr[20:15] == 6'd5);
assign row_uram_wea_dec[6]    = (ind_addr[20:15] == 6'd6);
assign row_uram_wea_dec[7]    = (ind_addr[20:15] == 6'd7);
assign row_uram_wea_dec[8]    = (ind_addr[20:15] == 6'd8);
assign row_uram_wea_dec[9]    = (ind_addr[20:15] == 6'd9);
assign row_uram_wea_dec[10]   = (ind_addr[20:15] == 6'd10);
assign row_uram_wea_dec[11]   = (ind_addr[20:15] == 6'd11);
assign row_uram_wea_dec[12]   = (ind_addr[20:15] == 6'd12);
assign row_uram_wea_dec[13]   = (ind_addr[20:15] == 6'd13);
assign row_uram_wea_dec[14]   = (ind_addr[20:15] == 6'd14);
assign row_uram_wea_dec[15]   = (ind_addr[20:15] == 6'd15);
assign row_uram_wea_dec[16]   = (ind_addr[20:15] == 6'd16);
assign row_uram_wea_dec[17]   = (ind_addr[20:15] == 6'd17);
assign row_uram_wea_dec[18]   = (ind_addr[20:15] == 6'd18);
assign row_uram_wea_dec[19]   = (ind_addr[20:15] == 6'd19);
assign row_uram_wea_dec[20]   = (ind_addr[20:15] == 6'd20);
assign row_uram_wea_dec[21]   = (ind_addr[20:15] == 6'd21);
assign row_uram_wea_dec[22]   = (ind_addr[20:15] == 6'd22);
assign row_uram_wea_dec[23]   = (ind_addr[20:15] == 6'd23);
assign row_uram_wea_dec[24]   = (ind_addr[20:15] == 6'd24);
assign row_uram_wea_dec[25]   = (ind_addr[20:15] == 6'd25);
assign row_uram_wea_dec[26]   = (ind_addr[20:15] == 6'd26);
assign row_uram_wea_dec[27]   = (ind_addr[20:15] == 6'd27);
assign row_uram_wea_dec[28]   = (ind_addr[20:15] == 6'd28);
assign row_uram_wea_dec[29]   = (ind_addr[20:15] == 6'd29);
assign row_uram_wea_dec[30]   = (ind_addr[20:15] == 6'd30);
assign row_uram_wea_dec[31]   = (ind_addr[20:15] == 6'd31);
assign row_uram_wea_dec[32]   = (ind_addr[20:15] == 6'd32);
assign row_uram_wea_dec[33]   = (ind_addr[20:15] == 6'd33);
assign row_uram_wea_dec[34]   = (ind_addr[20:15] == 6'd34);
assign row_uram_wea_dec[35]   = (ind_addr[20:15] == 6'd35);
assign row_uram_wea_dec[36]   = (ind_addr[20:15] == 6'd36);
assign row_uram_wea_dec[37]   = (ind_addr[20:15] == 6'd37);
assign row_uram_wea_dec[38]   = (ind_addr[20:15] == 6'd38);
assign row_uram_wea_dec[39]   = (ind_addr[20:15] == 6'd39);
assign row_uram_wea_dec[40]   = (ind_addr[20:15] == 6'd40);
assign row_uram_wea_dec[41]   = (ind_addr[20:15] == 6'd41);
assign row_uram_wea_dec[42]   = (ind_addr[20:15] == 6'd42);
assign row_uram_wea_dec[43]   = (ind_addr[20:15] == 6'd43);
assign row_uram_wea_dec[44]   = (ind_addr[20:15] == 6'd44);
assign row_uram_wea_dec[45]   = (ind_addr[20:15] == 6'd45);
assign row_uram_wea_dec[46]   = (ind_addr[20:15] == 6'd46);
assign row_uram_wea_dec[47]   = (ind_addr[20:15] == 6'd47);
assign row_uram_wea_dec[48]   = (ind_addr[20:15] == 6'd48);
assign row_uram_wea_dec[49]   = (ind_addr[20:15] == 6'd49);
assign row_uram_wea_dec[50]   = (ind_addr[20:15] == 6'd50);
assign row_uram_wea_dec[51]   = (ind_addr[20:15] == 6'd51);
assign row_uram_wea_dec[52]   = (ind_addr[20:15] == 6'd52);
assign row_uram_wea_dec[53]   = (ind_addr[20:15] == 6'd53);
assign row_uram_wea_dec[54]   = (ind_addr[20:15] == 6'd54);
assign row_uram_wea_dec[55]   = (ind_addr[20:15] == 6'd55);
assign row_uram_wea_dec[56]   = (ind_addr[20:15] == 6'd56);
assign row_uram_wea_dec[57]   = (ind_addr[20:15] == 6'd57);
assign row_uram_wea_dec[58]   = (ind_addr[20:15] == 6'd58);
assign row_uram_wea_dec[59]   = (ind_addr[20:15] == 6'd59);
assign row_uram_wea_dec[60]   = (ind_addr[20:15] == 6'd60);
assign row_uram_wea_dec[61]   = (ind_addr[20:15] == 6'd61);
assign row_uram_wea_dec[62]   = (ind_addr[20:15] == 6'd62);
assign row_uram_wea_dec[63]   = (ind_addr[20:15] == 6'd63);

assign col_uram_ena_dec[0]    = (ind_addr[20:15] == 6'd0);
assign col_uram_ena_dec[1]    = (ind_addr[20:15] == 6'd1);
assign col_uram_ena_dec[2]    = (ind_addr[20:15] == 6'd2);
assign col_uram_ena_dec[3]    = (ind_addr[20:15] == 6'd3);
assign col_uram_ena_dec[4]    = (ind_addr[20:15] == 6'd4);
assign col_uram_ena_dec[5]    = (ind_addr[20:15] == 6'd5);
assign col_uram_ena_dec[6]    = (ind_addr[20:15] == 6'd6);
assign col_uram_ena_dec[7]    = (ind_addr[20:15] == 6'd7);
assign col_uram_ena_dec[8]    = (ind_addr[20:15] == 6'd8);
assign col_uram_ena_dec[9]    = (ind_addr[20:15] == 6'd9);
assign col_uram_ena_dec[10]   = (ind_addr[20:15] == 6'd10);
assign col_uram_ena_dec[11]   = (ind_addr[20:15] == 6'd11);
assign col_uram_ena_dec[12]   = (ind_addr[20:15] == 6'd12);
assign col_uram_ena_dec[13]   = (ind_addr[20:15] == 6'd13);
assign col_uram_ena_dec[14]   = (ind_addr[20:15] == 6'd14);
assign col_uram_ena_dec[15]   = (ind_addr[20:15] == 6'd15);
assign col_uram_ena_dec[16]   = (ind_addr[20:15] == 6'd16);
assign col_uram_ena_dec[17]   = (ind_addr[20:15] == 6'd17);
assign col_uram_ena_dec[18]   = (ind_addr[20:15] == 6'd18);
assign col_uram_ena_dec[19]   = (ind_addr[20:15] == 6'd19);
assign col_uram_ena_dec[20]   = (ind_addr[20:15] == 6'd20);
assign col_uram_ena_dec[21]   = (ind_addr[20:15] == 6'd21);
assign col_uram_ena_dec[22]   = (ind_addr[20:15] == 6'd22);
assign col_uram_ena_dec[23]   = (ind_addr[20:15] == 6'd23);
assign col_uram_ena_dec[24]   = (ind_addr[20:15] == 6'd24);
assign col_uram_ena_dec[25]   = (ind_addr[20:15] == 6'd25);
assign col_uram_ena_dec[26]   = (ind_addr[20:15] == 6'd26);
assign col_uram_ena_dec[27]   = (ind_addr[20:15] == 6'd27);
assign col_uram_ena_dec[28]   = (ind_addr[20:15] == 6'd28);
assign col_uram_ena_dec[29]   = (ind_addr[20:15] == 6'd29);
assign col_uram_ena_dec[30]   = (ind_addr[20:15] == 6'd30);
assign col_uram_ena_dec[31]   = (ind_addr[20:15] == 6'd31);
assign col_uram_ena_dec[32]   = (ind_addr[20:15] == 6'd32);
assign col_uram_ena_dec[33]   = (ind_addr[20:15] == 6'd33);
assign col_uram_ena_dec[34]   = (ind_addr[20:15] == 6'd34);
assign col_uram_ena_dec[35]   = (ind_addr[20:15] == 6'd35);
assign col_uram_ena_dec[36]   = (ind_addr[20:15] == 6'd36);
assign col_uram_ena_dec[37]   = (ind_addr[20:15] == 6'd37);
assign col_uram_ena_dec[38]   = (ind_addr[20:15] == 6'd38);
assign col_uram_ena_dec[39]   = (ind_addr[20:15] == 6'd39);
assign col_uram_ena_dec[40]   = (ind_addr[20:15] == 6'd40);
assign col_uram_ena_dec[41]   = (ind_addr[20:15] == 6'd41);
assign col_uram_ena_dec[42]   = (ind_addr[20:15] == 6'd42);
assign col_uram_ena_dec[43]   = (ind_addr[20:15] == 6'd43);
assign col_uram_ena_dec[44]   = (ind_addr[20:15] == 6'd44);
assign col_uram_ena_dec[45]   = (ind_addr[20:15] == 6'd45);
assign col_uram_ena_dec[46]   = (ind_addr[20:15] == 6'd46);
assign col_uram_ena_dec[47]   = (ind_addr[20:15] == 6'd47);
assign col_uram_ena_dec[48]   = (ind_addr[20:15] == 6'd48);
assign col_uram_ena_dec[49]   = (ind_addr[20:15] == 6'd49);
assign col_uram_ena_dec[50]   = (ind_addr[20:15] == 6'd50);
assign col_uram_ena_dec[51]   = (ind_addr[20:15] == 6'd51);
assign col_uram_ena_dec[52]   = (ind_addr[20:15] == 6'd52);
assign col_uram_ena_dec[53]   = (ind_addr[20:15] == 6'd53);
assign col_uram_ena_dec[54]   = (ind_addr[20:15] == 6'd54);
assign col_uram_ena_dec[55]   = (ind_addr[20:15] == 6'd55);
assign col_uram_ena_dec[56]   = (ind_addr[20:15] == 6'd56);
assign col_uram_ena_dec[57]   = (ind_addr[20:15] == 6'd57);
assign col_uram_ena_dec[58]   = (ind_addr[20:15] == 6'd58);
assign col_uram_ena_dec[59]   = (ind_addr[20:15] == 6'd59);
assign col_uram_ena_dec[60]   = (ind_addr[20:15] == 6'd60);
assign col_uram_ena_dec[61]   = (ind_addr[20:15] == 6'd61);
assign col_uram_ena_dec[62]   = (ind_addr[20:15] == 6'd62);
assign col_uram_ena_dec[63]   = (ind_addr[20:15] == 6'd63);

assign col_uram_wea_dec[0]    = (ind_addr[20:15] == 6'd0);
assign col_uram_wea_dec[1]    = (ind_addr[20:15] == 6'd1);
assign col_uram_wea_dec[2]    = (ind_addr[20:15] == 6'd2);
assign col_uram_wea_dec[3]    = (ind_addr[20:15] == 6'd3);
assign col_uram_wea_dec[4]    = (ind_addr[20:15] == 6'd4);
assign col_uram_wea_dec[5]    = (ind_addr[20:15] == 6'd5);
assign col_uram_wea_dec[6]    = (ind_addr[20:15] == 6'd6);
assign col_uram_wea_dec[7]    = (ind_addr[20:15] == 6'd7);
assign col_uram_wea_dec[8]    = (ind_addr[20:15] == 6'd8);
assign col_uram_wea_dec[9]    = (ind_addr[20:15] == 6'd9);
assign col_uram_wea_dec[10]   = (ind_addr[20:15] == 6'd10);
assign col_uram_wea_dec[11]   = (ind_addr[20:15] == 6'd11);
assign col_uram_wea_dec[12]   = (ind_addr[20:15] == 6'd12);
assign col_uram_wea_dec[13]   = (ind_addr[20:15] == 6'd13);
assign col_uram_wea_dec[14]   = (ind_addr[20:15] == 6'd14);
assign col_uram_wea_dec[15]   = (ind_addr[20:15] == 6'd15);
assign col_uram_wea_dec[16]   = (ind_addr[20:15] == 6'd16);
assign col_uram_wea_dec[17]   = (ind_addr[20:15] == 6'd17);
assign col_uram_wea_dec[18]   = (ind_addr[20:15] == 6'd18);
assign col_uram_wea_dec[19]   = (ind_addr[20:15] == 6'd19);
assign col_uram_wea_dec[20]   = (ind_addr[20:15] == 6'd20);
assign col_uram_wea_dec[21]   = (ind_addr[20:15] == 6'd21);
assign col_uram_wea_dec[22]   = (ind_addr[20:15] == 6'd22);
assign col_uram_wea_dec[23]   = (ind_addr[20:15] == 6'd23);
assign col_uram_wea_dec[24]   = (ind_addr[20:15] == 6'd24);
assign col_uram_wea_dec[25]   = (ind_addr[20:15] == 6'd25);
assign col_uram_wea_dec[26]   = (ind_addr[20:15] == 6'd26);
assign col_uram_wea_dec[27]   = (ind_addr[20:15] == 6'd27);
assign col_uram_wea_dec[28]   = (ind_addr[20:15] == 6'd28);
assign col_uram_wea_dec[29]   = (ind_addr[20:15] == 6'd29);
assign col_uram_wea_dec[30]   = (ind_addr[20:15] == 6'd30);
assign col_uram_wea_dec[31]   = (ind_addr[20:15] == 6'd31);
assign col_uram_wea_dec[32]   = (ind_addr[20:15] == 6'd32);
assign col_uram_wea_dec[33]   = (ind_addr[20:15] == 6'd33);
assign col_uram_wea_dec[34]   = (ind_addr[20:15] == 6'd34);
assign col_uram_wea_dec[35]   = (ind_addr[20:15] == 6'd35);
assign col_uram_wea_dec[36]   = (ind_addr[20:15] == 6'd36);
assign col_uram_wea_dec[37]   = (ind_addr[20:15] == 6'd37);
assign col_uram_wea_dec[38]   = (ind_addr[20:15] == 6'd38);
assign col_uram_wea_dec[39]   = (ind_addr[20:15] == 6'd39);
assign col_uram_wea_dec[40]   = (ind_addr[20:15] == 6'd40);
assign col_uram_wea_dec[41]   = (ind_addr[20:15] == 6'd41);
assign col_uram_wea_dec[42]   = (ind_addr[20:15] == 6'd42);
assign col_uram_wea_dec[43]   = (ind_addr[20:15] == 6'd43);
assign col_uram_wea_dec[44]   = (ind_addr[20:15] == 6'd44);
assign col_uram_wea_dec[45]   = (ind_addr[20:15] == 6'd45);
assign col_uram_wea_dec[46]   = (ind_addr[20:15] == 6'd46);
assign col_uram_wea_dec[47]   = (ind_addr[20:15] == 6'd47);
assign col_uram_wea_dec[48]   = (ind_addr[20:15] == 6'd48);
assign col_uram_wea_dec[49]   = (ind_addr[20:15] == 6'd49);
assign col_uram_wea_dec[50]   = (ind_addr[20:15] == 6'd50);
assign col_uram_wea_dec[51]   = (ind_addr[20:15] == 6'd51);
assign col_uram_wea_dec[52]   = (ind_addr[20:15] == 6'd52);
assign col_uram_wea_dec[53]   = (ind_addr[20:15] == 6'd53);
assign col_uram_wea_dec[54]   = (ind_addr[20:15] == 6'd54);
assign col_uram_wea_dec[55]   = (ind_addr[20:15] == 6'd55);
assign col_uram_wea_dec[56]   = (ind_addr[20:15] == 6'd56);
assign col_uram_wea_dec[57]   = (ind_addr[20:15] == 6'd57);
assign col_uram_wea_dec[58]   = (ind_addr[20:15] == 6'd58);
assign col_uram_wea_dec[59]   = (ind_addr[20:15] == 6'd59);
assign col_uram_wea_dec[60]   = (ind_addr[20:15] == 6'd60);
assign col_uram_wea_dec[61]   = (ind_addr[20:15] == 6'd61);
assign col_uram_wea_dec[62]   = (ind_addr[20:15] == 6'd62);
assign col_uram_wea_dec[63]   = (ind_addr[20:15] == 6'd63);

assign init_en = 1;
// Write to URAMs when Write data is valid
always @(posedge S_AXI_ACLK) begin
   if (~S_AXI_ARESETN | soft_reset) begin
       row_uram_ena     <= 64'b0; 
       row_uram_wea     <= 64'b0;
       row_uram_addra   <= 12'b0;
       row_uram_dina    <= 64'b0;
       col_uram_ena     <= 64'b0;
       col_uram_wea     <= 64'b0;
       col_uram_addra   <= 12'b0;
       col_uram_dina    <= 64'b0;
   end
   else begin 
       //case (ind_addr[19:18]) 
       case (ind_addr[22:21]) 
          2'b00 : begin
             //row_uram_ena   <= WrDataValid & ~init_en ? row_uram_ena_dec  : 64'b0; 
             //row_uram_wea   <= WrDataValid & ~init_en ? row_uram_wea_dec  : 64'b0; 
             row_uram_ena   <= WrDataValid  ? row_uram_ena_dec  : 64'b0; 
             row_uram_wea   <= WrDataValid  ? row_uram_wea_dec  : 64'b0; 
	     //row_uram_addra <= {3'b0, ind_addr[11:3]};
	     row_uram_addra <= ind_addr[14:3];
	     row_uram_dina  <= Wr_Data;
             col_uram_ena   <= 64'b0; 
             col_uram_wea   <= 64'b0; 
	     col_uram_addra <= 12'b0; 
	     col_uram_dina  <= 64'b0;
	  end
	  2'b01 : begin
             row_uram_ena   <= 64'b0; 
             row_uram_wea   <= 64'b0; 
	     row_uram_addra <= 14'b0; 
	     row_uram_dina  <= 64'b0;
             //col_uram_ena   <= WrDataValid & ~init_en ? col_uram_ena_dec : 8'b0; 
             //col_uram_wea   <= WrDataValid & ~init_en ? col_uram_wea_dec : 8'b0; 
             col_uram_ena   <= WrDataValid ? col_uram_ena_dec : 64'b0; 
             col_uram_wea   <= WrDataValid ? col_uram_wea_dec : 64'b0; 
	     col_uram_addra <= ind_addr[14:3]; 
	     col_uram_dina  <= Wr_Data;
	  end
	  default : begin
            row_uram_ena     <= 64'b0; 
            row_uram_wea     <= 64'b0;
            row_uram_addra   <= 12'b0;
            row_uram_dina    <= 64'b0;
            col_uram_ena     <= 64'b0;
            col_uram_wea     <= 64'b0;
            col_uram_addra   <= 12'b0;
            col_uram_dina    <= 64'b0;
	  end
       endcase
   end
end   
wire uram_ena_ack_toggle;
assign uram_ena_ack_toggle = (~uram_ena_ack_d & uram_ena_ack)|(uram_ena_ack_d & ~uram_ena_ack);
reg axi_bvalid_int;
   always @( posedge S_AXI_ACLK ) begin
	if ( S_AXI_ARESETN == 1'b0 ) begin
	     axi_bvalid  <= 0;
	     axi_bvalid_int  <= 0;
	     axi_bresp   <= 2'b0;
	end 
	else begin
	    if (second_data) begin 
	      if ((~uram_ena_ack_d & uram_ena_ack)|(uram_ena_ack_d & ~uram_ena_ack)) begin
	             //axi_bvalid <= 1'b1;
	             axi_bvalid_int <= 1'b0;
	             axi_bvalid <= axi_bvalid_int;
	             axi_bresp  <= 2'b0; 
	      end
	    end
	    else if ((axi_awready && S_AXI_AWVALID && ~axi_bvalid && axi_wready && S_AXI_WVALID)) begin
	             axi_bvalid_int <= 1'b1;
	             /*if (second_data) begin
	               if ((~uram_ena_ack_d & uram_ena_ack)|(uram_ena_ack_d & ~uram_ena_ack)) begin
	                  //axi_bvalid <= 1'b1;
	                  axi_bvalid <= axi_bvalid_int;
	                  axi_bresp  <= 2'b0; 
	               end
		     end
		     else begin*/
	               axi_bvalid <= 1'b1;
	               axi_bresp  <= 2'b0; 
		     //end
	    end
	    else begin
	         if (S_AXI_BREADY && axi_bvalid) 
	           begin
	              axi_bvalid <= 1'b0; 
	              //axi_bvalid_int <= 1'b0; 
	           end  
	    end
	  end
     end   
/*	     if (axi_awready && S_AXI_AWVALID && ~axi_bvalid && axi_wready && S_AXI_WVALID)begin 
	       //if (axi_awaddr[5:2] == 4'h8 & ~(even_oddn & even_oddn_d)) begin
	       if (axi_awaddr[5:2] == 4'h8 & second_data) begin
	         if ((~uram_ena_ack_d & uram_ena_ack)|(uram_ena_ack_d & ~uram_ena_ack)) begin
	            axi_bvalid <= 1'b1;
	            axi_bresp  <= 2'b0; 
	         end 
	         else begin
	            axi_bvalid <= 1'b0;
	         end
	       end
	       else begin
	         axi_bvalid <= 1'b1;
	         axi_bresp  <= 2'b0; 
	       end
	     end */

assign op_uram_enb_dec[0]    = (ind_rdaddr[20:15] == 6'd0);
assign op_uram_enb_dec[1]    = (ind_rdaddr[20:15] == 6'd1);
assign op_uram_enb_dec[2]    = (ind_rdaddr[20:15] == 6'd2);
assign op_uram_enb_dec[3]    = (ind_rdaddr[20:15] == 6'd3);
assign op_uram_enb_dec[4]    = (ind_rdaddr[20:15] == 6'd4);
assign op_uram_enb_dec[5]    = (ind_rdaddr[20:15] == 6'd5);
assign op_uram_enb_dec[6]    = (ind_rdaddr[20:15] == 6'd6);
assign op_uram_enb_dec[7]    = (ind_rdaddr[20:15] == 6'd7);
assign op_uram_enb_dec[8]    = (ind_rdaddr[20:15] == 6'd8);
assign op_uram_enb_dec[9]    = (ind_rdaddr[20:15] == 6'd9);
assign op_uram_enb_dec[10]   = (ind_rdaddr[20:15] == 6'd10);
assign op_uram_enb_dec[11]   = (ind_rdaddr[20:15] == 6'd11);
assign op_uram_enb_dec[12]   = (ind_rdaddr[20:15] == 6'd12);
assign op_uram_enb_dec[13]   = (ind_rdaddr[20:15] == 6'd13);
assign op_uram_enb_dec[14]   = (ind_rdaddr[20:15] == 6'd14);
assign op_uram_enb_dec[15]   = (ind_rdaddr[20:15] == 6'd15);
assign op_uram_enb_dec[16]   = (ind_rdaddr[20:15] == 6'd16);
assign op_uram_enb_dec[17]   = (ind_rdaddr[20:15] == 6'd17);
assign op_uram_enb_dec[18]   = (ind_rdaddr[20:15] == 6'd18);
assign op_uram_enb_dec[19]   = (ind_rdaddr[20:15] == 6'd19);
assign op_uram_enb_dec[20]   = (ind_rdaddr[20:15] == 6'd20);
assign op_uram_enb_dec[21]   = (ind_rdaddr[20:15] == 6'd21);
assign op_uram_enb_dec[22]   = (ind_rdaddr[20:15] == 6'd22);
assign op_uram_enb_dec[23]   = (ind_rdaddr[20:15] == 6'd23);
assign op_uram_enb_dec[24]   = (ind_rdaddr[20:15] == 6'd24);
assign op_uram_enb_dec[25]   = (ind_rdaddr[20:15] == 6'd25);
assign op_uram_enb_dec[26]   = (ind_rdaddr[20:15] == 6'd26);
assign op_uram_enb_dec[27]   = (ind_rdaddr[20:15] == 6'd27);
assign op_uram_enb_dec[28]   = (ind_rdaddr[20:15] == 6'd28);
assign op_uram_enb_dec[29]   = (ind_rdaddr[20:15] == 6'd29);
assign op_uram_enb_dec[30]   = (ind_rdaddr[20:15] == 6'd30);
assign op_uram_enb_dec[31]   = (ind_rdaddr[20:15] == 6'd31);
assign op_uram_enb_dec[32]   = (ind_rdaddr[20:15] == 6'd32);
assign op_uram_enb_dec[33]   = (ind_rdaddr[20:15] == 6'd33);
assign op_uram_enb_dec[34]   = (ind_rdaddr[20:15] == 6'd34);
assign op_uram_enb_dec[35]   = (ind_rdaddr[20:15] == 6'd35);
assign op_uram_enb_dec[36]   = (ind_rdaddr[20:15] == 6'd36);
assign op_uram_enb_dec[37]   = (ind_rdaddr[20:15] == 6'd37);
assign op_uram_enb_dec[38]   = (ind_rdaddr[20:15] == 6'd38);
assign op_uram_enb_dec[39]   = (ind_rdaddr[20:15] == 6'd39);
assign op_uram_enb_dec[40]   = (ind_rdaddr[20:15] == 6'd40);
assign op_uram_enb_dec[41]   = (ind_rdaddr[20:15] == 6'd41);
assign op_uram_enb_dec[42]   = (ind_rdaddr[20:15] == 6'd42);
assign op_uram_enb_dec[43]   = (ind_rdaddr[20:15] == 6'd43);
assign op_uram_enb_dec[44]   = (ind_rdaddr[20:15] == 6'd44);
assign op_uram_enb_dec[45]   = (ind_rdaddr[20:15] == 6'd45);
assign op_uram_enb_dec[46]   = (ind_rdaddr[20:15] == 6'd46);
assign op_uram_enb_dec[47]   = (ind_rdaddr[20:15] == 6'd47);
assign op_uram_enb_dec[48]   = (ind_rdaddr[20:15] == 6'd48);
assign op_uram_enb_dec[49]   = (ind_rdaddr[20:15] == 6'd49);
assign op_uram_enb_dec[50]   = (ind_rdaddr[20:15] == 6'd50);
assign op_uram_enb_dec[51]   = (ind_rdaddr[20:15] == 6'd51);
assign op_uram_enb_dec[52]   = (ind_rdaddr[20:15] == 6'd52);
assign op_uram_enb_dec[53]   = (ind_rdaddr[20:15] == 6'd53);
assign op_uram_enb_dec[54]   = (ind_rdaddr[20:15] == 6'd54);
assign op_uram_enb_dec[55]   = (ind_rdaddr[20:15] == 6'd55);
assign op_uram_enb_dec[56]   = (ind_rdaddr[20:15] == 6'd56);
assign op_uram_enb_dec[57]   = (ind_rdaddr[20:15] == 6'd57);
assign op_uram_enb_dec[58]   = (ind_rdaddr[20:15] == 6'd58);
assign op_uram_enb_dec[59]   = (ind_rdaddr[20:15] == 6'd59);
assign op_uram_enb_dec[60]   = (ind_rdaddr[20:15] == 6'd60);
assign op_uram_enb_dec[61]   = (ind_rdaddr[20:15] == 6'd61);
assign op_uram_enb_dec[62]   = (ind_rdaddr[20:15] == 6'd62);
assign op_uram_enb_dec[63]   = (ind_rdaddr[20:15] == 6'd63);

// As a temporary workaround, where we are reading only first column submatrix, 
// Addresses 15:11 are used to decode lower 32 OP URAMs
// Upper 32 OP URAMs are not read at all
/*
assign op_uram_enb_dec[0]    = (ind_rdaddr[15:11] == 5'd0);
assign op_uram_enb_dec[1]    = (ind_rdaddr[15:11] == 5'd1);
assign op_uram_enb_dec[2]    = (ind_rdaddr[15:11] == 5'd2);
assign op_uram_enb_dec[3]    = (ind_rdaddr[15:11] == 5'd3);
assign op_uram_enb_dec[4]    = (ind_rdaddr[15:11] == 5'd4);
assign op_uram_enb_dec[5]    = (ind_rdaddr[15:11] == 5'd5);
assign op_uram_enb_dec[6]    = (ind_rdaddr[15:11] == 5'd6);
assign op_uram_enb_dec[7]    = (ind_rdaddr[15:11] == 5'd7);
assign op_uram_enb_dec[8]    = (ind_rdaddr[15:11] == 5'd8);
assign op_uram_enb_dec[9]    = (ind_rdaddr[15:11] == 5'd9);
assign op_uram_enb_dec[10]   = (ind_rdaddr[15:11] == 5'd10);
assign op_uram_enb_dec[11]   = (ind_rdaddr[15:11] == 5'd11);
assign op_uram_enb_dec[12]   = (ind_rdaddr[15:11] == 5'd12);
assign op_uram_enb_dec[13]   = (ind_rdaddr[15:11] == 5'd13);
assign op_uram_enb_dec[14]   = (ind_rdaddr[15:11] == 5'd14);
assign op_uram_enb_dec[15]   = (ind_rdaddr[15:11] == 5'd15);
assign op_uram_enb_dec[16]   = (ind_rdaddr[15:11] == 5'd16);
assign op_uram_enb_dec[17]   = (ind_rdaddr[15:11] == 5'd17);
assign op_uram_enb_dec[18]   = (ind_rdaddr[15:11] == 5'd18);
assign op_uram_enb_dec[19]   = (ind_rdaddr[15:11] == 5'd19);
assign op_uram_enb_dec[20]   = (ind_rdaddr[15:11] == 5'd20);
assign op_uram_enb_dec[21]   = (ind_rdaddr[15:11] == 5'd21);
assign op_uram_enb_dec[22]   = (ind_rdaddr[15:11] == 5'd22);
assign op_uram_enb_dec[23]   = (ind_rdaddr[15:11] == 5'd23);
assign op_uram_enb_dec[24]   = (ind_rdaddr[15:11] == 5'd24);
assign op_uram_enb_dec[25]   = (ind_rdaddr[15:11] == 5'd25);
assign op_uram_enb_dec[26]   = (ind_rdaddr[15:11] == 5'd26);
assign op_uram_enb_dec[27]   = (ind_rdaddr[15:11] == 5'd27);
assign op_uram_enb_dec[28]   = (ind_rdaddr[15:11] == 5'd28);
assign op_uram_enb_dec[29]   = (ind_rdaddr[15:11] == 5'd29);
assign op_uram_enb_dec[30]   = (ind_rdaddr[15:11] == 5'd30);
assign op_uram_enb_dec[31]   = (ind_rdaddr[15:11] == 5'd31);
assign op_uram_enb_dec[32]   = 1'b0; 
assign op_uram_enb_dec[33]   = 1'b0; 
assign op_uram_enb_dec[34]   = 1'b0; 
assign op_uram_enb_dec[35]   = 1'b0; 
assign op_uram_enb_dec[36]   = 1'b0; 
assign op_uram_enb_dec[37]   = 1'b0; 
assign op_uram_enb_dec[38]   = 1'b0; 
assign op_uram_enb_dec[39]   = 1'b0; 
assign op_uram_enb_dec[40]   = 1'b0; 
assign op_uram_enb_dec[41]   = 1'b0; 
assign op_uram_enb_dec[42]   = 1'b0; 
assign op_uram_enb_dec[43]   = 1'b0; 
assign op_uram_enb_dec[44]   = 1'b0; 
assign op_uram_enb_dec[45]   = 1'b0;
assign op_uram_enb_dec[46]   = 1'b0;
assign op_uram_enb_dec[47]   = 1'b0;
assign op_uram_enb_dec[48]   = 1'b0;
assign op_uram_enb_dec[49]   = 1'b0;
assign op_uram_enb_dec[50]   = 1'b0;
assign op_uram_enb_dec[51]   = 1'b0;
assign op_uram_enb_dec[52]   = 1'b0;
assign op_uram_enb_dec[53]   = 1'b0;
assign op_uram_enb_dec[54]   = 1'b0;
assign op_uram_enb_dec[55]   = 1'b0;
assign op_uram_enb_dec[56]   = 1'b0;
assign op_uram_enb_dec[57]   = 1'b0;
assign op_uram_enb_dec[58]   = 1'b0;
assign op_uram_enb_dec[59]   = 1'b0;
assign op_uram_enb_dec[60]   = 1'b0;
assign op_uram_enb_dec[61]   = 1'b0;
assign op_uram_enb_dec[62]   = 1'b0;
assign op_uram_enb_dec[63]   = 1'b0;
*/

   always @( posedge S_AXI_ACLK or negedge S_AXI_ARESETN)
     begin
	if ( S_AXI_ARESETN == 1'b0 )
	  begin
	     axi_arready <= 1'b0;
	     chan_araddr_valid <= 1'b0;
	  end 
	else
	  begin    
	     if (~axi_arready && S_AXI_ARVALID)
	       begin
	          axi_arready <= 1'b1;
		  chan_araddr_valid <= 1'b1;
	       end
	     else
	       begin
	          axi_arready <= 1'b0;
	       end
	  end 
     end       

   always @( posedge S_AXI_ACLK )
     begin
	if (~axi_arready && S_AXI_ARVALID)
	  begin
	     axi_araddr  <= S_AXI_ARADDR;
	  end
     end       
 always @(posedge S_AXI_ACLK) begin
   if (~S_AXI_ARESETN) begin
      RdCmdValid <= 1'b0;
   end
   else begin
      RdCmdValid <= S_AXI_ARVALID & S_AXI_ARREADY;
   end
 end

 always @(posedge S_AXI_ACLK) begin
   if (~S_AXI_ARESETN | soft_reset) begin
      op_uram_read <= 1'b0;
   end
   else begin
      //op_uram_read <= ((S_AXI_ARVALID & S_AXI_ARREADY & (S_AXI_ARADDR[4:2] == 3'h5)) | (op_uram_read & ~any_op_uram_enb));
      op_uram_read <= ((S_AXI_ARVALID & S_AXI_ARREADY & (S_AXI_ARADDR[5:2] == 4'h9)) | (~op_uram_doutb_valid & op_uram_doutb_valid_d & even_odd));// & ~any_op_uram_enb);
   end
 end
 
   always @( posedge S_AXI_ACLK )
     begin
	if ( S_AXI_ARESETN == 1'b0 )
	  begin
	     axi_rresp  <= 0;
	  end 
	else
	  begin    
	     if (axi_arready && S_AXI_ARVALID && ~S_AXI_RVALID)
	       begin
	          axi_rresp  <= 2'b0; 
	       end   
	  end
     end    

   assign slv_reg_rden = axi_arready & S_AXI_ARVALID & ~S_AXI_RVALID;

   always @(posedge S_AXI_ACLK) begin
     if (S_AXI_ARESETN == 1'b0) begin
        ps_RDATA_int <= 64'b0;
        ps_RVALID_int <= 1'b0;
     end
     else begin
        if (RdCmdValid & ~op_uram_read) begin
	      case ( axi_araddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
	  6'h00   : begin
	  //ps_RDATA_int <= {slv_reg0[31:8],ap_autorestart,slv_reg0[6:5],ap_continue,ap_ready,ap_idle,ap_done,ap_start};
	  ps_RDATA_int <= {slv_reg0[31:2],ap_done,slv_reg0[0]};
	  ps_RVALID_int <= 1'b1;
	  end
	  6'h01   : begin
	               ps_RDATA_int <= slv_reg1; 
	               ps_RVALID_int <= 1'b1;
                    end
	  6'h02   : begin 
	               ps_RDATA_int <= slv_reg2; 
	               ps_RVALID_int <= 1'b1;
                    end
	  6'h03   : begin 
	               ps_RDATA_int <= slv_reg3; 
	               ps_RVALID_int <= 1'b1;
                    end
	  6'h04   : begin
	               ps_RDATA_int <= {30'h0, Addr_AutoIncr, DUT_en};
	               ps_RVALID_int <= 1'b1;
	            end
	  6'h05   : begin
	               ps_RDATA_int <= {31'h0, dut_done};
	               ps_RVALID_int <= 1'b1;
	            end
	  6'h06   :  begin
	                ps_RDATA_int <= {30'h0, RWn, CmdValid};
	                ps_RVALID_int <= 1'b1;
	             end
	  6'h07   : begin
	               ps_RDATA_int <= ind_addr;
	               ps_RVALID_int <= 1'b1;
	            end   
	  6'h08   : begin
   	               ps_RDATA_int <= Wr_Data;
   	               ps_RVALID_int <= 1'b1;
   	            end   
          6'h0b   : begin
    	               ps_RDATA_int <= {27'h0, num_mat_done};
    	               ps_RVALID_int <= 1'b1;
   	            end   
          6'h0c   : begin
    	               ps_RDATA_int <= active_clk_count;
    	               ps_RVALID_int <= 1'b1;
   	            end   
          default : begin
    	               ps_RDATA_int <= {27'h0, num_mat_done};
    	               ps_RVALID_int <= 1'b1;
   	            end   
	endcase
     end
     else begin
      ps_RDATA_int  <= 32'b0;
      ps_RVALID_int <= 1'b0;
     end
   end 
end    

   // Indirect read address register
   always @(posedge S_AXI_ACLK) begin
      if (~S_AXI_ARESETN | soft_reset) begin
         ind_rdaddr    <= 32'b0;
      end
      else if (S_AXI_WVALID & S_AXI_WREADY & (axi_awaddr[5:2] == 4'ha)) begin
         ind_rdaddr <= S_AXI_WDATA[31:0];
	 $display("into indirect addressing");
      end
      //else if (op_uram_read) begin
      //else if (op_uram_doutb_ack) begin
      else if (~op_uram_doutb_valid & op_uram_doutb_valid_d) begin
         ind_rdaddr  <= ind_rdaddr + 2;
      end
   end   

   reg op_uram_doutb_valid_d;
   reg uram_ena_ack_d;
   always @(posedge S_AXI_ACLK) begin
      if (~S_AXI_ARESETN | soft_reset) begin
        op_uram_doutb_valid_d <= 'b0;
	uram_ena_ack_d        <= 'b0;
      end
      else begin
	op_uram_doutb_valid_d <= op_uram_doutb_valid;
	uram_ena_ack_d        <= uram_ena_ack;
      end
   end  

   always @(posedge S_AXI_ACLK) begin
      if (~S_AXI_ARESETN | soft_reset) begin
        op_uram_doutb_ack <= 'b0;
      end
      else if (op_uram_doutb_valid & ~op_uram_doutb_valid_d & ~op_uram_doutb_ack) begin
        op_uram_doutb_ack <= 'b1;
      end
      else begin
        op_uram_doutb_ack <= 'b0;
      end
   end   
   
   assign any_op_uram_enb = |op_uram_enb;

   //always @(posedge S_AXI_ACLK) begin
   //   if (~S_AXI_ARESETN ) begin
   //      ps_RVALID_int2 <= 1'b0;
   //      ps_RDATA_int2  <= 32'b0;
   //   end
   //   else begin
   //      ps_RVALID_int2 <= op_uram_enb5;
   //      if (op_uram_enb4) begin 
   //         ps_RDATA_int2 <= {16'h0, op_uram_doutb};
   //      end
   //      else if (op_uram_enb5) begin 
   //         ps_RDATA_int2 <= {op_uram_doutb, ps_RDATA_int2[15:0]};
   //      end
   //   end
   //end

   reg even_odd;
   always @(posedge S_AXI_ACLK) begin
      if (~S_AXI_ARESETN ) begin
         even_odd       <= 1'b0;
         ps_RVALID_int2 <= 1'b0;
         ps_RDATA_int2  <= 32'b0;
      end
      else begin
         //ps_RVALID_int2 <= op_uram_doutb_valid & even_odd;
         //ps_RVALID_int2 <= op_uram_doutb_valid & ~op_uram_doutb_valid_d & even_odd;
         //ps_RVALID_int2 <= op_uram_doutb_ack & even_odd;
         if (op_uram_doutb_valid & ~op_uram_doutb_valid_d) begin 
         //if (op_uram_doutb_ack) begin 
	   even_odd <= ~ even_odd; 
           
	   if (~even_odd) begin 
              ps_RDATA_int2 <= {16'h0, op_uram_doutb};
           end
           else begin 
              ps_RDATA_int2 <= {op_uram_doutb, ps_RDATA_int2[15:0]};
	      ps_RVALID_int2<= 1'b1;
           end
         end
	 else begin
	      ps_RVALID_int2<= 1'b0;
	 end
      end
   end
   //always @(posedge S_AXI_ACLK) begin
   //   if (~S_AXI_ARESETN | soft_reset ) begin
   //      even_oddn_d       <= 1'b0;
   //      second_data <= 'b0;
   //   end
   //   else begin
   //      even_oddn_d       <= even_oddn;
   //      if (axi_awaddr[5:2]=='h8 & axi_awready)
   //      begin
   //        second_data <= ~second_data;
   //      end
   //   end
   //end

   always @(posedge S_AXI_ACLK) begin
      if (~S_AXI_ARESETN | soft_reset ) begin
         op_uram_addrb <= 14'b0;
         op_uram_enb   <= 64'b0;
         op_uram_enb1   <= 1'b0;
         op_uram_enb2   <= 1'b0;
         op_uram_enb3   <= 1'b0;
         op_uram_enb4   <= 1'b0;
         op_uram_enb5   <= 1'b0;
         op_uram_read_d <= 1'b0;
      end
      else begin
         op_uram_read_d <= op_uram_read;
         op_uram_enb1   <= op_uram_read & ~op_uram_read_d; 
         op_uram_enb2   <= op_uram_enb1;
         op_uram_enb3   <= op_uram_enb2;
         op_uram_enb4   <= op_uram_enb3;
         op_uram_enb5   <= op_uram_enb4;
         op_uram_enb    <= op_uram_read ? op_uram_enb_dec : 64'b0;;
         //op_uram_addrb <=  ind_rdaddr[10:1];
         op_uram_addrb <=  ind_rdaddr[14:1];
      end
   end
   
   always @(posedge S_AXI_ACLK) begin
     if (~S_AXI_ARESETN) begin
       S_AXI_RVALID <= 1'b0;
       S_AXI_RDATA  <= 32'b0;
       //ps_RLAST  <= 1'b0;
     end
     else begin
       S_AXI_RVALID <= ((ps_RVALID_int | ps_RVALID_int2) | (S_AXI_RVALID & ~S_AXI_RREADY));
       S_AXI_RDATA  <= (({32{ps_RVALID_int}} & ps_RDATA_int) | ({32{ps_RVALID_int2}} & ps_RDATA_int2)); 
     end
   end  
endmodule
