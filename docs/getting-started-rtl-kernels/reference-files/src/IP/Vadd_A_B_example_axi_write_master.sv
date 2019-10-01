// This is a generated file. Use and modify at your own risk.
////////////////////////////////////////////////////////////////////////////////

// Description: This is an AXI4 write master module example. The module
// demonstrates how to issue AXI write transactions to a memory mapped slave.
// Given a starting address offset and a transfer size in bytes, it will issue
// one or more AXI transactions by generating incrementing AXI write transfers
// when data is transfered over the AXI4-Stream interface.

// Theory of operation:
// It uses a minimum subset of the AXI4 protocol by omitting AXI4 signals that
// are not used.  When packaged as a kernel or IP, this allows for optimizations
// to occur within the AXI Interconnect system to increase Fmax, potentially
// increase performance, and reduce latency/area. When C_INCLUDE_DATA_FIFO is
// set to 1, a depth 32 FIFO is provided for extra buffering.
//
// When ctrl_start is asserted, the ctrl_addr_offset (assumed 4kb aligned) and
// the transfer size in bytes is registered into the module.  the The bulk of the
// logic consists of counters to track how many transfers/transactions have been
// issued.  When the transfer size is reached, and all transactions are
// committed, then done is asserted.
//
// Usage:
// 1) assign ctrl_addr_offset to a 4kB aligned starting address.
// 2) assign ctrl_xfer_size_in_bytes to the size in bytes of the requested transfer.
// 3) Assert ctrl_start for once cycle.  At the posedge, the ctrl_addr_offset and
// ctrl_xfer_size_in_bytes will be registered in the module, and will start
// to issue write address transfers when the first data arrives on the s_axis
// interface.  If the the transfer size is larger than 4096
// bytes, multiple transactions will be issued.
// 4) As write data is presented on the axi4-stream interface, WLAST and
// additional write address transfers will be issued as necessary.
// 5) When the final B-channel transaction has been received, the module will assert
//    the ctrl_done signal for one cycle.  If a data FIFO is present, data may
//    still be present in the FIFO.  It will
// 6) Jump to step 1.
////////////////////////////////////////////////////////////////////////////////

