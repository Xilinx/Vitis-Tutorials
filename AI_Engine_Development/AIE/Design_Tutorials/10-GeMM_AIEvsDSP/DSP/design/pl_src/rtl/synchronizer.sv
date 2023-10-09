module synchronizer #
  (
   // Width of S_AXI data bus
   parameter integer C_S_AXI_DATA_WIDTH	= 32,
   // Width of S_AXI address bus
   parameter integer C_S_AXI_ADDR_WIDTH	= 10
   )
   (
    
    input                clk,
    //input                dsp_clk,
    input                S_AXI_ACLK,
    input                reset,
//S_AXI_ACLK to clk
    input 		 soft_reset_i, 
    // Row URAM Write interface 
    input      [63:0]    row_uram_ena_i,
    input      [63:0]    row_uram_wea_i,

    // Col URAM Write Interface
    input      [63:0]    col_uram_ena_i,
    input      [63:0]    col_uram_wea_i,

    //OP URAM Read Interface
    input      [63:0]    op_uram_enb_i,

    input                ps_app_done_i,
    output reg           dut_done_o,
    output reg [4:0]     num_mat_done_o,

    //output reg           mem_init_done_o,
    output reg [31:0]    active_clk_count_o,



//clk to S_AXI_ACLK
    output reg		 soft_ored_reset_o, 
    // Row URAM Write interface 
    output reg [63:0]    row_uram_ena_o,
    output reg [63:0]    row_uram_wea_o,

    // Col URAM Write Interface
    output reg [63:0]    col_uram_ena_o,
    output reg [63:0]    col_uram_wea_o,

    //OP URAM Read Interface
    output reg [63:0]    op_uram_enb_o,

    output reg           ps_app_done_o,
    input                dut_done_i,
    input      [4:0]     num_mat_done_i,

    input      [31:0]    active_clk_count_i,

    output reg [63:0]    op_uram_doutb_valid_ps, //sync with S_AXI_ACLK
    output reg [63:0]    op_uram_doutb_valid, //sync with clk - input to op URAM
    input                op_uram_doutb_ack_i,
    
    output reg           uram_ena_ack_o
    );

    // w.r.t dsp_clk
    //reg		       soft_reset_dsp_int; 
    // w.r.t clk
    reg		       soft_ored_reset_int; 
    reg      [63:0]    row_uram_ena_int;
    reg      [63:0]    row_uram_wea_int;
    reg      [63:0]    col_uram_ena_int;
    reg      [63:0]    col_uram_wea_int;
    reg      [63:0]    op_uram_enb_int;
    reg                ps_app_done_int;

     // w.r.t S_AXI_ACLK
    reg                dut_done_int;
    reg      [4:0]     num_mat_done_int;
    reg      [31:0]    active_clk_count_int;


always @(posedge S_AXI_ACLK) begin
  if (reset) begin
    dut_done_int         <= 'b0;
    num_mat_done_int     <= 'b0;
    active_clk_count_int <= 'b0;

    dut_done_o           <= 'b0; 
    num_mat_done_o       <= 'b0; 
    active_clk_count_o   <= 'b0; 
  end  
  else begin
    dut_done_int         <= dut_done_i;
    num_mat_done_int     <= num_mat_done_i;
    active_clk_count_int <= active_clk_count_i;

    dut_done_o           <= dut_done_int;
    num_mat_done_o       <= num_mat_done_int;
    active_clk_count_o   <= active_clk_count_int;
  end  
end  

//always @(posedge dsp_clk) begin
//  if (reset) begin
//    soft_reset_dsp_int     <= 'b0;
//  end  
//  else begin
//    soft_reset_dsp_int     <= soft_reset_i;
//    soft_reset_dsp_o       <= soft_reset_dsp_int;
//  end  
//end

always @(posedge clk) begin
    soft_ored_reset_int<= soft_reset_i | reset;
    soft_ored_reset_o  <= soft_ored_reset_int;
