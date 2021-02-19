//
// Copyright 2020 Xilinx, Inc.
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

// Core control module of RTC generation

`timescale 1ns/1ps

module rtc_gen_core (
  input	        clk,
  input		    reset_n,

// control_s signals  
  input [31:0]  work_mode,
  input [31:0]  cs_count,
  input [31:0]  time_format,  
  input [31:0]  time_set_val,
  input [31:0]  time_set_en,

  input         ap_start,
  output        ap_done,
  output reg    ap_idle,
  
// read master signals  
  output reg    ctrl_start,    // Pulse high for one cycle to begin reading
  input         ctrl_done,     // Pulses high for one cycle when transfer request is complete
  output [31:0] ctrl_xfer_size_in_bytes, // total read size in bytes
  input         mdata_tvalid,
  output        mdata_tready,
  input  [31:0] mdata_tdata,

// AXI Stream signals
  output reg    axis_m_tvalid,
  input         axis_m_tready,
  output [63:0] axis_m_tdata,
  output [7:0]  axis_m_tkeep,
  output reg    axis_m_tlast,
  
// Time value output for AXI read
  output [31:0] time_value
  
);

parameter       FONT_WIDTH = 160,
                FONT_HEIGHT = 240;
                
localparam      FONT_BUFF_DEPTH = FONT_WIDTH * FONT_HEIGHT * 4 * 11 / 32;   // font buffer depth
localparam      FONT_BUFF_WIDTH = $clog2(FONT_BUFF_DEPTH);                  // font buffer address bit width
localparam      ROW_CNT_WIDTH   = $clog2(FONT_HEIGHT);                      // row counter bit width
localparam      COL_CNT_WIDTH   = $clog2(FONT_WIDTH/8);                     // font height bit width
                

  wire [31 : 0] font_buf_din;     // font buffer data input
  wire [31 : 0] font_buf_dout;    // font buffer data output`
  wire [FONT_BUFF_WIDTH-1 : 0] font_buf_addr;   // font buffer address
  wire          font_buf_we;      // font buffer write enable in font load mode, active high
  wire          font_buf_cs;      // font buffer chip selection, active high
  
  reg  [FONT_BUFF_WIDTH-1 : 0] font_buf_read_addr;  // font buffer read address
  reg  [FONT_BUFF_WIDTH-1 : 0] font_buf_write_addr; // font buffer write address
  reg           font_buf_re;      // font buffer read enable in image generation mode, active high
  
  reg           ap_start_reg;     // registered ap_start signal
  wire          ap_start_trigger; // ap_start rising detection
  reg           ap_start_trigger_dly; // delayed ap_start_trigger, some operation need to be one-cycle later than others
  reg           ap_done_load;     // kernel ap control signal in font load mode
  reg           ap_done_gen;      // kernel ap control signal in RTC image generation mode
  reg           next_ap_done_gen;
  
  reg           next_axis_m_tvalid; // next state of AXI stream TVALID signal
  
  wire [7:0]    time_set_hours;     // hours of time setting
  wire [7:0]    time_set_minutes;   // minutes of time setting
  wire [7:0]    time_set_seconds;   // seconds of time setting
  wire [7:0]    time_set_centisecs; // centiseconds of time setting

  reg [3:0] state;          // image generation state machine variable
  reg [3:0] next_state;
  
  reg [ROW_CNT_WIDTH-1 : 0]  row_cnt;     // output image row counter
  reg [ROW_CNT_WIDTH-1 : 0]  next_row_cnt;
  reg [COL_CNT_WIDTH-1 : 0]  col_cnt;     // output image column counter (8 pixel will be output at each beat)
  reg [COL_CNT_WIDTH-1 : 0]  next_col_cnt;  

  reg [21:0]    cs_timer;       // centi-second timer
  reg [6:0]     cs_acc;         // centi-second accumulator
  reg [6:0]     next_cs_acc;    // next cs_acc
  reg [7:0]     cs_acc_reg;     // registered cs_acc for time image output
  reg [5:0]     sec_acc;        // second accumulator
  reg [5:0]     next_sec_acc;   // next sec_acc
  reg [7:0]     sec_acc_reg;    // registered sec_acc for time image output
  reg [5:0]     min_acc;        // minuate accumulator
  reg [5:0]     next_min_acc;   // next min_acc
  reg [7:0]     min_acc_reg;    // registered min_acc for time image output
  reg [4:0]     hour_acc;       // hour accumulator
  reg [4:0]     next_hour_acc;  // next hour_acc
  reg [7:0]     hour_acc_reg;   // registered hour_acc for time image output

  reg           time_set_en_reg;    // registered time_set_en signal

