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

// Tasks for tb_krnl_aes

/////////////////////////////////////////////////////////////////////////////////////////////////
// Initial VIPs
task automatic init_vips();

  axi4stream_ready_gen rgen_axis_slv;

  $display("Starting AXI Control Master: ctrl");
  ctrl = new("ctrl", tb_krnl_aes.axi_vip_mst_inst.inst.IF);
  ctrl.start_master();

  $display("Starting AXI4-Stream slave: axis_slv0");
  axis_slv0 = new("axis_slv0", tb_krnl_aes.axis_vip_slv_0.inst.IF);
  axis_slv0.start_slave();

  $display("Starting AXI4-Stream slave: axis_slv1");
  axis_slv1 = new("axis_slv1", tb_krnl_aes.axis_vip_slv_1.inst.IF);
  axis_slv1.start_slave();
  
  $display("Starting AXI4-Stream master: axis_mst0");
  axis_mst0 = new("axis_mst0", tb_krnl_aes.axis_vip_mst_0.inst.IF);
  axis_mst0.start_master();  

  $display("Starting AXI4-Stream master: axis_mst1");
  axis_mst1 = new("axis_mst1", tb_krnl_aes.axis_vip_mst_1.inst.IF);
  axis_mst1.start_master();    

  // random back pressure on tready

  rgen_axis_slv = new("axis_slv_random_backpressure_tready");
  rgen_axis_slv.set_ready_policy(XIL_AXI4STREAM_READY_GEN_RANDOM);
  rgen_axis_slv.set_low_time_range(0,12);
  rgen_axis_slv.set_high_time_range(1,12);
  rgen_axis_slv.set_event_count_range(3,5);
  
  axis_slv0.driver.send_tready(rgen_axis_slv);
  axis_slv1.driver.send_tready(rgen_axis_slv);

endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// Control interface blocking write
// The task will return when the BRESP has been returned from the kernel.
task automatic blocking_write_register (input bit [31:0] addr_in, input bit [31:0] data);
  axi_transaction   wr_xfer;
  axi_transaction   wr_rsp;
  wr_xfer = ctrl.wr_driver.create_transaction("wr_xfer");
  wr_xfer.set_driver_return_item_policy(XIL_AXI_PAYLOAD_RETURN);
  assert(wr_xfer.randomize() with {addr == addr_in;});
  wr_xfer.set_data_beat(0, data);
  ctrl.wr_driver.send(wr_xfer);
  ctrl.wr_driver.wait_rsp(wr_rsp);
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// Control interface blocking read
// The task will return when the BRESP has been returned from the kernel.
task automatic read_register (input bit [31:0] addr, output bit [31:0] rddata);
  axi_transaction   rd_xfer;
  axi_transaction   rd_rsp;
  bit [31:0] rd_value;
  rd_xfer = ctrl.rd_driver.create_transaction("rd_xfer");
  rd_xfer.set_addr(addr);
  rd_xfer.set_driver_return_item_policy(XIL_AXI_PAYLOAD_RETURN);
  ctrl.rd_driver.send(rd_xfer);
  ctrl.rd_driver.wait_rsp(rd_rsp);
  rd_value = rd_rsp.get_data_beat(0);
  rddata = rd_value;
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// start axi stream master to transfer data
task automatic start_stream_traffic(input axis_vip_mst_mst_t axis_mst, input bit[127:0] xfer_data[$]);
  int  i;
  axi4stream_transaction mst_xfer;
  mst_xfer = axis_mst.driver.create_transaction("axis master transaction");
  for (i = 0; i < `WORD_NUM; i = i + 1) begin
    mst_xfer.set_data_beat(xfer_data[i]);
    axis_mst.driver.send(mst_xfer);    
  end
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// use axi stream monitor to get received data by axi stream slave
task automatic stream_data_receive(input axis_vip_slv_slv_t axis_slv, inout bit[127:0] xfer_data[$]);
  ///////////////////////////////////////////////////////////////////////////
  int i;
  bit [127:0] data;
  axi4stream_monitor_transaction slv_xfer;
  for (i = 0; i < `WORD_NUM; i= i + 1) begin
  axis_slv.monitor.item_collected_port.get(slv_xfer);
  data = slv_xfer.get_data_beat();
  xfer_data[i] = data;
  end  
endtask

/////////////////////////////////////////////////////////////////////////////////////////////////
// apply backpressure on axi stream slave VIP
task automatic slv_random_backpressure_tready(input axis_vip_slv_slv_t axis_slv);
  $display("%t - Applying slv_random_backpressure_tready", $time);
  fork

  begin
    axi4stream_ready_gen rgen_axis_slv;
    rgen_axis_slv = new("axis_slv_random_backpressure_tready");
    rgen_axis_slv.set_ready_policy(XIL_AXI4STREAM_READY_GEN_RANDOM);
    rgen_axis_slv.set_low_time_range(0,12);
    rgen_axis_slv.set_high_time_range(1,12);
    rgen_axis_slv.set_event_count_range(3,5);
    axis_slv.driver.send_tready(rgen_axis_slv);
  end

join_none
endtask