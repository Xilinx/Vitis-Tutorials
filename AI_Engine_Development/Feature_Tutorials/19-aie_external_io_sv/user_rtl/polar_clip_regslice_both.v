// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================

`timescale 1ns/1ps

module polar_clip_regslice_both
#(parameter 
    DataWidth=32
)(
    input ap_clk ,
    input ap_rst,

    input [DataWidth-1:0] data_in , 
    input vld_in , 
    output ack_in ,
    output [DataWidth-1:0] data_out, 
    output vld_out,
    input ack_out,
    output apdone_blk
);
 

reg   [1:0] B_V_data_1_state;
wire   [DataWidth-1:0] B_V_data_1_data_in;
reg   [DataWidth-1:0] B_V_data_1_data_out;
wire    B_V_data_1_vld_reg;
wire    B_V_data_1_vld_in;
wire    B_V_data_1_vld_out;
reg   [DataWidth-1:0] B_V_data_1_payload_A;
reg   [DataWidth-1:0] B_V_data_1_payload_B;
reg    B_V_data_1_sel_rd;
reg    B_V_data_1_sel_wr;
wire    B_V_data_1_sel;
wire    B_V_data_1_load_A;
wire    B_V_data_1_load_B;
wire    B_V_data_1_state_cmp_full;
wire    B_V_data_1_ack_in;
wire    B_V_data_1_ack_out;

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        B_V_data_1_sel_rd <= 1'b0;
    end else begin
        if (((1'b1 == B_V_data_1_vld_out) & (1'b1 == B_V_data_1_ack_out))) begin
            B_V_data_1_sel_rd <= ~B_V_data_1_sel_rd;
        end else begin
            B_V_data_1_sel_rd <= B_V_data_1_sel_rd;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        B_V_data_1_sel_wr <= 1'b0;
    end else begin
        if (((1'b1 == B_V_data_1_vld_in) & (1'b1 == B_V_data_1_ack_in))) begin
            B_V_data_1_sel_wr <= ~B_V_data_1_sel_wr;
        end else begin
            B_V_data_1_sel_wr <= B_V_data_1_sel_wr;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        B_V_data_1_state <= 2'd0;
    end else begin
        if ((((2'd3 == B_V_data_1_state) & (1'b0 == B_V_data_1_vld_in) & (1'b1 == B_V_data_1_ack_out)) | ((2'd2 == B_V_data_1_state) & (1'b0 == B_V_data_1_vld_in)))) begin
            B_V_data_1_state <= 2'd2;
        end else if ((((2'd1 == B_V_data_1_state) & (1'b0 == B_V_data_1_ack_out)) | ((2'd3 == B_V_data_1_state) & (1'b0 == B_V_data_1_ack_out) & (1'b1 == B_V_data_1_vld_in)))) begin
            B_V_data_1_state <= 2'd1;
        end else if ((((2'd1 == B_V_data_1_state) & (1'b1 == B_V_data_1_ack_out)) | (~((1'b0 == B_V_data_1_ack_out) & (1'b1 == B_V_data_1_vld_in)) & ~((1'b0 == B_V_data_1_vld_in) & (1'b1 == B_V_data_1_ack_out)) & (2'd3 == B_V_data_1_state)) | ((2'd2 == B_V_data_1_state) & (1'b1 == B_V_data_1_vld_in)))) begin
            B_V_data_1_state <= 2'd3;
        end else begin
            B_V_data_1_state <= 2'd2;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == B_V_data_1_load_A)) begin
        B_V_data_1_payload_A <= B_V_data_1_data_in;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == B_V_data_1_load_B)) begin
        B_V_data_1_payload_B <= B_V_data_1_data_in;
    end
end

always @ (*) begin
    if ((1'b1 == B_V_data_1_sel)) begin
        B_V_data_1_data_out = B_V_data_1_payload_B;
    end else begin
        B_V_data_1_data_out = B_V_data_1_payload_A;
    end
end

assign B_V_data_1_ack_in = B_V_data_1_state[1'd1];
assign B_V_data_1_load_A = (~B_V_data_1_sel_wr & B_V_data_1_state_cmp_full);
assign B_V_data_1_load_B = (B_V_data_1_state_cmp_full & B_V_data_1_sel_wr);
assign B_V_data_1_sel = B_V_data_1_sel_rd;
assign B_V_data_1_state_cmp_full = ((B_V_data_1_state != 2'd1) ? 1'b1 : 1'b0);
assign B_V_data_1_vld_out = B_V_data_1_state[1'd0];

assign ack_in = B_V_data_1_ack_in;
assign B_V_data_1_data_in = data_in;
assign B_V_data_1_vld_in = vld_in;

assign vld_out = B_V_data_1_vld_out;
assign data_out = B_V_data_1_data_out;
assign B_V_data_1_ack_out = ack_out;

assign apdone_blk = ((B_V_data_1_state == 2'd3 && ack_out == 1'b0) | (B_V_data_1_state == 2'd1));

endmodule // both

module polar_clip_regslice_both_w1
#(parameter 
    DataWidth=1
)(
    input ap_clk ,
    input ap_rst,

    input data_in , 
    input vld_in , 
    output ack_in ,
    output data_out, 
    output vld_out,
    input ack_out,
    output apdone_blk
);

reg     [1:0] B_V_data_1_state;
wire    B_V_data_1_data_in;
reg     B_V_data_1_data_out;
wire    B_V_data_1_vld_reg;
wire    B_V_data_1_vld_in;
wire    B_V_data_1_vld_out;
reg     B_V_data_1_payload_A;
reg     B_V_data_1_payload_B;
reg     B_V_data_1_sel_rd;
reg     B_V_data_1_sel_wr;
wire    B_V_data_1_sel;
wire    B_V_data_1_load_A;
wire    B_V_data_1_load_B;
wire    B_V_data_1_state_cmp_full;
wire    B_V_data_1_ack_in;
wire    B_V_data_1_ack_out;

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        B_V_data_1_sel_rd <= 1'b0;
    end else begin
        if (((1'b1 == B_V_data_1_vld_out) & (1'b1 == B_V_data_1_ack_out))) begin
            B_V_data_1_sel_rd <= ~B_V_data_1_sel_rd;
        end else begin
            B_V_data_1_sel_rd <= B_V_data_1_sel_rd;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        B_V_data_1_sel_wr <= 1'b0;
    end else begin
        if (((1'b1 == B_V_data_1_vld_in) & (1'b1 == B_V_data_1_ack_in))) begin
            B_V_data_1_sel_wr <= ~B_V_data_1_sel_wr;
        end else begin
            B_V_data_1_sel_wr <= B_V_data_1_sel_wr;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        B_V_data_1_state <= 2'd0;
    end else begin
        if ((((2'd3 == B_V_data_1_state) & (1'b0 == B_V_data_1_vld_in) & (1'b1 == B_V_data_1_ack_out)) | ((2'd2 == B_V_data_1_state) & (1'b0 == B_V_data_1_vld_in)))) begin
            B_V_data_1_state <= 2'd2;
        end else if ((((2'd1 == B_V_data_1_state) & (1'b0 == B_V_data_1_ack_out)) | ((2'd3 == B_V_data_1_state) & (1'b0 == B_V_data_1_ack_out) & (1'b1 == B_V_data_1_vld_in)))) begin
            B_V_data_1_state <= 2'd1;
        end else if ((((2'd1 == B_V_data_1_state) & (1'b1 == B_V_data_1_ack_out)) | (~((1'b0 == B_V_data_1_ack_out) & (1'b1 == B_V_data_1_vld_in)) & ~((1'b0 == B_V_data_1_vld_in) & (1'b1 == B_V_data_1_ack_out)) & (2'd3 == B_V_data_1_state)) | ((2'd2 == B_V_data_1_state) & (1'b1 == B_V_data_1_vld_in)))) begin
            B_V_data_1_state <= 2'd3;
        end else begin
            B_V_data_1_state <= 2'd2;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == B_V_data_1_load_A)) begin
        B_V_data_1_payload_A <= B_V_data_1_data_in;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == B_V_data_1_load_B)) begin
        B_V_data_1_payload_B <= B_V_data_1_data_in;
    end
end

always @ (*) begin
    if ((1'b1 == B_V_data_1_sel)) begin
        B_V_data_1_data_out = B_V_data_1_payload_B;
    end else begin
        B_V_data_1_data_out = B_V_data_1_payload_A;
    end
end

assign B_V_data_1_ack_in = B_V_data_1_state[1'd1];
assign B_V_data_1_load_A = (~B_V_data_1_sel_wr & B_V_data_1_state_cmp_full);
assign B_V_data_1_load_B = (B_V_data_1_state_cmp_full & B_V_data_1_sel_wr);
assign B_V_data_1_sel = B_V_data_1_sel_rd;
assign B_V_data_1_state_cmp_full = ((B_V_data_1_state != 2'd1) ? 1'b1 : 1'b0);
assign B_V_data_1_vld_out = B_V_data_1_state[1'd0];

assign ack_in = B_V_data_1_ack_in;
assign B_V_data_1_data_in = data_in;
assign B_V_data_1_vld_in = vld_in;

assign vld_out = B_V_data_1_vld_out;
assign data_out = B_V_data_1_data_out;
assign B_V_data_1_ack_out = ack_out;

assign apdone_blk = ((B_V_data_1_state == 2'd3 && ack_out == 1'b0) | (B_V_data_1_state == 2'd1));

endmodule // both