// General assignment
  assign mdata_tready = 1'b1;       // always ready, because of no latency SRAM buffer
  assign ctrl_xfer_size_in_bytes = FONT_WIDTH * FONT_HEIGHT * 4 * 11 / 8;
  
  assign axis_m_tkeep = 8'b1111_1111;
  assign axis_m_tdata = {font_buf_dout[31:28], 4'hf,    // expand the 4-bit font buffer output to 8-bit
                         font_buf_dout[27:24], 4'hf,
                         font_buf_dout[23:20], 4'hf,
                         font_buf_dout[19:16], 4'hf,
                         font_buf_dout[15:12], 4'hf,
                         font_buf_dout[11: 8], 4'hf,
                         font_buf_dout[ 7: 4], 4'hf,
                         font_buf_dout[ 3: 0], 4'hf};
                         
  assign time_set_hours = time_set_val[31:24];
  assign time_set_minutes = time_set_val[23:16];
  assign time_set_seconds = time_set_val[15:8];
  assign time_set_centisecs = time_set_val[7:0];
 
  assign time_value = {2'b0, hour_acc, 2'b0, min_acc, 2'b0, sec_acc, 1'b0, cs_acc};

// -=========================================================================================-  
//  Function definition
// -=========================================================================================-
// Functions to map the number 0 - 99 to tens digit (0-9)
  function  [3:0] tens;
    input [7:0] number;
    begin
      if (number < 10) begin
        tens = 0;
      end else if (number < 20) begin
        tens = 1;
      end else if (number < 30) begin  
        tens = 2;
      end else if (number < 40) begin
        tens = 3;
      end else if (number < 50) begin
        tens = 4;
      end else if (number < 60) begin
        tens = 5;
      end else if (number < 70) begin
        tens = 6;
      end else if (number < 80) begin
        tens = 7;
      end else if (number < 90) begin
        tens = 8;
      end else begin
        tens = 9;
      end
    end
  endfunction // tens

// Functions to map the number 0 - 99 to units digit （0-9）      
  function  [3:0] units;
    input [7:0] number;
    begin
      if (number < 10) begin
        units = number;
      end else if (number < 20) begin
        units = number - 10;
      end else if (number < 30) begin  
        units = number - 20;
      end else if (number < 40) begin
        units = number - 30;
      end else if (number < 50) begin
        units = number - 40;
      end else if (number < 60) begin
        units = number - 50;
      end else if (number < 70) begin
        units = number - 60;
      end else if (number < 80) begin
        units = number - 70;
      end else if (number < 90) begin
        units = number - 80;
      end else if (number < 100) begin
        units = number - 90;
      end else begin
        units = 0;
      end
    end
  endfunction // units

// function for font buffer read address
  function [FONT_BUFF_WIDTH-1:0] font_buf_addr_calc;
    input [3:0] digit;
    input [ROW_CNT_WIDTH-1:0] row_cnt;
    input [COL_CNT_WIDTH-1:0] col_cnt;
    begin
      font_buf_addr_calc = FONT_HEIGHT * FONT_WIDTH/8 * digit + FONT_WIDTH/8 * row_cnt + col_cnt;
    end
  endfunction // font_buf_addr_calc


// kernel handshake handling
  always @ (posedge clk or negedge reset_n) begin : ap_start_registering
    if (!reset_n) begin
      ap_start_reg         <= 1'b0;
      ap_start_trigger_dly <= 1'b0;
    end else begin
      ap_start_reg         <= ap_start;
      ap_start_trigger_dly <= ap_start_trigger;
    end
  end // ap_start_registering
  
  assign ap_start_trigger = !ap_start_reg && ap_start;

  always @ (posedge clk or negedge reset_n) begin : ap_idle_handling_seq
    if (!reset_n) begin
      ap_idle <= 1'b1;
    end else if (ap_start_trigger) begin
      ap_idle <= 1'b0;
    end else if (ap_done) begin
      ap_idle <= 1'b1;
    end
  end // ap_idle_handling

  assign ap_done  = work_mode[0] ? ap_done_gen  : ap_done_load;      


// Font buffer instantiation
// Width: 32-bit, contain 8-pixel font data
// Depth: FONT_BUFF_DEPTH
  SPSR #(
    .WORD_DEPTH     (FONT_BUFF_DEPTH),
    .DATA_WIDTH     (32))
  inst_FontBuffer (
    .Q      (font_buf_dout  ),
    .CLK    (clk            ), 
    .CE     (font_buf_cs    ), 
    .WE     (font_buf_we    ), 
    .A      (font_buf_addr  ), 
    .D      (font_buf_din   )
  );
  
  assign font_buf_din = mdata_tdata;
  assign font_buf_addr = work_mode[0] ? font_buf_read_addr : font_buf_write_addr;
  assign font_buf_cs = font_buf_we | font_buf_re;
  assign font_buf_we = mdata_tvalid;