// default_nettype of none prevents implicit wire declaration.
`default_nettype none

module Vadd_A_B_example_axi_write_master #(
  // Set to the address width of the interface
  parameter integer C_M_AXI_ADDR_WIDTH  = 64,

  // Set the data width of the interface
  // Range: 32, 64, 128, 256, 512, 1024
  parameter integer C_M_AXI_DATA_WIDTH  = 32,

  // Width of the ctrl_xfer_size_in_bytes input
  // Range: 16:C_M_AXI_ADDR_WIDTH
  parameter integer C_XFER_SIZE_WIDTH   = C_M_AXI_ADDR_WIDTH,

  // Specifies the maximum number of AXI4 transactions that may be outstanding.
  parameter integer C_MAX_OUTSTANDING   = 32,

  // Includes a data fifo between the AXI4-Stream slave and the AXI4 write
  // channel master.  Depth is set to 32.
  parameter integer C_INCLUDE_DATA_FIFO = 1
)
(
  // AXI Interface
  input  wire                            aclk,
  input  wire                            areset,

  // Control signals
  input  wire                            ctrl_start,              // Pulse high for one cycle to begin reading
  output wire                            ctrl_done,               // Pulses high for one cycle when transfer request is complete
  // The following ctrl signals are sampled when ctrl_start is asserted
  input  wire [C_M_AXI_ADDR_WIDTH-1:0]   ctrl_addr_offset,        // Starting Address offset
  input  wire [C_XFER_SIZE_WIDTH-1:0]    ctrl_xfer_size_in_bytes, // Length in number of bytes, limited by the address width.

  // AXI4 master interface (write only)
  output wire                            m_axi_awvalid,
  input  wire                            m_axi_awready,
  output wire [C_M_AXI_ADDR_WIDTH-1:0]   m_axi_awaddr,
  output wire [7:0]                      m_axi_awlen,

  output wire                            m_axi_wvalid,
  input  wire                            m_axi_wready,
  output wire [C_M_AXI_DATA_WIDTH-1:0]   m_axi_wdata,
  output wire [C_M_AXI_DATA_WIDTH/8-1:0] m_axi_wstrb,
  output wire                            m_axi_wlast,

  input  wire                            m_axi_bvalid,
  output wire                            m_axi_bready,
  // AXI4-Stream interface
  input  wire                            s_axis_aclk,
  input  wire                            s_axis_areset,
  input  wire                            s_axis_tvalid,
  output wire                            s_axis_tready,
  input  wire  [C_M_AXI_DATA_WIDTH-1:0]  s_axis_tdata

);

timeunit 1ps;
timeprecision 1ps;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////
function integer f_min (
  input integer a,
  input integer b
);
  f_min = (a < b) ? a : b;
endfunction

/////////////////////////////////////////////////////////////////////////////
// Local Parameters
/////////////////////////////////////////////////////////////////////////////
localparam integer LP_DW_BYTES                   = C_M_AXI_DATA_WIDTH/8;
localparam integer LP_LOG_DW_BYTES               = $clog2(LP_DW_BYTES);
localparam integer LP_MAX_BURST_LENGTH           = 256;   // Max AXI Protocol burst length
localparam integer LP_MAX_BURST_BYTES            = 4096;  // Max AXI Protocol burst size in bytes
localparam integer LP_AXI_BURST_LEN              = f_min(LP_MAX_BURST_BYTES/LP_DW_BYTES, LP_MAX_BURST_LENGTH);
localparam integer LP_LOG_BURST_LEN              = $clog2(LP_AXI_BURST_LEN);
localparam integer LP_LOG_MAX_W_TO_AW            = 8; // Allow up to 256 outstanding w to aw transactions
localparam integer LP_TOTAL_LEN_WIDTH            = C_XFER_SIZE_WIDTH-LP_LOG_DW_BYTES;
localparam integer LP_TRANSACTION_CNTR_WIDTH     = LP_TOTAL_LEN_WIDTH-LP_LOG_BURST_LEN;
localparam [C_M_AXI_ADDR_WIDTH-1:0] LP_ADDR_MASK = LP_DW_BYTES*LP_AXI_BURST_LEN - 1;
localparam integer LP_FIFO_DEPTH                 = 32;
localparam integer LP_FIFO_READ_LATENCY          = 1; // 2: Registered output on BRAM, 1: Registered output on LUTRAM
localparam integer LP_FIFO_COUNT_WIDTH           = $clog2(LP_FIFO_DEPTH)+1;
localparam integer LP_OUTSTANDING_CNTR_WIDTH     = $clog2(C_MAX_OUTSTANDING+1);

/////////////////////////////////////////////////////////////////////////////
// Variables
/////////////////////////////////////////////////////////////////////////////
// Control
logic                                 done = 1'b0;
logic                                 has_partial_bursts;
logic                                 ctrl_start_d1 = 1'b0;
logic [C_M_AXI_ADDR_WIDTH-1:0]        addr_offset_r;
logic [LP_TOTAL_LEN_WIDTH-1:0]        total_len_r;
logic [LP_LOG_DW_BYTES   -1:0]        byte_remainder_r;
logic [LP_DW_BYTES   -1:0]            final_strb;
logic                                 start    = 1'b0;
logic                                 start_d1 = 1'b0;
logic [LP_TRANSACTION_CNTR_WIDTH-1:0] num_transactions;
logic [LP_LOG_BURST_LEN-1:0]          final_burst_len;
logic                                 single_transaction;
// Write data channel
logic                                 s_axis_tready_n;
logic                                 m_axi_wvalid_i;
logic                                 wxfer;       // Unregistered write data transfer
logic                                 wfirst = 1'b1;
logic                                 load_burst_cntr;
logic [LP_LOG_BURST_LEN-1:0]          wxfers_to_go;  // Used for simulation debug
logic [LP_TRANSACTION_CNTR_WIDTH-1:0] w_transactions_to_go;
logic                                 w_final_transaction;
logic                                 w_final_transfer;
logic                                 w_almost_final_transaction = 1'b0;
logic                                 w_running = 1'b0;
// Write address channel
logic                                 awxfer;
logic                                 awvalid_r    = 1'b0;
logic [C_M_AXI_ADDR_WIDTH-1:0]        addr;
logic                                 wfirst_d1    = 1'b0;
logic                                 wfirst_pulse = 1'b0;
logic [LP_LOG_MAX_W_TO_AW-1:0]        dbg_w_to_aw_outstanding;
logic                                 idle_aw;
logic [LP_TRANSACTION_CNTR_WIDTH-1:0] aw_transactions_to_go;
logic                                 aw_final_transaction;
// Write response channel
wire                                  bxfer;
logic [LP_TRANSACTION_CNTR_WIDTH-1:0] b_transactions_to_go;
logic                                 b_final_transaction;
logic                                 stall_aw;
logic [LP_OUTSTANDING_CNTR_WIDTH-1:0] outstanding_vacancy_count;

/////////////////////////////////////////////////////////////////////////////
// Control logic
/////////////////////////////////////////////////////////////////////////////
assign ctrl_done = done;

// Count the number of transfers and assert done when the last m_axi_bvalid is received.
always @(posedge aclk) begin
  done <= bxfer & b_final_transaction;
end

always @(posedge aclk) begin
  ctrl_start_d1 <= ctrl_start;
end

always @(posedge aclk) begin
  if (ctrl_start) begin
    // Round transfer size up to integer value of the axi interface data width. Convert to axi_arlen format which is length -1.
    total_len_r <= ctrl_xfer_size_in_bytes[0+:LP_LOG_DW_BYTES] > 0
                      ? ctrl_xfer_size_in_bytes[LP_LOG_DW_BYTES+:LP_TOTAL_LEN_WIDTH]
                      : ctrl_xfer_size_in_bytes[LP_LOG_DW_BYTES+:LP_TOTAL_LEN_WIDTH] - 1'b1;
    // Align transfer to burst length to avoid AXI protocol issues if starting address is not correctly aligned.
    addr_offset_r <= ctrl_addr_offset & ~LP_ADDR_MASK;
    byte_remainder_r <= ctrl_xfer_size_in_bytes[0+:LP_LOG_DW_BYTES]-1'b1;
  end
end

// Determine how many full burst to issue and if there are any partial bursts.
assign num_transactions = total_len_r[LP_LOG_BURST_LEN+:LP_TRANSACTION_CNTR_WIDTH];
assign has_partial_bursts = total_len_r[0+:LP_LOG_BURST_LEN] == '1 ? 1'b0 : 1'b1;

always @(posedge aclk) begin
  start <= ctrl_start_d1;
  final_burst_len  <= total_len_r[0+:LP_LOG_BURST_LEN];
end

// Special case if there is only 1 AXI transaction.
assign single_transaction = (num_transactions == '0) ? 1'b1 : 1'b0;

/////////////////////////////////////////////////////////////////////////////
// AXI Write Data Channel
/////////////////////////////////////////////////////////////////////////////
// Used to gate valid/ready signals with running so transfers don't occur before the
// xfer size is known.
always @(posedge aclk) begin
  if (areset) begin
    w_running <= 1'b0;
  end
  else begin
    w_running <= start            ? 1'b1 :
                 w_final_transfer ? 1'b0 :
                                    w_running ;
  end
end

generate
if (C_INCLUDE_DATA_FIFO == 1) begin : gen_fifo

  // xpm_fifo_sync: Synchronous FIFO
  // Xilinx Parameterized Macro, Version 2017.4
  xpm_fifo_sync # (
    .FIFO_MEMORY_TYPE    ( "distributed"        ) , // string; "auto", "block", "distributed", or "ultra";
    .ECC_MODE            ( "no_ecc"             ) , // string; "no_ecc" or "en_ecc";
    .FIFO_WRITE_DEPTH    ( LP_FIFO_DEPTH        ) , // positive integer
    .WRITE_DATA_WIDTH    ( C_M_AXI_DATA_WIDTH   ) , // positive integer
    .WR_DATA_COUNT_WIDTH ( LP_FIFO_COUNT_WIDTH  ) , // positive integer, not used
    .PROG_FULL_THRESH    ( 10                   ) , // positive integer, not used
    .FULL_RESET_VALUE    ( 1                    ) , // positive integer; 0 or 1
    .USE_ADV_FEATURES    ( "1F1F"               ) , // string; "0000" to "1F1F";
    .READ_MODE           ( "fwft"               ) , // string; "std" or "fwft";
    .FIFO_READ_LATENCY   ( LP_FIFO_READ_LATENCY ) , // positive integer;
    .READ_DATA_WIDTH     ( C_M_AXI_DATA_WIDTH   ) , // positive integer
    .RD_DATA_COUNT_WIDTH ( LP_FIFO_COUNT_WIDTH  ) , // positive integer, not used
    .PROG_EMPTY_THRESH   ( 10                   ) , // positive integer, not used
    .DOUT_RESET_VALUE    ( "0"                  ) , // string, don't care
    .WAKEUP_TIME         ( 0                    ) // positive integer; 0 or 2;
  )
  inst_xpm_fifo_sync (
    .sleep         ( 1'b0                     ) ,
    .rst           ( areset                   ) ,
    .wr_clk        ( aclk                     ) ,
    .wr_en         ( s_axis_tvalid            ) ,
    .din           ( s_axis_tdata             ) ,
    .full          ( s_axis_tready_n          ) ,
    .overflow      (                          ) ,
    .prog_full     (                          ) ,
    .wr_data_count (                          ) ,
    .almost_full   (                          ) ,
    .wr_ack        (                          ) ,
    .wr_rst_busy   (                          ) ,
    .rd_en         ( m_axi_wready & w_running ) ,
    .dout          ( m_axi_wdata              ) ,
    .empty         (                          ) ,
    .prog_empty    (                          ) ,
    .rd_data_count (                          ) ,
    .almost_empty  (                          ) ,
    .data_valid    ( m_axi_wvalid_i           ) ,
    .underflow     (                          ) ,
    .rd_rst_busy   (                          ) ,
    .injectsbiterr ( 1'b0                     ) ,
    .injectdbiterr ( 1'b0                     ) ,
    .sbiterr       (                          ) ,
    .dbiterr       (                          )
  );

  assign s_axis_tready = ~s_axis_tready_n;
  assign m_axi_wvalid = m_axi_wvalid_i & w_running;

end
else begin : gen_no_fifo
  // Gate valid/ready signals with running so transfers don't occur before the
  // xfer size is known.
  assign m_axi_wvalid  = s_axis_tvalid & w_running;
  assign m_axi_wdata   = s_axis_tdata;
  assign s_axis_tready = m_axi_wready & w_running;
end
endgenerate

assign wxfer = m_axi_wvalid & m_axi_wready;

assign w_final_transfer = m_axi_wlast & w_final_transaction & wxfer;
assign m_axi_wstrb   = m_axi_wlast & w_final_transaction ? final_strb : {(C_M_AXI_DATA_WIDTH/8){1'b1}};

always @(posedge aclk) begin
  final_strb[0] <= 1'b1;
  for (int i = 1; i < LP_DW_BYTES; i = i + 1) begin : loop
    final_strb[i] <= i > byte_remainder_r  ? 1'b0 : 1'b1;
  end
end

always @(posedge aclk) begin
  if (areset) begin
    wfirst <= 1'b1;
  end
  else begin
    wfirst <= wxfer ? m_axi_wlast : wfirst;
  end
end

// Load burst counter with partial burst if on final transaction or if there is only 1 transaction
assign load_burst_cntr = (wxfer & m_axi_wlast & w_almost_final_transaction) || (start & single_transaction);

Vadd_A_B_example_counter #(
  .C_WIDTH ( LP_LOG_BURST_LEN         ) ,
  .C_INIT  ( {LP_LOG_BURST_LEN{1'b1}} )
)
inst_burst_cntr (
  .clk        ( aclk            ) ,
  .clken      ( 1'b1            ) ,
  .rst        ( areset          ) ,
  .load       ( load_burst_cntr ) ,
  .incr       ( 1'b0            ) ,
  .decr       ( wxfer           ) ,
  .load_value ( final_burst_len ) ,
  .count      ( wxfers_to_go    ) ,
  .is_zero    ( m_axi_wlast     )
);

Vadd_A_B_example_counter #(
  .C_WIDTH ( LP_TRANSACTION_CNTR_WIDTH         ) ,
  .C_INIT  ( {LP_TRANSACTION_CNTR_WIDTH{1'b0}} )
)
inst_w_transaction_cntr (
  .clk        ( aclk                 ) ,
  .clken      ( 1'b1                 ) ,
  .rst        ( areset               ) ,
  .load       ( start                ) ,
  .incr       ( 1'b0                 ) ,
  .decr       ( wxfer & m_axi_wlast  ) ,
  .load_value ( num_transactions     ) ,
  .count      ( w_transactions_to_go ) ,
  .is_zero    ( w_final_transaction  )
);

always @(*) begin
  w_almost_final_transaction <= (w_transactions_to_go == 1) ? 1'b1 : 1'b0;
end

/////////////////////////////////////////////////////////////////////////////
// AXI Write Address Channel
/////////////////////////////////////////////////////////////////////////////
// The address channel samples the data channel and send out transactions when
// first beat of m_axi_wdata is asserted. This ensures that address requests are not
// sent without data on the way.

assign m_axi_awvalid = awvalid_r;
assign awxfer = m_axi_awvalid & m_axi_awready;

always @(posedge aclk) begin
  if (areset) begin
    awvalid_r <= 1'b0;
  end
  else begin
    awvalid_r <= ~idle_aw & ~awvalid_r & ~stall_aw ? 1'b1 :
                 m_axi_awready         ? 1'b0 :
                                         awvalid_r;
  end
end

assign m_axi_awaddr = addr;

always @(posedge aclk) begin
  addr <= start  ? addr_offset_r :
          awxfer ? addr + LP_DW_BYTES*LP_AXI_BURST_LEN :
                   addr;
end

assign m_axi_awlen   = aw_final_transaction || (start & single_transaction) ? final_burst_len : LP_AXI_BURST_LEN- 1;

Vadd_A_B_example_counter #(
  .C_WIDTH (LP_LOG_MAX_W_TO_AW),
  .C_INIT ({LP_LOG_MAX_W_TO_AW{1'b0}})
)
inst_w_to_aw_cntr (
  .clk        ( aclk                    ) ,
  .clken      ( 1'b1                    ) ,
  .rst        ( areset                  ) ,
  .load       ( 1'b0                    ) ,
  .incr       ( wfirst_pulse            ) ,
  .decr       ( awxfer                  ) ,
  .load_value ( '0                      ) ,
  .count      ( dbg_w_to_aw_outstanding ) ,
  .is_zero    ( idle_aw                 )
);

always @(posedge aclk) begin
  wfirst_d1 <= m_axi_wvalid & wfirst;
end

always @(posedge aclk) begin
  wfirst_pulse <= m_axi_wvalid & wfirst & ~wfirst_d1;
end

Vadd_A_B_example_counter #(
  .C_WIDTH ( LP_TRANSACTION_CNTR_WIDTH         ) ,
  .C_INIT  ( {LP_TRANSACTION_CNTR_WIDTH{1'b0}} )
)
inst_aw_transaction_cntr (
  .clk        ( aclk                   ) ,
  .clken      ( 1'b1                   ) ,
  .rst        ( areset                 ) ,
  .load       ( start                  ) ,
  .incr       ( 1'b0                   ) ,
  .decr       ( awxfer                 ) ,
  .load_value ( num_transactions       ) ,
  .count      ( aw_transactions_to_go  ) ,
  .is_zero    ( aw_final_transaction   )
);

/////////////////////////////////////////////////////////////////////////////
// AXI Write Response Channel
/////////////////////////////////////////////////////////////////////////////
assign m_axi_bready = 1'b1;
assign bxfer = m_axi_bready & m_axi_bvalid;

Vadd_A_B_example_counter #(
  .C_WIDTH ( LP_TRANSACTION_CNTR_WIDTH         ) ,
  .C_INIT  ( {LP_TRANSACTION_CNTR_WIDTH{1'b0}} )
)
inst_b_transaction_cntr (
  .clk        ( aclk                 ) ,
  .clken      ( 1'b1                 ) ,
  .rst        ( areset               ) ,
  .load       ( start                ) ,
  .incr       ( 1'b0                 ) ,
  .decr       ( bxfer                ) ,
  .load_value ( num_transactions     ) ,
  .count      ( b_transactions_to_go ) ,
  .is_zero    ( b_final_transaction  )
);

// Keeps track of the number of outstanding transactions. Stalls
// when the value is reached so that the FIFO won't overflow.
// If no FIFO present, then just limit at max outstanding transactions.
Vadd_A_B_example_counter #(
  .C_WIDTH ( LP_OUTSTANDING_CNTR_WIDTH                       ) ,
  .C_INIT  ( C_MAX_OUTSTANDING[0+:LP_OUTSTANDING_CNTR_WIDTH] )
)
inst_aw_to_b_transaction_cntr (
  .clk        ( aclk                              ) ,
  .clken      ( 1'b1                              ) ,
  .rst        ( areset                            ) ,
  .load       ( 1'b0                              ) ,
  .incr       ( awxfer                            ) ,
  .decr       ( bxfer                             ) ,
  .load_value ( {LP_OUTSTANDING_CNTR_WIDTH{1'b0}} ) ,
  .count      ( outstanding_vacancy_count         ) ,
  .is_zero    ( stall_aw                          )
);

endmodule : Vadd_A_B_example_axi_write_master

`default_nettype wire