end
always @(posedge clk) begin
  //if (reset) begin
  if (soft_ored_reset_o) begin
    //soft_ored_reset_int<= 'b0;
    row_uram_ena_int   <= 'b0;
    row_uram_wea_int   <= 'b0;
    col_uram_ena_int   <= 'b0;
    col_uram_wea_int   <= 'b0;
    ps_app_done_int    <= 'b0;

    //soft_ored_reset_o  <= 'b0;
    row_uram_ena_o     <= 'b0;
    row_uram_wea_o     <= 'b0;
    col_uram_ena_o     <= 'b0;
    col_uram_wea_o     <= 'b0;
    ps_app_done_o      <= 'b0;

  end  
  else begin
    //soft_ored_reset_int<= soft_reset_i | reset;
    row_uram_ena_int   <= row_uram_ena_i;
    row_uram_wea_int   <= row_uram_wea_i;
    col_uram_ena_int   <= col_uram_ena_i;
    col_uram_wea_int   <= col_uram_wea_i;
    ps_app_done_int    <= ps_app_done_i;

    //soft_ored_reset_o  <= soft_ored_reset_int;
    row_uram_ena_o     <= row_uram_ena_int;
    row_uram_wea_o     <= row_uram_wea_int;
    col_uram_ena_o     <= col_uram_ena_int;
    col_uram_wea_o     <= col_uram_wea_int;
    ps_app_done_o      <= ps_app_done_int;

  end  
end

reg [63:0] op_uram_enb_d;
reg [63:0] op_uram_enb_d1;
reg [63:0] op_uram_enb_d2;
//reg [63:0] op_uram_doutb_valid;
reg [63:0] op_uram_doutb_valid_int;
reg op_uram_doutb_ack;
reg op_uram_doutb_ack_int;

always @(posedge clk) begin
  //if (reset) begin
  if (soft_ored_reset_o) begin
    op_uram_enb_int    <= 'b0;
    op_uram_enb_o      <= 'b0;
    op_uram_enb_d      <= 'b0;
    op_uram_enb_d1     <= 'b0;
    op_uram_enb_d2     <= 'b0;
  end  
  else begin
    op_uram_enb_int    <= op_uram_enb_i;
    op_uram_enb_o      <= op_uram_enb_int;

    op_uram_enb_d      <= op_uram_enb_o;
    op_uram_enb_d1     <= op_uram_enb_d; 
    op_uram_enb_d2     <= op_uram_enb_d1; 
  end  
end
genvar i; 
for (i =0; i<64; i=i+1) begin
always @(posedge clk) begin
  //if (reset) begin
  if (soft_ored_reset_o) begin
    op_uram_doutb_valid [i] <= 'b0;
  end  
  else begin
    if (op_uram_doutb_ack) begin
      op_uram_doutb_valid[i] <= 1'b0;
    end
    else if (op_uram_enb_d1[i]) begin
      op_uram_doutb_valid[i] <= 1'b1;
    end
  end  
end
end

always @(posedge S_AXI_ACLK) begin
  if (reset) begin
    op_uram_doutb_valid_int <= 'b0;
    op_uram_doutb_valid_ps <= 'b0;
  end  
  else begin
    op_uram_doutb_valid_int <= op_uram_doutb_valid;
    op_uram_doutb_valid_ps <= op_uram_doutb_valid_int;
  end  
end

always @(posedge clk) begin
  //if (reset) begin
  if (soft_ored_reset_o) begin
    op_uram_doutb_ack_int <= 'b0;
    op_uram_doutb_ack     <= 'b0;
  end  
  else begin
    op_uram_doutb_ack_int <= op_uram_doutb_ack_i;
    op_uram_doutb_ack     <= op_uram_doutb_ack_int;
  end  
end  
reg uram_ena_ack_int;
reg uram_ena_ack;
always @(posedge S_AXI_ACLK) begin
  if (reset) begin
    uram_ena_ack_int <= 'b0;
    uram_ena_ack_o   <= 'b0;
  end  
  else begin
    uram_ena_ack_int <= uram_ena_ack;
    uram_ena_ack_o   <= uram_ena_ack_int;
  end  
end  
reg col_uram_ena_o_d;
reg row_uram_ena_o_d;
always @(posedge clk) begin
  if (soft_ored_reset_o) begin
    uram_ena_ack     <= 'b0;
    col_uram_ena_o_d <= 'b0;
    row_uram_ena_o_d <= 'b0;
  end  
  else begin
    col_uram_ena_o_d <= | col_uram_ena_o;
    row_uram_ena_o_d <= | row_uram_ena_o;
    if ((~col_uram_ena_o_d & |col_uram_ena_o) | (~row_uram_ena_o_d & |row_uram_ena_o)) begin
    //if ((col_uram_ena_o) | (row_uram_ena_o)) begin
      uram_ena_ack     <= ~uram_ena_ack;
    end
  end  
end

endmodule 