// -=========================================================================================-
//  real time clock accumulator
// -=========================================================================================-
  always @ (posedge clk or negedge reset_n) begin : cs_timer_control
    if (!reset_n) begin
      cs_timer <= 'h0;
    end else if (cs_timer == (cs_count[21:0] - 1'b1)) begin
      cs_timer <= 'h0;
    end else begin
      cs_timer <= cs_timer + 1'b1;
    end
  end // cs_timer_control

  always @ (posedge clk or negedge reset_n) begin : time_set_control
    if (!reset_n) begin
      time_set_en_reg <= 1'b0;
    end else begin
      time_set_en_reg <= time_set_en[0];
    end
  end // time_set_control

  always @ (posedge clk or negedge reset_n) begin : time_accumulator_control_seq
    if (!reset_n) begin
      cs_acc   <= 'h0;
      sec_acc  <= 'h0;
      min_acc  <= 'h0;
      hour_acc <= 'h0;
    end else if (!time_set_en_reg && time_set_en[0]) begin
      cs_acc   <= time_set_centisecs;
      sec_acc  <= time_set_seconds;
      min_acc  <= time_set_minutes;
      hour_acc <= time_set_hours;
    end else begin
      cs_acc   <= next_cs_acc;
      sec_acc  <= next_sec_acc;
      min_acc  <= next_min_acc;
      hour_acc <= next_hour_acc;
    end
  end // time_accumulator_control_seq
    
  always @ (*) begin : time_accumulator_control_comb
    next_cs_acc   = cs_acc;
    next_sec_acc  = sec_acc;
    next_min_acc  = min_acc;
    next_hour_acc = hour_acc;
    if (cs_timer == (cs_count - 1'b1)) begin
      if (cs_acc == 'd99) begin
        next_cs_acc = 'd0;
      end else begin
        next_cs_acc = cs_acc + 1'b1;
      end
      
      if ((cs_acc == 'd99) && (next_cs_acc == 'd0)) begin
        if (sec_acc == 'd59) begin
          next_sec_acc = 'd0;
        end else begin
          next_sec_acc = sec_acc + 1'b1;
        end
      end
      
      if ((sec_acc == 'd59) && (next_sec_acc == 'd0)) begin
        if (min_acc == 'd59) begin
          next_min_acc = 'd0;
        end else begin
          next_min_acc = min_acc + 1'b1;
        end
      end
      
      if ((min_acc == 'd59) && (next_min_acc == 'd0)) begin
        if (hour_acc == 'd23) begin
          next_hour_acc = 'd0;
        end else begin
          next_hour_acc = hour_acc + 1'b1;
        end
      end
    end
  end // time_accumulator_control_comb


// -=========================================================================================-  
//  Font library load control
// -=========================================================================================-
  always @ (posedge clk or negedge reset_n) begin : font_buffer_write_addr_control_seq
    if (!reset_n) begin
      font_buf_write_addr <= 'h0;
    end else if (!work_mode[0]) begin
      if (mdata_tvalid) begin
        if (font_buf_write_addr == FONT_BUFF_DEPTH-1) begin
          font_buf_write_addr <= 'h0;
        end else begin
          font_buf_write_addr <= font_buf_write_addr + 1'b1;
        end
      end
    end
  end // font_buffer_write_addr_control

  always @ (posedge clk or negedge reset_n) begin : read_master_handshake_seq
    if (!reset_n) begin
      ctrl_start    <= 1'b0;
    end else if (!work_mode[0]) begin
      if (ap_start_trigger_dly) begin
        ctrl_start  <= 1'b1;
      end else begin
        ctrl_start <= 1'b0;
      end
    end
  end // read_master_handshake
        
  always @ (posedge clk or negedge reset_n) begin : host_ap_handshake_seq
    if (!reset_n) begin
      ap_done_load  <= 1'b0;
    end else if (!work_mode[0]) begin
      if (ctrl_done) begin
        ap_done_load <= 1'b1;
      end else begin
        ap_done_load <= 1'b0;
      end
    end
  end // host_ap_handshake      
        

// -=========================================================================================-  
//  RTC image generation control
// -=========================================================================================-
  
  localparam ST_IDLE     = 4'd0,
             ST_DIGIT_1  = 4'd1,   // first digit of hour
             ST_DIGIT_2  = 4'd2,   // second digit of hour
             ST_DIGIT_3  = 4'd3,   // colon
             ST_DIGIT_4  = 4'd4,   // first digit of minute
             ST_DIGIT_5  = 4'd5,   // second digit of minute
             ST_DIGIT_6  = 4'd6,   // colon
             ST_DIGIT_7  = 4'd7,   // first digit of second
             ST_DIGIT_8  = 4'd8,   // second digit of second
             ST_DIGIT_9  = 4'd9,   // colon
             ST_DIGIT_10 = 4'd10,  // first digit of centisecond
             ST_DIGIT_11 = 4'd11;  // second digit of centisecond


// Image generation state machine
  always @ (posedge clk or negedge reset_n) begin : state_machine_seq
    if (!reset_n) begin
      state             <= ST_IDLE;
      row_cnt           <= 'h0;
      col_cnt           <= 'h0;
      axis_m_tvalid     <= 1'b0;
      ap_done_gen       <= 1'b0;
    end else begin
      state         <= next_state;
      row_cnt       <= next_row_cnt;
      col_cnt       <= next_col_cnt;
      axis_m_tvalid <= next_axis_m_tvalid;
      ap_done_gen   <= next_ap_done_gen;
    end
  end // state_machine_seq

  always @(*) begin : state_machine_comb
    next_state          = state;
    font_buf_re    = 1'b0;
    next_axis_m_tvalid = 1'b0;
    next_col_cnt = col_cnt;
    next_row_cnt = row_cnt;
    next_ap_done_gen = 1'b0;
    case (state)
      ST_IDLE     : begin
        if (ap_start_trigger_dly & work_mode[0]) begin
          next_state    = ST_DIGIT_1;
          font_buf_re   = 1'b1;
          next_axis_m_tvalid = 1'b1;
        end
      end
      
      ST_DIGIT_1  : begin   // output tens of hour
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;      
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_2;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
        
      ST_DIGIT_2  : begin  // output units of hour
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_3;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_3  : begin  // output colon
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_4;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_4  : begin  // output tens of minutes
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_5;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_5  : begin  // output units of minutes
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_6;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_6  : begin  // output colon
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_7;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_7  : begin  // output tens of seconds
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_8;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_8  : begin // output units of seconds
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;    
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            if (time_format[0]) begin
              next_state = ST_DIGIT_9;
            end else if (row_cnt == (FONT_HEIGHT-1)) begin
              next_state = ST_IDLE;
              next_row_cnt = 'h0;
              font_buf_re = 1'b0;
              next_axis_m_tvalid = 1'b0;
              next_ap_done_gen = 1'b1;
            end else begin
              next_state = ST_DIGIT_1;
              next_row_cnt = row_cnt + 1'b1;
            end
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_9  : begin // output colon
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_10;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_10 : begin // output tens of centiseconds
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            next_state = ST_DIGIT_11;
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end
      
      ST_DIGIT_11 : begin // output units of centiseconds
        font_buf_re = 1'b1;
        next_axis_m_tvalid = 1'b1;
        if (axis_m_tready) begin
          if (col_cnt == (FONT_WIDTH/8-1)) begin
            next_col_cnt = 'h0;
            if (row_cnt == (FONT_HEIGHT-1)) begin
              next_state = ST_IDLE;
              next_row_cnt = 'h0;
              font_buf_re = 1'b0;
              next_axis_m_tvalid = 1'b0;
              next_ap_done_gen = 1'b1;
            end else begin
              next_state = ST_DIGIT_1;
              next_row_cnt = row_cnt + 1'b1;
            end
          end else begin
            next_col_cnt = col_cnt + 1'b1;
          end
        end
      end      
      
      default: ;
      
    endcase
  end // state_machine_comb   

// registers time 
  always @ (posedge clk or negedge reset_n) begin : time_value_register
    if (!reset_n) begin
      hour_acc_reg  <= 'h0;
      min_acc_reg   <= 'h0;
      sec_acc_reg   <= 'h0;
      cs_acc_reg    <= 'h0;
    end else if (ap_start_trigger) begin
      hour_acc_reg  <= hour_acc;
      min_acc_reg   <= min_acc;
      sec_acc_reg   <= sec_acc;
      cs_acc_reg    <= cs_acc;
    end
  end

// Font buffer read address generation
  always @(*) begin : font_buffer_read_addr_control
    case (next_state)
      ST_DIGIT_1 :  font_buf_read_addr = font_buf_addr_calc(tens(hour_acc_reg), next_row_cnt, next_col_cnt);      // output tens of hour
      ST_DIGIT_2 :  font_buf_read_addr = font_buf_addr_calc(units(hour_acc_reg), next_row_cnt, next_col_cnt);     // output units of hour
      ST_DIGIT_3 :  font_buf_read_addr = font_buf_addr_calc(10, next_row_cnt, next_col_cnt);                    // output colon
      ST_DIGIT_4 :  font_buf_read_addr = font_buf_addr_calc(tens(min_acc_reg), next_row_cnt, next_col_cnt);    // output tens of minute
      ST_DIGIT_5 :  font_buf_read_addr = font_buf_addr_calc(units(min_acc_reg), next_row_cnt, next_col_cnt);   // output units of minute
      ST_DIGIT_6 :  font_buf_read_addr = font_buf_addr_calc(10, next_row_cnt, next_col_cnt);                    // output colon
      ST_DIGIT_7 :  font_buf_read_addr = font_buf_addr_calc(tens(sec_acc_reg), next_row_cnt, next_col_cnt);    // output tens of second
      ST_DIGIT_8 :  font_buf_read_addr = font_buf_addr_calc(units(sec_acc_reg), next_row_cnt, next_col_cnt);   // output units of second
      ST_DIGIT_9 :  font_buf_read_addr = font_buf_addr_calc(10, next_row_cnt, next_col_cnt);                    // output colon
      ST_DIGIT_10 : font_buf_read_addr = font_buf_addr_calc(tens(cs_acc_reg), next_row_cnt, next_col_cnt);  // output tens of centisecond
      ST_DIGIT_11 : font_buf_read_addr = font_buf_addr_calc(units(cs_acc_reg), next_row_cnt, next_col_cnt); // output units of centisecond
      default: font_buf_read_addr = 'h0;
    endcase
  end // font_buffer_read_addr_control
    
    
// AXI Steam TLAST generation
  always @(*) begin : tlast_gen
    axis_m_tlast = 1'b0;
    if ((state == ST_DIGIT_8) && !time_format[0]) begin
      if ((row_cnt == (FONT_WIDTH/8-1)) && (col_cnt == (FONT_HEIGHT-1))) begin
        axis_m_tlast = axis_m_tvalid;
      end
    end
    if ((state == ST_DIGIT_8) && !time_format[0]) begin
      if ((row_cnt == (FONT_WIDTH/8-1)) && (col_cnt == (FONT_HEIGHT-1))) begin
        axis_m_tlast = axis_m_tvalid;
      end
    end
  end
    
  
endmodule
      